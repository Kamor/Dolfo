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

/* TODO: The generic name for (wizard) spells and (divine) prayers is
 * castables, so the filenames/prefixes will be changed accordingly.
 *
 * -- Smacky 20140815 */

#include <global.h>

#ifdef NO_ERRNO_H
extern int  errno;
#else
#   include <errno.h>
#endif

spell spells[NROFREALSPELLS]          =
{
    {"firestorm",                   SPELL_TYPE_WIZARD, 2,
    4, 12, 3, 6,0, 1.0,
    65,      7,      4,      0,
    1, 4, 0, 5, SOUND_MAGIC_FIRE,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_DUST | SPELL_USE_BOOK | SPELL_USE_POTION,
    SPELL_DESC_DIRECTION,
    PATH_ELEMENTAL, "firebreath",SPELL_ACTIVE
    },

    {"icestorm",                    SPELL_TYPE_WIZARD, 2,
    4, 12, 3, 6,0,1.0,
    65,      7,      4,      0,
    1, 4, 0, 5, SOUND_MAGIC_ICE,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_BOOK | SPELL_USE_POTION | SPELL_USE_DUST,
    SPELL_DESC_DIRECTION,
    PATH_ELEMENTAL, "icestorm",SPELL_ACTIVE
    },

    {"minor healing",               SPELL_TYPE_PRIEST, 1,
    6, 8, 3, 6,3,1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_BALM | SPELL_USE_SCROLL | SPELL_USE_ROD | SPELL_USE_POTION | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_WIS | SPELL_DESC_TOWN,
    PATH_LIFE, "meffect_green",SPELL_ACTIVE
    },

    {"cure poison",                 SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,4, 1.0,/* potion only */
    0,       0,     0,      0,
    0, 0, 0, 0,   SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_POTION | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_WIS | SPELL_DESC_TOWN,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"cure disease",                SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,4, 1.0,/* balm only */
    0,       0,     0,      0,
    0, 0, 0, 0,   SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_BALM | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_WIS | SPELL_DESC_TOWN,
    PATH_LIFE,"meffect_purple",SPELL_ACTIVE
    },

    {"strength self",               SPELL_TYPE_WIZARD, 1,
    5, 12, 3, 6,0,1.0,
    0,       0,     0,      4,
    0, 0, 0, 3,  SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_POTION | SPELL_USE_ROD | SPELL_USE_SCROLL | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_TOWN,
    PATH_NATURE, "meffect_yellow",SPELL_ACTIVE
    },

    {"identify",                    SPELL_TYPE_WIZARD, 2,
    5, 24, 3, 6,2,1.0,
    0,       0,     0,      0,
    0, 0, 0, 3,  SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_SCROLL | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_TOWN,
    PATH_TRANSMUTATION, "meffect_pink",SPELL_ACTIVE
    },

    {"detect magic",                SPELL_TYPE_WIZARD, 2,
    5, 8, 3, 6,0,1.0,
    0,       0,     0,      0,
    0, 0, 0, 3,   SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_SCROLL | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_TOWN,
    PATH_ABJURATION, "meffect_pink",SPELL_ACTIVE
    },

    {"detect curse",                SPELL_TYPE_PRIEST, 2,
    5, 8, 3, 6,0,1.0,
    0,       0,     0,      0,
    0, 0, 0, 0,   SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_SCROLL | SPELL_USE_BOOK,
    SPELL_DESC_SELF | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_ABJURATION, "meffect_pink",SPELL_ACTIVE
    },

    {"remove curse",                SPELL_TYPE_PRIEST, 2,
    5, 24, 3, 6,2,1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_BOOK,     /* scroll */
    SPELL_DESC_SELF | SPELL_DESC_TOWN | SPELL_DESC_FRIENDLY | SPELL_DESC_WIS,
    PATH_ARCANE, "meffect_blue",SPELL_ACTIVE
    },

    {"remove damnation",            SPELL_TYPE_PRIEST, 2,
    5, 36, 3, 6,2,1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_BOOK, /* scroll*/
    SPELL_DESC_SELF | SPELL_DESC_TOWN | SPELL_DESC_FRIENDLY | SPELL_DESC_WIS,
    PATH_ARCANE, "meffect_blue",SPELL_ACTIVE
    },

    {"cause light wounds",          SPELL_TYPE_PRIEST, 1,
    3, 14, 3, 6,0,  1.0,/* scroll*/
    95,      5,      4,      4,
    1, 6, 0, 5,  SOUND_MAGIC_WOUND,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_BOOK,
    SPELL_DESC_DIRECTION | SPELL_DESC_WIS,
    PATH_DEATH,"cause_wounds",SPELL_ACTIVE
    },

    {"firebolt",                   SPELL_TYPE_WIZARD, 2,
    6, 12, 3, 6,0, 1.0,
    108,      8,      4,      0,
    1, 3, 0, 5, SOUND_MAGIC_FIRE,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_DUST | SPELL_USE_BOOK | SPELL_USE_POTION,
    SPELL_DESC_DIRECTION,
    PATH_ELEMENTAL, "firebolt", SPELL_ACTIVE
    },

    {"magic bullet",                SPELL_TYPE_WIZARD, 1,
    3, 14, 3, 6,0,1.0,
    95,      5,      4,      4,
    1, 6, 0, 5,   SOUND_MAGIC_BULLET1,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_BOOK,
    SPELL_DESC_DIRECTION,
    PATH_CHAOS, "bullet",SPELL_ACTIVE
    },

    {"frostbolt",                   SPELL_TYPE_WIZARD, 2,
    6, 12, 3, 6,0, 1.0,
    108,      8,      4,      0,
    1, 3, 0, 5, SOUND_MAGIC_ICE,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_DUST | SPELL_USE_BOOK | SPELL_USE_POTION,
    SPELL_DESC_DIRECTION,
    PATH_ELEMENTAL, "frostbolt", SPELL_ACTIVE
    },

    {"remove depletion",            SPELL_TYPE_PRIEST, 2,
    5, 24, 3, 6,4, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0,    SOUND_MAGIC_STAT,
    SPELL_USE_CAST,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"probe",                       SPELL_TYPE_WIZARD, 1,
    5, 8, 3, 6,0,1.0,
    1,      5,      4,      4,
    0, 0, 0, 1, SOUND_MAGIC_DEFAULT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_BOOK,
    SPELL_DESC_TOWN | SPELL_DESC_DIRECTION,
    PATH_SPIRIT, "probebullet",SPELL_ACTIVE
    },

    {"remove death sickness",            SPELL_TYPE_PRIEST, 2,
    5, 24, 3, 6,0, 1.0,
    0,       0,     0,      0,
    0,  0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST, /* npc/god only atm */
    SPELL_DESC_SELF | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"restoration",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"lightning",                   SPELL_TYPE_WIZARD, 2,
    6, 12, 3, 6,0, 1.0,
    108,      8,      4,      0,
    1, 3, 0, 5, SOUND_MAGIC_ELEC,
    SPELL_USE_CAST | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD | SPELL_USE_DUST | SPELL_USE_BOOK | SPELL_USE_POTION,
    SPELL_DESC_DIRECTION,
    PATH_ELEMENTAL, "lightning", SPELL_ACTIVE
    },

    {"remove slow",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"remove fear",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"remove snare",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"remove paralysis",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"remove confusion",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"remove blindness",            SPELL_TYPE_PRIEST, 2,
    5, 16, 3, 6,3, 1.0,
    0,       0,     0,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST | SPELL_USE_SCROLL | SPELL_USE_HORN | SPELL_USE_WAND | SPELL_USE_ROD,
    SPELL_DESC_SELF | SPELL_DESC_FRIENDLY | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_LIFE, "meffect_purple",SPELL_ACTIVE
    },

    {"hallowed ground",             SPELL_TYPE_PRIEST, 50,
    5, 35, 3, 6,3, 1.0,
    30,       0,     4,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST, SPELL_DESC_SELF | SPELL_DESC_WIS,
    PATH_SPIRIT, "meffect_purple",SPELL_ACTIVE
    },

    {"sanctuary",             SPELL_TYPE_PRIEST, 100,
    5, 35, 3, 6,3, 1.0,
    30,       0,     4,      0,
    0, 0, 0, 0, SOUND_MAGIC_STAT,
    SPELL_USE_CAST, SPELL_DESC_SELF | SPELL_DESC_TOWN | SPELL_DESC_WIS,
    PATH_SPIRIT, "meffect_purple",SPELL_ACTIVE
    }

};

