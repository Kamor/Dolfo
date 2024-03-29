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

#define RN_DEFAULT      1<< 0 /* Always use this form when a value is 2^x */
#define RN_DWARVEN      1<< 1
#define RN_ELVEN        1<< 2
#define RN_GNOMISH      1<< 3
#define RN_DROW         1<< 4
#define RN_ORCISH       1<< 5
#define RN_GOBLIN       1<< 6
#define RN_KOBOLD       1<< 7
#define RN_GIANT        1<< 8
#define RN_TINY         1<< 9
#define RN_GENIE        1<<10
#define RN_DEMONISH     1<<11
#define RN_DRACONISH    1<<12

_races          item_race_table[RACE_NAME_INIT] =
{
    {"",           RN_DEFAULT},   /* default value - human like */
    {"dwarven ",   RN_DWARVEN},
    {"elven ",     RN_ELVEN},
    {"gnomish ",   RN_GNOMISH},
    {"drow ",      RN_DROW},
    {"orcish ",    RN_ORCISH},
    {"goblin ",    RN_GOBLIN},
    {"kobold ",    RN_KOBOLD},    /* count also as tiny, but "unclean" */
    {"giant ",     RN_GIANT},     /* all demihumans "bigger as humans" */
    {"tiny ",      RN_TINY},      /* different small race (sprites, ...) */
    {"demonish ",  RN_DEMONISH},  /* this is usable from all sizes */
    {"draconish ", RN_DRACONISH}, /* usable from all sizes */
    {"ogre ",      RN_GIANT}      /* count as giant */
};

/* A mob's default base AC/WC depends on its level. This used to be two tables
 * but as all values were identical I merged them. */
static int MobACWC[MAXMOBLEVEL + 1] =
{
     1,
     2,  3,  4,  5,  6,  7,  8,  9, 10, 10, // 1-10
    11, 11, 12, 12, 13, 13, 14, 14, 15, 15, // 11-20
    16, 16, 17, 17, 18, 18, 19, 19, 20, 20, // 21-30
    20, 21, 21, 22, 22, 23, 23, 24, 24, 25, // 31-40
    25, 26, 26, 27, 27, 28, 28, 29, 29, 30, // 41-50
    30, 31, 31, 32, 32, 33, 33, 34, 34, 35, // 51-60
    35, 36, 36, 37, 37, 38, 38, 39, 39, 40, // 61-70
    40, 41, 41, 42, 42, 43, 43, 44, 44, 45, // 71-80
    45, 46, 46, 47, 47, 48, 48, 49, 49, 50, // 81-90
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60, // 91-100
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, // 101-110
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, // 111-120
    81, 82, 83, 84, 85, 86, 87              // 121-127
};

/* when we carry more as this of our weight_limit, we get encumbered. */
#define ENCUMBRANCE_LIMIT 0.35f

/* stat_bonus[] is a multiplier for any stat value. */
/* TODO: Currently this is just a steady and predictable progression from 1-200
 * subdivided into 4 broad categories (1-9 malus, 10-19 creation, 20-99 normal,
 * 100+ exceptional). This will be refined over time. */
const float stat_bonus[MAX_STAT + 1] =
{
    0.00f, // 0 is impossible
    0.10f, 0.20f, 0.30f, 0.40f, 0.50f, 0.60f, 0.70f, 0.80f, 0.90f, // 1-9 gives a malus
    1.00f, 1.01f, 1.02f, 1.03f, 1.04f, 1.05f, 1.06f, 1.07f, 1.08f, 1.09f, // 10-19 is the racial start range
    1.11f, 1.13f, 1.15f, 1.17f, 1.19f, 1.21f, 1.23f, 1.25f, 1.27f, 1.29f, 1.31f, 1.33f, 1.35f, 1.37f, 1.39f, 1.41f, 1.43f, 1.45f, 1.47f, 1.49f, 1.51f, 1.53f, 1.57f, 1.59f, 1.61f, 1.63f, 1.65f, 1.67f, 1.69f, 1.71f, 1.73f, 1.75f, 1.77f, 1.79f, 1.81f, 1.83f, 1.85f, 1.87f, 1.89f, 1.91f, 1.93f, 1.95f, 1.97f, 1.99f, 2.01f, 2.03f, 2.05f, 2.07f, 2.09f, 2.11f, 2.13f, 2.15f, 2.17f, 2.19f, 2.21f, 2.23f, 2.25f, 2.27f, 2.29f, 2.31f, 2.33f, 2.35f, 2.37f, 2.39f, 2.41f, 2.43f, 2.45f, 2.47f, 2.49f, 2.51f, 2.53f, 2.57f, 2.59f, 2.61f, 2.63f, 2.65f, 2.67f, 2.69f, 2.71f, 2.73f, // 20-99 is the 'normal' in play range
    2.78f, 2.83f, 2.88f, 2.93f, 2.98f, 3.03f, 3.08f, 3.13f, 3.18f, 3.23f, 3.28f, 3.33f, 3.38f, 3.43f, 3.48f, 3.53f, 3.58f, 3.63f, 3.68f, 3.73f, 3.78f, 3.83f, 3.88f, 3.93f, 3.98f, 4.03f,  // 100+ are exceptional values
};

static const char *drain_msg[STAT_NROF] =
{
    "Oh no! You are weakened!",
    "You're feeling clumsy!",
    "You feel less healthy",
    "You suddenly begin to lose your memory!",
    "Your spirit feels drained!",
    "Watch out, your mind is going!",
    "Your face gets distorted!",
};

static const char *gain_msg[STAT_NROF] =
{
    "You feel stronger.",
    "You feel more agile.",
    "You feel healthy.",
    "You feel smarter.",
    "You feel wiser.",
    "You feel more potent.",
    "You seem to look better.",
};

const char *lose_msg[STAT_NROF] =
{
    "You feel weaker!",
    "You feel clumsy!",
    "You feel less healthy!",
    "You feel stupid!",
    "You lose some of your memory!",
    "You feel less potent!",
    "You look ugly!",
};

const char *restore_msg[STAT_NROF] =
{
    "You feel your strength return.",
    "You feel your agility return.",
    "You feel your health return.",
    "You feel your memory return.",
    "You feel your wisdom return.",
    "You feel your spirits return.",
    "You feel your charisma return.",
};

const char *stat_name[STAT_NROF]  =
{
    "strength",
    "dexterity",
    "constitution",
    "intelligence",
    "wisdom",
    "power",
    "charisma",
};

const char *short_stat_name[STAT_NROF] =
{
    "Str",
    "Dex",
    "Con",
    "Int",
    "Wis",
    "Pow",
    "Cha",
};

/*The value (which can be negative) is added to the specified stat. */
void set_stat_value(living_t *stats, stat_nr_t stat, sint16 value)
{
    if (value == 0)
    {
        return;
    }

    switch (stat)
    {
        case STAT_STR:
        stats->Str += value;
        break;

        case STAT_DEX:
        stats->Dex += value;
        break;

        case STAT_CON:
        stats->Con += value;
        break;

        case STAT_INT:
        stats->Int += value;
        break;

        case STAT_WIS:
        stats->Wis += value;
        break;

        case STAT_POW:
        stats->Pow += value;
        break;

        case STAT_CHA:
        stats->Cha += value;
        break;

        default:
        LOG(llevBug, "BUG:: %s/set_stat_value(): Unknown stat (%d)!\n",
            __FILE__, stat);
    }
}

/* returns the specified stat.  See also set_stat_value(). */
stat_t get_stat_value(const living_t *const stats, const stat_nr_t stat)
{
    switch (stat)
    {
        case STAT_STR:
        return(stats->Str);

        case STAT_DEX:
        return(stats->Dex);

        case STAT_CON:
        return(stats->Con);

        case STAT_INT:
        return(stats->Int);

        case STAT_WIS:
        return(stats->Wis);

        case STAT_POW:
        return(stats->Pow);

        case STAT_CHA:
        return(stats->Cha);

        default:
        LOG(llevBug, "BUG:: %s/get_stat_value(): Unknown stat (%d)!\n",
            __FILE__, stat);
    }

    return 0;
}

/*
 * Ensures that all stats (str/dex/con/int/wis/pow/cha) are within the
 * stat limit.
 * not so "smart" as the solution before but simple, fast and easy.
 * MT-2004
 */

void check_stat_bounds(living_t *stats)
{
#if 0
    if (stats->Str > MAX_STAT)
        stats->Str = MAX_STAT;
    else if (stats->Str < MIN_STAT)
        stats->Str = MIN_STAT;

    if (stats->Dex > MAX_STAT)
        stats->Dex = MAX_STAT;
    else if (stats->Dex < MIN_STAT)
        stats->Dex = MIN_STAT;

    if (stats->Con > MAX_STAT)
        stats->Con = MAX_STAT;
    else if (stats->Con < MIN_STAT)
        stats->Con = MIN_STAT;

    if (stats->Int > MAX_STAT)
        stats->Int = MAX_STAT;
    else if (stats->Int < MIN_STAT)
        stats->Int = MIN_STAT;

    if (stats->Wis > MAX_STAT)
        stats->Wis = MAX_STAT;
    else if (stats->Wis < MIN_STAT)
        stats->Wis = MIN_STAT;

    if (stats->Pow > MAX_STAT)
        stats->Pow = MAX_STAT;
    else if (stats->Pow < MIN_STAT)
        stats->Pow = MIN_STAT;

    if (stats->Cha > MAX_STAT)
        stats->Cha = MAX_STAT;
    else if (stats->Cha < MIN_STAT)
        stats->Cha = MIN_STAT;
#else
    stat_t *stat;

    stat = &stats->Str;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Dex;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Con;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Int;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Wis;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Pow;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
    stat = &stats->Cha;
    *stat = MAX(MIN_STAT, MIN(*stat, MAX_STAT));
#endif
}

/*
 * This is mainly used by forces that can modify stats, which are caused by being hit with poison attacks, eating food, etc.
 * It's here to make sure the force doesn't make the player stats go out of range (< 1 or > MAX_STAT).
 */

object_t * check_obj_stat_buffs(object_t *ob, object_t *pl)
{
    if (ob->stats.Str <= -pl->stats.Str)
        ob->stats.Str = -(pl->stats.Str - 1);
    else if (ob->stats.Str > MAX_STAT - pl->stats.Str)
        ob->stats.Str = MAX_STAT - pl->stats.Str;

    if (ob->stats.Dex <= -pl->stats.Dex)
        ob->stats.Dex = -(pl->stats.Dex - 1);
    else if (ob->stats.Dex > MAX_STAT - pl->stats.Dex)
        ob->stats.Dex = MAX_STAT - pl->stats.Dex;

    if (ob->stats.Con <= -pl->stats.Con)
        ob->stats.Con = -(pl->stats.Con - 1);
    else if (ob->stats.Con > MAX_STAT - pl->stats.Con)
        ob->stats.Con = MAX_STAT - pl->stats.Con;

    if (ob->stats.Int <= -pl->stats.Int)
        ob->stats.Int = -(pl->stats.Int - 1);
    else if (ob->stats.Int > MAX_STAT - pl->stats.Int)
        ob->stats.Int = MAX_STAT - pl->stats.Int;

    if (ob->stats.Wis <= -pl->stats.Wis)
        ob->stats.Wis = -(pl->stats.Wis - 1);
    else if (ob->stats.Wis > MAX_STAT - pl->stats.Wis)
        ob->stats.Wis = MAX_STAT - pl->stats.Wis;

    if (ob->stats.Pow <= -pl->stats.Pow)
        ob->stats.Pow = -(pl->stats.Pow - 1);
    else if (ob->stats.Pow > MAX_STAT - pl->stats.Pow)
        ob->stats.Pow = MAX_STAT - pl->stats.Pow;

    if (ob->stats.Cha <= -pl->stats.Cha)
        ob->stats.Cha = -(pl->stats.Cha - 1);
    else if (ob->stats.Cha > MAX_STAT - pl->stats.Cha)
        ob->stats.Cha = MAX_STAT - pl->stats.Cha;

    return ob;
}

/* return 1 if we sucessfully changed a stat, 0 if nothing was changed. */
/* applied is set to 1 if we are applying the object, -1 if we are removing
 * the object.
 * It is the calling functions responsibilty to check to see if the object
 * can be applied or not.
 */
