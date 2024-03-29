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

/* define the following for skills utility debuging */

#include <global.h>

archetype_t *skillgroups[NROFSKILLGROUPS] =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

archetype_t *skills[NROFSKILLS]  =
{
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL
};

/* Link the skillgroups and skills archetypes to the skillgroups and skills
 * arrays for fast access. Also do some basic sanity checking of those arches
 * here and halt the server at any sign of trouble. */
void init_skills(void)
{
    archetype_t *at;
    int        i,
               failure = 0;

    LOG(llevSystem, "Initializing skills...\n");

    for (at = first_archetype; at; at = at->next)
    {
        if (at->clone.type == TYPE_SKILLGROUP)
        {
            i = at->clone.subtype;

            if (i < 0 ||
                i >= NROFSKILLGROUPS)
            {
                LOG(llevInfo, "  Skillgroup %s out of range (is: %d, must be: 0-%d!\n",
                    STRING_ARCH_NAME(at), i, NROFSKILLGROUPS - 1);
                failure = 1;
            }
            else if (skillgroups[i])
            {
                LOG(llevInfo, "  Duplicate skillgroup #%d found (original: %s, duplicate: %s!\n",
                    i, STRING_ARCH_NAME(skillgroups[i]), STRING_ARCH_NAME(at));
                failure = 1;
            }
            else
            {
                LOG(llevInfo, "  Adding skillgroup %s at #%d!\n",
                    STRING_ARCH_NAME(at), i);
                skillgroups[i] = at;
            }
        }
        else if (at->clone.type == TYPE_SKILL)
        {
            i = at->clone.stats.sp;

            if (i == -1)
            {
                LOG(llevInfo, "  Ignoring skill %s because it is unimplemented!\n",
                    STRING_ARCH_NAME(at));
            }
            else if (i < 0 ||
                     i >= NROFSKILLS)
            {
                LOG(llevInfo, "  Skill %s out of range (is: %d, must be: 0-%d!\n",
                    STRING_ARCH_NAME(at), i, NROFSKILLS - 1);
                failure = 1;
            }
            else if (skills[i])
            {
                LOG(llevInfo, "  Duplicate skill #%d found (original: %s, duplicate: %s!\n",
                    i, STRING_ARCH_NAME(skills[i]), STRING_ARCH_NAME(at));
                failure = 1;
            }
            else
            {
                LOG(llevInfo, "  Adding skill %s at #%d!\n",
                    STRING_ARCH_NAME(at), i);
                skills[i] = at;
            }
        }
    }

    for (i = 0; i < NROFSKILLGROUPS; i++)
    {
        if (!skillgroups[i])
        {
            LOG(llevInfo, "  Skillgroup #%d not found!\n", i);
            failure = 1;
        }
    }

    for (i = 0; i < NROFSKILLS; i++)
    {
        if (!skills[i])
        {
            LOG(llevInfo, "  Skill #%d not found!\n", i);
            failure = 1;
        }
        else if (skills[i]->clone.magic < 0 ||
                 skills[i]->clone.magic >= NROFSKILLGROUPS)
        {
            LOG(llevInfo, "  Skill %s does not belong to a valid skillgroup (is: %d, must be: 0-%d!\n",
                STRING_ARCH_NAME(skills[i]), skills[i]->clone.magic,
                NROFSKILLGROUPS - 1);
            failure = 1;
        }
    }

    if (failure)
    {
        LOG(llevError, "ERROR:: Fix the skill arches!\n");
    }
}

/* Ensures the player has all the required skillgroup objects and that the
 * shortcut pointers point to objects actually in his inv. Also calculates the
 * best skill/skillgroup. */