const char   *spellpathnames[NRSPELLPATHS]    =
{
    "Life", "Death", "Elemental", "Energy",
    "Spirit", "Protection", "Light", "Nether",
    "Nature", "Shadow", "Chaos", "Earth",
    "Conjuration", "Abjuration", "Transmutation", "Arcane"
};

static int  cardinal_adjust[9]  =
{
    -3, -2, -1, 0, 0, 0, 1, 2, 3
};
static int  diagonal_adjust[10] =
{
    -3, -2, -2, -1, 0, 0, 1, 2, 2, 3
};

archetype_t  *spellarch[NROFREALSPELLS];

static int   CheckSpellInstance(msp_t *msp, object_t *op);
static sint8 Disallowed(msp_t *msp, object_t *who, int nr);

void init_spells()
{
    static int  init_spells_done    = 0;
    int         i;

    if (init_spells_done)
        return;
    LOG(llevDebug, "Initializing spells...");
    init_spells_done = 1;
    for (i = 0; i < NROFREALSPELLS; i++)
        if (spells[i].archname)
        {
            if ((spellarch[i] = find_archetype(spells[i].archname)) == NULL)
                LOG(llevError, "Error: Spell %s needs arch %s, your archetype file is out of date.\n", spells[i].name,
                    spells[i].archname);
        }
        else
            spellarch[i] = (archetype_t *) NULL;
    LOG(llevDebug, "done.\n");
}

void dump_spells()
{
    int i;

    for (i = 0; i < NROFREALSPELLS; i++)
    {
        const char*name1 =  NULL, *name2 = NULL;
        if (spellarch[i])
        {
            name1 = spellarch[i]->name;

            if (spellarch[i]->clone.other_arch)
                name2 = spellarch[i]->clone.other_arch->name;
        }
        LOG(llevInfo, "%s: %s: %s\n", STRING_SAFE(spells[i].name), STRING_SAFE(name1), STRING_SAFE(name2));
    }
}

/* this must be adjusted if we ever include multi tile effects! */
int insert_spell_effect(char *archname, map_t *m, int x, int y)
{
    archetype_t  *effect_arch;
    object_t     *effect_ob;

    if (!archname || !m)
    {
        LOG(llevBug, "BUG: insert_spell_effect: archname or map NULL.\n");
        return 1;
    }

    if (!(effect_arch = find_archetype(archname)))
    {
        LOG(llevBug, "BUG: insert_spell_effect: Couldn't find effect arch (%s).\n", archname);
        return 1;
    }

    /* prepare effect */
    effect_ob = arch_to_object(effect_arch);
    effect_ob->map = m;
    effect_ob->x = x;
    effect_ob->y = y;

    if (!insert_ob_in_map(effect_ob, m, NULL, 0))
    {
        LOG(llevBug, "BUG: insert_spell_effect: effect arch (%s) out of map (%s) (%d,%d) or failed insertion.\n",
            archname, effect_ob->map->name, x, y);
        /* something is wrong - kill object_t */
        if (!QUERY_FLAG(effect_ob, FLAG_REMOVED))
        {
            remove_ob(effect_ob);
            move_check_off(effect_ob, NULL, MOVE_FLAG_VANISHED);
        }
        return 1;
    }


    return 0;
}

spell * find_spell(int spelltype)
{
    if (spelltype<0 || spelltype>NROFREALSPELLS)
        return NULL;
    return &spells[spelltype];
}

int check_spell_known(object_t *op, int spell_type)
{
    int i;
    for (i = 0; i < (int) CONTR(op)->nrofknownspells; i++)
        if (CONTR(op)->known_spells[i] == spell_type)
            return 1;
    return 0;
}

void do_learn_spell(object_t *op, int spell)
{
    if (op->type != PLAYER)
    {
        LOG(llevBug, "BUG: do_learn_spell(): not a player ->%s\n", op->name);
        return;
    }

    if (check_spell_known(op, spell))
    {
        ndi(NDI_UNIQUE, 0, op, "You already know the spell '%s'!", spells[spell].name);
        return;
    }

    play_sound_player_only(CONTR(op), SOUND_LEARN_SPELL, SOUND_NORMAL, 0, 0);
    CONTR(op)->known_spells[CONTR(op)->nrofknownspells++] = spell;
    send_spelllist_cmd(op, spells[spell].name, SPLIST_MODE_ADD);
    ndi(NDI_UNIQUE, 0, op, "You have learned the %s %s!",
                  (spells[spell].type == SPELL_TYPE_PRIEST) ? "prayer" :
                  "spell", spells[spell].name);
}

void do_forget_spell(object_t *op, int spell)
{
    int i;

    if (op->type != PLAYER)
    {
        LOG(llevBug, "BUG: do_forget_spell(): not a player: %s (%d)\n", STRING_OBJ_NAME(op), spell);
        return;
    }
    if (!check_spell_known(op, spell))
    {
        LOG(llevBug, "BUG: do_forget_spell(): spell %d not known\n", spell);
        return;
    }

    play_sound_player_only(CONTR(op), SOUND_LOSE_SOME, SOUND_NORMAL, 0, 0);
    ndi(NDI_UNIQUE, 0, op, "You lose knowledge of %s.", spells[spell].name);

    send_spelllist_cmd(op, spells[spell].name, SPLIST_MODE_REMOVE);

    for (i = 0; i < CONTR(op)->nrofknownspells; i++)
    {
        if (CONTR(op)->known_spells[i] == spell)
        {
            CONTR(op)->known_spells[i] = CONTR(op)->known_spells[--CONTR(op)->nrofknownspells];
            return;
        }
    }
    LOG(llevBug, "BUG: do_forget_spell(): couldn't find spell %d\n", spell);
}

void SP_stat_adjust(object_t *caster, int spell, float *value)
{
    stat_t dmg_mult_stat = SPELL_STAT_BASE;
    stat_t dmg_add_stat = SPELL_STAT_BASE;

    if (caster->type != PLAYER)
    {
        return;
    }

    if (spells[spell].type == SPELL_TYPE_WIZARD)
    {
        dmg_mult_stat = caster->stats.Pow;
        dmg_add_stat = caster->stats.Int;
    }
    else if (spells[spell].type == SPELL_TYPE_PRIEST)
    {
        dmg_mult_stat = caster->stats.Wis;
        dmg_add_stat = caster->stats.Cha;
    }

    *value += dmg_add_stat - SPELL_STAT_BASE;
    *value *= 1.0f + (float)MAX(0, dmg_mult_stat - SPELL_STAT_BASE) / (float)SPELL_MULT_DIV;
}