int change_abil(object_t *op, object_t *tmp)
{
    int    applied = (QUERY_FLAG(tmp, FLAG_APPLIED)) ? 1 : -1,
           i,
           j,
           success = 0;
    object_t refop;
    int    potion_max  = 0;

    /* remember what object was like before it was changed.  note that
     * refop is a local copy of op only to be used for detecting changes
     * found by fix_player.  refop is not a real object_t */
    memcpy(&refop, op, sizeof(object_t));

    if (op->type == PLAYER)
    {
        if (tmp->type == POTION)
        {
            for (j = 0; j < STAT_NROF; j++)
            {
                i = get_stat_value(&(CONTR(op)->orig_stats), j);

                /* Check to see if stats are within limits such that this can be
                 * applied.
                 */
                if (((i + applied * get_stat_value(&(tmp->stats), j))
                  <= (20 + tmp->stats.sp + get_stat_value(&(op->arch->clone.stats), j)))
                 && i
                  > 0)
                {
                    set_stat_value(&(CONTR(op)->orig_stats), j,
                                      (signed char) (applied * get_stat_value(&(tmp->stats), j)));
                    tmp->stats.sp = 0;/* Fix it up for super potions */
                }
                else
                {
                    /* potion is useless - player has already hit the natural maximum */
                    potion_max = 1;
                }
            }
            /* This section of code ups the characters normal stats also.  I am not
             * sure if this is strictly necessary, being that fix_player probably
             * recalculates this anyway.
             */
            for (j = 0; j < STAT_NROF; j++)
                set_stat_value(&(op->stats), j, (signed char) (applied * get_stat_value(&(tmp->stats), j)));
            check_stat_bounds(&(op->stats));
        } /* end of potion handling code */
    }

    /* reset attributes that fix_player doesn't reset since it doesn't search
     * everything to set */
    if (applied == -1)
    {
        op->path_attuned &= ~tmp->path_attuned,
        op->path_repelled &= ~tmp->path_repelled,
        op->path_denied &= ~tmp->path_denied;
    }
    /* call fix_player since op object could have whatever attribute due
     * to multiple items.  if fix_player always has to be called after
     * change_ability then might as well call it from here
     */
    FIX_PLAYER(op, "change_abil");

    if (tmp->attack[ATNR_CONFUSION])
    {
        success = 1;
        if (applied > 0)
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "Your hands begin to glow red.");
        else
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Your hands stop glowing red.");
    }
    if (QUERY_FLAG(op, FLAG_LIFESAVE) != QUERY_FLAG(&refop, FLAG_LIFESAVE))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You feel very protected.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You don't feel protected anymore.");
        }
    }
    if (QUERY_FLAG(op, FLAG_REFL_MISSILE) != QUERY_FLAG(&refop, FLAG_REFL_MISSILE))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "A magic force shimmers around you.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "The magic force fades away.");
        }
    }
    if (QUERY_FLAG(op, FLAG_REFL_CASTABLE) != QUERY_FLAG(&refop, FLAG_REFL_CASTABLE))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You feel more safe now, somehow.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Suddenly you feel less safe, somehow.");
        }
    }

    /* FIXME: This whole change_abil()/fix_player() paradigm is a stupid way to
     * handle this.
     *
     * -- Smacky 20151128 */
    if (QUERY_FLAG(tmp, FLAG_FLYING) ||
        QUERY_FLAG(tmp, FLAG_LEVITATE))
    {
        if (applied > 0)
        {
            success = 1;

            if (IS_AIRBORNE(&refop))
            {
                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You are already airborne but you feel a little more stable.");
            }
            else
            {
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You become airborne!");
            }
        }
        else
        {
            success = 1;

            if (IS_AIRBORNE(op))
            {
                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You are still airborne but you feel a little less stable.");
            }
            else
            {
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You are no longer airborne!");
            }
        }
    }

    /* becoming UNDEAD... a special treatment for this flag. Only those not
     * originally undead may change their status */
    if (!QUERY_FLAG(&op->arch->clone, FLAG_UNDEAD))
        if (QUERY_FLAG(op, FLAG_UNDEAD) != QUERY_FLAG(&refop, FLAG_UNDEAD))
        {
            success = 1;
            if (applied > 0)
            {
                SHSTR_FREE_AND_ADD_STRING(op->race, "undead");
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Your lifeforce drains away!");
            }
            else
            {
                if (op->arch->clone.race)
                {
                    SHSTR_FREE_AND_ADD_STRING(op->race, op->arch->clone.race);
                }
                else
                {
                    SHSTR_FREE(op->race);
                }

                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "Your lifeforce returns!");
            }
        }

    if (QUERY_FLAG(op, FLAG_STEALTH) != QUERY_FLAG(&refop, FLAG_STEALTH))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You walk more quietly.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You walk more noisily.");
        }
    }
    if (QUERY_FLAG(op, FLAG_SEE_INVISIBLE) != QUERY_FLAG(&refop, FLAG_SEE_INVISIBLE))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You see invisible things.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Your vision becomes less clear.");
        }
    }
    if (QUERY_FLAG(op, FLAG_IS_INVISIBLE) != QUERY_FLAG(&refop, FLAG_IS_INVISIBLE))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You become transparent.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You can see yourself.");
        }
    }
    /* blinded you can tell if more blinded since blinded player has minimal
     * vision */
    if (QUERY_FLAG(tmp, FLAG_BLIND))
    {
        success = 1;

        if (applied)
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You are blinded.");
            SET_FLAG(op, FLAG_BLIND);
            if (op->type == PLAYER)
                CONTR(op)->update_los = 1;
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "Your vision returns.");
            CLEAR_FLAG(op, FLAG_BLIND);
            if (op->type == PLAYER)
                CONTR(op)->update_los = 1;
        }
    }

    if (QUERY_FLAG(op, FLAG_SEE_IN_DARK) != QUERY_FLAG(&refop, FLAG_SEE_IN_DARK))
    {
        success = 1;
        if (applied > 0)
        {
            ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "Your vision is better in the dark.");
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You see less well in the dark.");
        }
    }

    if (QUERY_FLAG(op, FLAG_XRAYS) != QUERY_FLAG(&refop, FLAG_XRAYS))
    {
        success = 1;

        if (applied)
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Everything becomes transparent.");
            if (op->type == PLAYER)
                CONTR(op)->update_los = 1;
        }
        else
        {
            ndi(NDI_UNIQUE | NDI_GREY, 0, op, "Everything suddenly looks very solid.");
            if (op->type == PLAYER)
                CONTR(op)->update_los = 1;
        }
    }

    if (op->type == PLAYER &&
        tmp->type != TYPE_FOOD_FORCE &&
        tmp->subtype != SUBTYPE_FORCE_DOT)
    {
        if ((tmp->stats.hp || tmp->stats.maxhp))
        {
            success = 1;
            if (applied * tmp->stats.hp > 0 || applied * tmp->stats.maxhp > 0)
                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You feel much more healthy!");
            else
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You feel much less healthy!");
        }

        if ((tmp->stats.sp || tmp->stats.maxsp) && tmp->type != TYPE_SKILL)
        {
            success = 1;
            if (applied * tmp->stats.sp > 0 || applied * tmp->stats.maxsp > 0)
                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You feel one with the powers of magic!");
            else
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You suddenly feel very mundane.");
        }

        if ((tmp->stats.grace || tmp->stats.maxgrace))
        {
            success = 1;
            if (applied * tmp->stats.grace > 0 || applied * tmp->stats.maxgrace)
                ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "You feel closer to your deity!");
            else
                ndi(NDI_UNIQUE | NDI_GREY, 0, op, "You suddenly feel less holy.");
        }
    }

    /* Messages for changed resistance */
    for (i = 0; i < NROFATTACKS; i++)
    {
        if (op->resist[i] != refop.resist[i])
        {
            success = 1;
            if (op->resist[i] > refop.resist[i])
            {
                ndi(NDI_UNIQUE | NDI_GREEN, 0, op, "Your resistance to %s rises to %d%%.",
                              attack_name[i].name, op->resist[i]);
            }
            else
            {
                ndi(NDI_UNIQUE | NDI_BLUE, 0, op, "Your resistance to %s drops to %d%%.",
                              attack_name[i].name, op->resist[i]);
            }
        }
    }

    if (tmp->type != TYPE_SKILLGROUP && !potion_max)
    {
        for (j = 0; j < STAT_NROF; j++)
        {
            if ((i = get_stat_value(&(tmp->stats), j)) != 0)
            {
                success = 1;
                if (i * applied > 0)
                    ndi(NDI_UNIQUE | NDI_WHITE, 0, op, "%s", gain_msg[j]);
                else
                    ndi(NDI_UNIQUE | NDI_GREY, 0, op, "%s", lose_msg[j]);
            }
        }
    }
    return success;
}

/*
 * Stat draining by Vick 930307
 * (Feeling evil, I made it work as well now.  -Frank 8)
 */

void drain_stat(object_t *op)
{
    drain_specific_stat(op, RANDOM() % STAT_NROF);
}

void drain_specific_stat(object_t *op, int deplete_stats)
{
    object_t     *tmp;
    static archetype_t  *at = NULL;

    if (!at)
    {
        at = find_archetype("depletion");
        if (!at)
        {
            LOG(llevBug, "BUG: Couldn't find archetype depletion.\n");
            return;
        }
    }

    tmp = present_arch_in_ob(at, op);
    if (!tmp)
    {
        tmp = arch_to_object(at);
        tmp = insert_ob_in_ob(tmp, op);
        SET_FLAG(tmp, FLAG_APPLIED);

        if(op->type != PLAYER) /* create a temp., self destructing force for mobs */
        {
            tmp->stats.food = 122+(RANDOM()%70);
            tmp->speed = 1.0;
            update_ob_speed(tmp);
            SET_FLAG(tmp, FLAG_IS_USED_UP);
        }
    }

    set_stat_value(&tmp->stats, deplete_stats, -1);
    if(op->type == PLAYER)
        ndi(NDI_UNIQUE, 0, op, "%s", drain_msg[deplete_stats]);

    FIX_PLAYER(op, "drain_specific_stat");
}

/* Drain the "real" level of a player or mob.
 * This can be permanent or temporary (if we use drain on a mob).
 * It works more or less like the depletion force
 * mode: 0 means permanent(until active removed), 1 means temporary,
 * then the effect removes itself.
 */
void drain_level(object_t *op, int level, int mode, int ticks)
{
    object_t *force;
    int original_level = op->level;

    if (op->level <= 1) /* level 1 mobs can't get drained any further */
        return;

    if(mode==0)
        force = present_arch_in_ob(archetype_global._drain, op);
    else
        force = present_arch_in_ob_temp(archetype_global._drain, op);
    if (!force)
    {
        force = arch_to_object(archetype_global._drain);
        force = insert_ob_in_ob(force, op);
        SET_FLAG(force, FLAG_APPLIED);

        if(mode) /* create a temp., self destructing force */
        {
            force->stats.food = ticks;
            force->speed = 1.0;
            update_ob_speed(force);
            SET_FLAG(force, FLAG_IS_USED_UP);
        }
    }

    /* we don't want drain to make the mob's level be at a value that shouldn't exist */
    original_level = force->level + op->level;
    force->level += level;
    if (force->level < 1 || force->level >= original_level)
        force->level = original_level - 1; /* cap force->level at one below the mob's original undrained level */
    FIX_PLAYER(op, "drain_level"); /* will redirect to fix_monster() automatically */
    if(op->type == PLAYER)
        ndi(NDI_UNIQUE, 0, op, "You lose a level!");
}

/* helper function to catch double applied items from same type (like 2 helms at once for example) */
static inline int set_player_equipment(player_t *pl, object_t *optr, int num)
{
    /* this should not happen ... */
    if(pl->equipment[num])
    {
        CLEAR_FLAG(optr, FLAG_APPLIED);
        OBJECT_UPDATE_UPD(optr, UPD_FLAGS);
        LOG(llevDebug,"FIX_PLAYER BUG: Item %s for player %s on place %d is already set!\n", STRING_OBJ_NAME(optr), STRING_OBJ_NAME(pl->ob), num);
        return 0;
    }

    pl->equipment[num] = optr;

    return 1;
}

/* calculate speed through carrying, weight & encumbrance for fix_player() and fix_player_weight()
 * ignore some unlogical glitches. This code is in preparing for smooth/pixelwise moving and animation
 */
static inline void set_speed_encumbrance(object_t *op, player_t *pl)
{
    int tmp_speed_enc = pl->speed_enc;

    /* calculate the increase/decrease multiplier */
    pl->speed_enc = 1000 - (pl->speed_enc_base + pl->speed_reduce_from_disease);

    /* now... if we carry more as weight_linit-encumbrance limit (aka around 65%) then we have a speed decrease */
    if(op->carrying > (sint32) (pl->weight_limit - pl->speed_enc_limit))
    {
        float speed_factor;

        /* the * 1.15f is the trick. We add 15% because we don't want 0% speed (rooted) */
        speed_factor = (float)(op->carrying - (sint32) (pl->weight_limit - pl->speed_enc_limit))
            / ((float)pl->speed_enc_limit * 1.15f);
        /* speed_factor should be now something between 0.15 and x.0 (when weight ins normal) */

        /* we never "overtaxe" like in nethack. We want avoid a "side effect" rooting effect when
        * we draw away strength. The decreased damage and base speed is worse enough.
        * We *always* allow with 15% min moving
        */
        if (speed_factor < 0.0f)
            speed_factor = 0.0f;
        else if (speed_factor > 0.85f) /* we nerver lose more as 85% of our speed by weight. unlogical, but key game element */
            speed_factor = 0.85f;

        pl->speed_enc -= (int)(speed_factor*1000.0f); /* remove the missing f % from speed_enc */
    }

    if(pl->speed_enc == tmp_speed_enc) /* speed is unchanged! */
        return;

    if(pl->speed_enc < 150)
        pl->speed_enc = 150;
    else if(pl->speed_enc > 1000)
        pl->speed_enc = 1000;

    /* now we have ->speed filled with our base speed and speed_enc with % of speed we are allowed */
    op->speed = op->arch->clone.speed * (((float)pl->speed_enc) / 1000.0f);

    /*LOG(llevDebug, "SPEED - FIX_PLAYER(%s): enc=%d%% speed = %f\n", STRING_OBJ_NAME(op), pl->speed_enc, op->speed);*/

    /* lets have a fair min. speed.
    * When we have added smooth scrolling, the whole
    * handling will change so or so. MT-06.2005
    */
    if (op->speed < 0.15f)
        op->speed = 0.15f; /* ouch */
    else if (op->speed > 1.0f)
        op->speed = 1.0f;
    update_ob_speed(op);
}