void validate_skills(player_t *pl)
{
    object_t *op;
    int     i,
            chosen_skill = 0;

    if (!pl)
    {
        return;
    }

    op = pl->ob;

#ifdef DEBUG_SKILL_UTIL
    LOG(llevInfo, "Validating skills and skillgroups for player %s...\n",
        STRING_OBJ_NAME(op));
#endif

    for (i = 0; i < NROFSKILLGROUPS; i++)
    {
        object_t *this = pl->skillgroup_ptr[i];

        if (!this ||
            this->env != op)
        {
            object_t *new;

#ifdef DEBUG_SKILL_UTIL
            LOG(llevInfo, "  Adding default skillgroup %s!\n",
                STRING_OBJ_NAME(&skillgroups[i]->clone));
#endif
            new = arch_to_object(skillgroups[i]);
            pl->skillgroup_ptr[i] = insert_ob_in_ob(new, op);
        }
    }

    for (i = 0; i < NROFSKILLS; i++)
    {
        object_t *this = pl->skill_ptr[i];

        if (this)
        {
            if (this->env != op)
            {
#ifdef DEBUG_SKILL_UTIL
            LOG(llevInfo, "  Nulling pointer to skill %s!\n",
                STRING_OBJ_NAME(&skills[i]->clone));
#endif
                pl->skill_ptr[i] = NULL;
            }
            else
            {
                object_t *best = pl->highest_skill[this->magic];

                if ((this->skillgroup = pl->skillgroup_ptr[this->magic]) &&
                    (!best ||
                     this->stats.exp > best->stats.exp))
                {
                    pl->highest_skill[this->magic] = this;
                }

                if (op->chosen_skill == this)
                {
                    chosen_skill = 1;
                }
            }
        }
    }

    if (!chosen_skill)
    {
        op->chosen_skill = NULL;
    }

#ifdef DEBUG_SKILL_UTIL

    LOG(llevInfo, "Finding best skills of player %s...\n",
        STRING_OBJ_NAME(op));

    for (i = 0; i < NROFSKILLGROUPS; i++)
    {
        object_t *best = pl->highest_skill[i];

        if (best)
        {
            LOG(llevInfo, "  In skillgroup %s is %s with exp=%d, level=%d!\n",
                STRING_OBJ_NAME(&skillgroups[i]->clone), STRING_OBJ_NAME(best),
                best->stats.exp, best->level);
        }
    }
#endif
}

/* do_skill() - Main skills use function-similar in scope to cast_spell().
 * We handle all requests for skill use outside of some combat here.
 * We require a separate routine outside of fire() so as to allow monsters
 * to utilize skills. Success should be the amount of experience gained
 * from the activity. Also, any skills that monster will use, will need
 * to return a positive value of success if performed correctly. Usually
 * this is handled in calc_skill_exp(). Thus failed skill use re-
 * sults in a 0, or false value of 'success'.
 *  - b.t.  thomas@astro.psu.edu
 */