/*
 * cast_spell():
 * Fires spell "type" in direction "dir".
 * If "ability" is true, the spell is the innate ability of a monster.
 * (ie, don't check for MSP_FLAG_NO_SPELLS and don't add AT_MAGIC to attacktype).
 *
 * op is the creature that is owner of the object that is casting the spell
 * caster is the actual object (wand, potion) casting the spell. can be
 *    same as op.
 * dir is the direction to cast in.
 * ability is true if it is an ability casting the spell.  These can be
 *    cast in no magic areas.
 * item is the type of object that is casting the spell.
 * stringarg is any options that are being used.
 *
 * returns true for successful casting
 */

/* Oct 95 - added cosmetic touches for MULTIPLE_GODS hack -b.t. */

int cast_spell(object_t *op, object_t *caster, int dir, int type, int ability, SpellTypeFrom item, char *stringarg)
{
    spell        *s;
    const char   *godname = NULL;
    object_t     *target = NULL,
                 *cast_op,
                 *guild;
    int           success = 0,
                  duration,
                  points_used = 0;
    rv_t          rv;
    player_t     *pl = NULL;
    uint32        mask;
    objectlink_t *list = NULL;

    if (!(s = find_spell(type)))
    {
        LOG(llevBug, "BUG:: %s:cast_spell(): Unknown spell: %d from: %s (%s)\n",
            __FILE__, type, STRING_OBJ_NAME(op), STRING_OBJ_NAME(caster));
        return 0;
    }

    if (!op)
    {
        op = caster;
    }

    if (op->type == PLAYER)
    {
        if (!(pl = CONTR(op)))
        {
            return 0;
        }

        guild = pl->guild_force;
    }

    /* TODO: This cast_op v op v caster stuff needs tidying (whole func does). */
    /* script NPC can ALWAYS cast - even in no spell areas! */
    if (item == spellNPC)
    {
        target = op; /* if spellNPC, this comes useally from a script */
        op = caster; /* and caster is the NPC and op the target */
        goto dirty_jump; /* change the pointers to fit this function and jump */
    }

    /* It looks like the only properties we ever care about from the casting
    * object (caster) is spell paths and level.
    */
    cast_op = op;
    if (!caster)
    {
        if (item == spellNormal)
            caster = op;
    }
    else
    {
        if (caster->map) /* caster has a map? then we use caster */
            cast_op = caster;
    }

    if (Disallowed(MSP_KNOWN(cast_op), cast_op, type))
    {
        return 0;
    }

    if (pl &&
        !(pl->gmaster_mode & GMASTER_MODE_SA))
    {
        if (guild && !item)
        {
            if ((guild->weight_limit & GUILD_NO_MAGIC) &&
                !(spells[type].flags & SPELL_DESC_WIS))
            {
                ndi(NDI_UNIQUE, 0, op, "Your Guild membership prevents casting spells!");

                return 0;
            }
            else if ((guild->weight_limit & GUILD_NO_PRAYER) &&
                     (spells[type].flags & SPELL_DESC_WIS))
            {
                ndi(NDI_UNIQUE, 0, op, "Your Guild membership prevents casting prayers!");

                return 0;
            }
        }

        /* ok... its item == spellNPC then op is the target of this spell  */
        pl->rest_mode = 0;

        /* cancel player spells which are denied - only real spells (not potion, wands, ...) */
        if (item == spellNormal)
        {
            if (caster->path_denied & s->path)
            {
                ndi(NDI_UNIQUE, 0, op, "It is denied for you to cast that spell.");
                return 0;
            }

            // removed temporary guild restriction
            /*if (guild &&
                ((guild->level) ? guild->level < s->level : s->level > 0))
            {
                ndi(NDI_UNIQUE, 0, op, "That spell is too difficult for you to cast. Your guild allows spells up to level %d and you tried to cast a spell with level %d",
                    (guild->level) ? guild->level : 255, s->level);
                return 0;
            }*/

            if (!(spells[type].flags & SPELL_DESC_WIS))
            {
                if (op->stats.sp < (points_used = SP_level_spellpoint_cost(op, caster, type)))
                {
                    ndi(NDI_UNIQUE, 0, op, "You don't have enough mana.");
                    return 0;
                }
            }
            else
            {
                if (op->stats.grace < (points_used = SP_level_spellpoint_cost(op, caster, type)))
                {
                    ndi(NDI_UNIQUE, 0, op, "You don't have enough grace.");
                    return 0;
                }
            }

            /* If it is an ability, assume that the designer of the archetype knows what they are doing.*/
            if (!ability &&
                SK_level(caster) < s->level)
            {
                ndi(NDI_UNIQUE, 0, op, "You lack enough skill to cast that spell.");
                return 0;
            }
        }

        /* if it a prayer, grab the players god - if we have non, we can't cast - except potions */
        if ((spells[type].flags & SPELL_DESC_WIS) &&
            item != spellPotion &&
            (godname = determine_god(op)) == shstr_cons.none)
        {
            ndi(NDI_UNIQUE, 0, op, "You need a deity to cast a prayer!");
            return 0;
        }
    }

    /* ok.... now we are sure we are able to cast.
     * perhaps some else happens but first we look for
     * a valid target.
     */
    if (item == spellPotion) /* applying potions always go in the applier itself (aka drink or break) */
    {
        /*  if the potion casts an onself spell, don't use the facing direction (given by apply.c)*/
        if (spells[type].flags & SPELL_DESC_SELF)
        {
            target = op;
            dir = 0;
        }
    }
    else if (find_target_for_spell(op, caster, &target, dir, spells[type].flags) == 0)
    {
        /* little trick - if we fail we set target== NULL - thats mark its "yourself" */
        if (pl)
        {
            ndi(NDI_UNIQUE, 0, op, "You can't cast this spell on %s!",
                (target) ? QUERY_SHORT_NAME(target, op) : "yourself");
        }

        return 0;
    }

    /* LOG(llevInfo,"TARGET: op: %s target: %s\n", STRING_OBJ_NAME(op), STRING_OBJ_NAME(target)); */

    if (target)
    {
        /* tell player his spell is redirect to himself */
        if (target == op)
        {
            if (pl)
            {
                ndi(NDI_UNIQUE, 0, op, "You auto-target yourself!");
            }
        }
        else
        {
            /* if valid target is not in range for selected spell, skip here casting */
            if (!RV_GET_OBJ_TO_OBJ(op, target, &rv, RV_FLAG_DIAGONAL_D) ||
                rv.distance > (unsigned int)spells[type].range)
            {
                if (pl)
                {
                    ndi(NDI_UNIQUE, 0, op, "Your target is out of range!");
                }
                /*
                else
                    LOG(llevInfo,"cast_spell: %s out of range for %s from %s", STRING_OBJ_NAME(target), spells[type].name, STRING_OBJ_NAME(op));
                */

                return 0;
            }

            if (Disallowed(MSP_KNOWN(target), op, type))
            {
                return 0;
            }
        }
    }

    /* chance to fumble the spell by to low wisdom */
    /* FIXME: we have now spell_fumble */
    if (item == spellNormal && op->type == PLAYER && s->flags & SPELL_DESC_WIS
            && RANDOM_ROLL(0, 99) < s->level / (float) MAX(1, op->chosen_skill->level) * 1)
    {
        play_sound_player_only(CONTR(op), SOUND_FUMBLE_SPELL, SOUND_NORMAL, 0, 0);
        ndi(NDI_UNIQUE, 0, op, "You fumble the prayer because your wisdom is low.");

        if (s->sp == 0) /* Shouldn't happen... */
            return 0;
        return(RANDOM_ROLL(1, SP_level_spellpoint_cost(op, caster, type)));
    }

    else if (item == spellNormal && op->type == PLAYER && !(s->flags & SPELL_DESC_WIS))
    {
        int fumble = CONTR(op)->spell_fumble
            - (op->chosen_skill->level / 10)
            + s->fumble_factor;

        if (RANDOM_ROLL(0,99) < fumble)
        {
            ndi(NDI_UNIQUE, 0, op, "You bungle the spell because you have too much heavy equipment in use.");
            return(RANDOM_ROLL(0, SP_level_spellpoint_cost(op, caster, type)));
        }
    }


    dirty_jump:
    /* FIXME: Really it's way too late to worry now...
     *
     * -- Smacky 20200604 */
    /* a last sanity check: are caster and target *really* valid? */
    if ((caster &&
         !OBJECT_INPLAY(caster)) ||
        (target &&
         !OBJECT_INPLAY(target)))
    {
        return 0;
    }

    if (op == caster) // means cast spell/prayer not wand, etc.
    {
        if (target &&
            op != target &&
            target->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, target, "%s %ss the %s ~%s~ on you.",
                QUERY_SHORT_NAME(op, target),
                (spells[type].type == SPELL_TYPE_PRIEST) ? "invoke" : "cast",
                (spells[type].type == SPELL_TYPE_PRIEST) ? "prayer" : "spell",
                spells[type].name);
        }
    }

    switch ((enum spellnrs) type)
    {
        case SP_MINOR_HEAL:
        case SP_CURE_POISON:
        case SP_CURE_DISEASE:
        case SP_REMOVE_DEPLETION:
        case SP_REMOVE_DEATHSICK:
        case SP_RESTORATION:
        case SP_REMOVE_SLOW:
        case SP_REMOVE_FEAR:
        case SP_REMOVE_SNARE:
        case SP_REMOVE_PARALYZE:
        case SP_REMOVE_CONFUSED:
        case SP_REMOVE_BLIND:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = cast_heal(op, casting_level(caster, type), target, type);
        break;

        case SP_SANCTUARY:
        play_sound_player_only(pl, SOUND_TELEPORT, SOUND_NORMAL, 0, 0);
        (void)enter_map_by_name(pl->ob, pl->savebed_map, pl->orig_savebed_map, pl->bed_x, pl->bed_y, pl->bed_status);
        success = 1;
        break;

        case SP_STRENGTH:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = cast_change_attr(op, caster, target, dir, type);
        break;

        case SP_DETECT_MAGIC:
        case SP_DETECT_CURSE:
        case SP_IDENTIFY:
        case SP_REMOVE_CURSE:
        case SP_REMOVE_DAMNATION:
        if (target != op)
        {
            if (op->type == PLAYER)
            {
                ndi(NDI_UNIQUE, 0, op, "You cast ~%s~ on %s.",
                    spells[type].name, QUERY_SHORT_NAME(target, op));
            }

            if (target->type == PLAYER)
            {
                ndi(NDI_UNIQUE, 0, target, "%s casts ~%s~ on you.",
                    QUERY_SHORT_NAME(op, target), spells[type].name);
            }
        }

        if (type == SP_DETECT_MAGIC)
        {
            mask = THING_DECLARE_BIT_DETECT | THING_DECLARE_BIT_MAGICAL;
        }
        else if (type == SP_DETECT_CURSE)
        {
            mask = THING_DECLARE_BIT_DETECT | THING_DECLARE_BIT_CURSED |
                THING_DECLARE_BIT_DAMNED;
        }
        else if (type == SP_IDENTIFY)
        {
            mask = THING_DECLARE_BIT_DETECT | THING_DECLARE_BIT_IDENTIFIED;
        }
        else if (type == SP_REMOVE_CURSE)
        {
            mask = THING_DECLARE_BIT_REMOVE | THING_DECLARE_BIT_CURSED;
        }
        else // if (type == SP_REMOVE_DAMNATION)
        {
            mask = THING_DECLARE_BIT_REMOVE | THING_DECLARE_BIT_CURSED |
                THING_DECLARE_BIT_DAMNED;
        }

        (void)sparkly_create(spellarch[type], target, -1, 0, 0);
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = thing_declare(op, target, NULL, casting_level(caster, type), mask, &list);

        if (target != op &&
            op->type == PLAYER)
        {
            ndi(NDI_UNIQUE, 0, op, "The magic affected ~%d~ items.",
                success);
        }

        while (list)
        {
            objectlink_t *link = list;

            if (target->type == PLAYER)
            {
                ndi(NDI_UNIQUE, 0, target, "The magic affected %s.",
                    query_name(link->objlink.ob, target, ARTICLE_POSSESSIVE, 0));
            }

            objectlink_delink(&list, NULL, link);
        }
        break;

          /* spells after this use direction and not a target */
        case SP_ICESTORM:
        case SP_FIRESTORM:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        duration = spells[type].bdur;  /*  get the base duration */
        success = cast_cone(op, caster, dir, duration, type, spellarch[type], casting_level(caster, type), !ability);
        break;

        case SP_BULLET:
        case SP_CAUSE_LIGHT:
        case SP_PROBE:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = fire_arch(op, caster,op->x, op->y, dir, spellarch[type], type, casting_level(caster, type), 1);
        break;

        case SP_FROSTBOLT:
        case SP_FIREBOLT:
        case SP_S_LIGHTNING:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = fire_bolt(op,caster,dir,type,!ability);
        break;

        case SP_HALLOWED_GROUND:
        play_sound_map(MSP_KNOWN(op), spells[type].sound, SOUND_SPELL);
        success = cast_hallowed_ground(op, casting_level(caster, type), type);
        break;

        default:
        LOG(llevBug, "BUG: cast_spell() has invalid spell nr. %d\n", type);
    } /* end of switch */

    if (item == spellNPC)
        return success;

