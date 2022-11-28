/*
    Daimonin, the Massive Multiuser Online Role Playing Game
    Server Applicatiom

    Copyright (C) 2001 Michael Toennies

    A split from Crossfire, a Multiplayer game for X-windows.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    The author can be reached via e-mail to info@daimonin.org
*/

#include <global.h>

static void RemoveStuff(object_t *where);
static objectlink_t *get_linked_spawn(object_t *spawn);


// this is a desperate try to collect all the game adjustments spread everywhere at one point
// transfered Autogen and adjust_monster from spawn_point.c to adjust.c
// removed the call in spawn_point.c added the call in map.c
// now we can adjust the exp for level also here, instead in exp.c



 // Autogen wouldn't work on spawn point mob, it's a random based level adjustment logic
 // we want this random factor on each spawn and not once when creating spawn point mob
/* Would it be better to stick the autogen stuff in the
 * spawnpoint rather than each spawn mob?
 * PROS: * Probably more unclaimed attributes to play with in
 *         spawnpoints.
 *       * Easier/quicker for mappers (which is half the point) to set
 *         the autogen stuff once for spawnpoints with multiple
 *         potential spawns.
 *       * Can be simply nullified/changed from a script, though it is
 *         perfectly possible for a script to change the autogen values
 *         per mob, just an extra step.
 * CONS: * Less flexible to do it per spawnpoint than per mob. */


// generate mob level from monster->item_quality - parameter in editor have different name
// and map difficulty
sint16 gen_level(object_t *monster, sint16 diff)
{
    sint16 level = MAX(1, MIN(monster->level, MAXMOBLEVEL)); // this is saver here, so this function also cleans invalid ranges
    if (monster->item_quality) // we have map check in calling function,
    {
        // sint16 level = MAX(1, MIN(monster->level, MAXMOBLEVEL)),
        sint16 min,
               max;

        // sint16 diff = monster->map->difficulty; // we have map check in calling function,


        switch (monster->item_quality)
        {
            case 1: /* green */
                min = level_color[diff].green;
                max = level_color[diff].blue - 1;
                break;

            case 2: /* blue*/
                min = level_color[diff].blue;
                max = level_color[diff].yellow - 1;
                break;

            case 3: /* yellow */
                min = level_color[diff].yellow;
                max = level_color[diff].orange - 1;
                break;

            case 4: /* orange */
                min = level_color[diff].orange;
                max = level_color[diff].red - 1;
                break;

            case 5: /* red */
                min = level_color[diff].red;
                max = level_color[diff].purple - 1;
                break;

            case 6: /* purple */
                min = level_color[diff].purple;
                max = min + 1;
                break;

            default:
                min = level;
                max = min;
        }

        /* The old value of monster->level is the minimum the new value can
         * be. */

        /* FIXME: Currently the level cap is 127. This is because for some
         * reason level is sint8 in object.h. Making it unsigned or more
         * bits would seem a more sensible move, but as level is used all
         * over the place for all sorts of things, I can't be bothered to
         * do all the testing such a change would need, hence the cap. */

        level = RANDOM_ROLL(MAX(level, MIN(min, MAXMOBLEVEL)),
                                     MAX(level, MIN(max, MAXMOBLEVEL)));
    }
    return level;
}
extern float getLevExp(int level);
void Autogen(object_t *monster, sint16 diff)
{
    monster->level = gen_level(monster, diff); // gen_level send valid level range back!
    // so we need no more checks here

    // adjust exp by level, this can not be done before monster spawn
    // because of the above rnd level adjustment
    // so this is best place, where this should be done

    // accessing the static LevExp[monster->level] from exp.c returns always 0
    // using this extern function is working, i love c
    monster->stats.exp*=getLevExp(monster->level);
    // LOG(llevDebug, "Autogen level %d exp %d\n", monster->level, monster->stats.exp);

    // adjust exp by server_exp_factor, this could be done before,
    // but fix_monster is ignoring the spawn_point_mob and instead take this mob or get it from arches
    // so currently it's easiest to adjust this also here.
    monster->stats.exp*=server_exp_factor;

    // The code in adjust_monster was directly included in this function
    // but some of it is useful for command_spawn(), so has been separated out
    adjust_monster(monster);

    if (monster->randomitems && monster->masks)
    {
        monster->randomitems = branch_treasurelist(monster->randomitems);
    }

    SETUP_MOB_DATA(monster);
}

