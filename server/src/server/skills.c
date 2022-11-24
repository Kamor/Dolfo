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

#include "global.h"

static object_t *IsValidTarget(object_t *what, object_t*who);
static object_t *TargetInDirection(object_t *op, int dir);

static object_t *TargetInDirection(object_t *op, int dir)
{
    map_t    *m = op->map;
    sint16    x = op->x + OVERLAY_X(dir),
              y = op->y + OVERLAY_Y(dir);
    msp_t    *msp = MSP_GET2(m, x, y);
    object_t *this,
             *next;

    if (!msp)
    {
        return NULL;
    }

    FOREACH_OBJECT_IN_MSP(this, msp, next)
    {
        object_t *head = (this->head) ? this->head : this;

        if (IsValidTarget(head, op))
        {
            return head;
        }
    }

    return NULL;
}

/* attack_melee_weapon() - this handles melee weapon attacks -b.t.
 * For now we are just checking to see if we have a ready weapon here.
 * But there is a real neato possible feature of this scheme which
 * bears mentioning:
 * Since we are only calling this from do_skill() in the future
 * we may make this routine handle 'special' melee weapons attacks
 * (like disarming manuever with sai) based on player SK_level and
 * weapon type.
 */

int attack_melee_weapon(object_t *op, int dir, char *string)
{
    if (!QUERY_FLAG(op, FLAG_READY_WEAPON))
    {
        if (op->type == PLAYER)
            ndi(NDI_UNIQUE, 0, op, "You have no ready weapon to attack with!");
        return 0;
    }

    return skill_attack(NULL, op, dir, string);
}

/* attack_hth() - this handles all hand-to-hand attacks -b.t. */

/* July 5, 1995 - I broke up attack_hth() into 2 parts. In the first
 * (attack_hth) we check for weapon use, etc in the second (the new
 * function skill_attack() we actually attack.
 */

int attack_hth(object_t *pl, int dir, char *string)
{
    object_t *enemy = NULL;

    if (QUERY_FLAG(pl, FLAG_READY_WEAPON))
    {
        object_t *weapon,
               *next;

        FOREACH_OBJECT_IN_OBJECT(weapon, pl, next)
        {
            if (weapon->type != WEAPON ||
                !QUERY_FLAG(weapon, FLAG_APPLIED))
            {
                continue;
            }

            CLEAR_FLAG(weapon, FLAG_APPLIED);
            CLEAR_FLAG(pl, FLAG_READY_WEAPON);
            FIX_PLAYER(pl ,"attack hth");
            ndi(NDI_UNIQUE, 0, pl, "You unwield your weapon in order to attack.");
            OBJECT_UPDATE_UPD(weapon, UPD_FLAGS);
            break;
        }
    }

    return skill_attack(enemy, pl, dir, string);
}

/* skill_attack() - Core routine for use when we attack using a skills
 * system. There are'nt too many changes from before, basically this is
 * a 'wrapper' for the old attack system. In essence, this code handles
 * all skill-based attacks, ie hth, missile and melee weapons should be
 * treated here. If an opponent is already supplied by move_player(),
 * we move right onto do_skill_attack(), otherwise we find if an
 * appropriate opponent exists.
 *
 * This is called by move_player() and attack_hth()
 *
 * Initial implementation by -bt thomas@astro.psu.edu */
int skill_attack(object_t *tmp, object_t *pl, int dir, char *string)
{
    if (!dir)
    {
        dir = pl->facing;
    }

    /* If we don't yet have an opponent, find if one exists, and attack.
     * Legal opponents are the same as outlined in move_player(). */
    if (!tmp)
    {
        tmp = TargetInDirection(pl, dir);
    }
    else
    {
        tmp = IsValidTarget(tmp, pl);
    }

    if (!tmp || (tmp && get_friendship(pl, tmp) < FRIENDSHIP_HELP))
    {
        return do_skill_attack(tmp, pl, string);
    }

    return 0;
}

/* IsValidTarget() returns what if what is attackable by who or NULL. */
static object_t *IsValidTarget(object_t *what, object_t*who)
{
    /* PvP is not available unless both players are in PvP msps. */
    if ((what->type == PLAYER &&
         (who->type != PLAYER ||
          (MSP_KNOWN(what)->flags & MSP_KNOWN(who)->flags & MSP_FLAG_PVP))) ||
        what->type == MONSTER ||
        QUERY_FLAG(what, FLAG_CAN_ROLL) ||
        what->type == TYPE_DOOR)
    {
        return what;
    }

    return NULL;
}