#ifdef SPELLPOINT_LEVEL_DEPEND
    return success ? SP_level_spellpoint_cost(op, caster, type) : 0;
#else
    return success ? (s->sp * PATH_SP_MULT(op, s)) : 0;
#endif
}

/* Disallowed checks if who can cast spell nr here according to msp/map
 * flags. */
/* TODO: This is a bit basic yet and is only meant for boolean, map-level
 * anti-magic effects. Dynamic effects (eg, someone else's anti-magic shield)
 * coming soon! */
static sint8 Disallowed(msp_t *msp, object_t *who, int nr)
{
    if (!(spells[nr].flags & SPELL_DESC_WIS))
    {
        if ((msp->flags & MSP_FLAG_NO_SPELLS))
        {
            if (msp == MSP_KNOWN(who))
            {
                ndi(NDI_UNIQUE, 0, who, "Powerful forces prevent casting ~%s~ here!",
                    skills[SK_WIZARDRY_SPELLS]->clone.name);
            }

            return 1;
        }
    }
    else
    {
        if ((msp->flags & MSP_FLAG_NO_PRAYERS))
        {
            if (msp == MSP_KNOWN(who))
            {
                ndi(NDI_UNIQUE, 0, who, "Powerful forces prevent invoking ~%s~ here!",
                    skills[SK_DIVINE_PRAYERS]->clone.name);
            }

            return 1;
        }
    }

    if (!(spells[nr].flags & SPELL_DESC_TOWN) &&
        (msp->flags & MSP_FLAG_NO_HARM))
    {
        if (msp == MSP_KNOWN(who))
        {
            ndi(NDI_UNIQUE, 0, who, "Powerful forces prevent all harmful magic here!");
        }

        return 1;
    }

    if ((spells[nr].flags & SPELL_DESC_SUMMON) &&
        (msp->map->flags & MAP_FLAG_NO_SUMMON))
    {
        if (msp == MSP_KNOWN(who))
        {
            ndi(NDI_UNIQUE, 0, who, "Powerful forces prevent all summoning here!");
        }

        return 1;
    }

    return 0;
}