int do_skill(object_t *op, int dir, char *string)
{
    int success = 0;        /* needed for monster_skill_use() too */
    int skill   = op->chosen_skill->stats.sp;
    float ticks = 0.0f;

    /*LOG(llevNoLog,"DO SKILL: skill %s ->%d\n", op->chosen_skill->name, get_skill_time(op,skill));*/

    if (QUERY_FLAG(op, FLAG_PARALYZED))
    {
        success = 0;
        return success;
    }

    switch (skill)
    {
        case SK_MELEE_IMPACT:
        case SK_MELEE_SLASH:
        case SK_MELEE_CLEAVE:
        case SK_MELEE_PIERCE:
          (void)attack_melee_weapon(op, dir, NULL);
          break;
        case SK_FIND_TRAPS:
          success = find_traps(op, op->chosen_skill->level);
          break;
        case SK_REMOVE_TRAP:
          success = disarm_traps(op, dir, op->chosen_skill->level);
          break;
        case SK_THROWING:
          ndi(NDI_UNIQUE, 0, op, "This skill is not usable in this way.");
          return success;
          break;
        case SK_MAGIC_DEVICES:
        case SK_RANGE_BOW:
          ndi(NDI_UNIQUE, 0, op, "There is no special attack for this skill.");
          return success;
          break;
        case SK_WIZARDRY_SPELLS:
        case SK_DIVINE_PRAYERS:
          ndi(NDI_UNIQUE, 0, op, "This skill is not usable in this way.");
          return success;
          /*success = pray(op);*/
        case SK_RAGE:
            if (op->type == PLAYER && CONTR(op))
            {
                success = rage(op, dir, CONTR(op)->highest_skill[op->chosen_skill->subtype]->level);
            }
            else
            {
                success = rage(op, dir, op->chosen_skill->level);
            }
          break;
        default:
          LOG(llevDebug, "%s attempted to use unknown skill: %d\n", STRING_OBJ_NAME(op), op->chosen_skill->stats.sp);
          return success;
          break;
    }

    /* For players we now update the speed_left from using the skill.
     * Monsters have no skill use time because of the random nature in
     * which use_monster_skill is called already simulates this. -b.t.
     */

    if (op->type == PLAYER)
    {
        ticks = (float) (skills[skill]->clone.stats.food) * RANGED_DELAY_TIME;
        set_action_time(op, ticks);
    }

    /* this is a good place to add experience for successfull use of skills.
     * Note that add_exp() will figure out player/monster experience
     * gain problems.
     */

    if (success && skills[skill]->clone.magic < NROFSKILLGROUPS_ACTIVE)
    {
        exp_adjust(CONTR(op), op->chosen_skill->stats.sp, success, EXP_FLAG_CAP);
    }

    return success;
}

void dump_skills()
{
}

/* Returns the index of the named skill in the skills array or -1 if it doesn't
 * exist. */
int lookup_skill_by_name(char *name)
{
    int i;

    for (i = 0; i < NROFSKILLS; i++)
    {
        /* Unfortunately we need to be case insensitive here, so no hash
         * lookup. */
        if (!strcasecmp(skills[i]->clone.name, name))
        {
            return i;
        }
    }

    return -1;
}

/* check_skill_to_apply() - When a player tries to use an
 * object which requires a skill this function is called.
 * (examples are weapons like swords and bows)
 * It does 2 things: checks for appropriate skill in player inventory
 * and alters the player status by making the appropriate skill
 * the new chosen_skill.
 * -bt. thomas@astro.psu.edu
 */

/* This function was strange used.
 * I changed it, so it is used ONLY when a player try to apply something.
 * This function now sets no apply flag or needed one - but it checks and sets
 * the right skill.
 * TODO: monster skills and flags (can_use_xxx) including here.
 * For unapply, only call change_skill(object, NO_SKILL_READY) and fix_player() after it.
 * MT - 4.10.2002
 */
