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

/* This is a WIP. This file will be renamed thing.c and here will be
 * essentially entry point functions for the basic but high level manipulation
 * of things. TODO: MMore later. */

#include "global.h"

static object_t *ThingDeclare(object_t *ownr, object_t *what, sint32 level, uint32 mask, objectlink_t *list);
static object_t *ThingCanReach(object_t *who, object_t *what);
static object_t *ThingCanPickUp(object_t *who, object_t *what, object_t *where, uint32 nrof);
static object_t *ThingPickUp(object_t *who, object_t *what, object_t *where, uint32 nrof, object_t *from, object_t *to);
static object_t *ThingCanDiscard(object_t *who, object_t *what);
static object_t *ThingNoDiscardContainer(object_t *who, object_t *what);
static void      ThingFixAsNeeded(object_t *who, object_t *to, object_t *from, object_t *what);

/* describe terrain flags
 * we use strcat only - prepare the retbuf before call.
 */
static void describe_terrain(const object_t *const op, char *const retbuf)
{
    if (op->terrain_flag & TERRAIN_AIRBREATH)
        strcat(retbuf, "(air breathing)");
    if (op->terrain_flag & TERRAIN_WATERWALK)
        strcat(retbuf, "(water walking)");
    if (op->terrain_flag & TERRAIN_FIREWALK)
        strcat(retbuf, "(fire walking)");
    if (op->terrain_flag & TERRAIN_CLOUDWALK)
        strcat(retbuf, "(cloud walking)");
    if (op->terrain_flag & TERRAIN_WATERBREATH)
        strcat(retbuf, "(water breathing)");
    if (op->terrain_flag & TERRAIN_FIREBREATH)
        strcat(retbuf, "(fire breathing)");
}