int fire_bolt(object_t *op, object_t *caster, int dir, int type, int magic)
{
    map_t *m = op->map;
    sint16     x = op->x + OVERLAY_X(dir),
               y = op->y + OVERLAY_Y(dir);
    msp_t  *msp = MSP_GET2(m, x, y);
    object_t *tmp;
    float    tmp_dam;

    if (!msp ||
        !spellarch[type] ||
        !(tmp = arch_to_object(spellarch[type])))
    {
        return 0;
    }

    set_owner(tmp, op);
    tmp->level = casting_level(caster, type);

    tmp_dam = (float)SP_lvl_dam_adjust(tmp->level, type, spells[type].bdam);
    SP_stat_adjust(caster, type, &tmp_dam);

    /* give bonus or malus to damage depending on if the player/mob is attuned/repelled to that spell path */
    tmp->stats.dam = (int)(tmp_dam * PATH_DMG_MULT(op, find_spell(type)));
    tmp->stats.hp = spells[type].bdur;
    tmp->x = x;
    tmp->y = y;
    tmp->direction = dir;

    if (QUERY_FLAG(tmp, FLAG_IS_TURNABLE))
    {
        SET_ANIMATION(tmp, (NUM_ANIMATIONS(tmp) / NUM_FACINGS(tmp)) * tmp->direction);
    }

    set_owner(tmp, op);
    copy_owner(tmp, op);
    tmp->weight_limit = tmp->count; /* *very* important - miss this and the spells go really wild! */

    if (MSP_IS_RESTRICTED(msp))
    {
        if (!QUERY_FLAG(tmp, FLAG_REFLECTING))
        {
            return 0;
        }

        m = op->map;
        tmp->x = op->x;
        tmp->y = op->y;
        tmp->direction = absdir(dir + 4);

        if (QUERY_FLAG(tmp, FLAG_IS_TURNABLE))
        {
            SET_ANIMATION(tmp, (NUM_ANIMATIONS(tmp) / NUM_FACINGS(tmp)) * tmp->direction);
        }
    }

    if ((tmp = insert_ob_in_map(tmp, m, op, 0)))
    {
        move_bolt(tmp);
    }

    return 1;
}

/*  peterm  added a type field to fire_arch.  Needed it for making
    fireball etall level dependent.
    Later added a ball-lightning firing routine.
 * dir is direction, at is spell we are firing.  Type is index of spell
 * array.  If magic is 1, then add magical attacktype to spell.
 * op is either the owner of the spell (player who gets exp) or the
 * casting object owned by the owner.  caster is the casting object.
 */
int fire_arch(object_t *op, object_t *caster, sint16 x, sint16 y, int dir, archetype_t *at, int type, int level, int magic)
{
    object_t *tmp, *env;
    float     tmp_dam;

    if (at == NULL)
        return 0;
    for (env = op; env->env != NULL; env = env->env)
        ;
    if (env->map == NULL)
        return 0;
    tmp = arch_to_object(at);
    if (tmp == NULL)
        return 0;
    tmp->stats.sp = type;

    tmp_dam = (float) SP_lvl_dam_adjust(level, type, spells[type].bdam);
    SP_stat_adjust(caster, type, &tmp_dam);

    /* give bonus or malus to damage depending on if the player/mob is attuned/repelled to that spell path */
    tmp->stats.dam = (int) (tmp_dam * PATH_DMG_MULT(op, find_spell(type)));
    tmp->stats.hp = spells[type].bdur + SP_level_strength_adjust(op, caster, type);
    tmp->x = x, tmp->y = y;
    tmp->direction = dir;
    tmp->stats.grace = tmp->last_sp;
    tmp->stats.maxgrace = 60 + (RANDOM() % 12);

    if (get_owner(op) != NULL)
        copy_owner(tmp, op);
    else
        set_owner(tmp, op);
    tmp->level = level;

    if (QUERY_FLAG(tmp, FLAG_IS_TURNABLE))
        SET_ANIMATION(tmp, (NUM_ANIMATIONS(tmp) / NUM_FACINGS(tmp)) * dir);

    if ((tmp = insert_ob_in_map(tmp, op->map, op, 0)) == NULL)
        return 1;

    move_fired_arch(tmp);
    return 1;
}

int cast_cone(object_t *op, object_t *caster, int dir, int strength, int spell_type, archetype_t *spell_arch, int level, int magic)
{
    object_t *tmp;
    int     i, success = 0, range_min = -1, range_max = 1;
    uint32  count_ref;
    float    tmp_dam;

    if (!dir)
        range_min = -3,range_max = 4,strength /= 2;

    tmp = arch_to_object(spell_arch); /* thats our initial spell object_t */
    if (!tmp)
    {
        LOG(llevBug, "cast_cone(): arch_to_object() failed!? (%s)\n", spell_arch->name);
        return 0;
    }
    count_ref = tmp->count;
    for (i = range_min; i <= range_max; i++)
    {
        map_t *m = op->map;
        sint16     x = op->x + OVERLAY_X(absdir(dir + i)),
                   y = op->y + OVERLAY_Y(absdir(dir + i));
        msp_t  *msp = MSP_GET2(m, x, y);

        if (!msp ||
            MSP_IS_RESTRICTED(msp))
        {
            continue;
        }

        success = 1;
        if (!tmp)
            tmp = arch_to_object(spell_arch);
        set_owner(tmp, op);
        copy_owner(tmp, op);
        tmp->weight_limit = count_ref; /* *very* important - miss this and the spells go really wild! */

        tmp->level = level;
        tmp->x = x,tmp->y = y;

        if (dir)
            tmp->stats.sp = dir;
        else
            tmp->stats.sp = i;

        /* for b4, most cone effects have a fixed strength, means ldur = 0 */
        tmp->stats.hp = strength + (SP_level_strength_adjust(op, caster, spell_type) / 5);

        tmp_dam = (float) SP_lvl_dam_adjust(level, spell_type, spells[spell_type].bdam);
        SP_stat_adjust(caster, spell_type, &tmp_dam);

        /* lets check the originator of the spell (player, mob) has bonus/malus from spell path */
        tmp->stats.dam = (int) (tmp_dam * PATH_DMG_MULT(op, find_spell(spell_type)));

        tmp->stats.maxhp = tmp->count;
        if (!QUERY_FLAG(tmp, FLAG_FLYING))
            LOG(llevDebug, "cast_cone(): arch %s doesn't have flying 1\n", spell_arch->name);
        if ((!QUERY_FLAG(tmp, FLAG_WALK_ON) || !QUERY_FLAG(tmp, FLAG_FLY_ON)) && tmp->stats.dam)
            LOG(llevDebug, "cast_cone(): arch %s doesn't have walk_on 1 and fly_on 1\n", spell_arch->name);

        if (QUERY_FLAG(tmp, FLAG_IS_TURNABLE))
            SET_ANIMATION(tmp, (NUM_ANIMATIONS(tmp) / NUM_FACINGS(tmp)) * dir);

        if (!insert_ob_in_map(tmp, m, op, 0))
            return 0;
        if (tmp->other_arch)
            cone_drop(tmp);
        tmp = NULL;
    }

    if (tmp) /* can happens when we can't drop anything */
    {
        if (!QUERY_FLAG(tmp, FLAG_REMOVED))
            remove_ob(tmp); /* was not inserted */
    }
    return success;
}

/* this function checks to see if the cone pushes objects as well
 * as flies over and damages them */