int check_skill_to_apply(object_t *who, object_t *item)
{
    int skill = 0, tmp;
    int add_skill   = NO_SKILL_READY; /* perhaps we need a additional skill to use */
    player_t *pl;

    if (who->type != PLAYER ||
        !(pl = CONTR(who)))
    {
        return 1; /* this fctn only for players */
    }

    /* first figure out the required skills from the item */
    switch (item->type)
    {
        case WEAPON:
          if (pl->guild_force &&
              pl->guild_force->value &&
              item->item_level > pl->guild_force->value)
          {
              ndi(NDI_UNIQUE, 0, who, "That weapon is not permitted by your guild.");

              return 0;

          }

          tmp = item->subtype;

          if (tmp >= WEAP_POLE_IMPACT) /* we have a polearm! */
          {
              if (pl->guild_force &&
                  pl->guild_force->weight_limit &&
                  (pl->guild_force->weight_limit & GUILD_NO_POLEARM))
              {
                  ndi(NDI_UNIQUE, 0, who, "That weapon is not permitted by your guild.");

                  return 0;
              }

              tmp = item->subtype - WEAP_POLE_IMPACT; /* lets select the right weapon type */
              add_skill = SK_MELEE_MASTERY_POLE;
          }
          else if (tmp >= WEAP_2H_IMPACT) /* no, we have a 2h! */
          {
              if (pl->guild_force &&
                  pl->guild_force->weight_limit &&
                  (pl->guild_force->weight_limit & GUILD_NO_2H))
              {
                  ndi(NDI_UNIQUE, 0, who, "That weapon is not permitted by your guild.");

                 return 0;
              }

              tmp = item->subtype - WEAP_2H_IMPACT; /* lets select the right weapon type */
              add_skill = SK_MELEE_MASTERY_2H;
          }

          if (tmp == WEAP_1H_IMPACT)
              skill = SK_MELEE_IMPACT;
          else if (tmp == WEAP_1H_SLASH)
              skill = SK_MELEE_SLASH;
          else if (tmp == WEAP_1H_CLEAVE)
              skill = SK_MELEE_CLEAVE;
          else if (tmp == WEAP_1H_PIERCE)
              skill = SK_MELEE_PIERCE;
          break;
        case ARROW:
            if(item->subtype > 127)
            {
                skill = SK_THROWING;
                break;
            }

        case BOW:
          if (pl->guild_force &&
              pl->guild_force->weight_limit &&
             (pl->guild_force->weight_limit & GUILD_NO_ARCHERY))
          {
            ndi(NDI_UNIQUE, 0, who, "That weapon is not permitted by your guild.");

            return 0;
          }
          else
          {
            tmp = item->subtype;
            if (tmp == RANGE_WEAP_BOW)
                skill = SK_RANGE_BOW;
            else if (tmp == RANGE_WEAP_XBOWS)
                skill = SK_RANGE_XBOW;
            else
                skill = SK_RANGE_SLING;
            break;
          }
        case POTION:
          skill = SK_MAGIC_DEVICES; /* hm, this can be tricky when a player kills himself
                                     * applying a bomb potion... must watch it */
          break;
        case SCROLL:
          skill = SK_MAGIC_DEVICES; /* not literacy atm - we will change scroll back! */
          break;
        case ROD:
          skill = SK_MAGIC_DEVICES;
          break;
        case WAND:
          skill = SK_MAGIC_DEVICES;
          break;
        case HORN:
          skill = SK_MAGIC_DEVICES;
          break;
        default:
          LOG(llevDebug, "Warning: bad call of check_skill_to_apply()\n");
          LOG(llevDebug, "No skill exists for item: %s\n", STRING_OBJ_NAME(item));
          return 0;
    }

    /* this should not happen */
    if (skill == NO_SKILL_READY)
        LOG(llevBug, "BUG: check_skill_to_apply() called for %s and item %s with skill NO_SKILL_READY\n",
            STRING_OBJ_NAME(who), STRING_OBJ_NAME(item));

    /* lets check the additional skill if there is one */
    if (add_skill != NO_SKILL_READY)
    {
        if (!change_skill(who, add_skill))
        {
            return 0;
        }

        change_skill(who, NO_SKILL_READY);
    }

    /* if this skill is ready, all is fine. if not, ready it, if it can't readied, we
     * can't apply/use/do it */
    if (!who->chosen_skill || (who->chosen_skill && who->chosen_skill->stats.sp != skill))
    {
        if (!change_skill(who, skill))
        {
            return 0;
        }
    }
    return 1;
}