void adjust_monster(object_t *monster)
{
    int i;

    if (monster->type != MONSTER)
        return;

    /* monster->item_condition modifies the mob's attack_ and resist_
     * attributes *that are non-zero in the arch and unchanged in the
     * obj*. */
    if (monster->item_condition > 0)
    {
        for (i = 0; i < NROFATTACKS; i++)
        {
            int arcattack = monster->arch->clone.attack[i],
                objattack = monster->attack[i],
                arcresist = monster->arch->clone.resist[i],
                objresist = monster->resist[i];

            if (arcattack != 0 && objattack == arcattack)
            {
                objattack += RANDOM_ROLL(0 - monster->item_condition,
                                         monster->item_condition);
                monster->attack[i] = MAX(0, MIN(objattack, 200));
            }

            /* resistances of 100 mean immunity. Don't mess with that. */
            if (arcresist != 0 && arcresist != 100 && objresist == arcresist)
            {
                objresist += RANDOM_ROLL(0 - monster->item_condition,
                                         monster->item_condition);
                monster->resist[i] = MAX(-100, MIN(objresist, 100));
            }
        }
    }
}

/* central spawn point function.
 * Control, generate or remove the generated object.
 */

// op->map // pointer to map
// msp = MSP_KNOWN(op); // "field data" map pointer,x,y
// op->enemy // spawn point has active spawn
// op_enemy = NULL // spawn point has no active spawn
// op->enemy_count // object count (each active object has a count
// OBJECT_VALID(op->enemy, op->enemy_count //// validate object with object count

// op->last_eat brightness definition of spawn mob

// op->stats.sp == -1 // overwrite spawn chance
// op->stats.sp = 0 //? default
// op->last_grace <= -1) // "closed" spawnpoint
// op->last_grace >=0 // higher last_grace higher chance for an "overwritten" spawn chance


// op->inv // spawn point inventar // empty inventar = no spawn