void check_cone_push(object_t *op)
{
    msp_t  *msp = MSP_KNOWN(op);
    int        weight_move = 1000 + 1000 * op->level;
    object_t    *tmp,
              *next,
              *tmp2; /* object on the map */

    FOREACH_OBJECT_IN_MSP(tmp, msp, next)
    {
        int num_sections   = 1;

        /* don't move parts of objects */
        if (tmp->head)
        {
            continue;
        }

        /* don't move floors or immobile objects */
        if (QUERY_FLAG(tmp, FLAG_NO_PICK))
        {
            continue;
        }

        /* count the object's sections */
        for (tmp2 = tmp; tmp2; tmp2 = tmp2->more)
        {
            num_sections++;
        }

        /* move it. */
        if (RANDOM_ROLL(0, weight_move - 1) > tmp->weight / num_sections)
        {
            (void)move_ob(tmp, absdir(op->stats.sp), op);
        }
    }
}

/* drops an object based on what is in the cone's "other_arch" */
void cone_drop(object_t *op)
{
    object_t *new_ob  = arch_to_object(op->other_arch);
    new_ob->x = op->x;
    new_ob->y = op->y;
    new_ob->stats.food = op->stats.hp;
    new_ob->level = op->level;
    set_owner(new_ob, op->owner);
    if (op->chosen_skill)
    {
        new_ob->chosen_skill = op->chosen_skill;
        new_ob->skillgroup = op->chosen_skill->skillgroup;
    }
    insert_ob_in_map(new_ob, op->map, op, 0);
}


void move_cone(object_t *op)
{
    int     i;
    tag_t   tag;

    /* if no map then ignore op. */
    if (!op->map)
    {
        LOG(llevBug, "BUG: Tried to move_cone object %s without a map.\n", STRING_OBJ_NAME(op));
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        return;
    }

    /* lava saves it's life, but not yours  :) */
    if (QUERY_FLAG(op, FLAG_LIFESAVE))
    {
        (void)hit_map(op, MSP_KNOWN(op));
        return;
    }

    /* If no owner left, the spell dies out. */
    if (get_owner(op) == NULL)
    {
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        return;
    }

    /* Hit map returns 1 if it hits a monster.  If it does, set
     * food to 1, which will stop the cone from progressing.
     */
    tag = op->count;
    op->stats.food = hit_map(op, MSP_KNOWN(op));
    /* Check to see if we should push anything.
     * Cones with AT_PHYSICAL push whatever is in them to some
     * degree.  */
    /*
    if (op->attacktype & AT_PHYSICAL)
        check_cone_push(op);
    */
    if (!OBJECT_VALID(op, tag))
        return;

    if ((op->stats.hp -= 2) < 0)
    {
        if (op->stats.exp)
        {
            op->speed = 0;
            update_ob_speed(op);
            op->stats.exp = 0;
            op->stats.sp = 0; /* so they will join */
        }
        else
        {
            remove_ob(op);
            move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        }
        return;
    }

    if (op->stats.food)
        return;

    op->stats.food = 1;

    for (i = -1; i < 2; i++)
    {
        map_t *m = op->map;
        sint16     x = op->x + OVERLAY_X(absdir(op->stats.sp + i)),
                   y = op->y + OVERLAY_Y(absdir(op->stats.sp + i));
        msp_t  *msp = MSP_GET2(m, x, y);

        if (!msp ||
            MSP_IS_RESTRICTED(msp))
        {
            continue;
        }

        if (CheckSpellInstance(msp, op))
        {
            object_t *tmp = arch_to_object(op->arch);

            copy_owner(tmp, op);
            /* *very* important - this is the count value of the
             * *first* object we created with this cone spell.
             * we use it for identify this spell. Miss this
             * and CheckSpellInstance() will allow to create
             * 1000s in a single tile! */
            tmp->weight_limit = op->weight_limit;
            tmp->x = x;
            tmp->y = y;

            /* holy word stuff */
            /*
                    if(tmp->attacktype&AT_HOLYWORD||tmp->attacktype&AT_GODPOWER)
                    if(!tailor_god_spell(tmp,op)) return;
                    */
            tmp->level = op->level;
            tmp->stats.sp = op->stats.sp,tmp->stats.hp = op->stats.hp + 1;
            tmp->stats.maxhp = op->stats.maxhp;
            tmp->stats.dam = op->stats.dam;

            if (!insert_ob_in_map(tmp, m, op, 0))
            {
                return;
            }

            if (tmp->other_arch)
                cone_drop(tmp);
        }
    }
}

/* This function ensures that a AoE spell only put *one* instance/object of
 * themself in a tile. */
static int CheckSpellInstance(msp_t *msp, object_t *op)
{
    object_t *tmp,
           *next;

    FOREACH_OBJECT_IN_MSP(tmp, msp, next)
    {
        /* weight_limit is the ->count of the original spell object which started
         * this spell!
         */
        if (op->type == tmp->type &&
            op->weight_limit == tmp->weight_limit)
        {
            return 0; /* only one part for cone/explosion per tile! */
        }
    }

    return 1;
}

void forklightning(object_t *op, object_t *tmp)
{
    map_t  *m;
    sint16      x,
                y;
    msp_t   *msp;
    int         new_dir = 1, /* direction or -1 for left, +1 for right 0 if no new bolt */
                t_dir; /* stores temporary dir calculation */

    /* pick a fork direction.  tmp->stats.Con is the left bias
             i.e., the chance in 100 of forking LEFT
              Should start out at 50, down to 25 for one already going left
             down to 0 for one going 90 degrees left off original path*/

    if (RANDOM_ROLL(0, 99) < tmp->stats.Con)  /* fork left */
        new_dir = -1;

    /* check the new dir for a wall and in the map*/
    t_dir = absdir(tmp->direction + new_dir);
    m = tmp->map;
    x = tmp->x + OVERLAY_X(t_dir);
    y = tmp->y + OVERLAY_Y(t_dir);
    msp = MSP_GET2(m, x, y);

    if (!msp ||
        MSP_IS_RESTRICTED(msp))
    {
        new_dir = 0;
    }

    if (new_dir)
    {
        /* OK, we made a fork */
        object_t *new_bolt    = get_object();
        copy_object(tmp, new_bolt);

        /* reduce chances of subsequent forking */
        new_bolt->stats.Dex -= 10;
        tmp->stats.Dex -= 10;  /* less forks from main bolt too */
        new_bolt->stats.Con += 25 * new_dir; /* adjust the left bias */
        new_bolt->speed_left = -0.1f;
        new_bolt->stats.hp++;
        new_bolt->stats.dam /= 2;  /* reduce daughter bolt damage */
        new_bolt->stats.dam++;
        tmp->stats.dam /= 2;  /* reduce father bolt damage */
        tmp->stats.dam++;
        OBJECT_TURN(new_bolt, t_dir);
        new_bolt->x = x;
        new_bolt->y = y;
        if (!insert_ob_in_map(new_bolt, m, op, 0))
            return;
    }
}

/* reflwall - decides weither the (spell-)object sp_op will
 * be reflected from the given mapsquare. Returns 1 if true.
 * (Note that for living creatures there is a small chance that
 * reflect_spell fails.)
 * This function don't scale up now - it uses map tile flags. MT-2004
 */
int reflwall(msp_t *msp, object_t *sp_op)
{
    if (!(msp->flags & MSP_FLAG_REFL_CASTABLE))
    {
        return 0;
    }

    /* we have reflection. But there is a small chance it will fail. */
    if (sp_op->type == LIGHTNING) /* reflect always */
    {
        return 1;
    }

    if (!missile_reflection_adjust(sp_op, QUERY_FLAG(sp_op, FLAG_WAS_REFLECTED)))
    {
        return 0;
    }

    /* we get resisted - except a small fail chance */
    if ((RANDOM_ROLL(0, 99)) < 90 - sp_op->level / 10)
    {
        SET_FLAG(sp_op, FLAG_WAS_REFLECTED);
        return 1;
    }

    return 0;
}