/* do_skill_attack() - We have got an appropriate opponent from either
 * move_player() or skill_attack(). In this part we get on with
 * attacking, take care of messages from the attack and changes in invisible.
 * Returns true if the attack damaged the opponent.
 * -b.t. thomas@astro.psu.edu
 */

int do_skill_attack(object_t *tmp, object_t *op, char *string)
{
    int       hits = 0;
    float     ticks = 0.0f;
    int       i;
    object_t *targets[3] = { NULL, tmp, NULL };

    if (op->type == PLAYER)
    {
        if (!CONTR(op)->selected_weapon) /* ok... lets change to our hth skill */
        {
            if (CONTR(op)->skill_weapon)
            {
                if (!change_skill(op, CONTR(op)->skill_weapon->stats.sp))
                {
                    LOG(llevBug, "BUG: do_skill_attack() could'nt give new hth skill to %s\n", STRING_OBJ_NAME(op));
                    return 0;
                }
            }
            else
            {
                LOG(llevBug, "BUG: do_skill_attack(): no hth skill in player %s\n", STRING_OBJ_NAME(op));
                return 0;
            }
        }
        /* if we have 'ready weapon' but no 'melee weapons' skill readied
         * this will flip to that skill. This is only window dressing for
         * the players--no need to do this for monsters.
         */
        if (QUERY_FLAG(op, FLAG_READY_WEAPON)
             && (!op->chosen_skill || op->chosen_skill->stats.sp != CONTR(op)->set_skill_weapon))
        {
            change_skill(op, CONTR(op)->set_skill_weapon);
        }

        // Face the target.
        rv_t dir;
        int tmp_dir;
        
        if (!targets[1])
        {
            tmp_dir = op->facing;
        }
        else
        {
            if (targets[1] && RV_GET_OBJ_TO_OBJ(op, targets[1], &dir, RV_FLAG_DIAGONAL_D))
            {
                if (op->head)
                {
                    op->head->anim_enemy_dir = dir.direction;
                    op->head->facing = dir.direction;
                }
                else
                {
                    op->anim_enemy_dir = dir.direction;
                    op->facing = dir.direction;
                }
            }

            tmp_dir = dir.direction;
        }
        

        // Polearm weapons hit the targeted foe (or direction), plus 1 on either side of it
        if (op->chosen_skill->stats.sp >= SK_MELEE_CLEAVE && 
            op->chosen_skill->stats.sp <= SK_MELEE_SLASH &&
            CONTR(op)->selected_weapon->subtype >= WEAP_POLE_IMPACT)
        {
            if (!targets[1])
            {
                // Manually used the skill in a direction, but no mob in that direction.
                targets[1] = TargetInDirection(op, tmp_dir);
            }

            tmp_dir--;
            while (tmp_dir < 1)
            {
                tmp_dir += 8;
            }

            targets[0] = TargetInDirection(op, tmp_dir);

            tmp_dir += 2;
            while (tmp_dir > 8)
            {
                tmp_dir -= 8;
            }

            targets[2] = TargetInDirection(op, tmp_dir);
        }
    }

    for (i = 0; i < 3; i++)
    {
        /* print appropriate  messages to the player */
        if (targets[i] &&
            attack_ob(targets[i], op, NULL))
        {
            if (string)
            {
                if (op->type == PLAYER)
                {
                    ndi(NDI_UNIQUE, 0, op, "You %s %s!",
                        string, QUERY_SHORT_NAME(tmp, op));
                }
                else if (tmp->type == PLAYER)
                {
                    ndi(NDI_UNIQUE, 0, tmp, "%s %s you!",
                        QUERY_SHORT_NAME(op, tmp), string);
                }
            }
            hits++;
        }
    }

    /* set the skill delay from the attack so we can't use other skills during the cooldown time */
    if (op->type == PLAYER)
    {
        ticks = ABS(CONTR(op)->ob->weapon_speed);

        if (op->chosen_skill->stats.sp >= SK_MELEE_CLEAVE &&
            op->chosen_skill->stats.sp <= SK_MELEE_SLASH &&
            CONTR(op)->selected_weapon->subtype >= WEAP_POLE_IMPACT)
        {
            ticks += 3 * hits * WEAPON_SWING_TIME;
        }

        set_action_time(op, ticks);
    }

    return hits;
}


/* This is in the same spirit as the similar routine for spells
 * it should be used anytime a function needs to check the user's
 * level.
 */