extern int exp_level_adjust(object_t *ob);
void spawn_point(object_t *op)
{
    msp_t        *msp;
    uint8         oflags;
    sint8         i;
    sint16        b;
    int           rmt,
                  tag;
    object_t     *this,
                 *mob,
                 *next;
    objectlink_t *mask;

    if (!op->map) // no map
    {
        return;
    }

    msp = MSP_KNOWN(op);  // get map,x,y

    if (op->enemy) // check if there is an active spawn
    {
        if (OBJECT_VALID(op->enemy, op->enemy_count)) // validate object with object count
        {
            if (op->last_eat) // we have a brightness definition?
            {
                // we need abs to target brightness array and need calc sign to get the final brightness value
                b = brightness[ABS(op->last_eat)] * ((op->last_eat < 0) ? -1 : 1);

                if (MSP_IS_APPROPRIATE_BRIGHTNESS(msp, b)) // check if spawn is in his brightness "zone"
                {
                    return;
                }

                // spawn is no longer in his defined brightness zone, remove it
                remove_ob(op->enemy);
                move_check_off(op->enemy, NULL, MOVE_FLAG_VANISHED);
            }
            else
                return; // no definition for brightness and an active spawn, then there is nothing to do
        }
        op->enemy = NULL; // spawn point has nothing spawned, because it has not passed the validate check or it's removed because of brightness
    }

    /* a set sp value will override the spawn chance.
     * with that "trick" we force for map loading the
     * default spawns of the map because sp is 0 as default.
     */
    /*LOG(llevNoLog,"SPAWN...(%d,%d)",op->x, op->y);*/
    if (op->stats.sp == -1) // overwrite spawn chance
    {
        int gg;
        if (op->last_grace <= -1) // no spawn here, this spawn is closed
        {
            /*LOG(llevNoLog," closed (-1)\n");*/
            return;
        }
        if (op->last_grace && (gg = (RANDOM() % (op->last_grace + 1))))
        // only 1 is true, so higher last_grace, lesser the chance is for a success roll
        // with last_grace 0, we have 50% chance (1=33%, 2=25%,3=20%) for NO spawn here!
        // so success for spawn increases with higher last_grace
        // (0=50%, 1 = 67%, 2=75%, 3=80%)
        {
            /*LOG(llevNoLog," chance: %d (%d)\n",gg,op->last_grace);*/
            return; //
        }

        op->stats.sp = (RANDOM() % SPAWN_RANDOM_RANGE); // rnd (0-10000)?
    }
    /*LOG(llevNoLog," hit!: %d\n",op->stats.sp);*/

    if (!op->inv) // spawn point without inventory!
    {
        LOG(llevMapbug, "MAPBUG:: Spawn point[%s %d %d] without inventory!\n",
            STRING_MAP_PATH(op->map), op->x, op->y);
        // kill this spawn point - its useless and need to fixed from the map maker/generator
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        return;
    }

    /* now we move through the spawn point inventory and
     * get the mob with a number under this value AND nearest.
     */
    rmt = 0;
    mob = NULL;

    FOREACH_OBJECT_IN_OBJECT(this, op, next)
    {
        if (this->type != SPAWN_POINT_MOB)
        {
            continue; // only spawn type SPAWN_POINT_MOB
        }
        // compare count with our above random roll (0-10000)
        // and compare it with a possible before found object
        // so mob must have a lower count, than our roll and a higher count than the one before.
        // this means the mob closest to the roll, but not above get the spawn
        if ((int)this->enemy_count <= op->stats.sp &&
            (int)this->enemy_count >= rmt)
        {
            if (this->last_eat != 0) // if we have a brightness definition ...
            {
                b = brightness[ABS(this->last_eat)] * SGN(this->last_eat);

                if (MSP_IS_APPROPRIATE_BRIGHTNESS(msp, b) == 0)
                {
                    continue;
                }
            }
            // candidate this mob for spawn, memory it's count
            rmt = (int)this->enemy_count;
            mob = this;
        }
    }

    /* we try only ONE time a respawn of a pre setting - so careful! */
    rmt = op->stats.sp; // memory the op->stats.sp
    op->stats.sp = -1; // overwrite it with a forced spawn for next try
    if (!mob) // NO spawn
        return;

    /* This determines in which msp the spawned mob is inserted.
     *
     * 0.10.5/6 allowed the mapper to modify the insertion point via spawn
     * point attributes. However, this was never sensibly or even effectively
     * used and so has been withdrawn.
     *
     * This will insert the mob at the first available msp in the 7x7 grid
     * centered on the spawn point and withing LOS of the spawn point (so a
     * point in a house won't spawn in the street, but beware of windows). */
    oflags = OVERLAY_RANDOM | OVERLAY_SPECIAL | OVERLAY_WITHIN_LOS | OVERLAY_FIRST_AVAILABLE;


    // where mob is targeting the spawn point mob before this line
    // mob is targeting the clone (the real mob) after this line
    // we also lost the information to our spawn point mob



    object_t *spawn_mob=mob;
    if (!(mob = clone_object(mob, MONSTER, MODE_INVENTORY)) ||
        (i = overlay_find_free_by_flags(msp, mob, oflags)) == -1)
    {
        return; /* that happens when we have no free spot....*/
    }

    // let try this mess
    // insert_base_info_object(object_t *op)

    /* Browse mob->inv for random drops. If they do randomly drop, move their
     * ->inv to mob. Either way, then remove the rd object. */
    /* TODO: The old code logged a mapbug if DIRECTLY nested rds are found.
     * Right call, wrong place. This should be logged (and possibly fixed)
     * during map load. */
    FOREACH_OBJECT_IN_OBJECT(this, mob, next)
    {
        if (this->type == TYPE_RANDOM_DROP)
        {
            object_t *rd = this;

            if (RANDOM_ROLL(0, rd->weight_limit) == 0)
            {
                FOREACH_OBJECT_IN_OBJECT(this, rd, next)
                {
                    remove_ob(this);
                    insert_ob_in_ob(this, mob);
                }

                next = rd->below;
            }

            remove_ob(rd);
        }
    }

    FOREACH_PART_OF_OBJECT(this, mob, next)
    {
        this->map = op->map;
        this->x = msp->x + this->arch->clone.x + OVERLAY_X(i);
        this->y = msp->y + this->arch->clone.y + OVERLAY_Y(i);
    }

    // at this point we have spawned a mob with his inventory
    // this means, we didn't know if the exp definitions in mob comes from map or arches
    // looks also like all the monster adjustments coming now ...
    // are better put in monster generation (map loader) instead in each spawn again and again?
    // WOW we can't do anything with op->stats.exp here or before in loader
    // WHEN we call fixmonster overwriting level and exp

    mask = mob->masks = op->masks;

    while (mask)
    {
        give_artifact_abilities(mob, mask->objlink.art);
        mask = mask->next;
    }

    // transfered "high" dynamic exp calc from calc_skill_exp to spawnpoint
    // momster level adjustment and serverexpfactor should't be done on kill
    // build exp here or perhaps better when loading arch definitions
    // here we have no chance to differenciate between a set map exp or a set arch exp definition

    // this check should be really earlier, also before the above mask
    // if(op->stats.exp<=0) op->stats-exp=50; // serverdefaultmobexp todo

    // this don't work here because of fixmonster
    // op->stats.exp=server_exp_factor*exp_level_adjust(op);

    // we try this in map loader LoadObjects in SPAWN_POINT logic there
    Autogen(mob, op->map->difficulty);

    /* normal spawning may be interrupted by a script, allowing you to do
     * clever things to the mob. This is an apply event because trigger is
     * already used in the case of connected spawn points. */
    /* Note that beacons must be unique, so will not be registered in
     * script-interrupted spawns, and are in fact removed (because such spawns
     * can be infinite). */
    tag = mob->count;
    if (plugin_trigger_object_event(PLUGIN_EVENT_OBJECT_APPLY, op, mob, NULL, NULL, 0, 0, 0))
    {
        if (OBJECT_VALID(mob, tag))
        {
            mob->last_eat = 0;
            make_mob_homeless(mob);
        }

        return;
    }

    /* setup special monster -> spawn point values */
    op->last_eat = 0;
    if (mob->last_eat) /* darkness controled spawns */
    {
        op->last_eat = mob->last_eat;
        mob->last_eat = 0;
    }

    op->last_sp = rmt; /* this is the last rand() for what we have spawned! */

    op->enemy = mob; /* chain the mob to our spawn point */
    op->enemy_count = mob->count;

    /* perhaps we have later more unique mob setting - then we can store it here too.
     */
    this = arch_to_object(op->other_arch); /* create spawn info */
    this->owner = op; /* chain spawn point to our mob */
    this->owner_count = op->count;
    insert_ob_in_ob(this, mob);      /* and put it in the mob */

    SET_MULTI_FLAG(mob, FLAG_SPAWN_MOB); /* FINISH: now mark our mob as a spawn */
    fix_monster(mob); /* fix all the values and add in possible abilities or forces ... */

    /* *now* all is done - *now* put it on map */
    /* I am not 100% sure this change is best, but it seems that sometimes a
     * mob spawns on the same square as a spell is travelling over which
     * results in the immediate destruction on the mob (for example, it happens
     * often in planes/demon_plane/stoneglow/guild_wiz/stoneglow_wiz_novicet
     * where the 'mobs' (Training Targets) are weak and spawn frequently).
     * -- Smacky 20101024 */
#if 0
    if (!insert_ob_in_map(mob, mob->map, op, 0))
        LOG(llevBug, "BUG:: %s/spawn_point(): Could not insert mob (%s[%d]) in map!\n",
            __FILE__, STRING_OBJ_NAME(op), TAG(op));

    /* initialise any beacons in the newly spawned mob's inv */
    next = mob;
    while (next)
    {
        next = find_next_object(next, TYPE_BEACON, FNO_MODE_ALL, mob);
        if (next)
            object_initializers[TYPE_BEACON](next);
    }
#else
    /* If the above comment is correct, we hardly need to spam the log with
     * this message, and even if we do it is INFO not BUG and concerns mob, not
     * op.
     * -- Smacky 20101024 */
#if 0
    if (!insert_ob_in_map(mob, mob->map, op, 0))
    {
        LOG(llevInfo, "INFO:: %s/spawn_point(): Mob (%s[%d] map '%s') was destroyed during spawn!\n",
            __FILE__, STRING_OBJ_NAME(mob), TAG(mob), mob->map->path);
    }
    else
#else
    if (insert_ob_in_map(mob, op->map, op, 0))
#endif
    {
        /* initialise any beacons in the newly spawned mob's inv */
        next = mob;
        while (next)
        {
            next = find_next_object(next, TYPE_BEACON, FNO_MODE_ALL, mob);
            if (next)
                object_initializers[TYPE_BEACON](next);
        }
    }
#endif
}