/* this is a special function handling inventory changes for a player
 * where the result is only a different weight - and the issues (speed) around it.
 * Its for example called from split object functions
 * It can be called instead of fix_player() when we are sure that nothing has
 * changed as the player carrying value.
 * We adjust speed and send the new values to the client.
 */
void fix_player_weight(object_t *op)
{
    if(!op || !CONTR(op) || op->carrying == CONTR(op)->carrying_last)
        return;

    set_speed_encumbrance(op, CONTR(op));
}

/* ADJTHAC() outputs the latest thac0/m score according to its inputs.
 *
 * V is the current thac* score (ie, after previous ADJTHAC() calls) and is
 * is sint16, and N is the number to adjust this by (ie, the thac* of an
 * equipped piece of gear) and is sint8. The output (which will be V next
 * call and is therefore also sint16) depends on the values of these inputs
 * and a few simple rules of logic.
 *
 * A player's/monster's personal thac* is capped at -25 to 125. So thacm below
 * 1 means a guaranteed not fumble and thacm above 99 means a guaranteed
 * fumble. Similarly, thac0 below 1 means a guaranteed direct hit and thac0
 * above 100 means a guaranteed not direct hit. Values between those lows and
 * highs mean a roughly percentage chance of fumble or not direct hit
 * respectively. All this means equipment can adjust a player's thac* one way
 * while other equipment can adjust it the other.
 *
 * Also, a single piece of equipment can force it to one or other extreme.
 * This is done by giving the equipment thac* <= -126 or >= 126 (these ranges
 * of values are just to cover all possible signed 8 bit numbers but I
 * recommend arches/maps consistently use -127 or 127 for clarity). When
 * forced in this way, this effectively overrides the normal +/- of other
 * equipment.
 *
 * Additionally, while a -127 value can be overwritten by a subsequent 127, a
 * 127 cannot be changed by a -127. This means that an item of always fumble
 * [thacm 127] or never direct hit [thac0 127] always negates everything else
 * (remember too that fumble is tested before direct hit so takes precedence).
 * So Damned FumblyJoe's Beany of Ineptitude [thacm 127] is a very bad thing
 * to put on your head. */
/* For a blow-by-blow account of the development of this, see:
 * https://www.daimonin.org/12119/thac-update/ */
#define ADJTHAC(_V_, _N_) \
    (((_N_) == 0 || (_V_) == 32767) \
     ? (_V_) \
     : (((_N_) <= -126 || (_N_) >= 126) \
        ? SGN((_N_)) * 32767 \
        : (((_V_) == -32767) \
           ? (_V_) \
           : MAX(-32766, MIN((_V_) + (_N_), 32766)))))

/*
 * Updates all abilities given by applied objects in the inventory
 * of the given object.  Note: This function works for both monsters
 * and players; the "player" in the name is purely an archaic inheritance.
 */
/* July 95 - inserted stuff to handle new skills/exp system - b.t.
   spell system split, grace points now added to system  --peterm
 */
/* Oct 02 - i reworked this function in all parts. Beside a major speed up, i split it
 * in a player and a monster function. All calls goes still to fix_player, but it filters
 * the mobs out and call fix_monster. This function is still a heavy weight. There
 * is a lot of abuse and redundant call of this function, so it is worth to monitor it. MT
 */