static object_t *ThingDeclare(object_t *ownr, object_t *what, sint32 level, uint32 mask, objectlink_t *list)
{
    sint32 ok = 0;

    if (QUERY_FLAG(what, FLAG_UNIDENTIFIABLE))
    {
        return NULL;
    }

    if (level < what->level)
    {
        if (ownr->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, ownr, "%s %s too high level!",
                query_name(what, ownr, ARTICLE_INDEFINITE, 0),
                (what->nrof > 1) ? "are" : "is");
        }

        return NULL;
    }

    /* what is declared according to mask. This is a bit verbose because
     * REMOVE/BECOME/DETECT are inclusive, in that order.
     *
     * Also, we avoid false positive/redundant declares by querying relevant
     * flags and setting ok, then returning whatt/NULL accordingly. */
    if ((mask & THING_DECLARE_BIT_REMOVE))
    {
        if ((mask & (THING_DECLARE_BIT_MAGICAL | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (QUERY_FLAG(what, FLAG_IS_MAGICAL))
            {
                ok = 1;
                CLEAR_FLAG(what, FLAG_IS_MAGICAL);
                CLEAR_FLAG(what, FLAG_KNOWN_MAGICAL);
            }
        }

        if ((mask & (THING_DECLARE_BIT_DAMNED | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (QUERY_FLAG(what, FLAG_CURSED) ||
                QUERY_FLAG(what, FLAG_DAMNED))
            {
                ok = 1;
                CLEAR_FLAG(what, FLAG_CURSED);
                CLEAR_FLAG(what, FLAG_DAMNED);
                CLEAR_FLAG(what, FLAG_KNOWN_CURSED);
            }
        }

        if ((mask & THING_DECLARE_BIT_IDENTIFIED))
	{
            if (QUERY_FLAG(what, FLAG_IDENTIFIED))
            {
                ok = 1;
                CLEAR_FLAG(what, FLAG_IDENTIFIED);
            }
        }
    }

    if ((mask & THING_DECLARE_BIT_BECOME))
    {
        if ((mask & (THING_DECLARE_BIT_MAGICAL | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (!QUERY_FLAG(what, FLAG_IS_MAGICAL))
            {
                ok = 1;
                SET_FLAG(what, FLAG_IS_MAGICAL);
            }
        }

        if ((mask & (THING_DECLARE_BIT_DAMNED | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (!QUERY_FLAG(what, FLAG_CURSED) &&
                !QUERY_FLAG(what, FLAG_DAMNED))
            {
                ok = 1;
                SET_FLAG(what, FLAG_CURSED);
                SET_FLAG(what, FLAG_DAMNED);
            }
        }

        if ((mask & THING_DECLARE_BIT_IDENTIFIED))
	{
            if (!QUERY_FLAG(what, FLAG_IDENTIFIED))
            {
                ok = 1;
                SET_FLAG(what, FLAG_IDENTIFIED);
            }
        }
    }

    if ((mask & THING_DECLARE_BIT_DETECT))
    {
        if ((mask & (THING_DECLARE_BIT_MAGICAL | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (QUERY_FLAG(what, FLAG_IS_MAGICAL))
            {
                ok = 1;
                SET_FLAG(what, FLAG_KNOWN_MAGICAL);
            }
        }

        if ((mask & (THING_DECLARE_BIT_DAMNED | THING_DECLARE_BIT_IDENTIFIED)))
        {
            if (QUERY_FLAG(what, FLAG_CURSED) ||
                QUERY_FLAG(what, FLAG_DAMNED))
            {
                ok = 1;
                SET_FLAG(what, FLAG_KNOWN_CURSED);
            }
        }

        if ((mask & THING_DECLARE_BIT_IDENTIFIED))
	{
            if (!QUERY_FLAG(what, FLAG_IDENTIFIED))
            {
                ok = 1;
                SET_FLAG(what, FLAG_IDENTIFIED);
            }
        }
    }

    if (ok != 0)
    {
        objectlink_t *link = objectlink_get(OBJLNK_FLAG_OB);

        link->objlink.ob = what;
        (void)objectlink_link(&list, NULL, NULL, list, link); // FIFO
        OBJECT_UPDATE_UPD(what, UPD_FLAGS);
        return what;
    }

    return NULL;
}

/* ThingCanReach() ascertains if what is close enough that who can touch it. That
 * means what must be either (in a container) in who's inventory or (in a
 * container) on the same map square as who -- see below for some exceptions to
 * this basic rule.
 *
 * The return is NULL is what is not in reach of who or, if it is, an object
 * which is one of: what if what is directly on a map; a player or monster
 * (which may be who) if what is (in a container) in that creature's inv; a
 * container which is directly on a map.
 *
 * If who is a player an ndi is sent to the player when what is out of
 * reach. */
static object_t *ThingCanReach(object_t *who, object_t *what)
{
    object_t *e;

    /* A what that has yet to be inserted anywhere is always reachable. */
    if (!QUERY_FLAG(what, FLAG_INSERTED))
    {
        return what;
    }

    /* Set e to the outermost environment of what. Afterwards e may == what or
     * some other non-NULL value, e->env == NULL, and e->map may == NULL or
     * (more probably) non-NULL. */
    for (e = what; e->env; e = e->env);

    /* If either who or e are airborne and the other is not (and what is not
     * in who's inventory), who can't reach what. */
    /* TODO: This makes no distinction between flying/levitating. */
    if (e != who &&
        IS_AIRBORNE(who) != IS_AIRBORNE(e))
    {
        if (who->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, who, "%s is floating out of your reach!",
                query_name(what, who, ARTICLE_DEFINITE, 0));
        }

        return NULL;
    }

    /* SAs can reach things in other creature's invs (including in containers
     * in those invs). All others can only fiddle with themselves, unless e
     * is actively giving. */
    if (e->type == PLAYER ||
        (e != what &&
         e->type == MONSTER))
    {
        if (e != who &&
            !QUERY_FLAG(e, FLAG_IS_GIVING) &&
            !(GET_GMASTER_MODE(who) & GMASTER_MODE_SA))
        {
            e = NULL;
        }
    }
    /* SAs can reach things in any container (open or closed). For convenience
     * so can monsters. Other players are limited to their linked container. */
    else if (e != what &&
             e->type == CONTAINER)
    {
        if (who->type == PLAYER &&
            !(CONTR(who)->gmaster_mode & GMASTER_MODE_SA) &&
            e != CONTR(who)->container)
        {
            e = NULL;
        }
    }

    /* If e does not point to who and is not giving, it must be directly on
     * a map so check that it occupies the same space as who. */
    if (e &&
        e != who &&
        !QUERY_FLAG(e, FLAG_IS_GIVING))
    {
        if (e->map != who->map ||
            e->x != who->x ||
            e->y != who->y)
        {
            e = NULL;
        }
    }

    if (!e &&
        who->type == PLAYER)
    {
        ndi(NDI_UNIQUE, 0, who, "%s is out of your reach!",
            query_name(what, who, ARTICLE_DEFINITE, 0));
    }

    return e;
}

/* ThingCanPickUp() determines if who can pick up (if appropriate, nrof *) what
 * (into where if not NULL)>
 *
 * The return is what if who can pick it up, or NULL if who can't.
 *
 * If who is a player an ndi is sent to the player when what can't be picked
 * up. */
static object_t *ThingCanPickUp(object_t *who, object_t *what, object_t *where, uint32 nrof)
{
    sint32 tmp_nrof;
    sint32 weight;

    /* Picking up players would cause mayhem so it's not allowed. */
    if (what->type == PLAYER)
    {
        if (who->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, who, "%s really would not like being picked up!",
                QUERY_SHORT_NAME(what, who));
        }

        return NULL;
    }

    /* Multiparts are absolute no-nos because the server cannot cope with such
     * objects having an environment. Also it would be logically ridiculous to
     * have such physically large objects being pocketed. */
    /* TODO: This restriction will be lifted fo gmaster wiz players. */
    if (what->more ||
        what->head)
    {
        if (who->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, who, "%s %s too large for you to pick up!",
                query_name(what, who, ARTICLE_DEFINITE, 0),
                (what->nrof > 1) ? "are" : "is");
        }

        return NULL;
    }

    /* Normal players and mobs cannot pick up these items but MW/MM/SAs can. */
#ifdef DAI_DEVELOPMENT_CONTENT
    if (!(GET_GMASTER_MODE(who) & (GMASTER_MODE_MW | GMASTER_MODE_MM | GMASTER_MODE_SA)))
#else
    if (!(GET_GMASTER_MODE(who) & (GMASTER_MODE_MM | GMASTER_MODE_SA)))
#endif
    {
        object_t *from;

        /* If what is somebody else's ego item then who can't pick it up. */
        if (egoitem_check(who, what, 1) != 0)
        {
            return NULL;
        }

        /* No_picks can't be picked up. */
        if (QUERY_FLAG(what, FLAG_NO_PICK) ||
        /* If you can't see it, you can't pick it up. */
            IS_NORMAL_INVIS_TO(what, who))
        {
            if (who->type == PLAYER)
            {
                ndi(NDI_UNIQUE, 0, who, "%s %s not something you can pick up!",
                    query_name(what, who, ARTICLE_DEFINITE, 0),
                    (what->nrof > 1) ? "are" : "is");
            }

            return NULL;
        }

        for (from = what; from->env; from = from->env)
        {
            ;
        }

        /* When what is not already in (a container in) who's inv, check who
         * can lift it. */
        if (from != who)
        {
            sint32 who_limit = (who->type == PLAYER) ?
                (sint32)CONTR(who)->weight_limit :
                ((who->weight_limit > 0) ? who->weight_limit : 20000);

            // Hack to allow WEIGHT_OVERALL to not account for full stack.
            // TODO: Change WEIGHT_OVERALL
            tmp_nrof = what->nrof;
            what->nrof = nrof;
            weight = (sint32)(WEIGHT_OVERALL(what));
            what->nrof = tmp_nrof;

            if (who_limit < who->carrying + weight)
            {
                if (who->type == PLAYER)
                {
                    ndi(NDI_UNIQUE, 0, who, "%s %s too heavy to pick up!",
                        query_name(what, who, ARTICLE_DEFINITE, 0),
                        (what->nrof > 1) ? "are" : "is");
                }

                return NULL;
            }
        }
    }

    /* If what is applied and can't be unapplied (eg, it's cursed) then it
     * can't be picked up. */
    if (QUERY_FLAG(what, FLAG_APPLIED) &&
        apply_equipment(who, what, AP_UNAPPLY))
    {
        return NULL;
    }

    return what;
}

/* ThingPickUp() is the business end of picking up an object -- see thing_pick_up(). */
static object_t *ThingPickUp(object_t *who, object_t *what, object_t *where, uint32 nrof, object_t *from, object_t *to)
{
    player_t   *pl = (who->type == PLAYER) ? CONTR(who) : NULL;
    msp_t *msp;
    sint32 weight, tmp_nrof;

    /* For a pick up to inv (ie, where is NULL), who might have an appropriate
     * readied container in his inventory, so check that. */
    if (!where &&
        from != who)
    {
        object_t *this,
               *next;
        sint32  extra_weight = WEIGHT_OVERALL(what);

        FOREACH_OBJECT_IN_OBJECT(this, who, next)
        {
            /* The container must be applied (ready/open). */
            if (this->type == CONTAINER &&
                QUERY_FLAG(this, FLAG_APPLIED))
            {
                /* It must have enough space for what. */
                if (this->weight_limit >= this->carrying + extra_weight)
                {
                    /* Some containers can only hold specific classes of item
                     * (eg, pouches for valuables) while others can hold any.
                     * If we find a specific container that matches the item
                     * class being picked up, use that. Otherwise, use the
                     * first generic one found. */
                    /* TODO: Improve and expand these specific containers. */
                    if (!where &&
                        !this->race)
                    {
                        where = this;
                    }
                    else if (this->race == what->race)
                    {
                        where = this;
                        break;
                    }
                }
            }
        }
    }

    /* Check that who actually can pick up nrof * what. There are several
     * reasons why what can't be picked up -- see ThingCanPickUp(). */
    if (!ThingCanPickUp(who, what, where, nrof))
    {
        return NULL;
    }

    /* At this point we've determined that its theoretically OK for who to pick
     * up nrof * what. Now we need to determine the type of pick up (to
     * inventory or to container), either of which involves further side
     * effects/checks. */
    /* If where is (still) NULL (see above) we're trying to pick up what to
     * who's inv. */
    if (!where)
    {
        /* If what is a container be sure to unlink (close) it for all players
         * viewing it. */
        if (what->type == CONTAINER)
        {
            container_unlink(NULL, what);
        }
    }
    /* When where is non-NULL we're trying to put what in it. */
    else
    {
        tmp_nrof = what->nrof;
        what->nrof = nrof;
        weight = (sint32)(WEIGHT_OVERALL(what));
        what->nrof = tmp_nrof;

        /* Apart from corpses, certain types of object can not be put in
         * containers. */
        if ((where->subtype & 1) != SUBTYPE_CONTAINER_CORPSE)
        {
            /* Containers can't be put in other containers. As per #00099 from
             * Shroud, corpses are an exception to this. */
            if (what->type == CONTAINER)
            {
                if (pl)
                {
                    ndi(NDI_UNIQUE, 0, who, "Containers can't be put in other containers!");
                }

                return NULL;
            }
            /* where may be a container which cannot hold what. */
            else if (where->race &&
                     where->race != what->race)
            {
                if (pl)
                {
                    ndi(NDI_UNIQUE, 0, who, "Only ~%s~ can be put into %s!",
                        where->race, query_name(where, who, ARTICLE_DEFINITE, 0));
                }

                return NULL;
            }
        }

        /* Check that where has enough space for what. */
        /* TODO: If nrof > 1 perhaps split what to fit? */
        else if (where->weight_limit < where->carrying + weight)
        {
            if (pl)
            {
                ndi(NDI_UNIQUE, 0, who, "%s %s too heavy to fit in %s!",
                   query_name(what, who, ARTICLE_DEFINITE, 0),
                   (nrof > 1) ? "are" : "is",
                   query_name(where, who, ARTICLE_DEFINITE, 0));
            }

            return NULL;
        }

        /* If an applyable light is lit (has glow_radius) force it to be
         * FLAG_APPLIED so we can unapply it below. This prevents players storing
         * lit torches in bags. */
        if (what->type == TYPE_LIGHT_APPLY &&
            what->glow_radius)
        {
            SET_FLAG(what, FLAG_APPLIED);
            apply_light(who, what);
        }

        /* Picking up to a container not in who's inventory is similar to
         * dropping in some respects so do the necessary checks -- see
         * ThingCanDiscard(). */
        if (to != who &&
            !ThingCanDiscard(who, what))
        {
            return NULL;
        }
    }

    /* Now we've determined the pick up is possible, so lets do it! */
    /* When a monster is picked up, it becomes a homeless mob. */
    if (what->type == MONSTER &&
        !QUERY_FLAG(what, FLAG_HOMELESS_MOB))
    {
        make_mob_homeless(what);
    }

    /* what can have a PICKUP script on it which, if it returns true, aborts
     * the actual pick up. */
    if (trigger_object_plugin_event(EVENT_PICKUP, what, who, where, NULL,
            (int *)&nrof, NULL, NULL, SCRIPT_FIX_ALL, NULL))
    {
        return NULL;
    }

    /* where can also have a PICKUP script on it which, if it returns true,
     * aborts the actual pick up. */
    if (where &&
        trigger_object_plugin_event(EVENT_PICKUP, where, who, what, NULL,
            (int *)&nrof, NULL, NULL, SCRIPT_FIX_ALL, NULL))
    {
        return NULL;
    }

    /* Picking things up from or to outside of who's inventory interrupts
     * resting. */
    if ((from != who ||
         to != who) &&
        pl)
    {
        pl->rest_mode = 0;
    }

    /* Below here is the actual code to move what to a new environment (either
     * where or who). Also we send some appropriate messages to the client. */
    /* Picking up unpaid items from a shop floor is preliminary to buying
     * them. */
    if ((msp = MSP_KNOWN(who)) &&
        what->map &&
        QUERY_FLAG(what, FLAG_UNPAID) &&
        !QUERY_FLAG(what, FLAG_SYS_OBJECT) &&
        what->layer)
    {
        what = clone_object(what, 0, MODE_NO_INVENTORY);
        what->nrof = nrof;

        if (pl)
        {
            object_t *shop;

            MSP_GET_SYS_OBJ(msp, SHOP_FLOOR, shop);
            ndi(NDI_UNIQUE, 0, who, "You can buy %s for ~%s~ from %s.\n",
                query_name(what, who, ARTICLE_DEFINITE, 0),
                query_cost_string(what, who, F_BUY, COSTSTRING_SHORT),
                query_name(shop, who, ARTICLE_DEFINITE, 0));
        }
    }
    else
    {
        if (nrof < what->nrof)
        {
            if (pl)
            {
                SET_FLAG(who, FLAG_NO_FIX_PLAYER);
            }

            what = get_split_ob(what, nrof);
        }
        else if (!QUERY_FLAG(what, FLAG_REMOVED))
        {
            if (what->map)
            {
                remove_ob(what);

                if (move_check_off(what, NULL, MOVE_FLAG_VANISHED) > MOVE_RETURN_SUCCESS)
                {
                    return NULL;
                }
            }
            else
            {
                if (pl)
                {
                    SET_FLAG(who, FLAG_NO_FIX_PLAYER);
                }

                remove_ob(what);
            }
        }
    }

    /* As long as what is not LOOT, insert it into its new environment. */
    if (what->type != LOOT)
    {
        what = insert_ob_in_ob(what, (where) ? where : who);
    }

    return what;
}

/* ThingCanDiscard() determines if who can discard what.
 *
 * The return is what if who can discard it, or NULL if who can't.
 *
 * If who is a player an ndi is sent to the player when what can't be
 * discarded. */
static object_t *ThingCanDiscard(object_t *who, object_t *what)
{
    /* Players have a few specific rules. */
    if (who->type == PLAYER)
    {
        /* Locked items can't be dropped. */
        if (QUERY_FLAG(what, FLAG_INV_LOCKED))
        {
            ndi(NDI_UNIQUE, 0, who, "%s %s locked!",
                query_name(what, who, ARTICLE_DEFINITE, 0),
                (what->nrof > 1) ? "are" : "is");
            return NULL;
        }

        /* Under some circumstances a player cannot discard a container with
         * something in it. */
        if (what->type == CONTAINER)
        {
            /* Always, certain contents prevent dropping. */
            if (ThingNoDiscardContainer(who, what))
            {
                return NULL;
            }
        }
    }
    /* Monsters can't drop no drops. */
    else if (QUERY_FLAG(what, FLAG_NO_DROP))
    {
        return NULL;
    }

    /* If what is applied and can't be unapplied (eg, it's cursed) then it
     * can't be discarded. */
    if (QUERY_FLAG(what, FLAG_APPLIED))
    {
        if (apply_equipment(who, what, AP_UNAPPLY))
        {
            return NULL;
        }

        if (!QUERY_FLAG(what, FLAG_SYS_OBJECT)) // else called later
        {
            FIX_PLAYER(who, "ThingCanDiscard");
        }
    }

    return what;
}

/* ThingNoDiscardContainer() recursively checks containers for no drops and locked
 * items inside, returning a pointer to the first such object found (container
 * cannot be discarded) or NULL if none are found (container can be
 * discarded).
 *
 * An ndi is sent to who (this function is only called for players) when the
 * container cannot be discarded. */
static object_t *ThingNoDiscardContainer(object_t *who, object_t *what)
{
    object_t *this,
           *next;

    FOREACH_OBJECT_IN_OBJECT(this, what, next)
    {
        if (QUERY_FLAG(this, FLAG_SYS_OBJECT))
        {
            continue;
        }

        if (this->type == CONTAINER &&
            this->inv)
        {
            object_t *that = ThingNoDiscardContainer(who, this);

            if (that)
            {
                return that;
            }
        }

        if (QUERY_FLAG(this, FLAG_NO_DROP))
        {
            ndi(NDI_UNIQUE, 0, who, "First remove all ~NO-DROP~ items from %s!",
               query_name(what, who, ARTICLE_DEFINITE, 0));
            return this;
        }
        else if (QUERY_FLAG(this, FLAG_INV_LOCKED))
        {
            ndi(NDI_UNIQUE, 0, who, "First remove all locked items from %s!",
               query_name(what, who, ARTICLE_DEFINITE, 0));
            return this;
        }
    }

    return NULL;
}
/* ThingFixAsNeeded() carries out any necessary fixing post-pick up/drop. */
/* TODO: While I think this works well enough, ALL fixing needs
 * reworking/making more efficient. But that is a separate job. */
/* TODO: Currently monster encumbrance probably isn't handled. */
static void ThingFixAsNeeded(object_t *who, object_t *to, object_t *from, object_t *what)
{
    object_t *needfixing[4];
    uint8   i;
    int     is_sys_object = QUERY_FLAG(what, FLAG_SYS_OBJECT);

    if (from == who ||
        from == to)
    {
        from = NULL;
    }

    if (to == who)
    {
        to = NULL;
    }

    needfixing[0] = who;
    needfixing[1] = to;
    needfixing[2] = from;
    needfixing[3] = NULL;

    for (i = 0; i <= 3; i++)
    {
        object_t *this = needfixing[i];

        if (!this)
        {
            return;
        }

        if (this->type == PLAYER)
        {
            CLEAR_FLAG(this, FLAG_NO_FIX_PLAYER);
        }

        if (is_sys_object)
        {
            FIX_PLAYER(this, "pick up/drop"); // also fixes monsters
        }
        else if (this->type == PLAYER)
        {
            fix_player_weight(this);
        }
    }
}

/* describe_resistance generates the visible naming for resistances.
 * returns a static array of the description.  This can return
 * a big buffer.
 * if newline is true, we don't put parens around the description
 * but do put a newline at the end.  Useful when dumping to files
 */
char * describe_resistance(const object_t *const op, int newline)
{
    static char buf[LARGE_BUF];
    char        buf1[LARGE_BUF];
    int         tmpvar, flag = 1;

    buf[0] = 0;

    for (tmpvar = 0; tmpvar < NROFATTACKS; tmpvar++)
    {
        if (op->resist[tmpvar])
        {
            if (flag)
            {
                if (!newline)
                    strcat(buf, "(resists: ");
            }
            else
            {
                if (!newline)
                    strcat(buf, ", ");
            }

            sprintf(buf1, "%s%+d%%%c",
                attack_name[tmpvar].abbr, op->resist[tmpvar],
                (newline) ? '\n' :'\0');

            flag = 0;
            strcat(buf, buf1);
        }
    }
    if (!newline && !flag)
        strcat(buf, ") ");
    return buf;
}

/* describe_attacks generates the visible naming for attack forms.
 * returns a static array of the description.  This can return
 * a big buffer.
 * if newline is true, we don't put parens around the description
 * but do put a newline at the end.  Useful when dumping to files
 */
char * describe_attack(const object_t *const op, int newline)
{
    static char buf[LARGE_BUF];
    char        buf1[LARGE_BUF];
    int         tmpvar, flag = 1;

    buf[0] = 0;

    for (tmpvar = 0; tmpvar < NROFATTACKS; tmpvar++)
    {
        if (op->attack[tmpvar])
        {
            if (flag)
            {
                if (!newline)
                    strcat(buf, "(attacks: ");
            }
            else
            {
                if (!newline)
                    strcat(buf, ", ");
            }

            sprintf(buf1, "%s%+d%%%c",
                attack_name[tmpvar].abbr, op->attack[tmpvar],
                (newline) ? '\n' :'\0');

            flag = 0;
            strcat(buf, buf1);
        }
    }
    if (!newline && !flag)
        strcat(buf, ") ");
    return buf;
}

/* ITEMDESCRIBE() prints an appropriate parenthetical description of an
 * item's bonus/malus to BUF according to the other inputs. The output is the
 * return of sprintf, that is an int where negstive means error. More
 * importantly BUF now points to the parenthetical description.
 *
 * This macro is written to be compatible with current code and to be easily
 * adapted and expanded to the better code I will release somewhen. */
#define ITEMDESCRIBE(_BUF_, _ATT_, _HIV_, _HID_, _LOV_, _LOD_, _SGN_, _DES_) \
    (((_HIV_) > (_LOV_) && \
      (_ATT_) >= (_HIV_)) \
     ? sprintf((_BUF_), "(%s %s)", (_HID_), (_DES_)) \
     : (((_LOV_) < (_HIV_) && \
         (_ATT_) <= (_LOV_)) \
        ? sprintf((_BUF_), "(%s %s)", (_LOD_), (_DES_)) \
        : sprintf((_BUF_), "(%s%+d)", (_DES_), (sint32)((_ATT_) * (_SGN_)))))

/*
 * Returns a pointer to a static buffer which contains a
 * description of the given object.
 * If it is a monster, lots of information about its abilities
 * will be returned.
 * If it is an item, lots of information about which abilities
 * will be gained about its user will be returned.
 * If it is a player, it writes out the current abilities
 * of the player, which is usually gained by the items applied.
 */
/* i rewrite this to describe *every* object in the game.
 * That includes a description of every flag, etc.
 * MT-2003 */
char * describe_item(const object_t *const op)
{
    object_t     *tmp,
               *next;
    int         attr, val, more_info = 0, id_true = 0;
    char        buf[MEDIUM_BUF];
    static char retbuf[LARGE_BUF*3];

    retbuf[0] = '\0';

    /* we start with living objects like mobs */
    if (op->type == PLAYER)
    {
        player_t *pl = CONTR(op);

        describe_terrain(op, retbuf);
        sprintf(strchr(retbuf, '\0'), "(regen: hp %+d, mana %+d, grace %+d)",
            pl->gen_hp, pl->gen_sp, pl->gen_grace);
    }
    else if (op->type == MONSTER)
    {
        describe_terrain(op, retbuf);

        if (QUERY_FLAG(op, FLAG_UNDEAD))
            strcat(retbuf, "(undead)");
        if (QUERY_FLAG(op, FLAG_INVULNERABLE))
            strcat(retbuf, "(invulnerable)");
        if (QUERY_FLAG(op, FLAG_NO_ATTACK))
            strcat(retbuf, "(never attacks)");
        if (QUERY_FLAG(op, FLAG_CAN_PASS_THRU))
            strcat(retbuf, "(pass through doors)");
        if (QUERY_FLAG(op, FLAG_SEE_INVISIBLE))
            strcat(retbuf, "(see invisible)");

        if (QUERY_FLAG(op, FLAG_USE_WEAPON))
            strcat(retbuf, "(melee attack)");
        if (QUERY_FLAG(op, FLAG_READY_BOW))
            strcat(retbuf, "(range attack)");
        if (QUERY_FLAG(op, FLAG_USE_ARMOUR))
            strcat(retbuf, "(wear armour)");
        if (QUERY_FLAG(op, FLAG_USE_RING))
            strcat(retbuf, "(wear ring)");
        if (QUERY_FLAG(op, FLAG_FRIENDLY))
            strcat(retbuf, "(NPC)");
        if (QUERY_FLAG(op, FLAG_UNAGGRESSIVE))
            strcat(retbuf, "(unaggressive)");
        else
            strcat(retbuf, "(aggressive)");
        if (QUERY_FLAG(op, FLAG_HITBACK))
            strcat(retbuf, "(hitback)");
        if (QUERY_FLAG(op, FLAG_READY_SPELL))
            strcat(retbuf, "(spellcaster)");
        if (QUERY_FLAG(op, FLAG_CAN_OPEN_DOOR))
            strcat(retbuf, "(open doors)");
        if (QUERY_FLAG(op, FLAG_RANDOM_MOVE))
            strcat(retbuf, "(wandering)");

        /* lets check the inv for spell ABILITY objects.
             * This was previous a randomitems search, but that
             * was wrong because its possible we use a random list to
             * generate different instances of this mob/item
             */
        FOREACH_OBJECT_IN_OBJECT(tmp, op, next)
        {
            if (tmp && (tmp->type == ABILITY))
            {
                strcat(retbuf, "(");
                strcat(retbuf, STRING_SAFE(tmp->name));
                strcat(retbuf, ")");
            }
        }

        if (ABS(op->speed) > MIN_ACTIVE_SPEED)
        {
            switch ((int) ((ABS(op->speed)) * 15))
            {
                case 0:
                  strcat(retbuf, "(very slow)");
                  break;
                case 1:
                  strcat(retbuf, "(slow)");
                  break;
                case 2:
                  strcat(retbuf, "(normal speed)");
                  break;
                case 3:
                case 4:
                  strcat(retbuf, "(fast)");
                  break;
                case 5:
                case 6:
                  strcat(retbuf, "(very fast)");
                  break;
                case 7:
                case 8:
                case 9:
                case 10:
                  strcat(retbuf, "(extremely fast)");
                  break;
                default:
                  strcat(retbuf, "(lightning fast)");
                  break;
            }
        }
    }
    else /* here we handle items... */
    {
        if (QUERY_FLAG(op, FLAG_IDENTIFIED) || QUERY_FLAG(op, FLAG_BEEN_APPLIED))
            id_true = 1; /* we only need calculate this one time */

        if (op->item_level)
        {
            if (op->item_skill)
            {
                sprintf(strchr(retbuf, '\0'), "(req. level %d in %s)",
                    op->item_level,
                    STRING_OBJ_NAME(skillgroups[op->item_skill - 1]));
            }
            else
            {
                sprintf(strchr(retbuf, '\0'), "(req. level %d)",
                    op->item_level);
            }
        }

        /* terrain flags have no double use... if valid, show them */
        if (id_true && op->terrain_type)
            describe_terrain(op, retbuf);

        /* now lets deal with special cases */
        switch (op->type)
        {
            case WAND:
            case ROD:
            case HORN:
              if (id_true)
              {
                  sprintf(buf, "(%1.2f sec)", ((float) op->last_grace / pticks_second));
                  strcat(retbuf, buf);
              }
              break;

              /* Armor type objects */
            case ARMOUR:
            case HELMET:
            case SHOULDER:
            case LEGS:
            case SHIELD:
            case BOOTS:
            case GLOVES:
            case GIRDLE:
            case BRACERS:
            case CLOAK:
              if (id_true)
              {
                  if (op->last_sp)
                  {
                      sprintf(buf, "(encumbrance%+.1f%%)", op->last_sp / 10.0);
                      strcat(retbuf, buf);
                  }
                  /* Do this in all cases - otherwise it gets confusing - does that
                             * item have no penality, or is it not fully identified for example.
                             */
                  if (op->last_heal)
                  {
                      sprintf(buf, "(casting penalty%+.1f%%)", op->last_heal/10.0);
                      strcat(retbuf, buf);
                  }
              }
            case WEAPON:
            case RING:
            case AMULET:
            case FORCE:
             if (op->stats.hp ||
                 op->stats.sp ||
                 op->stats.grace)
             {
                 more_info = 1;
             }

            case BOW:
            case ARROW:
              if (id_true)
              {
                  float dps_swing = 0.0f;

                  if (op->type == BOW || op->type == ARROW)
                      dps_swing = ((float) op->last_grace * WEAPON_SWING_TIME);

                  if (op->stats.dam)
                  {
                      int i, tmp_dam=0;

                      /* Show DPS */
                      /* These are "direct" damage dealers where we have a swing speed & dmg */
                      if(op->type == WEAPON || (op->type == ARROW && op->subtype > 127) || op->type == BOW ||
                          op->type == PLAYER || op->type == MONSTER)
                      {
                          float dps = (float)op->stats.dam;

                          /* adjust damage with quality. DON'T adjust by /10 - this will
                          * give us a better value. Docs has to explain that the DPS of weapons
                          * is shown for a (ideal) level 10 char. Note that ->magic will be added
                          * to dam AND wc - for dam it will be handled as 0.1 additional damage.
                          * For this items this is only the "base DPS"
                          */
                          if(op->type == WEAPON || op->type == ARROW || op->type == BOW)
                              dps = (dps * (op->item_quality / 100.0f)) + op->magic;

                          if(!dps_swing) /* not a bow or arrow then use weapon_speed as swing speed */
                              dps_swing = op->weapon_speed;

                          if(dps_swing)
                              dps /= dps_swing;

                          /* we can have more as one attack - and so more as 100% damage (or less)
                          * so we need to "collect" and multiply the attack damage
                          * ONLY use the real damage attack forms
                          */
                          for(i=0;i<=LAST_ATNR_ATTACK;i++)
                              tmp_dam += op->attack[i];
                          tmp_dam += op->attack[ATNR_INTERNAL];

                          if(tmp_dam)
                              dps *= ((float)tmp_dam)/100.0f;

                          sprintf(buf, "(dps %.1f)", dps);
                      }
                      /* all what a player can apply has dam*10 value */
                      else if( op->type == AMULET || op->type == RING ||
                          op->type == BOOTS || op->type == HELMET || op->type == BRACERS || op->type == GIRDLE ||
                          op->type == CLOAK || op->type == ARMOUR || op->type == SHIELD || op->type == GLOVES ||
                          op->type == SHOULDER || op->type == LEGS || (op->type == ARROW && op->subtype < 127))
                          sprintf(buf, "(dam%+.1f)", ((float)op->stats.dam)/10.0f);
                      else
                          sprintf(buf, "(dam%+d)", op->stats.dam);
                      strcat(retbuf, buf);
                  }

                  /* An item's thacm from -125 to 125 contributes to a
                   * player's overall thacm (fumble-+thacm). But this is
                   * overridden by an item with thacm <= -126 (never fumble).
                   * This is further overridden by an item with thacm >= 126
                   * (always fumble). */
                  if (op->stats.thacm)
                  {
                      ITEMDESCRIBE(buf, op->stats.thacm, 126, "always", -126, "never", 1, "fumble");
                      strcat(retbuf, buf);
                  }

                  /* An item's thac0 from -125 to 125 contributes to a
                   * player's overall thac0 but in the opposite direction to
                   * its sign (direct hit-+(-thac0)). But this is overridden
                   * by an item with thac0 <= -126 (always direct hit). This
                   * is further overridden by an item with thac0 >= 126 (never
                   * direct hit). */
                  if (op->stats.thac0)
                  {
                      ITEMDESCRIBE(buf, op->stats.thac0, 126, "never", -126, "always", -1, "direct hit");
                      strcat(retbuf, buf);
                  }

                  if (op->stats.wc)
                  {
                      sprintf(buf, "(wc%+d)", op->stats.wc);
                      strcat(retbuf, buf);
                  }

                  if (op->stats.ac)
                  {
                      sprintf(buf, "(ac%+d)", op->stats.ac);
                      strcat(retbuf, buf);
                  }

                  if (op->type == ARROW || op->type == BOW)
                  {
                      if(op->last_grace)
                      {
                          sprintf(buf, "(%1.2f sec)", dps_swing);
                          strcat(retbuf, buf);
                      }

                      if (op->last_sp)
                      {
                          sprintf(buf, "(range%+d)", op->last_sp);
                          strcat(retbuf, buf);
                      }
                  }
                  else if (op->type == WEAPON)
                  {
                      sprintf(buf, "(%1.2f sec)", op->weapon_speed);
                      strcat(retbuf, buf);

                      if (op->level > 0)
                      {
                          sprintf(buf, "(improved %d/%d)", op->last_eat, op->level);
                          strcat(retbuf, buf);
                      }
                  }
              }

              sprintf(buf, "%s", describe_resistance(op, 0));
              strcat(retbuf, buf);

              sprintf(buf, "%s", describe_attack(op, 0));
              strcat(retbuf, buf);
              break;

            case FOOD:
            case DRINK:
              if (id_true)
              {
                  /* An item's thacm from -125 to 125 contributes to a
                   * player's overall thacm (fumble-+thacm). But this is
                   * overridden by an item with thacm <= -126 (never fumble).
                   * This is further overridden by an item with thacm >= 126
                   * (always fumble). */
                  if (op->stats.thacm)
                  {
                      ITEMDESCRIBE(buf, op->stats.thacm, 126, "always", -126, "never", 1, "fumble");
                      strcat(retbuf, buf);
                  }

                  /* An item's thac0 from -125 to 125 contributes to a
                   * player's overall thac0 but in the opposite direction to
                   * its sign (direct hit-+(-thac0)). But this is overridden
                   * by an item with thac0 <= -126 (always direct hit). This
                   * is further overridden by an item with thac0 >= 126 (never
                   * direct hit). */
                  if (op->stats.thac0)
                  {
                      ITEMDESCRIBE(buf, op->stats.thac0, 126, "never", -126, "always", -1, "direct hit");
                      strcat(retbuf, buf);
                  }

                  if (op->last_eat <= 0)
                  {
                      strcat(retbuf, "(no restorative qualities)");
                  }
                  else
                  {
                    if (op->subtype==1)
                    {
                          sprintf(strchr(retbuf, '\0'), "(health per second for %d seconds: hp %+d%%, mana %+d%%, grace %+d%%)",
                          op->last_eat, op->stats.hp, op->stats.sp, op->stats.grace);
                    }
                    else
                    {
                         sprintf(strchr(retbuf, '\0'), "(health per second for %d seconds: hp %+d, mana %+d, grace %+d)",
                          op->last_eat, op->stats.hp, op->stats.sp, op->stats.grace);
                    }
                  }
              }
              break;

            case POTION:
              if (id_true)
              {
                  /* An item's thacm from -125 to 125 contributes to a
                   * player's overall thacm (fumble-+thacm). But this is
                   * overridden by an item with thacm <= -126 (never fumble).
                   * This is further overridden by an item with thacm >= 126
                   * (always fumble). */
                  if (op->stats.thacm)
                  {
                      ITEMDESCRIBE(buf, op->stats.thacm, 126, "always", -126, "never", 1, "fumble");
                      strcat(retbuf, buf);
                  }

                  /* An item's thac0 from -125 to 125 contributes to a
                   * player's overall thac0 but in the opposite direction to
                   * its sign (direct hit-+(-thac0)). But this is overridden
                   * by an item with thac0 <= -126 (always direct hit). This
                   * is further overridden by an item with thac0 >= 126 (never
                   * direct hit). */
                  if (op->stats.thac0)
                  {
                      ITEMDESCRIBE(buf, op->stats.thac0, 126, "never", -126, "always", -1, "direct hit");
                      strcat(retbuf, buf);
                  }
                  if (op->last_sp)
                  {
                      sprintf(buf, "(range%+d)", op->last_sp);
                      strcat(retbuf, buf);
                  }
              }
              /* potions, scrolls, ...*/
            default:
              /* no more infos for all items we don't have handled in the switch */
              return retbuf;
        }

        /* these counts for every "normal" item a player deals with - most times equipement */
        if (id_true)
        {
            for (attr = 0; attr < 7; attr++)
            {
                if ((val = get_stat_value(&(op->stats), attr)) != 0)
                {
                    sprintf(buf, "(%s%+d)", short_stat_name[attr], val);
                    strcat(retbuf, buf);
                }
            }

            if (op->stats.exp)
            {
                sprintf(buf, "(speed %+d)", op->stats.exp);
                strcat(retbuf, buf);
            }
        }
    }

    /* some special info for some kind of identified items */
    if (id_true && more_info)
    {
        sprintf(strchr(retbuf, '\0'), "(regen: hp %+d, mana %+d, grace %+d)",
            op->stats.hp, op->stats.sp, op->stats.grace);
    }

    /* here we deal with all the special flags */
    if (id_true || op->type == MONSTER || op->type == PLAYER)
    {
        if (QUERY_FLAG(op, FLAG_SEE_INVISIBLE))
            strcat(retbuf, "(see invisible)");
        if (QUERY_FLAG(op, FLAG_MAKE_ETHEREAL))
            strcat(retbuf, "(makes ethereal)");
        if (QUERY_FLAG(op, FLAG_IS_ETHEREAL))
            strcat(retbuf, "(ethereal)");
        if (QUERY_FLAG(op, FLAG_MAKE_INVISIBLE))
            strcat(retbuf, "(makes invisible)");
        if (QUERY_FLAG(op, FLAG_IS_INVISIBLE))
            strcat(retbuf, "(invisible)");
        if (QUERY_FLAG(op, FLAG_XRAYS))
            strcat(retbuf, "(xray-vision)");
        if (QUERY_FLAG(op, FLAG_SEE_IN_DARK))
            strcat(retbuf, "(infravision)");
        if (QUERY_FLAG(op, FLAG_LIFESAVE))
            strcat(retbuf, "(lifesaving)");
        if (QUERY_FLAG(op, FLAG_REFL_CASTABLE))
            strcat(retbuf, "(reflect castables)");
        if (QUERY_FLAG(op, FLAG_REFL_MISSILE))
            strcat(retbuf, "(reflect missiles)");
        if (QUERY_FLAG(op, FLAG_STEALTH))
            strcat(retbuf, "(stealth)");
        if (QUERY_FLAG(op, FLAG_FLYING))
            strcat(retbuf, "(flying)");
        if (QUERY_FLAG(op, FLAG_LEVITATE))
            strcat(retbuf, "(levitate)");
    }
    if (id_true)
    {
        if (op->slaying != NULL)
        {
            strcat(retbuf, "(slay ");
            strcat(retbuf, op->slaying);
            strcat(retbuf, ")");
        }

        strcat(retbuf, describe_attack(op, 0));
        DESCRIBE_PATH(retbuf, op->path_attuned, "Attuned");
        DESCRIBE_PATH(retbuf, op->path_repelled, "Repelled");
        DESCRIBE_PATH(retbuf, op->path_denied, "Denied");

        if ((op->type != HORN &&
             op->type != ROD &&
             op->type != WAND) &&
            (op->stats.maxhp ||
             op->stats.maxsp ||
             op->stats.maxgrace))
        {
            sprintf(strchr(retbuf, '\0'), "(health: hp %+d, mana %+d, grace %+d)",
                op->stats.maxhp, op->stats.maxsp, op->stats.maxgrace);
        }
    }
    return retbuf;
}

/* thing_declare() is used to 'identify' one or more things. Identify does not
 * necessarily mean what you think.
 *
 * Here is an overview of what declare means:
 *
 * Objects have a few flags which broadly indicate specific info is (set) or
 * is not (clear) known about them. This knowledge applies to /all/ future
 * observers, which is ridiculous if you think about it (but that's a well
 * established mechanic in this sort of game so lets go with it). So we'll say
 * the object 'declares' itself as whatever even though this is usually down
 * to, eg, an identify spell. An object may declare as known_magical,
 * known_cursed, or identified. Identified implies known_*. Further known_*
 * declares may be added in future. For an object to be known_* it must be *
 * in the first place!
 *
 * who is the object that ultimately causes the declaration (eg, a spell).
 * where is an object, the inventory of which (in whole or in part) is
 * declared, or NULL. what is a thing to declare, or NULL. level is, eg, the
 * level of the identify spell. mask is some combination of
 * THING_DECLARE_BIT_*.
 *
 * The return is the number of actual declares. */
/* TODO: Write proper docs. */
sint32 thing_declare(object_t *who, object_t *where, object_t *what, sint32 level, uint32 mask, objectlink_t **list)
{
    sint32 nrof = 0;

    /* Sanity. */
    if (!OBJECT_INPLAY(who) ||
        (OBJECT_INPLAY(where) &&
         OBJECT_INPLAY(what) &&
         where != what &&
         where != what->env) ||
        level <= 0 ||
        !list)
    {
        LOG(llevBug, "BUG:: thing_declare(): Sanity!\n");
        return -1;
    }

    /* No where and no what. Declare all things on same msp as e. */
    if (!where &&
        !what)
    {
        object_t *e;
        object_t *this;
        object_t *next;
        object_t *ownr = (OBJECT_VALID(who->owner, who->owner_count))
            ? get_owner(who) : who;

        for (e = who; e->env; e = e->env);

        FOREACH_OBJECT_IN_MSP(this, MSP_KNOWN(e), next)
        {
            if (this == e)
            {
                continue;
            }

            if (ThingDeclare(ownr, this, level, mask, *list))
            {
                nrof++;
            }
        }
    }
    /* where and either no what or what is where. Declare normal inventory of
     * where (descending into containers or not). */
    else if (!what ||
        what == where)
    {
        object_t *this;
        object_t *next;
        object_t *container = what;
        object_t *wh = (where->type != CONTAINER) ? where : who;
        object_t *ownr = (OBJECT_VALID(wh->owner, wh->owner_count))
            ? get_owner(wh) : wh;

        FOREACH_OBJECT_IN_OBJECT(this, where, next)
        {
            ITERATIVE_BROWSE_NORMAL_INVENTORY(container, this, ownr, next);

            if (QUERY_FLAG(this, FLAG_SYS_OBJECT))
            {
                continue;
            }

            if (ThingDeclare(ownr, this, level, mask, *list))
            {
                nrof++;
            }
        }
    }
    /* May be where or not but always what (if where then what must be
     * directly in its inventory). Declare single object (what). */
    else
    {
        object_t *wh = (where &&
            where->type != CONTAINER) ? where : who;
        object_t *ownr = (OBJECT_VALID(wh->owner, wh->owner_count))
            ? get_owner(wh) : wh;

        if (ThingDeclare(ownr, what, level, mask, *list))
        {
            nrof++;
        }
    }

    return nrof;
}

/* thing_pick_up() is called when who attempts to pick up nrof * what. If where is
 * non-NULL it is a container into which what will be put (pick up to
 * container) otherwise what is moved into who's inv (pick up to inventory).
 *
 * As a special case, when what is of type LOOT we loop through the inventory
 * of what, picking up each of those objects. The empty loot object is then
 * removed (but if who is a player he is messaged that he picked up the loot
 * object, not each of its contents). In this way we can pick up many items
 * with one call.
 *
 * The return is what (but possibly with a different ->nrof, certainly with a
 * different ->env, etc than before the function) if what was picked up or NULL
 * if it wasn't. */
object_t *thing_pick_up(object_t *who, object_t *what, object_t *where, uint32 nrof)
{
    player_t    *pl = NULL;
    object_t    *from,
                *to = who,
                *outof;
    uint32      tmp_nrof;

    /* Sanity checks: must be a who and only players and monsters can pick
     * things up; must be a what; if there is a where, it must be a
     * container (and for non-gmaster wiz players, it must be their linked
     * container). */
    if (!who ||
        (who->type != MONSTER &&
         (who->type != PLAYER ||
          !(pl = CONTR(who)))) ||
        !what ||
        (where &&
         (((!pl ||
            (pl->gmaster_mode & GMASTER_MODE_SA)) &&
           where->type != CONTAINER) ||
          (pl &&
           where != pl->container))))
    {
        return NULL;
    }

    /* Make sure who can reach what (and where if there is one) -- see
     * ThingCanReach(). */
    if (!(from = ThingCanReach(who, what)) ||
        (where &&
         !(to = ThingCanReach(who, where))))
    {
        return NULL;
    }

    outof = what->env;

    /* Ensure nrof is a sensible value. */
    nrof = MAX(1, MIN(nrof, MAX_OBJ_NROF));

    if (nrof > what->nrof)
    {
        /* In shops any number of unpaid items can be picked up off the floor,
         * regardless of stack size. */
        if (!MSP_KNOWN(who) ||
            !what->map ||
            !QUERY_FLAG(what, FLAG_UNPAID))
        {
            nrof = what->nrof;
        }
    }

    if (where &&
        !where->env &&
        where->map &&
        QUERY_FLAG(what, FLAG_NO_DROP))
    {
        ndi(NDI_UNIQUE, 0, who, "You cannot put a no-drop item into this container.",
                        query_name(what, who, ARTICLE_DEFINITE, 0));
        return NULL;
    }

    /* When what is loot we go through a special process. */
    if (what->type == LOOT)
    {
        object_t *looted,
                 *next;

        if (pl)
        {
            SET_FLAG(who, FLAG_NO_FIX_PLAYER);
        }

        FOREACH_OBJECT_IN_OBJECT(looted, what, next)
        {
            /* Nested loots will be thrown away. */
            if (looted->type == LOOT)
            {
                remove_ob(looted);
            }
            /* Attempt to pick up each of the contents of the loot. */
            else
            {
                uint32 nr = MAX(1, MIN(looted->nrof, MAX_OBJ_NROF));

                (void)ThingPickUp(who, looted, where, nr, from, to);
            }
        }

        /* If there is still loot we couldn't pick up... */
        if (what->inv)
        {
            /* When the loot is being given we want to give the script a chance
             * to deal with anything left over. */
            if (QUERY_FLAG(from, FLAG_IS_GIVING))
            {
                /* If who is a player, keep him uptodate with what is going on
                 * (he already knows why).  */
                if (pl)
                {
                    pl = NULL; // prevents futher 'you pick up...' messages
                    ndi(NDI_UNIQUE, 0, who, "You receive as much of %s as you can.",
                        query_name(what, who, ARTICLE_DEFINITE, 0));
                }
            }
            /* Otherwise... */
            else
            {
                /* If who is a player, keep him uptodate with what is going on
                 * (he already knows why).  */
                if (pl)
                {
                    pl = NULL; // prevents futher 'you pick up...' messages
                    ndi(NDI_UNIQUE, 0, who, "You take what you can of %s and leave the rest.",
                        query_name(what, who, ARTICLE_DEFINITE, 0));
                }

                /* Insert all the remaining contents of the loot in wherever the
                 * loot itself is. */
                if (what->map)
                {
                    FOREACH_OBJECT_IN_OBJECT(looted, what, next)
                    {
                        remove_ob(looted);
                        looted->x = what->x;
                        looted->y = what->y;
                        (void)insert_ob_in_map(looted, what->map, NULL, INS_NO_WALK_ON);
                    }
                }
                else if (what->env)
                {
                    FOREACH_OBJECT_IN_OBJECT(looted, what, next)
                    {
                        remove_ob(looted);
                        (void)insert_ob_in_ob(looted, what->env);
                    }
                }
            }
        }

        if (!(QUERY_FLAG(what, FLAG_REMOVED)))
        {
            remove_ob(what);
        }
    }
    /* When what is not loot, try to pick it up -- see ThingPickUp(). If this fails,
     * just return NULL. */
    else if (!(what = ThingPickUp(who, what, where, nrof, from, to)))
    {
        return NULL;
    }

    /* Give players an appropriate message to tell them what they've done. */
    /* TODO: Currently the client plays a pick up sound whenever a pick up is
     * attempted. This means when the server decides the attempt has failed, a
     * sound is still played. This should be moved server-side. Then also the
     * client can handle the 'you pick up...' messages on receipt of the sound
     * cmd. */
    if (pl)
    {
        char    buf[LARGE_BUF];
        object_t *into = (what->env &&
            what->env->type == CONTAINER) ? what->env : where;

        tmp_nrof = what->nrof;
        what->nrof = nrof;

        if (from != who &&
            QUERY_FLAG(from, FLAG_IS_GIVING))
        {
            sprintf(buf, "%s %s you %s",
                QUERY_SHORT_NAME(from, who),
                (from->nrof > 1) ? "give" : "gives",
                query_name(what, who, ARTICLE_DEFINITE, 0));

            if (into)
            {
                sprintf(strchr(buf, '\0'), " and you put %s into %s",
                    (nrof > 1) ? "them" : "it",
                    query_name(into, who, ARTICLE_DEFINITE, 0));
            }
        }
        else
        {
            if (outof &&
                outof != who)
            {
                sprintf(buf, "Taking %s out of %s, you ",
                    (nrof > 1) ? "them" : "it",
                    query_name(outof, who, ARTICLE_DEFINITE, 0));
            }
            else
            {
                sprintf(buf, "You ");
            }

            if (from == who)
            {
                sprintf(strchr(buf, '\0'), "%s %s into %s",
                    (to == who) ? "transfer" : "put",
                    query_name(what, who, ARTICLE_DEFINITE, 0),
                    (into) ? query_name(into, who, ARTICLE_DEFINITE, 0) : "your inventory");
            }
            else
            {
                sprintf(strchr(buf, '\0'), "pick up %s",
                    query_name(what, who, ARTICLE_DEFINITE, 0));

                if (into)
                {
                    sprintf(strchr(buf, '\0'), " and put %s into %s",
                        (nrof > 1) ? "them" : "it",
                        query_name(into, who, ARTICLE_DEFINITE, 0));
                }
            }
        }

        what->nrof = tmp_nrof;

        ndi(NDI_UNIQUE, 0, who, "%s.", buf);
    }

    /* Fix any involved players/monsters who need it -- see ThingFixAsNeeded(). */
    ThingFixAsNeeded(who, to, from, what);
    return what;
}

/* thing_drop_to_floor() is called when who attempts to drop nrof * what.
 *
 * The return is what (but possibly with a different ->nrof, certainly with a
 * different ->env and ->map, etc than before the function) if what was dropped
 * or NULL if it wasn't. */
object_t *thing_drop_to_floor(object_t *who, object_t *what, uint32 nrof)
{
    player_t   *pl = NULL;
    msp_t *msp;
    object_t   *shop,
             *from,
             *outof;
    int       reinsert = 1;

    /* Sanity checks: must be a who and only players and monsters can drop
     * things; must be a what. */
    if (!who ||
        (who->type != MONSTER &&
         (who->type != PLAYER ||
          !(pl = CONTR(who)))) ||
        !what)
    {
        return NULL;
    }

    for (from = what; from->env; from = from->env)
    {
        ;
    }

    /* who can't drop what unless it is in (a container in) who's inventory. */
    if (from != who)
    {
        return NULL;
    }

    outof = what->env;
    msp = MSP_KNOWN(who);
    MSP_GET_SYS_OBJ(msp, SHOP_FLOOR, shop);

    /* In a shop a container that is not empty cannot be dropped. */
    if (shop &&
        what->type == CONTAINER &&
        what->inv)
    {
        if (pl)
        {
            ndi(NDI_UNIQUE, 0, who, "First take everything out of %s!",
                query_name(what, who, ARTICLE_DEFINITE, 0));
        }

        return NULL;
    }

    /* Sometimes who cannot discard what -- see ThingCanDiscard(). */
    if (!ThingCanDiscard(who, what))
    {
        return NULL;
    }

    /* Now we've determined the drop is possible, so lets do it! */
    /* If what is a container be sure to unlink (close) it for all players
     * viewing it. */
    if (what->type == CONTAINER)
    {
        container_unlink(NULL, what);
    }

    /* what can have a DROP script on it which, if it returns true, aborts the
     * actual drop. */
    if (trigger_object_plugin_event(EVENT_DROP, what, who, NULL, NULL,
            (int *)&nrof, NULL, NULL, SCRIPT_FIX_ALL, NULL))
    {
        return NULL;
    }

    /* Dropping things interrupts resting. */
    if (pl)
    {
        pl->rest_mode = 0;
    }

    /* Below here is the actual code to move what to the map (specifically the
     * sqaure beneath who's feet). Also we send some appropriate messages to
     * the client. */
    if (nrof &&
        what->nrof != nrof)
    {
        what = get_split_ob(what, nrof);

        /* If the client is mistaken in how many of the item the player has,
         * the server will return NULL in get_split_ob. */
        if (what == NULL)
        {
            ndi(NDI_UNIQUE, 0, who, "You do not have that many.\n");
            return NULL;
        }
    }
    else if (!QUERY_FLAG(what, FLAG_REMOVED))
    {
        remove_ob(what);
    }

    /* Give players an appropriate message to tell them what they've done. */
    /* TODO: Currently the client plays a drop sound whenever a drop is
     * attempted. This means when the server decides the attempt has failed, a
     * sound is still played. This should be moved server-side. Then also the
     * client can handle the 'you drop...' message on receipt of the sound
     * cmd. */
    if (pl)
    {
        char buf[MEDIUM_BUF];

        if (outof &&
            outof != who)
        {
            sprintf(buf, "Taking %s out of %s, you",
                (nrof > 1) ? "them" : "it", query_name(outof, who, ARTICLE_DEFINITE, 0));
        }
        else
        {
            sprintf(buf, "You ");
        }

        ndi(NDI_UNIQUE, 0, who, "%s drop %s.",
            buf, query_name(what, who, ARTICLE_DEFINITE, 0));
    }

    /* No drops vanish for non-SAs. */
    if (pl &&
        !(pl->gmaster_mode & GMASTER_MODE_SA) &&
        QUERY_FLAG(what, FLAG_NO_DROP))
    {
        ndi(NDI_UNIQUE, 0, who, "~NO-DROP~: %s vanishes to nowhere!",
            query_name(what, who, ARTICLE_DEFINITE, 0));
        reinsert = 0;
    }
    /* In a shop there are special rules. */
    else if (shop &&
             !QUERY_FLAG(what, FLAG_SYS_OBJECT) &&
             what->layer)
    {
        sint64 price;

        /* An unpaid item is just put back in the shop. */
        if (QUERY_FLAG(what, FLAG_UNPAID))
        {
            if (pl)
            {
                ndi(NDI_UNIQUE, 0, who, "The shop magic puts %s back in storage.",
                    query_name(what, who, ARTICLE_DEFINITE, 0));
            }

            reinsert = 0;
        }
        /* Coins and worthless items just drop to the floor like normal but
         * everything else is sold to the shop. */
        else if (what->type != MONEY &&
                 (price = query_cost(what, who, F_SELL)) > 0)
        {
            moneyblock_t  money;
            object_t       *loot;

            (void)enumerate_coins(price, &money);
            loot = create_financial_loot(&money, who, MODE_NO_INVENTORY);

            if (pl)
            {
                ndi(NDI_UNIQUE, 0, who, "You sell %s for ~%s~.",
                    query_name(what, who, ARTICLE_DEFINITE, 0),
                    query_cost_string(what, who, F_SELL, COSTSTRING_FULL));
            }

            SHSTR_FREE_AND_ADD_STRING(loot->name, "your fee");
            (void)thing_pick_up(who, loot, NULL, 1);
            reinsert = 0;
        }
    }

    /* Yay! we're finally done. Insert what into its new map if necessary. */
    if (reinsert)
    {
        what->x = who->x;
        what->y = who->y;
        what = insert_ob_in_map(what, who->map, who, 0);

        /* Reinsert who at the top of the object list. */
        SET_FLAG(who, FLAG_NO_APPLY);
        remove_ob(who);
        (void)insert_ob_in_map(who, who->map, who, INS_NO_MERGE | INS_NO_WALK_ON);
        CLEAR_FLAG(who, FLAG_NO_APPLY);
    }

    /* Fix any involved players/monsters who need it -- see ThingFixAsNeeded(). */
    ThingFixAsNeeded(who, NULL, NULL, what);

    return what;
}