/* make_mob_homeless() turns mob (which is basically already spawned) into
 * a 'homeless mob' (ie, not chained to a spawn point).
 *
 * Once a mob has been made homeless, his old spawn point is free to spawn
 * another. This will lead to a situation with multiple instances of the 'same'
 * mob -- not an issue normally, but there are two considerations:
 *   1 TODO: this is perhaps undesirable with eg, boss mobs, quest
 *     givers/targets, etc so we should have a boolean arch attribute. If 1,
 *     don't allow this decoupling (return 1 and the caller can take
 *     appropriate action);
 *   2 In theory rare situations, abusive player actions and scripts, etc could
 *     lead to there being many tens or even hundreds of homeless mobs running
 *     about eating up processing time. While this is unlikely to be a problem
 *     in practice, we take a precaution by adding an IS_USED_UP fuse to
 *     homeless mobs (which is kind of nice anyway). */
void make_mob_homeless(object_t *mob)
{
    RemoveStuff(mob);
    CLEAR_MULTI_FLAG(mob, FLAG_SPAWN_MOB);
    SET_MULTI_FLAG(mob, FLAG_HOMELESS_MOB);
    SET_FLAG(mob, FLAG_IS_USED_UP);

    /* If the mob already has a fuse, use that, else give it one. */
    if (mob->stats.food <= 0)
    {
        /* This is kind of an arbitrary value that seems reasonable. Note that
         * the actual length of time depends on the mob's speed and whether he
         * is idle or moving/fighting. Generally this is approx seconds with
         * moving/fighting mobs having about half the life expectancy of idle
         * ones. */
        mob->stats.food = 300;
    }

    fix_monster(mob);
}