#ifdef DEBUG_FIX_PLAYER
void fix_player(object_t *op, char *debug_msg)
#else
void fix_player(object_t *op)
#endif
{
    int                 snare_penalty = 0,slow_penalty = 0, ring_count = 0, skill_level_drain=0, skill_level_max = 1;
    int                 tmp_item, old_glow, max_boni_hp = 0, max_boni_sp = 0, max_boni_grace = 0;
    int                 i, j, inv_flag, inv_see_flag, light, ac;
    float               weapon_speed_mod = 0.0f;
    sint8               tmpthac0;
    sint8               tmpthacm;
    sint16              adjthac0;
    sint16              adjthacm;
    int                 temp_fumble=0;
    int                 resists_boni[NROFATTACKS], resists_mali[NROFATTACKS];
    int                 potion_resist_boni[NROFATTACKS], potion_resist_mali[NROFATTACKS], potion_attack[NROFATTACKS];
    object_t           *tmp, *tmp_ptr, *skill_weapon = NULL, *applied_skill = NULL;
    player_t           *pl;
    float               f;
    msp_t               *msp;
    uint32              opflags[NUM_FLAGS_32];
    sint32 maxhp;
    sint32 hp;
    sint16 maxsp;
    sint16 sp;
    sint16 maxgrace;
    sint16 grace;

    /*LOG(llevDebug,"FIX_PLAYER called (%s} %s\n", STRING_OBJ_NAME(op), QUERY_FLAG(op, FLAG_NO_FIX_PLAYER)?"IGNORED":"");*/
    if (QUERY_FLAG(op, FLAG_NO_FIX_PLAYER))
    {
        /* we are calling fix_player with this flag for example when manually applying an item */
#ifdef DEBUG_FIX_PLAYER_SKIPPED
        LOG(llevDebug, "FIX_PLAYER(%s [%x]): >> *SKIP*\n", STRING_OBJ_NAME(op), op->count);
#endif
        return;
    }

    msp = MSP_KNOWN(op);

    /* ok, in crossfire, fix_player is called for objects not for players
     * we redirect mobs to fix_monster() and let only player pass
     */
    if (op->type == MONSTER)
    {
#ifdef DEBUG_FIX_PLAYER
        LOG(llevDebug, "fix_player(%s [%x]): >> non player - redirect to fix_monster (%s)\n",
            STRING_OBJ_NAME(op),op->count, debug_msg);
#else
        LOG(llevDebug, "fix_player(%s [%x]): >> non player - redirect to fix_monster\n",
            STRING_OBJ_NAME(op),op->count);
#endif
        fix_monster(op);
        return;
    }

    /* for secure */
    if (op->type != PLAYER)
    {
#ifdef DEBUG_FIX_PLAYER
        LOG(llevDebug, "fix_player(): called from non Player/Mob object: %s [%x] (type %d) (%s)\n",
                STRING_OBJ_NAME(op), op->count, op->type, debug_msg);
#else
        LOG(llevDebug, "fix_player(): called from non Player/Mob object: %s [%x] (type %d)\n",
            STRING_OBJ_NAME(op), op->count, op->type);
#endif
        return;
    }

#ifdef DEBUG_FIX_PLAYER
    LOG(llevDebug, "FIX_PLAYER(%s [%x]): >> %s\n", STRING_OBJ_NAME(op), op->count, debug_msg);
#endif

    /* Remember op->flags. They PROBABLY won't be changed here but various
     * illegalities can force an inv object to be unapplied, etc which
     * presumably can have knock-on effects. */
    for (i = 0; i < NUM_FLAGS_32; i++)
    {
        opflags[i] = op->flags[i];
    }

    /* This avoids looping this function if, for example, we need to insert or
     * remove objects during the fix. Of course that should be avoided anyway,
     * but sometimes may be necessary for a real fix (eg, validate_skills()).
     * This flag is cleared again at the end of the function. */
    SET_FLAG(op, FLAG_NO_FIX_PLAYER);

    pl = CONTR(op);
    inv_flag = inv_see_flag = ac = 0;

    pl->speed_enc_base = pl->speed_reduce_from_disease = 0;
    pl->speed_enc = pl->dist_dps = pl->dist_wc = -1;
    pl->dist_action_time = 0;
    op->stats.Str = pl->orig_stats.Str;
    op->stats.Dex = pl->orig_stats.Dex;
    op->stats.Con = pl->orig_stats.Con;
    op->stats.Int = pl->orig_stats.Int;
    op->stats.Wis = pl->orig_stats.Wis;
    op->stats.Pow = pl->orig_stats.Pow;
    op->stats.Cha = pl->orig_stats.Cha;
    validate_skills(pl);
    pl->guild_force = pl->selected_weapon = pl->skill_weapon = NULL;
    pl->quest_one_drop = pl->quests_done = pl->quests_type_kill = pl->quests_type_normal = NULL;
    pl->wc_bonus = pl->dam_bonus = pl->spell_fumble = pl->exp_bonus = pl->encumbrance = 0;
    pl->set_skill_weapon = pl->set_skill_archery = NO_SKILL_READY;
    pl->reserved_hp = pl->reserved_sp = pl->reserved_grace = 0;
    /* the default skill groups for non guild players */
    /* FIXME: We should just read the guild_force arch defaults here. */
    /* As this is a default, only give 50% of the exp that you would actually
     * get if you'd really trained those skills -- see aggro.c. Guild members
     * get a better exchange rate (encourages guild membership).
     * -- Smacky 20100113 */
    pl->base_skill_group[0] = SKILLGROUP_PHYSIQUE;
    pl->base_skill_group[1] = SKILLGROUP_AGILITY;
    pl->base_skill_group[2] = SKILLGROUP_WISDOM;
    pl->base_skill_group_exp[0] = 25;
    pl->base_skill_group_exp[1] = 15;
    pl->base_skill_group_exp[2] = 10;

    /* for players, we adjust with the values */
    op->weight_limit = op->arch->clone.weight_limit;

    /* Start with the racial base for thac*. Note that this starts the ball
     * rolling and follows exactly the rules for ADJTHAC(). So, for example, a
     * race with thac0 0 will always direct hit, unless he has crappy
     * equipment (or a few other external factors). */
    adjthac0 = op->arch->clone.stats.thac0;
    adjthacm = op->arch->clone.stats.thacm;

    /* natural wc/ac/dam settings */
    op->stats.wc = op->arch->clone.stats.wc;
    op->stats.ac = op->arch->clone.stats.ac;
    op->stats.dam = op->arch->clone.stats.dam;

    op->stats.maxhp = op->arch->clone.stats.maxhp;
    op->stats.maxsp = op->arch->clone.stats.maxsp;
    op->stats.maxgrace = op->arch->clone.stats.maxgrace;

    pl->levhp[1] = (char) op->stats.maxhp;
    pl->levsp[1] = (char) op->stats.maxsp + 15;
    pl->levgrace[1] = (char) op->stats.maxgrace + 15;

    old_glow = op->glow_radius;
    light = op->arch->clone.glow_radius;

    op->speed = op->arch->clone.speed;
    op->weapon_speed = op->arch->clone.weapon_speed;
    op->path_attuned = op->arch->clone.path_attuned;
    op->path_repelled = op->arch->clone.path_repelled;
    op->path_denied = op->arch->clone.path_denied;
    op->terrain_flag = op->arch->clone.terrain_flag;        /* reset terrain moving abilities */

    /* only adjust skills which has no own level/exp values */
    if (op->chosen_skill &&
        op->chosen_skill->last_eat == NONLEVELING &&
        op->chosen_skill->skillgroup)
    {
        op->chosen_skill->level = op->chosen_skill->skillgroup->level;
    }

    pl->gen_sp = pl->gen_grace = pl->gen_hp = 20;

    SHSTR_FREE(op->slaying);

    /* HOTFIX: we parted refl_xxx from can_refl_xxx */
    CLEAR_FLAG(op, FLAG_REFL_MISSILE);
    CLEAR_FLAG(op, FLAG_REFL_CASTABLE);

    CLEAR_FLAG(op, FLAG_EATING);

    if (QUERY_FLAG(op, FLAG_IS_INVISIBLE))
        inv_flag = 1;
    if (QUERY_FLAG(op, FLAG_SEE_INVISIBLE))
        inv_see_flag = 1;
    if (!QUERY_FLAG(&op->arch->clone, FLAG_XRAYS))
        CLEAR_FLAG(op, FLAG_XRAYS);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_CAN_PASS_THRU))
        CLEAR_MULTI_FLAG(op, FLAG_CAN_PASS_THRU);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_IS_ETHEREAL))
        CLEAR_MULTI_FLAG(op, FLAG_IS_ETHEREAL);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_IS_INVISIBLE))
        CLEAR_MULTI_FLAG(op, FLAG_IS_INVISIBLE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_SEE_INVISIBLE))
        CLEAR_FLAG(op, FLAG_SEE_INVISIBLE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_LIFESAVE))
        CLEAR_FLAG(op, FLAG_LIFESAVE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_STEALTH))
        CLEAR_FLAG(op, FLAG_STEALTH);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_BLIND))
        CLEAR_FLAG(op, FLAG_BLIND);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_SLOWED))
        CLEAR_FLAG(op, FLAG_SLOWED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_FEARED))
        CLEAR_FLAG(op, FLAG_FEARED);
    /* rooted is set when a snare effect has reached 100% */
    if (!QUERY_FLAG(&op->arch->clone, FLAG_ROOTED))
        CLEAR_FLAG(op, FLAG_ROOTED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_CONFUSED))
        CLEAR_FLAG(op, FLAG_CONFUSED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_PARALYZED))
        CLEAR_FLAG(op, FLAG_PARALYZED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_FLYING))
        CLEAR_MULTI_FLAG(op, FLAG_FLYING);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_LEVITATE))
        CLEAR_MULTI_FLAG(op, FLAG_LEVITATE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_REFL_CASTABLE))
        CLEAR_FLAG(op, FLAG_REFL_CASTABLE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_REFL_MISSILE))
        CLEAR_FLAG(op, FLAG_REFL_MISSILE);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_UNDEAD))
        CLEAR_FLAG(op, FLAG_UNDEAD);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_SEE_IN_DARK))
        CLEAR_FLAG(op, FLAG_SEE_IN_DARK);

    memset(&potion_resist_boni, 0, sizeof(potion_resist_boni));
    memset(&potion_resist_mali, 0, sizeof(potion_resist_mali));
    memset(&potion_attack, 0, sizeof(potion_attack));

    /* initializing player arrays from the values in player archetype clone:  */
    memset(&pl->equipment, 0, sizeof(pl->equipment));
    memcpy(&op->attack, &op->arch->clone.attack, sizeof(op->attack));
    memcpy(&op->resist, &op->arch->clone.resist, sizeof(op->resist));
    for (i = 0; i < NROFATTACKS; i++)
    {
        if (op->resist[i] > 0)
        {
            resists_boni[i] = op->resist[i];
            resists_mali[i] = 0;
        }
        else
        {
            resists_mali[i] = -(op->resist[i]);
            resists_boni[i] = 0;
        }
    }
    for (i = 0; i < NROFSKILLGROUPS; i++)
        pl->highest_skill[i]=NULL;

    /* ok, now we browse the inventory... there is not only our equipment - there
     * are all our skills, forces and hidden system objects.
     */
    for (tmp = op->inv; tmp != NULL; tmp = tmp_ptr)
    {
        uint32 tmpflags[NUM_FLAGS_32];

        /* Remember tmp->flags. They PROBABLY won't be changed here but various
         * illegalities can force an object to be unapplied, etc. */
        for (i = 0; i < NUM_FLAGS_32; i++)
        {
            tmpflags[i] = tmp->flags[i];
        }

        tmp_ptr = tmp->below;
        /*
         * add here more types we can and must skip.
         */
        if (tmp->type == SCROLL
             || tmp->type == TYPE_SKILLGROUP
             || tmp->type == POTION
             || tmp->type == CONTAINER
             || tmp->type == TYPE_LIGHT_REFILL
             || tmp->type == MONSTER)
            continue;
        else if(tmp->type == TYPE_GUILD_FORCE && tmp->subtype == SUBTYPE_GUILD_IN)
        {
            pl->guild_force = tmp;
            op->stats.Str += tmp->stats.Str;
            op->stats.Int += tmp->stats.Int;
            op->stats.Dex += tmp->stats.Dex;
            op->stats.Con += tmp->stats.Con;
            op->stats.Wis += tmp->stats.Wis;
            op->stats.Pow += tmp->stats.Pow;
            op->stats.Cha += tmp->stats.Cha;
            tmpthac0 = tmp->stats.thac0;
            tmpthacm = tmp->stats.thacm;
            adjthac0 = ADJTHAC(adjthac0, tmpthac0);
            adjthacm = ADJTHAC(adjthacm, tmpthacm);
            op->stats.ac += tmp->stats.ac;
            op->stats.dam += tmp->stats.dam;
            op->stats.wc += tmp->stats.wc;
            op->stats.maxhp += tmp->stats.maxhp;
            op->stats.maxsp += tmp->stats.maxsp;
            op->stats.maxgrace += tmp->stats.maxgrace;
            pl->gen_hp += tmp->stats.hp;
            pl->gen_sp += tmp->stats.sp;
            pl->gen_grace += tmp->stats.grace;
            op->weapon_speed -= tmp->weapon_speed;
            pl->weapon_sp -= (int)(tmp->weapon_speed * pticks_second);
            pl->base_skill_group[0] = tmp->last_eat;
            pl->base_skill_group[1] = tmp->last_sp;
            pl->base_skill_group[2] = tmp->last_heal;
            pl->base_skill_group_exp[0] = tmp->last_grace;
            pl->base_skill_group_exp[1] = tmp->magic;
            pl->base_skill_group_exp[2] = tmp->state;

            for (i=0; i<NROFATTACKS; i++)
            {
                if (tmp->resist[i] > 0)
                    resists_boni[i] += tmp->resist[i];
                else
                    resists_mali[i] -= tmp->resist[i];
                op->attack[i] += tmp->attack[i];
            }
            continue;
        }
        else if(tmp->type == TYPE_QUEST_CONTAINER)
        {
            /* one drop container */
            /* TODO: this should be replaced with a switch statement */
            if(tmp->subtype == SUBTYPE_QUEST_CONTAINER_ONEDROP)
            {
                pl->quest_one_drop = tmp;
                pl->quest_one_drop_count = tmp->count;
            }
            else if(tmp->subtype == SUBTYPE_QUEST_CONTAINER_DONE)
            {
                pl->quests_done = tmp;
                pl->quests_done_count = tmp->count;
            }
            else if(tmp->subtype == SUBTYPE_QUEST_CONTAINER_NORMAL)
            {
                pl->quests_type_normal = tmp;
                pl->quests_type_normal_count = tmp->count;
            }
            else if(tmp->subtype == SUBTYPE_QUEST_CONTAINER_KILL)
            {
                pl->quests_type_kill = tmp;
                pl->quests_type_kill_count = tmp->count;
            }
            else /* this really should not happens... */
            {
                LOG(llevBug,"BUG: fix_player(): found illegal quest container (st: %d) in player %s\n",
                    tmp->subtype, STRING_OBJ_NAME(op));
                remove_ob(tmp);
            }
            continue;
        }
        /* all skills, not only the applied ones */
       else  if (tmp->type == TYPE_SKILL)
        {
            /* create list of highest skills */
            if(!pl->highest_skill[tmp->magic] || tmp->stats.exp > pl->highest_skill[tmp->magic]->stats.exp)
                pl->highest_skill[tmp->magic] = tmp;

            /* TODO: no "crafting/trade" skills here */
            /* get highest single skill - thats our "main" skill */
            if (tmp->level > skill_level_max)
                skill_level_max = tmp->level;
            /* lets remember the best bare hand skill */
            if (tmp->stats.dam > 0)
            {
                if (!skill_weapon || skill_weapon->stats.dam < tmp->stats.dam)
                    skill_weapon = tmp;
            }
        }

        /* this is needed, because our applied light can be overruled by a light giving
        * object like holy glowing aura force or something
        */
        if (tmp->glow_radius > light)
        {
            /* don't use this item when it is a 'not applied TYPE_LIGHT_APPLY' */
            if (tmp->type != TYPE_LIGHT_APPLY || QUERY_FLAG(tmp, FLAG_APPLIED))
                light = tmp->glow_radius;
        }

        /* this checks all applied items in the inventory */
        if (QUERY_FLAG(tmp, FLAG_APPLIED))
        {
            switch (tmp->type) /* still applied stuff */
            {
                case WAND:
                case ROD:
                case HORN:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_BOW))
                        continue;

                    for (i = 0; i < STAT_NROF; i++)
                        set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));
                    break;

                /* throw/arrow dam & wc are dynmically calculated in the do_throw() function */
                case ARROW:
                    if(tmp->subtype < 127) /* its amun, not a throw weapon */
                    {
                        if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_AMUN))
                            continue;
                        break;
                    }

                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_BOW))
                        continue;
                break;

                /* bow dam/wc are also added in the fire functions */
                case BOW:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_BOW))
                        continue;

                    if (tmp->subtype == RANGE_WEAP_BOW)
                        pl->set_skill_archery = SK_RANGE_BOW;
                    else if (tmp->subtype == RANGE_WEAP_XBOWS)
                        pl->set_skill_archery = SK_RANGE_XBOW;
                    else
                        pl->set_skill_archery = SK_RANGE_SLING;

                    for (i = 0; i < STAT_NROF; i++)
                        set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));

                    break;

                case WEAPON:
                  /* Weapons need weapon speed. If it has none, lets unapply
                   * it. We could use a default value but if it fails this
                   * basic requirement, who knows what else is wrong with
                   * it? */
                  if (!tmp->weapon_speed)
                  {
                      LOG(llevBug, "BUG:: %s:fix_player(): %s applied weapon %s[%d] without weapon speed -- unapplying!\n",
                          __FILE__, STRING_OBJ_NAME(op), STRING_OBJ_NAME(tmp), TAG(tmp));
                      ndi(NDI_UNIQUE | NDI_RED, 0, op, "You sense that your weapon is bugged so you unapply it!");
                      (void)apply_equipment(op, tmp, AP_UNAPPLY | AP_IGNORE_CURSE);
                      pl->equipment[PLAYER_EQUIP_WEAPON1] = NULL;

                      continue;
                  }
                  else
                  {
                      if (!set_player_equipment(pl, tmp, PLAYER_EQUIP_WEAPON1))
                      {
                          continue;
                      }

                      pl->selected_weapon = tmp; /* thats our weapon */
                      pl->encumbrance += (sint16) (3 * tmp->weight / 1000);

                      if ((i = tmp->subtype % 4) == WEAP_1H_IMPACT)
                      {
                          pl->set_skill_weapon = SK_MELEE_IMPACT;
                      }
                      else if (i == WEAP_1H_SLASH)
                      {
                          pl->set_skill_weapon = SK_MELEE_SLASH;
                      }
                      else if (i == WEAP_1H_CLEAVE)
                      {
                          pl->set_skill_weapon = SK_MELEE_CLEAVE;
                      }
                      else
                      {
                          pl->set_skill_weapon = SK_MELEE_PIERCE;
                      }

                      /* wc and damage of a melee weapon only count for melee! */
                      op->stats.wc += (tmp->stats.wc + tmp->magic); /* add the weapon wc direct to the player wc */
                      op->stats.dam = (tmp->stats.dam + tmp->magic);
                      op->weapon_speed = tmp->weapon_speed;

                      if (tmp->slaying)
                      {
                          SHSTR_FREE_AND_ADD_STRING(op->slaying, tmp->slaying);
                      }

                      ac += tmp->stats.ac;
                      temp_fumble += tmp->last_heal;
                      tmpthac0 = tmp->stats.thac0;
                      tmpthacm = tmp->stats.thacm;
                      adjthac0 = ADJTHAC(adjthac0, tmpthac0);
                      adjthacm = ADJTHAC(adjthacm, tmpthacm);
                      for (i = 0; i < STAT_NROF; i++)
                      {
                          set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));
                      }
                  }

                  break;

                case TYPE_LIGHT_APPLY:
                    if (tmp->glow_radius > light)
                        light = tmp->glow_radius;
                    break;

                case TYPE_SKILL:
                    applied_skill = tmp;
                    break;

                    /* all armours + rings and amulets */
                case RING:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_RRING + ring_count))
                        continue;
                  ring_count++;
                  goto fix_player_no_armour;
                case AMULET:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_AMULET))
                        continue;
                  if(tmp->last_grace)
                      pl->exp_bonus += tmp->last_grace;
                  goto fix_player_no_armour;

                case BRACERS:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_BRACER))
                        continue;
                  goto fix_player_jump1;
                case ARMOUR:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_MAIL))
                        continue;
                  pl->encumbrance += (int) tmp->weight / 1000;
                  goto fix_player_jump1;
                case SHIELD:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_SHIELD))
                        continue;
                  pl->encumbrance += (int) tmp->weight / 2000;
                  goto fix_player_jump1;
                case GIRDLE:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_GIRDLE))
                        continue;
                  goto fix_player_jump1;
                case HELMET:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_HELM))
                        continue;
                  goto fix_player_jump1;
                case SHOULDER:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_SHOULDER))
                        continue;
                    goto fix_player_jump1;
                case LEGS:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_LEGS))
                        continue;
                    goto fix_player_jump1;
                case BOOTS:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_BOOTS))
                        continue;
                  goto fix_player_jump1;
                case GLOVES:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_GAUNTLET))
                        continue;
                  goto fix_player_jump1;
                case CLOAK:
                    if(!set_player_equipment(pl, tmp, PLAYER_EQUIP_CLOAK))
                        continue;

                  fix_player_jump1:
                  /* thats now used for ALL armours except rings and amulets */
                  pl->speed_enc_base += tmp->last_sp;

                  fix_player_no_armour: /* jump in for non armour like rings, ... */
                  /* i must control this - at last food has for used_up forces
                             * a differnet meaning
                             * add some of this below when used from other applied objects too!
                             */
                  max_boni_hp += tmp->stats.maxhp;
                  max_boni_sp += tmp->stats.maxsp;
                  max_boni_grace += tmp->stats.maxgrace;
                  pl->gen_sp += tmp->stats.sp;
                  pl->gen_grace += tmp->stats.grace;
                  pl->gen_hp += tmp->stats.hp;
                  temp_fumble += tmp->last_heal; /* spell fumble chance */

                  /* I moved a line from here down. Things here ONLY apply when the pl
                     is actually wearing armour, which means that natural stat boni
                     are only applied when they have something on. Other issues may
                     come up with this fact.
                  */

                  tmpthac0 = tmp->stats.thac0;
                  tmpthacm = tmp->stats.thacm;
                  adjthac0 = ADJTHAC(adjthac0, tmpthac0);
                  adjthacm = ADJTHAC(adjthacm, tmpthacm);

                  for (i = 0; i < STAT_NROF; i++)
                      set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));

                  if (tmp->stats.wc)
                      pl->wc_bonus += (tmp->stats.wc + tmp->magic);
                  if (tmp->stats.dam)
                      pl->dam_bonus += (tmp->stats.dam + tmp->magic);
                  if (tmp->stats.ac)
                      ac += (tmp->stats.ac + tmp->magic);
                  break;

                  /* more exotic stuff! */

                  /* now calc resistance and stuff for all the rest applyable objects! */
                  /* i am not 100% sure this is safe for *all* objects - i have used for that
                         * reason not default here.
                         */
                case TYPE_FOOD_FORCE:
                  SET_FLAG(op, FLAG_EATING);
                  goto fix_player_jump_resi;

                case TYPE_AGE_FORCE:
                  pl->age_force = tmp; /* store our age force */
                  pl->age = tmp->stats.hp;
                  pl->age_max = tmp->stats.maxhp;
                  pl->age_add = tmp->stats.sp;
                  pl->age_changes = tmp->stats.grace;
                  if (pl->age >= (sint16) (((float) pl->age_max / 100.0f) * 60.0f))
                      SET_FLAG(op, FLAG_IS_AGED);
                  else
                      CLEAR_FLAG(op, FLAG_IS_AGED);
                  goto fix_player_jump_resi;

                case FORCE:
                    if(tmp->subtype == SUBTYPE_FORCE_SNARE)
                    {
                        if(tmp->last_heal >= 100)
                            SET_FLAG(op, FLAG_ROOTED);
                        snare_penalty += tmp->last_heal;
                    }
                    else if(tmp->subtype == SUBTYPE_FORCE_PARALYZE)
                        SET_FLAG(op, FLAG_PARALYZED);
                    else if(tmp->subtype == SUBTYPE_FORCE_CONFUSED)
                        SET_FLAG(op, FLAG_CONFUSED);
                    else if(tmp->subtype == SUBTYPE_FORCE_BLIND)
                        SET_FLAG(op, FLAG_BLIND);
                    else if(tmp->subtype == SUBTYPE_FORCE_FEAR)
                        SET_FLAG(op, FLAG_FEARED);
                    else if(tmp->subtype == SUBTYPE_FORCE_SLOWED) /* slowness */
                    {
                        slow_penalty += tmp->last_heal;
                        SET_FLAG(op, FLAG_SLOWED);
                    }
                    else if(tmp->subtype == SUBTYPE_FORCE_DRAIN) /* level drain */
                    {
                            skill_level_drain += tmp->level;
                    }
                    else if(tmp->subtype == SUBTYPE_FORCE_POISON) /* applied poison  */
                    {
                        for (i = 0; i < STAT_NROF; i++)
                            set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));
                    }
                    else if(tmp->subtype == SUBTYPE_FORCE_POTION) /* applied potion  */
                    {
                        /* no protection from potion effect -resist only! */
                        for (i = 0; i < STAT_NROF; i++)
                            set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));
                        /* collect highest boni & malus - only highest one count,
                                   * no adding potion effects of same resist!
                                   */
                        tmpthac0 = tmp->stats.thac0;
                        tmpthacm = tmp->stats.thacm;
                        adjthac0 = ADJTHAC(adjthac0, tmpthac0);
                        adjthacm = ADJTHAC(adjthacm, tmpthacm);

                        for (i = 0; i < NROFATTACKS; i++)
                        {
                            /* collect highest/lowest resistance */
                            if (tmp->resist[i] > potion_resist_boni[i])
                                potion_resist_boni[i] = tmp->resist[i];
                            else if (tmp->resist[i] < potion_resist_mali[i])
                                potion_resist_mali[i] = tmp->resist[i];

                            /* collect highest attack!
                                           * Remember: no attack mali - this is a unsigned value
                                           */
                            if (tmp->attack[i] > potion_attack[i])
                                potion_attack[i] = tmp->attack[i];
                        }
                        break;
                    }
                    else
                    {
                        pl->speed_enc_base += tmp->last_sp;

                        for (i = 0; i < STAT_NROF; i++)
                            set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));
                        if (tmp->stats.wc)
                            pl->wc_bonus += (tmp->stats.wc + tmp->magic);
                        /* force effects goes to damage before calculation */
                        if (tmp->stats.dam)
                            op->stats.dam += (tmp->stats.dam + tmp->magic);
                        if (tmp->stats.ac)
                            ac += (tmp->stats.ac + tmp->magic);
                        if (tmp->weapon_speed)
                            weapon_speed_mod += tmp->weapon_speed;
                        if (tmp->subtype == SUBTYPE_FORCE_RESERVATION)
                        {
                            pl->reserved_hp += tmp->stats.hp;
                            pl->reserved_sp += tmp->stats.sp;
                            pl->reserved_grace += tmp->stats.grace;
                        }
                        else
                        {
                            if (tmp->stats.maxhp && tmp->type != TYPE_AGE_FORCE)
                                op->stats.maxhp += tmp->stats.maxhp;
                            if (tmp->stats.maxsp && tmp->type != TYPE_AGE_FORCE)
                                op->stats.maxsp += tmp->stats.maxsp;
                            if (tmp->stats.maxgrace && tmp->type != TYPE_AGE_FORCE)
                                op->stats.maxgrace += tmp->stats.maxgrace;
                        }
                    }
                    goto fix_player_jump_resi;

                case DISEASE:
                case SYMPTOM:
                  pl->speed_reduce_from_disease = tmp->last_sp;
                  for (i = 0; i < STAT_NROF; i++)
                      set_stat_value(&(op->stats), i, get_stat_value(&(tmp->stats), i));

                fix_player_jump_resi:

                  tmpthac0 = tmp->stats.thac0;
                  tmpthacm = tmp->stats.thacm;
                  adjthac0 = ADJTHAC(adjthac0, tmpthac0);
                  adjthacm = ADJTHAC(adjthacm, tmpthacm);

                  /* calculate resistance and attacks */
                  for (i = 0; i < NROFATTACKS; i++)
                  {
                      /* we add resists boni/mali */
                      if (tmp->resist[i] > 0)
                      {
                          if(tmp->resist[i]>=100)
                              resists_boni[i] = 100;
                          else if(resists_boni[i]<100)
                              resists_boni[i] += ((100 - resists_boni[i]) * tmp->resist[i]) / 100;
                      }
                      else if (tmp->resist[i] < 0)
                          resists_mali[i] += ((100 - resists_mali[i]) * (-tmp->resist[i])) / 100;

                      /* and we use adding attack boni - i set this for 120% max...
                                     * exclude all what is damaging itself with attack[]
                                    */
                      if (tmp->type != DISEASE &&
                          tmp->type != SYMPTOM &&
                          (tmp->type != FORCE ||
                           tmp->subtype != SUBTYPE_FORCE_POISON))
                      {
                          if (tmp->attack[i] > 0)
                          {
                              if ((op->attack[i] + tmp->attack[i]) <= 120)
                                  op->attack[i] += tmp->attack[i];
                              else
                                  op->attack[i] = 120;
                          }
                      }
                  }
                  break;

                  /* catch item which are "applied" but should not -
                         * or we forgot to catch them here!
                         */
                default:
                  LOG(llevDebug, "DEBUG: fix_player(): unexpected applied object %s (%d)(clear flag now!)\n", STRING_OBJ_NAME(tmp), tmp->type);
                  CLEAR_FLAG(tmp, FLAG_APPLIED);
                  OBJECT_UPDATE_UPD(tmp, UPD_FLAGS);
                  continue;
            }/*switch*/

            op->terrain_flag |= tmp->terrain_type;    /* we just add a given terrain */
            op->path_attuned |= tmp->path_attuned;
            op->path_repelled |= tmp->path_repelled;
            op->path_denied |= tmp->path_denied;
            if (QUERY_FLAG(tmp, FLAG_LIFESAVE))
                SET_FLAG(op, FLAG_LIFESAVE);
            if (QUERY_FLAG(tmp, FLAG_REFL_CASTABLE))
                SET_FLAG(op, FLAG_REFL_CASTABLE);
            if (QUERY_FLAG(tmp, FLAG_REFL_MISSILE))
                SET_FLAG(op, FLAG_REFL_MISSILE);
            if (QUERY_FLAG(tmp, FLAG_STEALTH))
                SET_FLAG(op, FLAG_STEALTH);
            if (QUERY_FLAG(tmp, FLAG_UNDEAD) && !QUERY_FLAG(&op->arch->clone, FLAG_UNDEAD))
                SET_FLAG(op, FLAG_UNDEAD);
            if (QUERY_FLAG(tmp, FLAG_XRAYS))
                SET_FLAG(op, FLAG_XRAYS);
            if (QUERY_FLAG(tmp, FLAG_BLIND))
                SET_FLAG(op, FLAG_BLIND);
            if (QUERY_FLAG(tmp, FLAG_SEE_IN_DARK))
                SET_FLAG(op, FLAG_SEE_IN_DARK);

            if (QUERY_FLAG(tmp, FLAG_SEE_INVISIBLE))
                SET_FLAG(op, FLAG_SEE_INVISIBLE);
            if (QUERY_FLAG(tmp, FLAG_MAKE_INVISIBLE))
                SET_MULTI_FLAG(op, FLAG_IS_INVISIBLE);
            if (QUERY_FLAG(tmp, FLAG_CAN_PASS_THRU))
                SET_MULTI_FLAG(op, FLAG_CAN_PASS_THRU);
            if (QUERY_FLAG(tmp, FLAG_MAKE_ETHEREAL))
                SET_MULTI_FLAG(op, FLAG_IS_ETHEREAL);
            if (QUERY_FLAG(tmp, FLAG_LEVITATE))
                SET_MULTI_FLAG(op, FLAG_LEVITATE);

            if (QUERY_FLAG(tmp, FLAG_FLYING))
                SET_MULTI_FLAG(op, FLAG_FLYING);
        } /* if applied */

        /* Update the client if anything has changed about tmp. */
        for (i = 0; i < NUM_FLAGS_32; i++)
        {
            if (tmp->flags[i] != tmpflags[i])
            {
                OBJECT_UPDATE_UPD(tmp, UPD_FLAGS);
                break;
            }
        }
    } /* Item is equipped - end of for loop going through items. */

    /* lets check we have applied an amunition type equal to our distance weapon */
    if(pl->equipment[PLAYER_EQUIP_AMUN])
    {
        if(!pl->equipment[PLAYER_EQUIP_BOW] || pl->equipment[PLAYER_EQUIP_BOW]->type != BOW ||
            pl->equipment[PLAYER_EQUIP_AMUN]->subtype != pl->equipment[PLAYER_EQUIP_BOW]->subtype)
        {
            /* wrong amunition - unapply it on the fly */
            CLEAR_FLAG(pl->equipment[PLAYER_EQUIP_AMUN], FLAG_APPLIED);
            OBJECT_UPDATE_UPD(pl->equipment[PLAYER_EQUIP_AMUN], UPD_FLAGS);
            pl->equipment[PLAYER_EQUIP_AMUN] = NULL;
        }

    }

    /* setup thze distance weapon && calculate the distance dps/wc/time values for the client (visual values) */
    if(pl->equipment[PLAYER_EQUIP_BOW])
    {
        if(pl->equipment[PLAYER_EQUIP_BOW]->type == BOW || pl->equipment[PLAYER_EQUIP_BOW]->type == ARROW)
        {
            object_t *skill_ptr;
            int tmp_dam, tmp_wc, tmp_time;

            if((tmp = pl->equipment[PLAYER_EQUIP_BOW])->type == ARROW)
            {
                skill_ptr = pl->skill_ptr[SK_THROWING];
                tmp_dam = tmp_wc = tmp_time =0;

            }
            else /* its a bow.. calc the arrow in */
            {
                if (tmp->subtype == RANGE_WEAP_BOW)
                    skill_ptr = pl->skill_ptr[SK_RANGE_BOW];
                else if (tmp->subtype == RANGE_WEAP_XBOWS)
                    skill_ptr = pl->skill_ptr[SK_RANGE_XBOW];
                else
                    skill_ptr = pl->skill_ptr[SK_RANGE_SLING];

                tmp_dam = tmp->stats.dam + tmp->magic;
                tmp_wc = tmp->stats.wc + tmp->magic;
                tmp_time = tmp->last_grace;

                /* map it over to the real arrow */
                tmp = pl->equipment[PLAYER_EQUIP_AMUN];
            }

            /* a last sanity check with emergency jmp */
            if(!skill_ptr || !tmp)
                goto no_skill_ptr_jmp;

            tmp_dam += tmp->stats.dam + tmp->magic;
            tmp_wc += tmp->stats.wc + tmp->magic;
            tmp_time += tmp->last_grace;

            if (pl->guild_force &&
                pl->guild_force->weapon_speed)
            {
               tmp_time -= (int)(pl->guild_force->weapon_speed * pticks_second);
            }

            /* we don't add op->stats.wc here because its melee.... our wc modifier comes from the skill.
             * but we add in wc modifier from equipment - means ATM a ring wc+2 will add wc to melee AND
             * to distance!
             */
            pl->dist_wc = (int)((float)(tmp_wc + skill_ptr->stats.wc) * stat_bonus[op->stats.Dex])+ pl->wc_bonus;

            /* lets calculate the real dmg and dps */
            pl->dist_dps = (int) ((float) tmp_dam * LEVEL_DAMAGE(skill_ptr->level)); /* dmg level adjusted */

            /* and now we adjust it by the stats and devide by 10 - thats the "real dmg" now */
            pl->dist_dps = (int)(((float)pl->dist_dps *
                (stat_bonus[op->stats.Str]/2.0f + stat_bonus[op->stats.Dex]/2.0f)) /10.0f);

            /* the damage bonus of rings and stuff are added AFTER dmg adjustment! */
            f = (float) (pl->equipment[PLAYER_EQUIP_BOW]->item_condition) / 100.0f;
            //f = (float)(pl->equipment[PLAYER_EQUIP_BOW]->item_quality) / 100.0f;
            pl->dist_dps = (int)((float)pl->dist_dps * f)+ pl->dam_bonus;
            if(pl->dist_dps < 0)
                pl->dist_dps = 0;

            /* in pl->dist_dps is now only the "real" dmg this item does - now calculate human readable dps */
            for(tmp_item=i=0;i<=LAST_ATNR_ATTACK;i++) /* collect the dmg parts */
                tmp_item += tmp->attack[i];

            f = (float) (tmp_time + skill_ptr->last_grace) * WEAPON_SWING_TIME; /* the action time */

            /* because we transfer dps as INT to the client, we store it right shifted >>1 */
            pl->dist_dps = (int) (( ((float)pl->dist_dps*((float)tmp_item/100.0f))/ f)*10.0f);

            /* last work - cast the swing time to INT for transfering */
            pl->dist_action_time = (int)(f * 1000.0f);

        }
        else /* show '**' in the range weapon area of the client */
        {
            no_skill_ptr_jmp:
                pl->dist_dps = -2;
                pl->dist_wc = -1;
        }
    }
    else /* show '--' in the range weapon area of the client */
    {
        pl->dist_dps = -1;
        pl->dist_wc = -1;
    }

    /* Weapons: the best bare hand skill and/or the applied weapon.
    * If we have no weapon selected, use the bare hand skill for attacking and wc/dam .
    * if we have a weapon, use the weapon and forget the bare hand skill. */
    /* TODO: Not sure burying this in fix_player() is such a good idea.
     * Rather, do these tweaks as part of choose_skill()? (Though that brings
     * its own problems.)
     *
     * -- Smacky 20190802 */
    if (pl->set_skill_weapon == NO_SKILL_READY) /* ok, no weapon - we must use our hands */
    {
        f = 1.0f;
        if (skill_weapon)
        {
            pl->skill_weapon = skill_weapon;

            if (pl->guild_force &&
                pl->guild_force->weapon_speed)
            {
                op->weapon_speed = skill_weapon->weapon_speed -
                                   pl->guild_force->weapon_speed;
            }
            else
            {
                op->weapon_speed = skill_weapon->weapon_speed;
            }

            if (skill_weapon->slaying)
            {
                SHSTR_FREE_AND_ADD_STRING(op->slaying, skill_weapon->slaying);
            }

            /* now we must add this special skill attack */
            for (i = 0; i < NROFATTACKS; i++)
            {
                op->attack[i] += skill_weapon->attack[i];
                if (op->attack[i] > 120)
                    op->attack[i] = 120;
            }

            /* FIXME? ATM all hit/miss and overall damage boni for hth skills
             * totally based on just having the skill at all. This compensates
             * for lack of weapon. But should we further tweak per skill level
             * as weapon skills do? Also, currently no hth skills have thac*
             * anyway...
             *
             * -- Smacky 20190802 */
            tmpthac0 = skill_weapon->stats.thac0;
            tmpthacm = skill_weapon->stats.thacm;
            adjthac0 = ADJTHAC(adjthac0, tmpthac0);
            adjthacm = ADJTHAC(adjthacm, tmpthacm);
            op->stats.wc += skill_weapon->stats.wc; /* skill = weapon - we must add the wc now */
            ac += skill_weapon->stats.ac;
            op->stats.dam += skill_weapon->stats.dam; /* and the damage */
            op->stats.dam = (sint16) ((float) op->stats.dam * LEVEL_DAMAGE(skill_weapon->level));
        }
        else
        {
            LOG(llevInfo, "INFO:: %s:fix_player(): Player %s has no HTH skill!\n",
                __FILE__, STRING_OBJ_NAME(op));
        }
    }
    else /* weapon in hand */
    {
        f = (float) (pl->equipment[PLAYER_EQUIP_WEAPON1]->item_condition) / 100.0f;
        //f *= (float)(pl->equipment[PLAYER_EQUIP_WEAPON1]->item_quality) / 100.0f;

        /* ouch - weapon without the skill applied... */
        if (!pl->skill_ptr[pl->set_skill_weapon])
        {
            LOG(llevBug, "BUG: fix_player(): player %s has weapon selected but not the skill #%d!!!\n",
                STRING_OBJ_NAME(op),   pl->set_skill_weapon);
           (void)apply_equipment(op, pl->equipment[PLAYER_EQUIP_WEAPON1],
               AP_UNAPPLY | AP_IGNORE_CURSE);
           pl->equipment[PLAYER_EQUIP_WEAPON1] = NULL;
        }
        else
        {
            sint8 slevel = pl->skill_ptr[pl->set_skill_weapon]->level;

            if (slevel <= 10)
            {
                op->stats.wc += 1;
            }
            else if ((slevel < 30 && op->stats.wc <= 20) ||
                (slevel < 50 && op->stats.wc <= 25) ||
                (slevel <= 80 && op->stats.wc <= 30) ||
                (slevel > 80 && op->stats.wc <= 35))
            {
                op->stats.wc += (int)((slevel / 10) + 1);
            }

            op->stats.dam = (sint16) ((float) op->stats.dam * LEVEL_DAMAGE(slevel));
        }
    }

    check_stat_bounds(&(op->stats));

    /* now the last adds - stat boni to dam and wc! */
    op->stats.dam = (int)((float)op->stats.dam * stat_bonus[op->stats.Str] /10.0f);
    if (op->stats.dam < 0)
        op->stats.dam = 0;

    op->stats.dam = (sint16)((float) op->stats.dam * f) + pl->dam_bonus; /* and finally the item condition + dmg bonus! */

    /* now collect all our real armour stuff - this will now add *after* all force
     * or non potion effects effecting resist,attack or protection - also this wii
     * give us a sorted adding.
     * But the most important point is that we calc *here* and only here the equipment
     * quality modifier for players.
     * Note, that for bows/arrows the calculation is done "on the fly" when the
     * throw/fire object is created!
     */
    for (j = 0; j < PLAYER_EQUIP_MAX; j++)
    {
        if (pl->equipment[j])
        {
            tmp = pl->equipment[j];
            //f = (float) tmp->item_quality / 100.0f * (float) tmp->item_condition / 100.0f;
            f = (float) tmp->item_condition / 100.0f;

            /* calculate resistance and attacks */
            for (i = 0; i < NROFATTACKS; i++)
            {
                /* we add resists boni/mali */
                if (tmp->resist[i] > 0 && resists_boni[i]<100)
                {
                    tmp_item = (int) ((float) tmp->resist[i] * f);
                    if(tmp_item>=100)
                        resists_boni[i]=100;
                    else
                        resists_boni[i] += ((100 - resists_boni[i]) * tmp_item) / 100;
                }
                else if (tmp->resist[i] < 0)
                    resists_mali[i] += ((100 - resists_mali[i]) * (-tmp->resist[i])) / 100;

                /* and we use adding attack boni - i set this for 120% max...
                         * exclude all what is damaging itself with attack[]
                        */
                if (tmp->type != BOW && tmp->type != ARROW && tmp->type != ROD && tmp->type != WAND && tmp->type != HORN)
                {
                    if (tmp->attack[i] > 0)
                    {
                        tmp_item = tmp->attack[i];
                        if(i >= ATNR_GODPOWER) /* Important: Don't add condition to damage attack - we did it to dmg! */
                            tmp_item = (int) ((float)tmp_item * f);
                        if ((op->attack[i] + tmp_item) <= 120)
                            op->attack[i] += tmp_item;
                        else
                            op->attack[i] = 120;
                    }
                }
            }
        }
    }

    /* 'total resistance = total protections - total vulnerabilities'.
     * If there is an uncursed potion in effect, granting more protection
     * than that, we take: 'total resistance = resistance from potion'.
     * If there is a cursed (and no uncursed) potion in effect, we take
     * 'total resistance = vulnerability from cursed potion'.
     */

    /* now we add in all our values... we add in our potions effects as well as
      * our attack boni and/or protections.
      */
    for (j = 1,i = 0; i < NROFATTACKS; i++,j <<= 1)
    {
        if (potion_attack[i])
        {
            if ((op->attack[i] + potion_attack[i]) > 120)
                op->attack[i] = 120;
            else
                op->attack[i] += potion_attack[i];
        }

        /* add in the potion resists boni/mali */
        if (potion_resist_boni[i] > 0 && resists_boni[i] <100)
        {
            if(potion_resist_boni[i]>=100)
                resists_boni[i] = 100;
            else
                resists_boni[i] += ((100 - resists_boni[i]) * potion_resist_boni[i]) / 100;
        }

        if (potion_resist_mali[i] < 0)
            resists_mali[i] += ((100 - resists_mali[i]) * (-potion_resist_mali[i])) / 100;

        /* and generate the REAL resists of the player! */
        op->resist[i] = resists_boni[i] - resists_mali[i];
    }

    /* calculate the max. value what the player can carry */
    /* TODO: This is an approximation of what was here before -- allows 5kg for
     * each point of Str > 10.
     *
     * -- Smacky 20150714 */
    /* I (hopefully) temporarily changed this until I can figure out a good way
     * to adjust stat boni. It's the same as the old version (i.e. it doesn't
     * use the stat_bonus table).
     * -- _people_ 20160601
     */
    pl->weight_limit = op->weight_limit + 5000 * op->stats.Str - 10;
    pl->gen_sp += MAX(op->stats.Int - 15, 0) / 2;
    pl->gen_grace += MAX(op->stats.Cha - 15, 0) / 2;

    /* we calculate this: we get weight_limit - ENCUMBRANCE_LIMIT %. Thats around 35% of the
    * value ATM. Thats our base. Then we check how much of this last 35% we really carry. Thats
    * our encumbrance value in % that we assign to speed_enc
    */
    pl->speed_enc_limit = (int) ((float) pl->weight_limit * ENCUMBRANCE_LIMIT); /* = 35% of our max value */

    /* CALCULATE REAL PLAYER SPEED */
    set_speed_encumbrance(op, pl);

    op->glow_radius = light;

    /* we must do -old_gow + light */
    if (old_glow != light)
    {
        adjust_light_source(msp, light - old_glow);
    }

    /* for player, max hp depend on general level, sp on magic exp, grace on wisdom exp level
     * NOTE: all values are adjusted from clone at function start.
     */
    skill_level_max = skill_level_max-skill_level_drain;
    if(skill_level_max <1)
        skill_level_max=1;
    op->level = skill_level_max;

    /* These are base values for level 1s. */
    maxhp = op->arch->clone.stats.maxhp * 3;
    maxsp = op->arch->clone.stats.maxsp * 2 + 15;
    maxgrace = op->arch->clone.stats.maxgrace * 2 + 15;

    /* For each level up to our (drained) main we gain hp.
     * For each level up to our magic skillgroup we gain mana.
     * For each level up to our wisdom skillgroup we gain grace.
     * Why start at 2? Simplicity. levhp[n], etc is only written by
     * exp_adjust() on levelup and as levels start at 1, [1] is
     * never written. Instead this is the base value above. */
    for (i = 2; i <= pl->ob->level; i++)
    {
        maxhp += pl->levhp[i];

        if (i <= pl->skillgroup_ptr[SKILLGROUP_MAGIC]->level)
        {
            maxsp += pl->levsp[i];
        }

        if (i <= pl->skillgroup_ptr[SKILLGROUP_WISDOM]->level)
        {
            maxgrace += pl->levgrace[i];
        }
    }

    /* now adjust with the % of the stats mali/boni. */
    maxhp = (sint32)((float)maxhp * stat_bonus[op->stats.Con]) + max_boni_hp;
    maxhp -= pl->reserved_hp;
    if (maxhp < 1)
        maxhp = 1;
    maxsp = (sint16)((float)maxsp * stat_bonus[op->stats.Pow]) + max_boni_sp;
    maxsp -= pl->reserved_sp;
    if (maxsp < 1)
        maxsp = 1;
    maxgrace = (sint16)((float)maxgrace * stat_bonus[op->stats.Wis]) + max_boni_grace;
    maxgrace -= pl->reserved_grace;
    if (maxgrace < 1)
        maxgrace = 1;

    /* now we calculate the regeneration points we get every reg heart beat */
    pl->reg_hp_num = (int)((float)maxhp * (float)pl->gen_hp / 1000.0f);
    if (pl->reg_hp_num < 1)
        pl->reg_hp_num = 1;
    pl->reg_sp_num = (int)((float)maxsp * (float)pl->gen_sp / 1000.0f);
    if (pl->reg_sp_num < 1)
        pl->reg_sp_num = 1;
    pl->reg_grace_num = (int)((float)maxgrace * (float)pl->gen_grace / 1000.0f);
    if (pl->reg_grace_num < 1)
        pl->reg_grace_num = 1;

    hp = op->stats.hp;
    op->stats.maxhp = maxhp;
    op->stats.hp = (hp >= 0 && hp <= maxhp) ? hp : maxhp;
    sp = op->stats.sp;
    op->stats.maxsp = maxsp;
    op->stats.sp = (sp >= 0 && sp <= maxsp) ? sp : maxsp;
    grace = op->stats.grace;
    op->stats.maxgrace = maxgrace;
    op->stats.grace = (grace >= 0 && grace <= maxgrace) ? grace : maxgrace;

    /* Calculate the spell fumble here because it is affected by int.
        read the above by temp_fumble to learn more.
    */
    pl->spell_fumble = temp_fumble + pl->encumbrance - op->stats.Int;

    /* adjthac* are correct values now according to race, guild, equipment,
     * melee skill, etc. But they are in the sint16 range whereas we now need
     * sint8. So convert and copy the values to the player object. */
    op->stats.thac0 = (adjthac0 == -32767 || adjthac0 == 32767)
        ? SGN(adjthac0) * 127
        : MAX(-25, MIN(adjthac0, 125));
    op->stats.thacm = (adjthacm == -32767 || adjthacm == 32767)
        ? SGN(adjthacm) * 127
        : MAX(-25, MIN(adjthacm, 125));

    /* wc and ac are level independent in the thac system - but Dex effects both now */
    f = stat_bonus[op->stats.Dex];
    op->stats.ac = (int) ((float)(op->stats.ac + ac) * f);
    op->stats.wc = (int) ((float)(op->stats.wc + pl->wc_bonus) * f);


    /* adjust swing speed and move speed by slow penalty */
    if(QUERY_FLAG(op,FLAG_FEARED))
    {
        int malus;

        malus = op->stats.wc/8; /* 15% mali wc */
        if(!malus)
            malus=1;
        op->stats.wc -= malus;

        malus = op->stats.ac/8; /* 15% mali ac */
        if(!malus)
            malus=1;
        op->stats.ac -= malus;

        slow_penalty +=15; /* add a 15% slowness factor to swing & movement */
    }

    /* first, we cap slow effects to 80% */
    if(slow_penalty > 80)
        slow_penalty = 80;

    if(slow_penalty)
    {
        snare_penalty += slow_penalty;
        op->weapon_speed *= ((float)(100+slow_penalty)/100.0f);
    }

    /* we don't cap snare effects - if >=100% we set root flag */
    if(snare_penalty)
    {
        if(snare_penalty >= 100)
        {
            SET_FLAG(op,FLAG_ROOTED);
            op->weapon_speed = 0.15f;
        }
        else
        {
            op->speed *= ((float)(100-snare_penalty)/100.0f);
        }
    }

    op->weapon_speed += weapon_speed_mod;

    /* this is right, root & paralyze flags will handle no movement
     * for all other movement penalties, this is the cap
     */
    if(op->weapon_speed < 0.15f)
        op->weapon_speed = 0.15f;

    /* thats for the client ... */
    if(QUERY_FLAG(op,FLAG_PARALYZED))
    {
        pl->weapon_sp = 0;
        pl->speed = 0;
    }
    else
    {
        if(QUERY_FLAG(op,FLAG_ROOTED))
            pl->speed = 0;
        else
        {
            pl->weapon_sp = (int) (op->weapon_speed * 1000.0f);
            pl->speed = op->speed;
        }
    }

    /* in stats.dam is now the real base damage we feed the attack function with. For the client
    * and for probing we need to show a better "human readable" value - damage per scond = DPS
    */
    for(tmp_item=i=0;i<=LAST_ATNR_ATTACK;i++) /* we can have more or less 100% damage output by attack modifiers */

        /* Screen through to make sure we aren't adding corruption attacks to DPS.
         * Some attacks such as psionic attack for HP and either mana or grace through corruption.
         * So corruption is the only one that needs to be ignored.
         */
        if (i != ATNR_CORRUPTION)
            tmp_item += op->attack[i];

    /* because we transfer dps as INT to the client, we store it right shifted >>1 */
    pl->dps = (int) (( ((float)op->stats.dam*((float)tmp_item/100.0f))/ op->weapon_speed)*10.0f);

    /* If op wasn't invisible and now is, or was and now isn't, update the
     * layer. */
    if (!inv_flag &&
        QUERY_FLAG(op, FLAG_IS_INVISIBLE))
    {
        CLEAR_FLAG(op, FLAG_IS_INVISIBLE);
        msp_rebuild_slices_without(msp, op);
        SET_FLAG(op, FLAG_IS_INVISIBLE);
        msp_rebuild_slices_with(msp, op);
        OBJECT_UPDATE_VIS(op);
    }
    else if (inv_flag &&
             !QUERY_FLAG(op, FLAG_IS_INVISIBLE))
    {
        SET_FLAG(op, FLAG_IS_INVISIBLE);
        msp_rebuild_slices_without(msp, op);
        CLEAR_FLAG(op, FLAG_IS_INVISIBLE);
        msp_rebuild_slices_with(msp, op);
        OBJECT_UPDATE_VIS(op);
    }

    /* If op couldn't see invisible and now can, or could and now can't, send a
     * new below inventory to his client. */
    if ((!inv_see_flag &&
         QUERY_FLAG(op, FLAG_SEE_INVISIBLE)) ||
        (inv_see_flag &&
         !QUERY_FLAG(op, FLAG_SEE_INVISIBLE)))
    {
        esrv_send_below(pl);
    }

    CLEAR_FLAG(op, FLAG_NO_FIX_PLAYER);

    /* Update the client if anything has changed about op. */
    for (i = 0; i < NUM_FLAGS_32; i++)
    {
        if (op->flags[i] != opflags[i])
        {
            OBJECT_UPDATE_UPD(op, UPD_FLAGS);
            break;
        }
    }
}