int SK_level(object_t *op)
{
    object_t *head    = op->head ? op->head : op;
    int     level;

    if (head->type == PLAYER && head->chosen_skill && head->chosen_skill->level != 0)
    {
        level = head->chosen_skill->level;
    }
    else
    {
        level = head->level;
    }

    if (level <= 0)
    {
        LOG(llevBug, "BUG: SK_level(arch %s, name %s): level <= 0\n", op->arch->name, STRING_OBJ_NAME(op));
        level = 1;   /* safety */
    }

    return level;
}

/* The FIND_TRAPS skill. This routine is taken mostly from the
 * command_search loop. It seemed easier to have a separate command,
 * rather than overhaul the existing code - this makes sure things
 * still work for those people who don't want to have skill code
 * implemented. */
int find_traps(object_t *op, int level)
{
    uint8 i,
          found = 0,
          aware = 0;

    /* Search the squares in the 8 directions. */
    for (i = 0; i < 9; i++)
    {
        map_t  *m = op->map;
        sint16      x = op->x + OVERLAY_X(i),
                    y = op->y + OVERLAY_Y(i);
        msp_t   *msp = MSP_GET2(m, x, y);
        object_t     *next,
                   *this;

        /* Ensure the square isn't out of bounds. */
        if (!msp)
        {
            continue;
        }
 
        next = msp->last;

        while ((this = next))
        {
            /* this is the object on the map, that is the current object under
             * consideration. */
            object_t *that = this;

            next = this->below;

            /* op and entities are opaque to find traps. */
            if (that == op ||
                that->type == PLAYER ||
                that->type == MONSTER)
            {
                continue;
            }

            /* Otherwise, check that and (if necessary) inventory of that. */
            while (that)
            {
                if (that->type == RUNE && that->stats.Cha > 1)
                {
                    if (trap_see(op, that, level))
                    {
                        trap_show(that, this);
                        found++;
                    }
										
										// i have deactivated aware logic, because trap_see should do the checks
										// also this optional check here targets only the level of trap, not the hiddeness of trap
										// todo remove aware logic complete from this function
										
                    /*else if (that->level <= (level * 1.8f))
                    {
                        aware = 1;
                    }*/
                }

                that = find_next_object(that, RUNE, FNO_MODE_CONTAINERS, that);
            }
        }
    }

   /* Only players get messages. */
    if (op->type == PLAYER &&
        CONTR(op))
    {
        if (!found)
        {
            ndi(NDI_UNIQUE, 0, op, "You find no new traps this time%s...",
                (aware) ? ", but you find signs of traps hidden beyond your skill" : "");
        }
        else
        {
            ndi(NDI_UNIQUE, 0, op, "You find %d new traps%s!",
                found,
                (aware) ? " and signs of more traps hidden beyond your skill" : "");
        }
    }

    return 0;
}

/* disarm_traps() - This skill will disarm any previously discovered trap
 * the algorithm is based (almost totally) on the old command_disarm() - b.t.
 */

int disarm_traps(object_t *op, int dir, int level)
{
    uint8 i;

    for (i = 0; i < 9; i++)
    {
        map_t *m = op->map;
        sint16     x = op->x + OVERLAY_X(i),
                   y = op->y + OVERLAY_Y(i);
        msp_t  *msp = MSP_GET2(m, x, y);
        object_t    *this,
                  *next;

        if (!msp)
        {
            continue;
        }

        FOREACH_OBJECT_IN_MSP(this, msp, next)
        {
            object_t *that,
                   *next2;

            FOREACH_OBJECT_IN_OBJECT(that, this, next2)
            {
                if (that->type == RUNE &&
                    that->stats.Cha <= 1)
                {
                    this = that;
                    break;
                }
            }

            if (this->type == RUNE &&
                this->stats.Cha <= 1)
            {
							// this is redundant here, we have same check in trap_disarm todo
                if (QUERY_FLAG(this, FLAG_SYS_OBJECT) ||
                    QUERY_FLAG(this, FLAG_IS_INVISIBLE))
                {
                    trap_show(this, this);
                }

                trap_disarm(op, this, level);
                return 0;
            }
        }
    }

    ndi(NDI_UNIQUE, 0, op, "You have found no nearby traps to remove yet!");
    return 0;
}

int rage(object_t *op, int dir, int level)
{
    object_t *force = present_arch_in_ob(archetype_global._rage_effect, op);

    if (force != NULL)
    {
        remove_ob(force);
        return 0;
    }
    else
    {
        force = arch_to_object(archetype_global._rage_effect);
    }

    force->stats.hp *= level;
    force->weapon_speed -= level / 165.0f;

    SET_FLAG(force, FLAG_APPLIED);
    insert_ob_in_ob(force, op);

    return 1;
}