int learn_skill(object_t *pl, int skillnr)
{
    player_t *p;
    object_t *skill = NULL;


    if(pl->type != PLAYER)
        return 2;

    p = CONTR(pl);

    if (skillnr >= 0 &&
        skillnr < NROFSKILLS)
    {
        skill = arch_to_object(skills[skillnr]);
    }

    if (!skill)
        return 2;

    if (p->skill_ptr[skillnr])
    {
        ndi(NDI_UNIQUE, 0, pl, "You already know the skill '%s'!", skill->name);
        return 0;
    }

    /* Everything is cool. Give'em the skill */
    insert_ob_in_ob(skill, pl);
    CONTR(pl)->skill_ptr[skillnr] = skill;

    if(p && (p->state & ST_PLAYING))
    {
        play_sound_player_only(CONTR(pl), SOUND_LEARN_SPELL, SOUND_NORMAL, 0, 0);
        ndi(NDI_UNIQUE, 0, pl, "You have learned the skill %s!",
            skill->name);
        send_skilllist_cmd(p, skillnr, SPLIST_MODE_ADD);
    }

    FIX_PLAYER(pl, "learn skill");

    return 1;
}

int unlearn_skill(object_t *op, int skillnr)
{
    player_t *pl = CONTR(op);
    object_t *skill = pl->skill_ptr[skillnr];

    if (op->type != PLAYER)
        return 0;

    if (skillnr < 0 ||
        skillnr >= NROFSKILLS)
    {
        return 0;
    }

    if (!pl->skill_ptr[skillnr])
    {
        ndi(NDI_UNIQUE, 0, op, "You don't know that skill!");
        return 0;
    }

    (void)exp_adjust(pl, skillnr, skill->stats.exp, EXP_FLAG_NOSCRIPT);
    pl->skill_ptr[skill->stats.sp] = NULL;
    remove_ob(skill);
    ndi(NDI_UNIQUE, 0, op, "Removed skill!");
    FIX_PLAYER(pl->ob, "unlearn skill");

    return 1;
}

/* use_skill() - similar to invoke command, it executes the skill in the
 * direction that the user is facing. Returns false if we are unable to
 * change to the requested skill, or were unable to use the skill properly.
 * -b.t.
 */

int use_skill(object_t *op, char *string)
{
    int sknum   = -1;

    /* the skill name appears at the begining of the string,
     * need to reset the string to next word, if it exists. */
    /* first eat name of skill and then eat any leading spaces */

    if (string && (sknum = lookup_skill_by_name(string)) >= 0)
    {
        int len;

        if (sknum == -1)
        {
            ndi(NDI_UNIQUE, 0, op, "Unable to find skill by name %s", string);
            return 0;
        }

        len = strlen(skills[sknum]->clone.name);

        /* All this logic goes and skips over the skill name to find any
         * options given to the skill.
         */
        if (len >= (int) strlen(string))
        {
            *string = 0x0;
        }
        else
        {
            while (len--)
            {
                string++;
            }
            while (*string == 0x20)
            {
                string++;
            }
        }
        if (strlen(string) == 0)
            string = NULL;
    }

#ifdef DEBUG_SKILL_UTIL
    LOG(llevDebug, "use_skill() got skill: %s\n", sknum > -1 ? skills[sknum]->clone.name : "none");
#endif

    /* Change to the new skill, then execute it. */
    if (!change_skill(op, sknum))
    {
        return 0;
    }

    if (op->chosen_skill->subtype != SUBTYPE_SKILL_USE &&
            op->chosen_skill->subtype != SUBTYPE_SKILL_AUTOMAN)
    {
        ndi(NDI_UNIQUE, 0, op, "You can't use ~%s~ in this way!",
            op->chosen_skill->name);
    }
    else
    {
        if (check_skill_action_time(op, op->chosen_skill) && /* are we idle from other action? */
            do_skill(op, op->facing, string))
        {
            return 1;
        }
    }

    return 0;
}

/* change_skill() attempts to change who's skill to nr. On success, return 1;
 * on failure, return 0.
 *
 * Success means did who end up with nr as chosen skill? So if who's chosen
 * skill is already nr the function changes nothing but still returns 1. OTOH
 * if who does not possess skill nr the function sttill changes nothing but
 * returns 0.
 *
 * When a new skill is requested but unknown to the player, he is messaged to
 * this effect. But when the skill is known (therefore changed to), no message
 * is given; this is because (a) usually the skill is used immediately so the
 * change is apparent through context/action and (b) the client is sent the new
 * skill nr anyway so a message would just be spam. Either way, functions which
 * call this one should therefore rely on this in-built notification and not
 * spam there own success/failure messages.
 *
 * If nr is NO_SKILL_READY, any old skill is unapplied, but no new skill is
 * applied. The return is 0. */