/** Adjust the monster's datas for level, map settings and game settings
 * when put in play.
 */

void fix_monster(object_t *op)
{
    sint8     tmpthac0;
    sint8     tmpthacm;
    sint16    adjthac0;
    sint16    adjthacm;
    int wc_mali=0, ac_mali=0, snare_penalty=0, slow_penalty=0;
    object_t *base, *tmp, *next, *spawn_info=NULL, *bow=NULL, *wc_tmp;

    // wow, so op->head means, we have a pointer to our head somewhere
    if (op->head) /* don't adjust tails or player - only single objects or heads */
        return;

#ifdef DEBUG_FIX_MONSTER
    LOG(llevDebug, "FIX_MONSTER(%s [%x]): called\n", STRING_OBJ_NAME(op), op->count);
#endif


    // instead of using this base info shit, we should get the infos from our spawn point mob
    // this logic below kills everything we adjusted before
    // also using such a logic will not allow changes of base object, because this would stack

    base = insert_base_info_object(op); /* will insert or/and return base info */

    //LOG(llevDebug, "FIX_MONSTER base level %d op level %d\n", base->level, op->level);
    //LOG(llevDebug, "FIX_MONSTER base exp %d op exp %d\n", base->stats.exp, op->stats.exp);
    op->level = base->level;

    if (!QUERY_FLAG(&op->arch->clone, FLAG_BLIND))
        CLEAR_FLAG(op, FLAG_BLIND);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_SLOWED))
        CLEAR_FLAG(op, FLAG_SLOWED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_FEARED))
        CLEAR_FLAG(op, FLAG_FEARED);
    /* rooted is set when a snare effect has reached 100% */
    if (!QUERY_FLAG(&op->arch->clone, FLAG_ROOTED))
        CLEAR_FLAG(op, FLAG_ROOTED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_CONFUSED))
        CLEAR_FLAG(op, FLAG_CONFUSED);
    if (!QUERY_FLAG(&op->arch->clone, FLAG_PARALYZED))
        CLEAR_FLAG(op, FLAG_PARALYZED);

    CLEAR_FLAG(op, FLAG_READY_BOW);
    CLEAR_FLAG(op, FLAG_READY_SPELL);

    FOREACH_OBJECT_IN_OBJECT(tmp, op, next)
    {
        /* handle forces */
        if(tmp->type == FORCE && QUERY_FLAG(tmp, FLAG_APPLIED))
        {
            if(tmp->subtype == SUBTYPE_FORCE_FEAR)
            {
                SET_FLAG(op, FLAG_FEARED);
            }
            else if(tmp->subtype == SUBTYPE_FORCE_SNARE)
            {
                snare_penalty += tmp->last_heal;

                if(snare_penalty >= 100)
                    SET_FLAG(op, FLAG_ROOTED);
            }
            else if(tmp->subtype == SUBTYPE_FORCE_SLOWED)
            {
                slow_penalty += tmp->last_heal;
                SET_FLAG(op, FLAG_SLOWED);
            }
            else if(tmp->subtype == SUBTYPE_FORCE_PARALYZE)
            {
                SET_FLAG(op, FLAG_PARALYZED);
            }
            else if(tmp->subtype == SUBTYPE_FORCE_DRAIN) /* level drain */
            {
                op->level -= tmp->level;
                if(op->level < 1)
                    op->level = 1;
            }
            else if(tmp->subtype == SUBTYPE_FORCE_DEPLETE) /* depletion */
            {
                /* nothing special here - depletion is at first an anti player feature */
                wc_mali = tmp->stats.Str+tmp->stats.Con+tmp->stats.Int;
                ac_mali = tmp->stats.Dex+tmp->stats.Wis+tmp->stats.Pow;
            }
        }
        else if (tmp->type == BOW && !bow  && tmp->subtype == 128)
        {
            bow = tmp;
            SET_FLAG(op, FLAG_READY_BOW);
            SET_FLAG(tmp, FLAG_APPLIED);
        }
        else if (tmp->type == ABILITY)
            SET_FLAG(op, FLAG_READY_SPELL);
        else if(tmp->type == SPAWN_POINT_INFO)
            spawn_info = tmp;
    }

    /* assign amun to our ability bow - and put them inside the inventory of it
     * one time a bit more work here but then fast access to amun for range firing
     * mobs.
     */
    if(bow)
    {
        object_t *tmp2;

        for (tmp = op->inv; tmp; tmp = tmp2)
        {
            if(tmp->type == ARROW && tmp->race == bow->race)
            {
                remove_ob(tmp);
                insert_ob_in_ob(tmp, bow);
            }
            tmp2=tmp->below;
        }

    }

    /* pre adjust */
    op->stats.maxhp = (base->stats.maxhp * (op->level + 2) + (sint32)((float)(op->level) / 1.25f) * base->stats.maxhp) / 10; //-7
    if(op->stats.maxhp <= 0)
        op->stats.maxhp = 1;
    op->stats.maxsp = base->stats.maxsp * (op->level + 1);
    if(op->stats.maxsp <= 0)
        op->stats.maxsp = 1;
    op->stats.maxgrace = base->stats.maxgrace * (op->level + 1);
    if(op->stats.maxgrace <= 0)
        op->stats.maxgrace = 1;

    /* remember: -1 is a place holder - if == -1, we fill in max value.
     * if the value is != -1, object has explicit set to a different value
     * (for example to simulate on a map a injured target) or its previous
     * damaged and saved
     */

    if (op->stats.hp == -1)
        op->stats.hp = op->stats.maxhp;
    if (op->stats.sp == -1)
        op->stats.sp = op->stats.maxsp;
    if (op->stats.grace == -1)
        op->stats.grace = op->stats.maxgrace;

    /* cap the pools to <=max */
    if (op->stats.hp > op->stats.maxhp)
        op->stats.hp = op->stats.maxhp;
    if (op->stats.sp > op->stats.maxsp)
        op->stats.sp = op->stats.maxsp;
    if (op->stats.grace > op->stats.maxgrace)
        op->stats.grace = op->stats.maxgrace;

    /* Here we do a simplified version of the ADJTHAC process above. Why
     * simplified? Because monsters don't use equipment or have guilds. But
     * they should be equally susceptible to castables and other forces (but
     * this is not handled well for players either -- and this won't be
     * addressed for a while). */
    adjthac0 = base->stats.thac0;
    adjthacm = base->stats.thacm;

    /* Here we increase thac for the lowest levels (so more likely to fumble
     * and less likely to dh). This is because VLLMs are likely to be fighting
     * VLLPs. */
    if (op->level < 10)
    {
        tmpthac0 = 10 - op->level;
        tmpthacm = 10 - op->level;
        adjthac0 = ADJTHAC(adjthac0, tmpthac0);
        adjthacm = ADJTHAC(adjthacm, tmpthacm);
    }

    op->stats.thac0 = (adjthac0 == -32767 || adjthac0 == 32767)
        ? SGN(adjthac0) * 127
        : MAX(-25, MIN(adjthac0, 125));
    op->stats.thacm = (adjthacm == -32767 || adjthacm == 32767)
        ? SGN(adjthacm) * 127
        : MAX(-25, MIN(adjthacm, 125));

    /* + level/10 to catch up the equipment improvements of the players. */
    op->stats.ac = base->stats.ac + (op->level / 10) - ac_mali;
    op->stats.wc = base->stats.wc + (op->level / 10) - wc_mali;
    wc_tmp = &op->arch->clone;
    if(op->stats.wc == (wc_tmp->stats.wc + ((int)op->level / 10)))
          op->stats.wc = MobACWC[op->level];
    if(op->stats.ac == (wc_tmp->stats.ac + ((int)op->level / 10)))
         op->stats.ac = MobACWC[op->level];

    op->stats.dam = base->stats.dam;

    /* adjust swing speed and move speed by slow penalty */
    if(QUERY_FLAG(op,FLAG_FEARED))
    {
        int malus;

        malus = op->stats.wc/8; /* 15% mali wc */
        if(!malus)
            malus=1;
        op->stats.wc -= malus;

        malus = op->stats.ac/8; /* 15% mali ac */
        if(!malus)
            malus=1;
        op->stats.ac -= malus;

        slow_penalty +=15; /* add a 15% slowness factor to swing & movement */
    }

    set_mobile_speed(op, 0);

    /* first, we cap slow effects to 80% */
    if(slow_penalty > 80)
        slow_penalty = 80;

    if(slow_penalty)
    {
        snare_penalty += slow_penalty;
        op->weapon_speed *= ((float)(100+slow_penalty)/100.0f);
    }

    /* we don't cap snare effects - if >=100% we set root flag */
    if(snare_penalty)
    {
        if(snare_penalty >= 100)
        {
            SET_FLAG(op,FLAG_ROOTED);
            op->weapon_speed = 0.15f;
        }
        else
        {
            op->speed *= ((float)(100-snare_penalty)/100.0f);
        }
    }

    /* post adjust */
    // this is funny simply overwriting all early npc's up to level 10 with 3 damage
    // that's how balancing works, useless arch definitions overwritten by stupid formula ;-)
    // op->stats.dam = (sint16)(op->stats.dam / 1800.0f * op->level * op->level +3);

    // dividing arches / 1800 is nearly same than ignoring it
    // so theoretical we can clean arches and use dynamic on 0 damage
    // and allow map editor to hardcode the damage

    // lets try level*3, this is a nice, fair and challenging early game
    // public server has no late game, so who cares ;-)
    op->stats.dam=3*op->level;


    /* Set up AI in op->custom_attrset */
    if (!MOB_DATA(op))
    {
        SETUP_MOB_DATA(op);
    }

    /* insert a quick jump in the MOB_DATA(() to its spawn info
     * Wonderful, i can skip the spawn point info loop now.
     * Thats a nice speed increase! MT
     */
    MOB_DATA(op)->spawn_info = spawn_info;
}