/* RemoveStuff() is a recursive function to remove all 'dangerous' objects from
 * where. */
static void RemoveStuff(object_t *where)
{
    object_t *this,
           *next;

    FOREACH_OBJECT_IN_OBJECT(this, where, next)
    {
        if (this->type == TYPE_BEACON)
        {
            remove_ob(this);
        }
        else if (this->type == SPAWN_POINT_INFO)
        {
            object_t *owner = get_owner(this);

            if (owner)
            {
                owner->enemy = NULL;
                owner->enemy_count = 0;
            }

            remove_ob(this);
        }
        else if (this->inv)
        {
            RemoveStuff(this);
        }
    }
}

/*
 * Linked Spawns (Mobs) Code.
 * We implement "linked mobs" with the spawn points - so its "linked spawns"
 * Linked spawns is nothing more as a 2 or more linked spawn points who are
 * triggered by a signal at once.
 * An Example: 3 linked spawn points are around a campfire. On a hill is another
 * one linked to it - so its 4 linked spawns. If now the scout on the hill get
 * attacked & find a target, the signal goes to all linked mobs. So, the 3
 * mobs on the campfire joins in the fight.
 * Important: Linked spawns are a  pre AI mechanism and has nothing to do with "social mobs"
 * which are part of the AI. Linked mobs are used to trigger a situation like
 * "as you attack all mobs from left and 2 behind the trees graps their weapon and start
 * fighting" - after we create with the linked mobs system this situation we give it to
 * the AI as usual.
 *
 * Beside that we can use the linked spawns for more tricky technical setup, related to quests:
 * Instead of trigger a linked attack, we check with the linked list for special events.
 * Like: if all linked spawns point mobs are killed, call a script. Spawn a master mob or
 * do some other special event. This is a often used quest setup. We can chain in this way
 * (by using a script) different maps like if all mobs of linked group A are killed, we open
 * the castle doors and trigger linked group B or call the castle guards by triggering the AI.
 */