/* TODO: As the function is only relevant to players, the parameter should be
 * player_t, not object_t. */
sint8 change_skill(object_t *who, sint16 nr)
{
    object_t *old,
             *new = NULL;
    sint8    success;

    /* Sanity checks. */
    if (!who ||
        who->type != PLAYER ||
        nr < NO_SKILL_READY ||
        nr >= NROFSKILLS)
    {
        return 0;
    }

    /* Check if a skill is already in use. If so, is it the one we want? If it
     * is, nothing more to do. Huzzah! */
    if ((old = who->chosen_skill))
    {
        if (old->stats.sp == nr)
        {
            return 1;
        }
    }

    /* Try to apply the specified skill nr. */
    if (nr != NO_SKILL_READY)
    {
        /* Do not change chosen skill at all. */
        if (!(new = CONTR(who)->skill_ptr[nr]))
        {
            ndi(NDI_UNIQUE, 0, who, "You lack the ~%s~ skill!",
                skills[nr]->clone.name);
            success = 0;
        }
        /* Ready new as chosen skill and apply the object. */
        else
        {
#ifdef DEBUG_SKILL_UTIL
            LOG(llevDebug, "DEBUG:: change_skill(): %s[%d] %s to %s!\n",
                STRING_OBJ_NAME(who), TAG(who),
                (old) ? STRING_OBJ_NAME(old) : "NONE", STRING_OBJ_NAME(new));
#endif
            send_ready_skill(CONTR(who), new->name);
            who->chosen_skill = new;
            SET_FLAG(new, FLAG_APPLIED);
            OBJECT_UPDATE_UPD(new, UPD_FLAGS);
            success = 1;
        }
    }
    /* Clear chosen skill. */
    else
    {
        who->chosen_skill = NULL;
        success = 0;
    }

    /* Unapply old object. */
    if (old &&
        (nr == NO_SKILL_READY ||
         new))
    {
        CLEAR_FLAG(old, FLAG_APPLIED);
        OBJECT_UPDATE_UPD(old, UPD_FLAGS);
    }

    return success;
}

/* sets the action timer for skills like throwing, archery, casting... */
void set_action_time(object_t *op, float t)
{
    if (op->type != PLAYER)
        return;

    CONTR(op)->ob->weapon_speed_left += ABS(t);

    /* update the value for the client */
    CONTR(op)->action_timer = (int) (CONTR(op)->ob->weapon_speed_left / pticks_second / WEAPON_SWING_TIME * 1000.0f);

    /* a little trick here, we want the server to always update the client with the new skill delay time,
     * even if the value is identical to the one sent before. this is because the client pre-emptively ticks
     * this down, so the client would need to be updated again that there is a new delay time.
     * to do this, we make it negative if the last value sent was positive, and the client uses ABS on it */
    if (CONTR(op)->last_action_timer > 0)
        CONTR(op)->action_timer *= -1;
}

/* player only: we check the action time for a skill.
 * if skill action is possible, return true.
 * if the action is not possible, drop the right message
 * and/or queue the command.
 */
int check_skill_action_time(object_t *op, object_t *skill)
{
    if(!skill)
        return 0;

    if (CONTR(op)->ob->weapon_speed_left > 0.0f)
    {
        /* update the value for the client */
        CONTR(op)->action_timer = (int) (CONTR(op)->ob->weapon_speed_left / pticks_second / WEAPON_SWING_TIME * 1000.0f);
        return 0;
    }

    return 1;
}