/* insert and initialize base info object in object op
 * Return: ptr to inserted base_info
 */
object_t * insert_base_info_object(object_t *op)
{
    object_t     *tmp, *head;
    objectlink_t *ol;
    object_t     *outermost;

    op->head != NULL ? (head = op->head) : (head = op);

    if (op->type == PLAYER)
    {
        LOG(llevBug, "insert_base_info_object() Try to inserting base_info in player %s!\n", STRING_OBJ_NAME(head));
        return NULL;
    }

    if ((tmp = find_base_info_object(head)))
        return tmp;


    // why we get our baseobject from arch, when we have spawn point mob?
    tmp = get_object();
    tmp->arch = op->arch;
    /* we don't need to trigger the
     * treasurelist link/unlink stuff here.
     * IF we ever need the original treasurelist
     * in the baseinfo, just removes this lines,
     * it will do no harm and copy_object will link it in.
     */
    ol = head->randomitems;
    head->randomitems = NULL;
    copy_object_data(head, tmp); /* copy without put on active list */
    head->randomitems = ol;
    tmp->type = TYPE_BASE_INFO;
    tmp->speed_left = tmp->speed;
    tmp->speed = 0.0f; /* ensure this object will not be active in any way */
    tmp->face = archetype_global._base_info->clone.face;
    SET_FLAG(tmp, FLAG_NO_DROP);
    CLEAR_FLAG(tmp, FLAG_ANIMATE);
    CLEAR_FLAG(tmp, FLAG_FRIENDLY);
    CLEAR_FLAG(tmp, FLAG_IN_ACTIVELIST);
    insert_ob_in_ob(tmp, head); /* and put it in the mob */

    /* Store position (for returning home after aggro is lost...) */
    /* Has to be done after insert_ob_in_ob() */
    outermost = head;
    while(outermost != NULL && outermost->map == NULL)
        outermost = outermost->env;
    if(outermost && outermost->map)
    {
        tmp->x = outermost->x;
        tmp->y = outermost->y;
        SHSTR_FREE_AND_ADD_REF(tmp->slaying, outermost->map->path);
    }
    else
    {
        SHSTR_FREE(tmp->slaying);
        LOG(llevDebug, "insert_base_info_object(): Can't set up home location for '%s' - not even close to a map.\n", STRING_OBJ_NAME(head));
    }

    return tmp;
}