void move_bolt(object_t *op)
{
    object_t *tmp;
    int     w, r;

    if (!op->map)
    {
        return;
    }

    if (--(op->stats.hp) < 0)
    {
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        return;
    }

    (void)hit_map(op, MSP_KNOWN(op));

    if (!op->stats.sp)
    {
        sint8   dir = op->direction;
        map_t  *m = op->map;
        sint16  x = op->x + OVERLAY_X(dir);
        sint16  y = op->y + OVERLAY_Y(dir);
        msp_t  *msp = MSP_GET2(m, x, y);

        op->stats.sp = 1;

        if (!msp ||
            !dir ||
            (msp->flags & MSP_FLAG_NO_SPELLS))
        {
            return;
        }

        w = MSP_IS_RESTRICTED(msp);
        r = reflwall(msp, op);

        if (w ||
            r)
        {
            /* We're about to bounce */
            if (!QUERY_FLAG(op, FLAG_REFLECTING))
            {
                return;
            }

            op->stats.sp = 0;

            if ((dir & 1))
            {
                dir = absdir(dir + 4);
            }
            else
            {
                msp_t *msp_l,
                         *msp_r;

                m = op->map;
                x = op->x + OVERLAY_X(absdir(dir - 1));
                y = op->y + OVERLAY_Y(absdir(dir - 1));
                msp_l = MSP_GET2(m, x, y);
                m = op->map;
                x = op->x + OVERLAY_X(absdir(dir + 1));
                y = op->y + OVERLAY_Y(absdir(dir + 1));
                msp_r = MSP_GET2(m, x, y);

                if (MSP_IS_RESTRICTED(msp_l) &&
                    MSP_IS_RESTRICTED(msp_r))
                    dir = absdir(dir + 4);
                else if (MSP_IS_RESTRICTED(msp_l))
                    dir = absdir(dir + 2);
                else if (MSP_IS_RESTRICTED(msp_r))
                    dir = absdir(dir - 2);
            }

            OBJECT_TURN(op, dir); /* A bolt *must* be IS_TURNABLE */
            return;
        }
        /* disabling this line allows back bouncing! */
/*        else if (!CheckSpellInstance(msp, op) ) */
        else
        {
            /* Create a copy of this object and put it ahead */
            tmp = get_object();
            copy_object(op, tmp);
            tmp->speed_left = -0.1f;
            tmp->weight_limit = op->weight_limit;
            tmp->stats.sp = 0;
            tmp->x += OVERLAY_X(tmp->direction);
            tmp->y += OVERLAY_Y(tmp->direction);

            if (!insert_ob_in_map(tmp, op->map, op, 0))
            {
                return;
            }

            /* Possibly create forks of this object going off in other directions. */
            if (RANDOM_ROLL(0, 99) < tmp->stats.Dex)
            {
                /* stats.Dex % of forking */
                forklightning(op, tmp);
            }

            if (tmp)
            {
                if (!tmp->stats.food)
                {
                    tmp->stats.food = 1;
                    move_bolt(tmp);
                }
                else
                {
                    tmp->stats.food = 0;
                }
            }
        }
    }
}

/* explode object will remove the exploding object from a container and set on map.
 * for this action, out_of_map() is called.
 * If the object is on a map, we assume that the position is controlled when object
 * is inserted or moved, so no need to recontrol. MT. */
void explode_object(object_t *op)
{
    tag_t       op_tag  = op->count;
    object_t     *tmp,
               *env;

    play_sound_map(MSP_KNOWN(op), SOUND_OB_EXPLODE, SOUND_NORMAL);
    if (op->other_arch == NULL)
    {
        LOG(llevBug, "BUG: explode_object(): op %s without other_arch\n", STRING_OBJ_NAME(op));
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        return;
    }

    if (op->env) /* object is in container, try to drop on map! */
    {
        map_t  *m;
        sint16  x,
                y;

        for (env = op; env->env != NULL; env = env->env)
            ;

        m = env->map;
        x = env->x;
        y = env->y;

        if (!(m = OUT_OF_MAP(m, x, y)))
        {
            LOG(llevBug, "BUG: explode_object(): env out of map (%s)\n", STRING_OBJ_NAME(op));
            remove_ob(op);
            move_check_off(op, NULL, MOVE_FLAG_VANISHED);
            return;
        }

        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
        op->x = x;
        op->y = y;

        if (!insert_ob_in_map(op, m, op, 0))
            return;
    }

    /*  peterm:  hack added to make fireballs and other explosions level
     *  dependent:
     */
    /*  op->stats.sp stores the spell which made this object here. */
    tmp = arch_to_object(op->other_arch);
    switch (tmp->type)
    {
        case CONE:
          {
              int   type    = tmp->stats.sp;

              if (!type)
                  type = op->stats.sp;
              copy_owner(tmp, op);
              cast_cone(op, op, 0, spells[type].bdur, type, op->other_arch, op->level, 0);
              break;
          }
    }

    /* remove the firebullet */
    if (OBJECT_VALID(op, op_tag))
    {
        remove_ob(op);
        move_check_off(op, NULL, MOVE_FLAG_VANISHED);
    }
}

void drain_rod_charge(object_t *rod)
{
    rod->stats.hp -= spells[rod->stats.sp].sp;
    if (QUERY_FLAG(rod, FLAG_ANIMATE))
        fix_rod_speed(rod);
}

void fix_rod_speed(object_t *rod)
{
    rod->speed = (ABS(rod->arch->clone.speed) * rod->stats.hp) / (float) rod->stats.maxhp;
    if (rod->speed < 0.02f)
        rod->speed = 0.02f;
    update_ob_speed(rod);
}


/* i changed this function to take care about target system.
 * This function examines target & (real) hitter and the
 * given spell flags. We have to decide some flags including
 * PvP flags - note that normally player are friendly to other
 * players except for pvp - there they are enemys.
 * op is owner/object which casted the spell, item is the object
 * which was used (scroll, potion, pet).
 */
int find_target_for_spell(object_t *op, object_t *item, object_t **target, int dir, uint32 flags)
{
    object_t *tmp;

    /*  LOG(llevInfo,"FIND_TARGET_for_spell: op: %s used: %s dir %d flags: %x\n", op->name, item?item->name:"<none>", dir, flags);*/

    *target = NULL; /* default target is - nothing! */

    if (flags & SPELL_DESC_DIRECTION) /* we cast something on the map... no target */
        return 1;

    if (op->type == PLAYER) /* a player has invoked this spell */
    {
        /* try to cast on self but only when really no friendly or enemy is set */
        if ((flags & SPELL_DESC_SELF) && !(flags & (SPELL_DESC_ENEMY | SPELL_DESC_FRIENDLY)))
        {
            *target = op; /* self ... and no other tests */
            return 1;
        }

        tmp = CONTR(op)->target_ob;

        /* lets check our target - we have one? friend or enemy? */
        if (!OBJECT_INPLAY(tmp) || tmp == CONTR(op)->ob || CONTR(op)->target_tag != tmp->count)
        {
            /* no valid target, or we target self! */
            if (flags & SPELL_DESC_SELF) /* can we cast this on self? */
            {
                *target = op; /* right, we are target */
                return 1;
            }
        }
        else /* we have a target and its not self */
        {
            /* Friend spells are allowed on friends, neutrals and other players */
            if (flags & SPELL_DESC_FRIENDLY)
            {
                if (get_friendship(op, tmp) > FRIENDSHIP_ATTACK)
                {
                    *target = tmp;
                    return 1;
                }
            }

            /* enemy spells only allowed on enemies or neutrals */
            if (flags & SPELL_DESC_ENEMY)
            {
                if (get_friendship(op, tmp) < FRIENDSHIP_HELP)
                {
                    *target = tmp;
                    return 1;
                }
            }

            /* Self spells are always allowed */
            if (flags & SPELL_DESC_SELF)
            {
                *target = op;
                return 1;
            }
        }
    }
    else /* thats a mob OR rune/firewall/.. OR a pet/summon controlled from player */
    {
        /* we use op->enemy as target from non player caster.
         * we need to set this from outside and for healing spells,
         * we must set from outside temporary the enemy to a friendly unit.
         * This is safe because we do no AI stuff here - we simply USE the
         * target here even the stuff above looks like we select one...
         * its only a fallback.
         */

        /* sanity check for a legal target */
        if (OBJECT_VALID(op->enemy, op->enemy_count))
        {
            *target = op->enemy;
            return 1;
        }
    }

    return 0; /* invalid target/spell or whatever */
}