/* This is the basic implementation. MT-07.2005 */

static objectlink_t *get_linked_spawn(object_t *spawn)
{
    objectlink_t        *ol;

    for(ol = spawn->map->linked_spawn_list;ol;ol = ol->next)
    {
        /* slaying string is the linked spawn ID */
        if(ol->objlink.ob->slaying == spawn->slaying)
            return ol;

    }

    return NULL;
}

/* LIFO queue for the base links - spawns point of every list
 * are in a LIFO queue too
 * called when a map with linked spawn is loaded
 */
objectlink_t *add_linked_spawn(object_t *spawn)
{
    objectlink_t        *ol;

    ol = get_linked_spawn(spawn);
    if(!ol) /* new one? create base link */
    {
        ol = objectlink_get(OBJLNK_FLAG_OB);
        ol->next = spawn->map->linked_spawn_list;
        spawn->map->linked_spawn_list = ol; /* add base link it to the map */
    }
    ol->value++;
    spawn->attacked_by = ol->objlink.ob; /* add this spawn point the the base link */
    ol->objlink.ob = spawn;

#ifdef DEBUG_LINK_SPAWN
    LOG( llevDebug, "LINK_SPAWN::add: map: %s (%d,%d) - %d\n",
         STRING_SAFE(spawn->map->path), spawn->x, spawn->y, ol->value);
#endif
    return ol;
}

/* remove the link spawn base link list.
 * called when a map struct is removed.
 */
void remove_linked_spawn_list(map_t *map)
{
    objectlink_t *ol, *tmp;

    if(!(ol = map->linked_spawn_list))
        return;

#ifdef DEBUG_LINK_SPAWN
    LOG( llevDebug, "LINK_SPAWN::remove linked spawns for map %s\n", STRING_SAFE(map->path ));
#endif

    for(;ol;ol = tmp)
    {
        tmp = ol->next;
        return_poolchunk(ol, pool_objectlink);
    }

    map->linked_spawn_list = NULL;
}

/* send a signal through the linked mobs
*/
void send_link_spawn_signal(object_t *spawn, object_t *target, int signal)
{
    objectlink_t        *ol = get_linked_spawn(spawn);

    if(!ol) /* sanity check */
    {
        LOG( llevDebug, "BUG LINK_SPAWN::send_link_spawn_signal(): (map: %s (%d,%d))- LinkID: %s\n",
            spawn->map?STRING_SAFE(spawn->map->path):"NULL", spawn->x, spawn->y, STRING_SAFE(spawn->slaying));
        return;
    }

    /* Assign an enemy to all linked spawns */
    if(signal & LINK_SPAWN_ENEMY)
    {
        object_t *obj;

        for(obj=ol->objlink.ob; obj; obj = obj->attacked_by)
        {
            if(obj==spawn) /* we don't need to set here the caller */
                continue;

            /* obj = spawn point - obj->enemy = possible spawn */
            if(obj->enemy && obj->enemy->enemy != target)
            {
                struct mob_known_obj *enemy;

                /* we can now do here wonderful things like:
                 * - random assign from a linked spawn pool mobs who attack
                 * - check target is in group and decide then how much from the
                 *   linked pool we throw in the fight
                 * and much more...
                 */
                enemy = update_npc_knowledge(obj->enemy, target, NULL, FRIENDSHIP_ATTACK, 0);

                /* in this mode, we don't force the register */
                if (enemy && !obj->enemy->enemy)
                {
                    /* we need a "set_enemy" in the ai code beside "choose_enemy" */
                    obj->enemy->enemy = enemy->obj;
                    MOB_DATA(obj->enemy)->enemy = enemy;
                    obj->enemy->enemy_count = enemy->obj_count;
                    MOB_DATA(obj->enemy)->idle_time = 0;
                    set_mobile_speed(obj->enemy, 0);


#ifdef DEBUG_LINK_SPAWN
                    LOG( llevDebug, "LINK_SPAWN::target enemy: map %s (%d,%d)\n",
                         STRING_SAFE(obj->map->path), obj->x, obj->y);
#endif
                }
            }
        }
    }
}