/* find base_info in *op
 * Return: ptr to inserted base_info
 */
object_t * find_base_info_object(object_t *op)
{
    object_t *tmp,
           *next;

    FOREACH_OBJECT_IN_OBJECT(tmp, op, next)
    {
        if (tmp->type == TYPE_BASE_INFO)
            return tmp;
    }

    return NULL; /* no base_info in this object found */
}

/** Set set the movement speed of a mobile.
 *
 * The base speed of a mob is stored in base->speed_left.
 * The actual speed is stored in op->speed as base->speed_left * factor.
 *
 * Possible factors:
 * 1 = mob is slowed (by magic) (minimum factor)
 * 2 = normal mob speed - moving normal
 * 3 = mob is moving fast
 * 4 = mob is running/attack speed
 * 5 = mob is hasted and moving full speed (maximum factor)
 *
 * @input op mob object to set speed for
 * @input factor a speed factor for forcing a speed, or 0 to automatically
 * compute a factor based on the AI state, spells etc.
 */
void set_mobile_speed(object_t *op, int factor)
{
    object_t *base;
    float   old_speed;
    int actual_factor;

    base = insert_base_info_object(op); /* will insert or/and return base info */

    old_speed = op->speed;

    if (factor) /* if factor != 0, we force a setting of this speed */
        actual_factor = factor;
    else /* we will generate the speed by setting of the mobile */
    {
        /* TODO: more logic when we add haste/slow spells */

        if(op->type == MONSTER && MOB_DATA(op))
            actual_factor = MOB_DATA(op)->move_speed_factor; /* AI-selected speed */
        else if (OBJECT_VALID(op->enemy, op->enemy_count))
            actual_factor = 4; /* Attack speed */
        else
            actual_factor = 2; /* Backup */
    }

    op->speed = base->speed_left * CLAMP(actual_factor, 1, 5);

     //LOG(llevNoLog,"SET SPEED: %s ->%f (=%d*%f) o:%f\n", STRING_OBJ_NAME(op), op->speed, actual_factor, base->speed_left, old_speed);
    /* update speed if needed */
    if ((old_speed && !op->speed) || (!old_speed && op->speed))
        update_ob_speed(op);
}