int reduction_dir[OVERLAY_7X7][3] =
{
    {0,0,0}, /* 0 */
    {0,0,0}, /* 1 */
    {0,0,0}, /* 2 */
    {0,0,0}, /* 3 */
    {0,0,0}, /* 4 */
    {0,0,0}, /* 5 */
    {0,0,0}, /* 6 */
    {0,0,0}, /* 7 */
    {0,0,0}, /* 8 */
    {8,1,2}, /* 9 */
    {1,2, -1}, /* 10 */
    {2,10,12}, /* 11 */
    {2,3, -1}, /* 12 */
    {2,3,4}, /* 13 */
    {3,4, -1}, /* 14 */
    {4,14,16}, /* 15 */
    {5,4, -1}, /* 16 */
    {4,5,6}, /* 17 */
    {6,5, -1}, /* 18 */
    {6,20,18}, /* 19 */
    {7,6, -1}, /* 20 */
    {6,7,8}, /* 21 */
    {7,8, -1}, /* 22 */
    {8,22,24}, /* 23 */
    {8,1, -1}, /* 24 */
    {24,9,10}, /* 25 */
    {9,10, -1}, /* 26 */
    {10,11, -1}, /* 27 */
    {27,11,29}, /* 28 */
    {11,12, -1}, /* 29 */
    {12,13, -1}, /* 30 */
    {12,13,14}, /* 31 */
    {13,14, -1}, /* 32 */
    {14,15, -1}, /* 33 */
    {33,15,35}, /* 34 */
    {16,15, -1}, /* 35 */
    {17,16, -1}, /* 36 */
    {18,17,16}, /* 37 */
    {18,17, -1}, /* 38 */
    {18,19, -1}, /* 39 */
    {41,19,39}, /* 40 */
    {19,20, -1}, /* 41 */
    {20,21, -1}, /* 42 */
    {20,21,22}, /* 43 */
    {21,22, -1}, /* 44 */
    {23,22, -1}, /* 45 */
    {45,47,23}, /* 46 */
    {23,24, -1}, /* 47 */
    {24,9, -1}
}; /* 48 */

/* Recursive routine to step back and see if we can
 * find a path to that monster that we found.  If not,
 * we don't bother going toward it.  Returns 1 if we
 * can see a direct way to get it
 * Modified to be map tile aware -.MSW
 */
/* hm, how fast is this function? MT-2004 */
int can_see_monsterP(map_t *m, int x, int y, int dir)
{
    sint16     dx,
               dy;
    msp_t  *msp;

    if (dir < 0)
    {
        return 0;  /* exit condition:  invalid direction */
    }

    dx = x + OVERLAY_X(dir);
    dy = y + OVERLAY_Y(dir);
    msp = MSP_GET2(m, dx, dy);

    if (!msp ||
        MSP_IS_RESTRICTED(msp))
    {
        return 0;
    }

    /* yes, can see. */
    if (dir < 9)
    {
        return 1;
    }

    return can_see_monsterP(m, x, y, reduction_dir[dir][0])
         | can_see_monsterP(m, x, y, reduction_dir[dir][1])
         | can_see_monsterP(m, x, y, reduction_dir[dir][2]);
}

/* July 1995 - changed slightly (SK_level) for ALLOW_SKILLS - b.t. */
/* now based on caster's level instead of on op's level and caster's    *
 * path modifiers.  --DAMN                      */
int SP_level_strength_adjust(object_t *op, object_t *caster, int spell_type)
{
    int level   = casting_level(caster, spell_type);
    int adj     = (level - spells[spell_type].level);
    if (adj < 0)
        adj = 0;
    if (spells[spell_type].ldur)
        adj /= spells[spell_type].ldur;
    else
        adj = 0;
    return adj;
}

/* April 2007 - i changed the spell cost to a better configurable, human
 * readable system. We have with level a low and with spl_max a high points border now,
 * where sp is the base cost and spl the additional points we add all spl_level over
 * the low border level. If its >spl_max we use spl_max.
 * Thats allows a unique setting for different spells.
 */
int SP_level_spellpoint_cost(object_t *op, object_t *caster, int spell_type)
{
    spell  *s       = find_spell(spell_type);
    int     level   = casting_level(caster, spell_type);
    int     sp        = s->sp;

#ifdef SPELLPOINT_LEVEL_DEPEND
    /* level defines our lower border */
    if(level > s->level && s->spl)
    {
        /* we add every spl_level spl points to the spell costs */
        level -= s->level;
        level /= s->spl_level;

        sp += level * s->spl;
        /* spl_max defines a upper border of sp costs */
        if(s->spl_max>0 && sp > s->spl_max)
            sp = s->spl_max;
    }
#endif /* SPELLPOINT_LEVEL_DEPEND */

    /* at path bonus/malus */
    sp = (int) ((float) sp * (float) PATH_SP_MULT(caster, s));

    return sp;
}

/*  look_up_spell_by_name:  peterm
    this function attempts to find the spell spname in spells[].
    if it doesn't exist, or if the op cannot cast that spname,
    -1 is returned.  */


int look_up_spell_by_name(object_t *op, const char *spname)
{
    int gmaster_mode = GET_GMASTER_MODE(op);
    int numknown;
    int spnum;
    int plen;
    int spellen;
    int i;

    if (!spname)
    {
        return -1;
    }

    if (!op)
    {
        numknown = NROFREALSPELLS;
    }
    else
    {
        numknown = ((gmaster_mode & GMASTER_MODE_SA)) ? NROFREALSPELLS : CONTR(op)->nrofknownspells;
    }

    plen = strlen(spname);

    for (i = 0; i < numknown; i++)
    {
        spnum = ((gmaster_mode & GMASTER_MODE_SA)) ? i : CONTR(op)->known_spells[i];
        spellen = strlen(spells[spnum].name);

        if (strncmp(spname, spells[spnum].name, MIN(spellen, plen)) == 0)
            return spnum;
    }
    return -1;
}

/* we use our damage/level tables to adjust the base_dam. Normally, the damage increase
 * with the level of the caster - or if the caster is a living object, with the level
 * of the used skill. */
int SP_lvl_dam_adjust(int level, int spell_type, int base_dam)
{
    /* get a base damage when we don't have one from caller */
    if (base_dam == -1)
        base_dam = spells[spell_type].bdam;

    /* ensure a legal level value */
    if (level < 0)
        level = 1;

    return (int)((float)base_dam * LEVEL_DAMAGE(level) * 0.1f);
}