/* Leeches a health indicator (that is takes some off leechee and gives a
 * proportion to leecher). */
void leech_hind(object_t *leecher, object_t *leechee, uint8 attack, sint16 plose,
                sint16 pmod, uint8 chance)
{
    sint16 pgain = 0;
    char   buf[TINY_BUF];

    /* Sanity -- no damage, nothing to do. */
    if (plose <= 0)
    {
        return;
    }

    /* First do the damage/leech. */
    if (attack == ATNR_CORRUPTION && // Corruption on player
        leechee->type == PLAYER)
    {
        /* Sanity -- no hind to leech, nothing to do. */
        if (leechee->stats.grace <= 0)
        {
            return;
        }

        if ((leechee->stats.grace -= plose) < 0)
        {
            leechee->stats.grace = 0;
        }

        pgain = RANDOM_ROLL(1, (int)(plose / 100.0f * chance));

        if (leecher->type == PLAYER)
        {
            if ((leecher->stats.grace += pgain) > leecher->stats.maxgrace)
            {
                leecher->stats.grace = leecher->stats.maxgrace;
            }
        }
        /* mobs only have sp ATM */
        else
        {
            if ((leecher->stats.sp += pgain) > leecher->stats.maxsp)
            {
                leecher->stats.sp = leecher->stats.maxsp;
            }
        }
    }
    else if (attack != ATNR_LIFESTEAL) // Psionic or corruption on mob
    {
        /* Sanity -- no hind to leech, nothing to do. */
        if (leechee->stats.sp <= 0)
        {
            return;
        }

        if ((leechee->stats.sp -= plose) < 0)
        {
            leechee->stats.sp = 0;
        }

        pgain = RANDOM_ROLL(1, (int)(plose / 100.0f * chance));

        if ((leecher->stats.sp += pgain) > leecher->stats.maxsp)
        {
            leecher->stats.sp = leecher->stats.maxsp;
        }
    }
    else // Lifesteal
    {
        /* Sanity -- no hind to leech, nothing to do. */
        if (leechee->stats.hp <= 0)
        {
            return;
        }

        /* FIXME: not 100% sure it is wise to directly lower hp like this
         * rather than calling damage_ob(). */
        if ((leechee->stats.hp -= plose) < 0)
        {
            leechee->stats.hp = 0;
        }

        pgain = RANDOM_ROLL(1, (int)(plose / 100.0f * chance));

        if ((leecher->stats.hp += pgain) > leecher->stats.maxhp)
        {
            leecher->stats.hp = leecher->stats.maxhp;
        }
    }

    /* Then say what has happened. */
    sprintf(buf, "%s",
            (attack == ATNR_PSIONIC) ? "mana" :
            ((attack == ATNR_CORRUPTION) ? "grace" : "damage"));

    if (leechee->type == PLAYER)
    {
        ndi(NDI_PURPLE, 0, leechee, "%s hits you for %d (%d) %s. It is a leech attack!",
                      leecher->name, (int)plose, ((int)plose) - pmod, buf);
    }

    /* i love C... ;) */
    if ((leecher->type == PLAYER ||
         ((leecher = get_owner(leecher)) &&
         leecher->type == PLAYER)))
    {
        ndi(NDI_ORANGE, 0, leecher, "You hit %s for %d (%d) %s with %s. It is a leech attack!",
                      leechee->name, (int)plose, ((int)plose) - pmod, buf,
                      attack_name[attack].name);
    }
}
