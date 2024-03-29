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
/* This file is really too large.  With all the .h files
 * around, this file should be better split between them - things
 * that deal with objects should be in objects.h, things dealing
 * with players in player.h, etc.  As it is, everything just seems
 * to be dumped in here.
 */

#ifndef __DEFINE_H
#define __DEFINE_H

// Guild flags to restrict certain skill usages.
#define GUILD_NO_MAGIC   1
#define GUILD_NO_PRAYER  2
#define GUILD_NO_ARCHERY 4
#define GUILD_NO_2H      8
#define GUILD_NO_POLEARM 16

/* spell list commands for client spell list */
/* TODO: Rename as such lists may be skills, spells, or prayers, and move to
 * protocol.h */
#define SPLIST_MODE_ADD    0
#define SPLIST_MODE_REMOVE 1
#define SPLIST_MODE_UPDATE 2

/* this marks no skill used */
#define CHOSEN_SKILL_NO (99999)

// #define PORTAL_DESTINATION_NAME "Town portal destination"

/* TYPE DEFINES */
/* Only add new values to this list if somewhere in the program code,
 * it is actually needed.  Just because you add a new monster does not
 * mean it has to have a type defined here.  That only needs to happen
 * if in some .c file, it needs to do certain special actions based on
 * the monster type, that can not be handled by any of the numerous
 * flags
 * Also, if you add new entries, try and fill up the holes in this list.
 */
/* type 0 will be undefined and shows a non valid type information */
/* TODO:  The marker ' // R' means all code referring to these obsolete types
 * has been removed so following a tidy of the arches and scripts they can be
 * removed from this list too (and the Lua one).
 *
 * -- Smacky 200160815 */
#define PLAYER                  1
#define BULLET                  2
#define ROD                     3
#define TREASURE                4
#define POTION                  5
#define FOOD                    6
#define POISON                  7
#define BOOK                    8
/* 9 is free. */
#define FBULLET                 10 // R
#define FBALL                   11 // R
#define LIGHTNING               12
#define ARROW                   13
#define BOW                     14
#define WEAPON                  15
#define ARMOUR                  16
#define PEDESTAL                17
#define ALTAR                   18
#define BUFF_FORCE              19
#define TYPE_DOOR               20
#define TYPE_KEY                21
#define MAP                     22 // R
/* 23 is free. */
#define TYPE_SPARKLY            24
#define MMISSILE                25 // R
#define TIMED_GATE              26
#define TRIGGER                 27 /* triggered when applied, resets after interval */
#define TYPE_FLOORMASK          28
#define MAGIC_EAR               29
#define TRIGGER_BUTTON          30
#define TRIGGER_ALTAR           31
#define TRIGGER_PEDESTAL        32
#define SHIELD                  33
#define HELMET                  34
#define HORN                    35
#define MONEY                   36
#define LOOT                    37
#define MONEYCARD               38
#define AMULET                  39
#define PLAYERMOVER             40
#define TELEPORTER              41
#define CREATOR                 42
#define TYPE_SKILL              43
#define TYPE_SKILLGROUP         44
#define EARTHWALL               45 // R
/* 46 is free. */
#define BOMB                    47
#define THROWN_OBJ              48
#define BLINDNESS               49 // R
#define GOD                     50
/*  peterm:  detector is an object which notices the presense of
     another object and is triggered like buttons.  */
#define DETECTOR                51
#define SPEEDBALL               52 // R
#define DEAD_OBJECT             53
#define DRINK                   54
#define MARKER                  55 /* inserts an invisible, weightless
                                   force into a player with a specified string. */
#define HOLY_ALTAR              56
#define PLAYER_CHANGER          57
/* 58 is free */
#define PEACEMAKER              59 // R
#define GEM                     60
#define FIRECHEST               61 // R
#define FIREWALL                62
#define ANVIL                   63
#define CHECK_INV               64  /* by b.t. thomas@nomad.astro.psu.edu */
/* 65 is free. */
#define EXIT                    66
#define TYPE_AGE_FORCE          67  /* this object is a age force */
#define SHOP_FLOOR              68
#define SHOP_MAT                69
#define RING                    70
#define FLOOR                   71 /* this is a floor tile -> native layer 0 */
/* 72 is free. */
#define INORGANIC               73  /* metals and minerals */
#define TYPE_LIGHT_APPLY        74  /* new light source for player */
/* 75 is free. */
#define TRAP_PART               76 // R
#define WALL                    77 /* this is a wall. put it always in layer 1 if not set is_floor */
#define LIGHT_SOURCE            78 /* thats now light sources on the map (invisible lights) */
#define MISC_OBJECT             79 /* misc. objects are for objects without a function
                                      in the engine. Like statues, clocks, chairs,...
                                      If perhaps we create a function where we can sit
                                      on chairs, we create a new type and remove all
                                      chairs from here. */
#define MONSTER                 80 /* yes, thats a real, living creature */
#define SPAWN_POINT             81 /* a spawn point or monster generator object_t */
#define TYPE_LIGHT_REFILL       82  /* refilling item for TYPE_LIGHT_APPLY */
#define SPAWN_POINT_MOB         83 /* inactive default mob inside spawn point inv.
                                    * This object is somewhat special because its generated
                                    * ITS IS ONLY A COPY AND NOT A REAL MONSTER NOR A ACTIVE OBJECT.
                                    */
#define SPAWN_POINT_INFO        84  /* this is put inside a mob, created from a spawn point.
                                         * It is used to track back the mob to the spawn point.
                                         */
#define SPELLBOOK               85
#define ORGANIC                 86 /* body parts which can't be eaten - dragon scales for example */
#define CLOAK                   87
#define CONE                    88
#define AURA                    89 // R
#define SPINNER                 90
#define GATE                    91
#define BUTTON                  92
#define CF_HANDLE               93 /* Only triggered when applied */
#define PIT                     94 /* PIT are called HOLE in crossfire) - normal hole is type EXIT */
#define TRAPDOOR                95
#define WORD_OF_RECALL          96 // R
#define PARAIMAGE               97 // R
#define SIGN                    98
#define BOOTS                   99
#define GLOVES                  100
#define TYPE_BASE_INFO          101 /* This object holds the real base stats of mobs or other active objects */
#define TYPE_RANDOM_DROP        102 /* only true for spawn points atm: if found, there is a random check against
                                     * carrying. If greater as carrying - all ->inv is droped.
                                     */
/* 103 is free. */
#define BRACERS                 104
#define HOMESTONE               105
#define SAVEBED                 106
#define POISONCLOUD             107
#define FIREHOLES               108 // R
#define WAND                    109
#define ABILITY                 110
#define SCROLL                  111
#define DIRECTOR                112
#define GIRDLE                  113
#define FORCE                   114
/* 115 is free (POTION_EFFECT in 0.10.6). */
#define TYPE_JEWEL              116    /* to make it different from GEM - thats needed to have a better */
#define TYPE_NUGGET             117    /* use from the artifacts file */
#define TYPE_EVENT_OBJECT       118    /* event/script object_t */
#define TYPE_WAYPOINT_OBJECT    119    /* waypoint object_t */
#define TYPE_QUEST_CONTAINER    120    /* used to store quest infos in players */
/* 121 is free. */
#define CONTAINER               122
/* 123 is free. */
/* 124 is free. */
#define TYPE_WEALTH             125     /* this is a "raw" (abstract) wealth object. When generated
                                         * its tranformed in real money depending on the enviroment
                                         * where its generated. (most times by treasure list and
                                         * tranformed to money by using level of mob or map to generating
                                         * a fitting amount of money basing on the base setting).
                                         */
#define TYPE_AI                 126     /* MOB AI definition object_t */
#define TYPE_AGGRO_HISTORY      127        /* aggro history - core base object for aggro handling. */
#define TYPE_DAMAGE_INFO        128        /* info object for aggro, group damage, exp sharing and DOT handling */
#define TYPE_QUEST_TRIGGER      129        /* a quest trigger describes & interact with quests & other quest things */
/* 130 is free. */
#define TYPE_QUEST_OBJECT       131        /* a quest object - it will be set to what we need. Its better & cleaner to
                                         * use a own type for it instead of marking "real" object with fancy flags
                                         */
#define TYPE_TIMER              132     /* Trigger a connection after a time period */
#define TYPE_ENV_SENSOR         133     /* Triggers depending on environment (TOD, brightness etc) */
#define TYPE_CONN_SENSOR        134     /* Triggers on other connections */
#define TYPE_PEARL              135        /* gem/jewel type pearl */
#define TYPE_QUEST_INFO         136
#define TYPE_BEACON             137     /* Beacons are used mostly to locate objects by scripts */
#define DEEP_SWAMP              138 // R
/* 139 is free. */
#define TYPE_GUILD_FORCE        140
#define CANCELLATION            141 // R
#define SHOULDER                142     /* armour... */
#define LEGS                    143     /* armour... */
#define TYPE_FOOD_FORCE         144
#define TYPE_FOOD_BUFF_FORCE    145
#define TYPE_QUEST_UPDATE       146
#define TYPE_SHOP_CONTAINER     147
/* 148 is free. */
/* 149 is free. */
/* 150 is free. */
#define BALL_LIGHTNING          151 // R
/* 152 is free. */
#define SWARM_SPELL             153 // R
#define RUNE                    154
/* 155 is free. */
#define POWER_CRYSTAL           156
#define CORPSE                  157
#define DISEASE                 158
#define SYMPTOM                 159
#define TYPE_VOID_CONTAINER     255 /* pure internal system object_t */
/* END TYPE DEFINE */

/* START SUB TYPE 1 DEFINE */
/* SPECIAL FLAGS */
/* because we use now a complexer, client controled fire & throw system,
 * we must mark different types/subtypes items for throwing like potions,
 * dust, weapons, special ammun, etc.
 */

#define SUBTYPE_MISSILE_THROW 128
/* These are very special subtypes - used by different types.
 * The reason is, that different items can be missiles - amuns & throw items.
 * First, we have the amun types - arrows for bows, or bolds for xbows.
 * They need a firing weapon and get fired indirekt. Daimonin don't allow
 * to throw amun - its really senseless to try to throw a bolt and hope
 * to do any serious damage with it.
 * For throwing we have some more types.
 * First, we have the 'real' throw weapons - like darts or shurikens.
 * Second, there are weapons which can be used as hand weapons but
 * also as throw weapons. A hammer like mjoellnir or a simple spear, for example.
 * Third, there are special throw items. ATM, we have
 * Potions - like firestorm
 * Dust - like dust of blinding... etc.
 * Dusts are also from ' type POTION' but use a different base arch.
 * These special items will handled different - we allow the player to throws
 * ANY dust or potion. But only when the potion/dust is from subtype _missile_xx
 * we include the skill id and only these items will do a range effect.
 * Every potion/dust will have a level (like a spell or rod). If this level is
 * 40% higher as our throw skill (and more as 5 level higher), then throwing
 * will fail, like we try to use a to high item device.
 * If we apply a throw potion/dust, the effect will triggered (a firestorm potion
 * will explode) but no skill id will be inserted in the firestorm objects.
 * So, the player and perhaps some mobs will take damage - but it will not give
 * any exp when this will happen. MNT-11-2002
 * have not included this 40% throw thing - perhaps later. MT-2003
 */
#define SUBTYPE_MISSILE_BOW     1 /* ammunition for bows = arrows */
#define SUBTYPE_MISSILE_CBOW    2 /* bolts */
#define SUBTYPE_MISSILE_SSTONE  3 /* sling stones */
/* these are our special throw weapons - they are called AMMUNITION too */
#define SUBTYPE_MISSILE_IMPACT  (SUBTYPE_MISSILE_THROW | 0)  /* impact */
#define SUBTYPE_MISSILE_SLASH   (SUBTYPE_MISSILE_THROW | 1)  /* slash */
#define SUBTYPE_MISSILE_PIERCE  (SUBTYPE_MISSILE_THROW | 2)  /* pierce */
#define SUBTYPE_MISSILE_CLEAVE  (SUBTYPE_MISSILE_THROW | 3)  /* cleave */
#define SUBTYPE_MISSILE_BOULDER (SUBTYPE_MISSILE_THROW | 4)  /* special case - impact damage too */
/* boulders get picked by giants and thrown by them */

/* some fancy macros to determintate the kidn of hand weapon */
#define WEAPON_IS_1H 0
#define WEAPON_IS_2H 1
#define WEAPON_IS_POLE 2

/* definitions for weapontypes */
/* one hand weapons - allows shields in second hand */
#define WEAP_1H_IMPACT  0  /* impact damage: clubs, maces, hammers, etc. */
#define WEAP_1H_SLASH   1  /* slash: swords, scimitar */
#define WEAP_1H_PIERCE  2  /* pierce: daggers, rapier */
#define WEAP_1H_CLEAVE  3  /* cleave: axes */

/* two-hand weapons. you need base 1h skill + two-hand mastery for use */
/* exp goes always in 1h skill - mastery skills are indirect skills */
#define WEAP_2H_IMPACT  4  /* impact damage: clubs, maces, hammers, etc. */
#define WEAP_2H_SLASH   5  /* slash */
#define WEAP_2H_PIERCE  6  /* pierce */
#define WEAP_2H_CLEAVE  7  /* cleave */

/* same like 2h but for polearms */
#define WEAP_POLE_IMPACT    8  /* impact damage: clubs, maces, hammers, etc. */
#define WEAP_POLE_SLASH     9  /* slash -swords */
#define WEAP_POLE_PIERCE    10  /* pierce: rapier */
#define WEAP_POLE_CLEAVE    11  /* cleave: axes */

#define RANGE_WEAP_BOW      1  /* range weapons - bows */
#define RANGE_WEAP_XBOWS    2  /* crossbows */
#define RANGE_WEAP_SLINGS   3  /* slings */
#define RANGE_WEAP_FIREARMS 4  /* firearms - not implemented */

/* some skills are auto-used, some should not be able to use with fire xxx (use_skill) */
#define SUBTYPE_SKILL_NORMAL    0   /* normal skill but not direct usable with use_skill() */
#define SUBTYPE_SKILL_USE       1   /* skill can be used with fire and use_skill() */
#define SUBTYPE_SKILL_INDIRECT  2   /* skill is used indirect by server */
#define SUBTYPE_SKILL_AUTOMAN   3   /* Can be used automatically or manually */

/* spellbooks can be different types: mages, cleric and so on.
 * to remove identification from arch name, i use ST1 for it MT
 */
#define SUBTYPE_SPELLBOOK_CLERIC 1
#define SUBTYPE_SPELLBOOK_MAGE   2

/* container can be different items: normal containers, mob corpse or player corpse.
 * all containers subtype >=64 are "personlized" - in the slaying field of this containers
 * is not the key but the name of the player which can as only person access the container.
 * is the container subtype >= 128 then its a group container - the group name will open/close it.
 * if the container subtype >=192 then the clan name will open it.
 */

#define SUBTYPE_CONTAINER_NORMAL                0
#define SUBTYPE_CONTAINER_CORPSE                1
#define SUBTYPE_CONTAINER_DEAD_PL               2

#define SUBTYPE_CONTAINER_NORMAL_player         64
#define SUBTYPE_CONTAINER_CORPSE_player         65

#define SUBTYPE_CONTAINER_NORMAL_group          128
#define SUBTYPE_CONTAINER_CORPSE_group          129

#define SUBTYPE_CONTAINER_NORMAL_clan           192
#define SUBTYPE_CONTAINER_CORPSE_clan           193

/* sub type for EXIT objects - if set, a teleport sound is played */
#define SUBTYPE_EXIT_SOUND_NO                   0
#define SUBTYPE_EXIT_SOUND                      1

/* sub types for doors */
#define SUBTYPE_DOOR_NORMAL 0
#define SUBTYPE_DOOR_CURTAIN 1 /* make different sound */

/* subtypes for TYPE_QUEST_CONTAINER objects */
#define SUBTYPE_QUEST_CONTAINER_ONEDROP        0
#define SUBTYPE_QUEST_CONTAINER_DONE    1
#define SUBTYPE_QUEST_CONTAINER_NORMAL    2
#define SUBTYPE_QUEST_CONTAINER_KILL    3

/* subtypes for QUEST_TRIGGER */
#define SUBTYPE_QUEST_TRIGGER_NORMAL      0
#define SUBTYPE_QUEST_TRIGGER_KILL        1
#define SUBTYPE_QUEST_TRIGGER_KILLITEM   2
#define SUBTYPE_QUEST_TRIGGER_ITEM        3

/* subtypes for TYPE_QUEST_UPDATE */
#define SUBTYPE_QUEST_UPDATE_ARBITRARY 0
#define SUBTYPE_QUEST_UPDATE_KILLITEM  1
#define SUBTYPE_QUEST_UPDATE_KILL      2

/* for guild force */
#define SUBTYPE_GUILD_IN        1
#define SUBTYPE_GUILD_OLD       2

/* subtypes for TYPE_CONN_SENSOR */
#define SUBTYPE_CONN_SENSOR_NAND        0 /* NAND / NOT */
#define SUBTYPE_CONN_SENSOR_AND         1 /* AND */
#define SUBTYPE_CONN_SENSOR_OR          2 /* OR */
#define SUBTYPE_CONN_SENSOR_XOR         3 /* XOR */

/* sub tpye defines for fast force identification */
#define SUBTYPE_FORCE_NORMAL      0
#define SUBTYPE_FORCE_DEPLETE     1
#define SUBTYPE_FORCE_DRAIN       2
#define SUBTYPE_FORCE_SLOWED      3
#define SUBTYPE_FORCE_FEAR        4
#define SUBTYPE_FORCE_SNARE       5
#define SUBTYPE_FORCE_PARALYZE    6
#define SUBTYPE_FORCE_CONFUSED    7
#define SUBTYPE_FORCE_BLIND       8
#define SUBTYPE_FORCE_POISON      9
#define SUBTYPE_FORCE_DEATHSICK   10
#define SUBTYPE_FORCE_POTION      11
#define SUBTYPE_FORCE_DOT         12 // For DOT effects other than poison
#define SUBTYPE_FORCE_RESERVATION 13

#define SUBTYPE_TRAP_HALLOWED_GROUND 1
#define SUBTYPE_TRAP_NO_SHOW 128

/* END SUB TYPE 1 DEFINE */

/* i change the spellpathes to 16 bit. Thats enough and will
 * safe 6 bytes - MT, 05-2005
 */
#define PATH_NULL   0x00000000      /* 0 */

#define PATH_LIFE        0x00000001      /* 1 */
#define PATH_DEATH        0x00000002      /* 2 */
#define PATH_ELEMENTAL    0x00000004        /* 4 */
#define PATH_ENERGY        0x00000008      /* 8 */
#define PATH_SPIRIT        0x00000010        /* 16 */
#define PATH_PROTECTION    0x00000020      /* 32 */
#define PATH_LIGHT        0x00000040        /* 64 */
#define PATH_NETHER        0x00000080        /* 128 */

#define PATH_NATURE            0x00000100  /* 256 */
#define PATH_SHADOW            0x00000200  /* 512 */
#define PATH_CHAOS            0x00000400    /* 1024 */
#define PATH_EARTH            0x00000800   /* 2048 */
#define PATH_CONJURATION    0x00001000     /* 4096 */
#define PATH_ABJURATION        0x00002000     /* 8192 */
#define PATH_TRANSMUTATION    0x00004000   /* 16384 */
#define PATH_ARCANE            0x00008000  /*  32768 */

#define NRSPELLPATHS    16

/* Terrain type flags
 * These values are used from terrain_typ and terrain_flag
 * Arches without terrain flags become AIRBREATH as default.
 * These values also define the enviroment of the map tile position
 */

#define TERRAIN_NOTHING         0
#define TERRAIN_AIRBREATH       1   /* thats default - walk on earth */
#define TERRAIN_WATERWALK       2   /* walk on water - deep water too */
#define TERRAIN_WATERBREATH     4   /* you can move underwater as on land */
#define TERRAIN_FIREWALK        8   /* walk over lava or fire */
#define TERRAIN_FIREBREATH      16  /* you can move in fire environment (fire elemental dimension, hell,..) */
#define TERRAIN_CLOUDWALK       32  /* move "on clouds" in the air - is not flying. Similiar to the story of the magic bean*/

/* Flag structure now changed.
 * Each flag is now a bit offset, starting at zero.  The macros
 * will update/read the appropriate flag element in the object
 * structure.
 *
 * Hopefully, since these offsets are integer constants set at run time,
 * the compiler will reduce the macros something as simple as the
 * old system was.
 *
 * Flags now have FLAG as the prefix.  This to be clearer, and also
 * to make sure F_ names are not still being used anyplace.
 *
 * The macros below assume that the flag size for each element is 32
 * bits.  IF it is smaller, bad things will happen.  See structs.h
 * for more info.
 *
 * All functions should use the macros below.  In process of converting
 * to the new system, I find several files that did not use the previous
 * macros.
 *
 * If any FLAG's are added, be sure to add them to the flag_links structure
 * in common/loader.c, if necessary.
 *
 * flags[0] is 0 to 31
 * flags[1] is 32 to 63
 * flags[2] is 64 to 95
 * flags[3] is 96 to 127
 */
/* *_FLAG() manipulate the object flags, where O is an object_t * and F is an
 * integer, specifically one of the FLAG_* constants. The output is 1 or 0.
 * This depends mostly on which macro has been called and should not be
 * thought of as success or failure.
 *
 * But QUERY does output 1 or 0 depending on whether O has F set or not.
 *
 * SET sets F on O and outputs 1, regardless of the previous state.
 *
 * CLEAR unsets F on O and outputs 0, regardless of the previous state.
 *
 * SET_OR_CLEAR does either of the above depending on V. V may be any type
 * because we only care if it's non-zero or not (remember, NULL is zero). */
#define QUERY_FLAG(_O_, _F_) \
    (((_O_)->flags[(_F_) / 32] & (1U << ((_F_) % 32))) \
     ? 1 : 0)
#define SET_FLAG(_O_, _F_) \
    (((_O_)->flags[(_F_) / 32] |= (1U << ((_F_) % 32))) \
     ? 1 : 1)
#define CLEAR_FLAG(_O_, _F_) \
    (((_O_)->flags[(_F_) / 32] &= ~(1U << ((_F_) % 32))) \
     ? 0 : 0)
#define SET_OR_CLEAR_FLAG(_O_, _F_, _V_) \
    (((_V_)) \
     ? SET_FLAG((_O_), (_F_)) \
     : CLEAR_FLAG((_O_), (_F_)))

/* TODO: I am not convinced *_MULTI_FLAG() is the way to go, but I can sort it
 * out later.
 *
 * -- Smacky 20200802 */
/* this is rarely used but needed for some flags, which are
 * used for intern handling like INVISIBLE or WALK_OFF. Because
 * some core functions like remove_ob() use this, it will be better
 * we set this ONE time outside instead of every time in remove_ob():
 * we skip the call for the head in this way. */
#define SET_MULTI_FLAG(_O_, _F_) \
    { \
        object_t *_tos_; \
        for (_tos_ = (_O_); _tos_; _tos_ = _tos_->more) \
        { \
            SET_FLAG(_tos_, (_F_)); \
        } \
    }
#define CLEAR_MULTI_FLAG(_O_, _F_) \
    { \
        object_t *_tos_; \
        for (_tos_ = (_O_); _tos_; _tos_ = _tos_->more) \
        { \
            CLEAR_FLAG(_tos_, (_F_)); \
        } \
    }

/* convenience macros to determine what kind of things we are dealing with */

#define IS_WEAPON(_O_) \
    ((_O_)->type == ARROW || \
     (_O_)->type == BOW || \
     (_O_)->type == WEAPON)

#define IS_ARMOR(_O_) \
    ((_O_)->type == ARMOUR || \
     (_O_)->type == SHIELD || \
     (_O_)->type == HELMET || \
     (_O_)->type == CLOAK || \
     (_O_)->type == BOOTS || \
     (_O_)->type == GLOVES || \
     (_O_)->type == BRACERS || \
     (_O_)->type == GIRDLE || \
     (_O_)->type == SHOULDER || \
     (_O_)->type == LEGS)

#define IS_ARROW(_O_) \
    ((_O_)->type == ARROW || \
     (_O_)->type == MMISSILE || \
     (_O_)->type == BULLET)

#define IS_AIRBORNE(_O_) \
    (QUERY_FLAG((_O_), FLAG_FLYING) || \
     QUERY_FLAG((_O_), FLAG_LEVITATE))

#define IS_DEVICE(_O_) \
    ((_O_)->type == HORN || \
     (_O_)->type == ROD || \
     (_O_)->type == WAND)

#define IS_GOD_SPELL(_O_) \
    ((_O_)->type == CONE || \
     (_O_)->type == FBULLET || \
     (_O_)->type == FBALL || \
     (_O_)->type == SWARM_SPELL)

/* the flags */

/* NOTE: you MUST set the FLAG_xx to V_xxx array in loader.l too when
 * you change something here! Search for NUM_FLAGS in loader.l for more.
 */
/* WARNING: The first 8 bit are used from the map2 cmd as direct mapped data.
 * The order must stay as it is here!
 */
#define FLAG_SLEEP          0 /* NPC is sleeping */
#define FLAG_CONFUSED       1 /* confused... random dir when moving and problems to do actions */
#define FLAG_PARALYZED      2 /* Object is paralyzed */
#define FLAG_SCARED         3 /* Monster is scared. This is "run away" panic - don't confuse it with fear */
#define FLAG_EATING         4 /* target is eating/resting */
#define FLAG_IS_INVISIBLE   5 /* only THIS invisible can be seen with seen_invisible */
#define FLAG_IS_ETHEREAL    6 /* object is etheral  - means transparent and special protected */
#define FLAG_BLIND          7 /* If set, object cannot see (the map) with eyes */

#define FLAG_NO_PICK        8 /* Object can't be picked up */
#define FLAG_WALK_ON        9 /* Applied when it's walked upon */
#define FLAG_NO_PASS        10 /* Nothing can pass */
#define FLAG_ANIMATE        11 /* The object looks at archetype for faces */
#define FLAG_INITIALIZED    12 /* Used by some types to keep track of initialiization after map load (never saved) */

#define FLAG_FLYING         13 /* Not affected by WALK_ON or SLOW_MOVE) */
/* 14 is free. */
#define FLAG_FRIENDLY       15 /* Will help players */
/*
 *  FLAG_INSERTED, REMOVED and BEEN_APPLIED are direct used from
 *  object.c/can_merge() - change it too when you move this flag! See also
 *  FLAG_APPLIED
 */
#define FLAG_REMOVED        16 /* Object is not in any map or invenory */
#define FLAG_BEEN_APPLIED   17 /* The object has been applied in the past - its "identified by using" */
#define FLAG_INSERTED       18 /* Object is or has been in a map or invenory */
#define FLAG_UNIDENTIFIABLE 19 /* Object should not be identified */
#define FLAG_IS_NEUTRAL     20 /* alignment of this object: we need the explicit neutral setting for items */
#define FLAG_SEE_INVISIBLE  21 /* Will see invisible player */
#define FLAG_CAN_ROLL       22 /* Object can be rolled */
#define FLAG_OBSCURESVIEW   23 /* Object obscures view */

#define FLAG_IS_TURNABLE    24 /* Object can change face with direction */
#define FLAG_WALK_OFF       25 /* Object is applied when left */
#define FLAG_FLY_ON         26 /* As WALK_ON, but only with FLAG_FLYING */
#define FLAG_FLY_OFF        27 /* As WALK_OFF, but only with FLAG_FLYING */
#define FLAG_IS_USED_UP     28 /* When (--food<0) the object will exit */
/* IDENTIFIED, KNOWN_CURSED, and KNOWN_MAGICAL are three distinct flags.
 * IDENTIFIED *implies* the other two, so it makes little sense to flag an
 * object as IDENTIFIED only, but the onus here is on the arch/map/function
 * setting the flags. It is not appropriate to set KNOWN_* on the presence of
 * IDENTIIFIED. To this end, the KNOWN flags should be queried directly, not
 * presumed by querying IDENTIFIED.
 * -- Smacky 20090312 */
/* An UNIDENTIFIABLE object cannot be further identified (or known). But it
 * retains those flags set already and they may subsequently be cleared (but
 * not re-set).
 *
 * For example, object foo is KNOWN_MAGICAL, KNOWN_CURSED, and UNIDENTIFIABLE.
 * Foo's  */
#define FLAG_IDENTIFIED     29 /* item is identified */
#define FLAG_REFLECTING     30 /* Object reflects from walls (lightning) */
#define FLAG_CHANGING       31 /* Changes to other_arch when anim is done*/

/* Start of values in flags[1] */
#define FLAG_SPLITTING      32 /* Object splits into stats.food other objs */
#define FLAG_HITBACK        33 /* Object will hit back when hit */
#define FLAG_ALLOWSVIEW     34 /* Object allows view (windows, fences, etc) */
#define FLAG_BLOCKSVIEW     35 /* Object blocks view */
#define FLAG_UNDEAD         36 /* Monster is undead */
#define FLAG_FIX_PLAYER     37 /* fix_player() WILL be called one time (use it dynamically in scripts!) */
#define FLAG_UNAGGRESSIVE   38 /* Monster doesn't attack players */
#define FLAG_REFL_MISSILE   39 /* object will give missile reflection */

#define FLAG_REFL_CASTABLE  40 /* object will give spell reflection */
#define FLAG_NO_SPELLS      41 /* Spells (some) can't pass this object_t */
#define FLAG_NO_FIX_PLAYER  42 /* fix_player() won't be called */
#define FLAG_IS_EVIL        43 /* alignment flags */
#define FLAG_TEAR_DOWN      44 /* at->faces[hp*animations/maxhp] at hit */
#define FLAG_RUN_AWAY       45 /* Object runs away from nearest player
                                * but can still attack at a distance.
                                * don't confuse it with FLAG_SCARED
                                */
#define FLAG_PASS_THRU      46 /* Objects with can_pass_thru can pass
                                * thru this object as if it wasn't there
                                */
#define FLAG_CAN_PASS_THRU  47 /* Can pass thru... */

#define FLAG_FEARED         48 /* player or monster is feared - attacks and acts alot more worse */
#define FLAG_IS_GOOD        49 /* alignment flag */
#define FLAG_NO_DROP        50 /* Object can't be dropped */
#define FLAG_FIGHT_HPREG    51 /* monster (player - not implemented): mob can reg hp when fighting! */
#define FLAG_READY_SPELL    52 /* (Monster) can learn and cast spells */
#define FLAG_SURRENDERED    53 /* (Monster) has surrendered and count not as "targetable enemy" */
#define FLAG_ROOTED         54 /* monster or player is rooted/snared - can't move but fight */
#define FLAG_SLOWED         55 /* monster/player is slowed */

#define FLAG_USE_ARMOUR     56 /* (player/deity) INTERNAL flag to allow armour use (for example from the deity) */
#define FLAG_USE_WEAPON     57 /* (player/deity) INTERNAL same as armour: can wield weapons */
#define FLAG_USE_RING       58 /* (player/deity) INTERNAL can use rings and amulet (not implemented for gods ATM!) */
#define FLAG_IN_ACTIVELIST  59 /* INTERNAL (not saved): mark object as in active list */
#define FLAG_READY_BOW      60 /* mob or player has a bow readied */
#define FLAG_XRAYS          61 /* X-ray vision */
#define FLAG_NO_APPLY       62 /* Avoids step_on/fly_on to this object_t */
#define FLAG_CAN_STACK        63 /* a clean flag to mark stackable items */

/* Start of values in flags[2] */
#define FLAG_LIFESAVE       64 /* Saves a players' life once, then destr. */
#define FLAG_IS_MAGICAL     65 /* item is magic - intern used.
                                * player use FLAG_KNOWN_MAGICAL
                                */
#define FLAG_SENTIENT       66 /* Object has limited ai */
#define FLAG_STAND_STILL    67 /* NPC will not (ever) move */
#define FLAG_RANDOM_MOVE    68 /* NPC will move randomly */
#define FLAG_ONLY_ATTACK    69 /* NPC will evaporate if there is no enemy */
#define FLAG_NO_SEND        70 // do not send data about this object to clients
#define FLAG_STEALTH        71 /* Will wake monsters with less range */
#define FLAG_IS_GIVING      72 /* Monster is giving items from their inv */
#define FLAG_IS_LINKED      73 /* The object is linked with other objects */
#define FLAG_CURSED         74 /* The object is cursed */
#define FLAG_DAMNED         75 /* The object is _very_ cursed */
#define FLAG_SEE_ANYWHERE   76 /* The object will be visible behind walls (disabled MT2003 - read los.c) */
#define FLAG_KNOWN_MAGICAL  77 /* The object is known to be magical */
#define FLAG_KNOWN_CURSED   78 /* The object is known to be cursed */
#define FLAG_CAN_OPEN_DOOR  79 /* object (mob, player,..) can open a door */

#define FLAG_IS_THROWN      80 /* Object is designed to be thrown. */
#define FLAG_EGOLOCK        81 // not (un/re)bindable unless ->egomode < 0
/* flag 82 is free */
#define FLAG_IS_MALE        83 /* gender flags. it effects only player & mobs */
#define FLAG_IS_FEMALE      84 /* is not female nor male, it is a neuter */
/* don't move flag_applied without adjusting object.c/can_merge()! */
#define FLAG_APPLIED        85 /* Object is ready for use by living */
#define FLAG_INV_LOCKED     86 /* Item will not be dropped from inventory */
#define FLAG_IS_WOODED      87 /* Item is wooded terrain */

#define FLAG_IS_HILLY       88 /* Item is hilly/mountain terrain */
#define FLAG_LEVITATE       89 /* levitating... similiar to flying */
#define FLAG_READY_WEAPON   90 /* (Monster or Player) has a weapon readied */
/* 91 is free. */
#define FLAG_USE_DMG_INFO   92 /* used for asynchron dmg of AoE spells */
#define FLAG_SEE_IN_DARK    93 /* if set ob not effected by darkness */
#define FLAG_IS_CAULDRON    94 /* container can make alchemical stuff */
#define FLAG_DUST           95 /* item is a 'powder', effects throwing */

/* Start of values in flags[3] */
#define FLAG_NO_STEAL           96 /* Item can't be stolen */
#define FLAG_ONE_HIT            97 /* Monster can only hit once before going
                                    * away (replaces ghosthit)
                                    */
#define FLAG_CLIENT_SENT        98 /* THIS IS A DEBUG FLAG ONLY.  We use it to
                                    * detect cases were the server is trying
                                    * to send an upditem when we have not
                                    * actually sent the item.
                                    */
#define FLAG_BERSERK            99  /* monster will attack closest living object_t */
#define FLAG_NO_ATTACK          100 /* object will not attack */
#define FLAG_INVULNERABLE       101 /* monster can't be damaged */

#define FLAG_QUEST_ITEM         102 /* this is a special quest object_t */

#define FLAG_IS_TRAPED          103 /* object is traped - most common a container with
                                     * a known trap inside. This info so useful for client
                                     * below and inventory look.
                                     */
#define FLAG_PROOF_PHYSICAL     104
#define FLAG_PROOF_ELEMENTAL    105
#define FLAG_PROOF_MAGICAL      106
#define FLAG_PROOF_SPHERICAL    107
#define FLAG_NO_INVENTORY       108 /* special flag to avoid load/save of the inventory of an object_t */
#define FLAG_DONATION_ITEM      109 /* checked by /generate command to stop generation of these items */
#define FLAG_SYS_OBJECT         110 /* thats old invisible - now sys_object (which are invisible) */
#define FLAG_HOMELESS_MOB       111
#define FLAG_UNPAID             112 /* Object hasn't been paid for yet */
#define FLAG_IS_AGED            113 /* if set, object falls under heavy ageing effects */
#define FLAG_MAKE_INVISIBLE     114 /* if a applyable item has this set, he makes the wearer invisible */
#define FLAG_MAKE_ETHEREAL      115 /* same as make_invisibile but for ethereal */
/* flag 116 is free */
#define FLAG_IS_NAMED           117 /* object is named - for artifacts like Stormbringer.
                                     * Named object don't have a race or material tag
                                     */
#define FLAG_SPAWN_MOB          118 /* monster with this flag are created by spawn point
                                     * and have a spawn info object inside inventory
                                     * See also 111.
                                     */
#define FLAG_NO_TELEPORT        119 /* objects with this flags will not be teleported
                                     * from teleporters. Except, they are in a inventory
                                     * of a teleporter object.
                                     */
#define FLAG_CORPSE             120 /* if set, this object (usally mob) will drop corpse using race name->
                                     * all item of the mob will put in the corpse and/or if slaying of corpse
                                     * is set only the player which killed the mob can access the corpse until
                                     * it decayed - then items drop on ground and all can grap it */
#define FLAG_CORPSE_FORCED      121 /* normally, corpses will only be placed when the mob has some items to drop.
                                     * this flag will drop a corpse even the corpse is empty */
#define FLAG_PLAYER_ONLY        122 // object can only be applied (if appliable at all) by a player
#define FLAG_NO_PRAYERS         123
#define FLAG_ONE_DROP           124 /* if this flag is set, the item marked with it will flaged
                                     * start equipment when a player gets it (item is inserted
                                     * in player inventory and/or touched by a player)
                                     */
#define FLAG_PERM_CURSED        125  /* object will set to cursed when monster or player applies it.
                                      * remove curse will remove cursed 1 but not this flag.
                                      */
#define FLAG_PERM_DAMNED        126   /* same as perm_cursed but for damned */

#define FLAG_DOOR_CLOSED        127   /* this object works like a closed door. Main function
                                       * is to trigger the right map flags, so a moving objects
                                       * know that spot is blocked by a door and he must open it first->
                                       */

/* Start of values in flags[4] */
#define FLAG_WAS_REFLECTED      128   /* object was reflected (arrow, throw object...) */
#define FLAG_IS_MISSILE         129   /* object is used as missile (arrow, potion, magic bullet, ...) */
/* flag 130 is free */
/* flag 131 is free */

#define FLAG_IS_ASSASSINATION   132     /* If a attacking force and slaying is set, this is 3 times damage */
/* flag 133 is free */
#define FLAG_NO_SAVE            134     /* don't save this object - remove it before we save */
#define FLAG_PASS_ETHEREAL      135     /* can_pass light for ethereal */

#define NUM_FLAGS    135                    // should always be equal to the last defined flag
#define NUM_FLAGS_32 ((NUM_FLAGS / 32) + 1) // the number of uint32 we need to store all flags

/* Values can go up to 127 before the size of the flags array in the
 * object structure needs to be enlarged.
 */

#define NROFNEWOBJS(xyz)    ((xyz)->stats.food)

/* Note: These values are only a default value, resizing can change them */
#define INV_SIZE        12  /* How many items can be viewed in inventory */
#define LOOK_SIZE       6   /* ditto, but for the look-window */
#define MAX_INV_SIZE        40  /* For initializing arrays */
#define MAX_LOOK_SIZE       40  /* ditto for the look-window */

#define F_BUY 0
#define F_SELL 1
#define F_TRUE 2    /* True value of item, unadjusted */

#define ARCH_MAX_TYPES       512 /* important - this must be higher as max type number! */
/* GET_?_FROM_DIR if used only for positional firing where dir is X and Y
   each of them signed char, concatenated in a int16 */
#define GET_X_FROM_DIR(dir) (signed char) (  dir & 0xFF )
#define GET_Y_FROM_DIR(dir) (signed char) ( (dir & 0xFF00) >> 8)
#define SET_DIR_FROM_XY(X,Y) (signed char)X + ( ((signed char)Y)<<8)
#define FIRE_DIRECTIONAL 0
#define FIRE_POSITIONAL  1

/*
 * Use of the state-variable in player objects:
 */
/* static player state flags */
#define ST_BORN                 0x01 /* this char was never played before and is new */
#define ST_RESURRECT            0x02 /* player is dead but can be resurrected */
/* these state flags are dynamic and not saved */
#define ST_PLAYING              0x20
#define ST_ZOMBIE               0x40
#define ST_DEAD                 0x80
#define ST_KICKED               0x100

#ifdef SERVER_SEND_FACES
# define BLANK_FACE_NAME         "blank.111"
# define NEXT_ITEM_FACE_NAME     "next_item.101"
# define PREVIOUS_ITEM_FACE_NAME "prev_item.101"
# define NO_FLOOR_FACE_NAME      "no_floor.101"
#endif

/* Simple function we use below to keep adding to the same string
 * but also make sure we don't overwrite that string.
 */
static inline void safe_strcat(char *dest, const char *orig, int *curlen, int maxlen)
{
    if (*curlen == (maxlen - 1))
        return;
    strncpy(dest + *curlen, orig, maxlen - *curlen - 1);
    dest[maxlen - 1] = 0;
    *curlen += (int)strlen(orig);
    if (*curlen > (maxlen - 1))
        *curlen = maxlen - 1;
}


#define DESCRIBE_PATH(retbuf, variable, name) \
    if(variable) { \
      int i,j=0; \
      strcat(retbuf,"(" name ": "); \
      for(i=0; i<NRSPELLPATHS; i++) \
        if(variable & (1<<i)) { \
          if (j) \
            strcat(retbuf,", "); \
          else \
            j = 1; \
          strcat(retbuf, spellpathnames[i]); \
        } \
      strcat(retbuf,")"); \
    }


#define DESCRIBE_PATH_SAFE(retbuf, variable, name, len, maxlen) \
    if(variable) { \
      int i,j=0; \
      safe_strcat(retbuf,"(" name ": ", len, maxlen); \
      for(i=0; i<NRSPELLPATHS; i++) \
        if(variable & (1<<i)) { \
          if (j) \
            safe_strcat(retbuf,", ", len, maxlen); \
          else \
            j = 1; \
          safe_strcat(retbuf, spellpathnames[i], len, maxlen); \
        } \
      safe_strcat(retbuf,")", len, maxlen); \
    }

/* Flags for apply_equipment() */
enum apply_flag
{
    /* Basic flags, always use one of these */
    AP_NULL                                                     = 0,
    AP_APPLY                                                    = 1,
    AP_UNAPPLY                                                  = 2,
    AP_BASIC_FLAGS                                              = 15,
    /* Optional flags, for bitwise or with a basic flag */
    AP_IGNORE_CURSE                                             = 32,
    AP_QUIET                                                    = 64
};

/* Cut off point of when an object is put on the active list or not */
#define MIN_ACTIVE_SPEED    0.00001f

/* Bresenham line drawing algorithm. Implemented as macros for
 * flexibility and speed. */

/* Bresenham init */
/* dx & dy are input only and will not be changed.
 * All other parameters are the outputs which will be initialized */
#define BRESENHAM_INIT(dx, dy, fraction, stepx, stepy, dx2, dy2) \
    { \
        (dx2) = (dx) << 1; \
        (dy2) = (dy) << 1; \
        if ((dy) < 0) { (dy2) = -(dy2);  (stepy) = -1; } else { (stepy) = 1; } \
        if ((dx) < 0) { (dx2) = -(dx2);  (stepx) = -1; } else { (stepx) = 1; } \
        if((dx2) > (dy2)) (fraction) = (dy2) - (dx)*(stepx); else (fraction) = (dx2) - (dy)*(stepy); \
    }

/* Bresenham line stepping macro */
/* x,y are input-output and will be always be changed
 * fraction is also input-output, but should be initialized with
 * BRESENHAM_INIT.
 * stepx, stepy, dx2 and dy2 are input only and should also
 * be initialized by BRESENHAM_INIT
 */
#define BRESENHAM_STEP(x,y,fraction,stepx,stepy,dx2,dy2) \
    if ((dx2) > (dy2)) { \
        if ((fraction) >= 0) { \
            (y) += (stepy); \
            (fraction) -= (dx2); \
        } \
        (x) += (stepx); \
        (fraction) += (dy2); \
    } else { \
        if ((fraction) >= 0) { \
            (x) += (stepx); \
            (fraction) -= (dy2); \
        } \
        (y) += (stepy); \
        (fraction) += (dx2); \
    }

/* PvP stat flags are used on the archetype_global.pvp_stat_force object in a player's inventory to keep track of PvP stats such as total
 * kills, kills per PvP match, and possibly a rank system.
 */
#define PVP_STATFLAG_KILLS_TOTAL  1 // All players killed by this player during this player's existence.
#define PVP_STATFLAG_KILLS_ROUND  2 // The players killed during this PvP match.
#define PVP_STATFLAG_DEATH_TOTAL  4 // How many times this player has died in PvP.
#define PVP_STATFLAG_DEATH_ROUND  8 // How many times this player has died during this PvP match.

#define PLUGINS
#endif /* ifndef __DEFINE_H */

/* TODO: Move to own file. */

#ifndef __LOS_H
#define __LOS_H

/* LOS_FLAG_* are flags for the LoS array. */
#define LOS_FLAG_VISIBLE      0        // visible
#define LOS_FLAG_IGNORE       (1 << 0) // ignore for blocksview/visible changes!
#define LOS_FLAG_OBSCURESVIEW (1 << 1) // visible but will obscure all behind
#define LOS_FLAG_ALLOWSVIEW   (1 << 2) // visible/nonblocking as normal but part of a continuous wall
#define LOS_FLAG_BLOCKSVIEW   (1 << 3) // visible but will block all behind
#define LOS_FLAG_OBSCURED     (1 << 4) // sight is partly blocked/in shadow/indistinct
#define LOS_FLAG_BLOCKED      (1 << 5) // sight is blocked
#define LOS_FLAG_OUT_OF_MAP   (1 << 6) // not visible because not part of legal map
#define LOS_FLAG_INTERNAL     (1 << 7) // internal use

/* LOS_TARGET_* are modes for targeting. */
#define LOS_TARGET_SELF   0
#define LOS_TARGET_ENEMY  1
#define LOS_TARGET_FRIEND 2
#define LOS_TARGET_TALK   3
#define LOS_TARGET_MOUSE  4

/* LOS_SET_TARGET() sets the various target pointers for _PL_ according to the
 * other parameters. Most important of these is _M_ (mode). This should be one
 * of LOS_TARGET_SELF, LOS_TARGET_ENEMY, or LOS_TARGET_FRIEND. When it is SELF,
 * essentially _O_ and _I_ are ignored and _PL_->ob and 0 are used instead.
 * Once all are set, the relevant client is updated.
 *
 * The macro also sets _PL_->ob->enemy/enemy_count. Here when _M_ is ENEMY,
 * they are set according to _O_. Otherwise they are NULL/0. */
#define LOS_SET_TARGET(_PL_, _O_, _M_, _I_) \
    if ((_M_) == LOS_TARGET_SELF) \
    { \
        (_PL_)->target_ob = (_PL_)->ob; \
        (_PL_)->target_tag = 0; \
        (_PL_)->target_index = 0; \
        (_PL_)->target_level = (_PL_)->ob->level; \
        (_PL_)->target_colr = NDI_YELLOW; \
    } \
    else \
    { \
        (_PL_)->target_ob = (_O_); \
        (_PL_)->target_tag = TAG((_O_)); \
        (_PL_)->target_index = (_I_); \
        (_PL_)->target_level = (_O_)->level; \
        if ((_O_)->level < level_color[(_PL_)->ob->level].yellow) \
        { \
            if ((_O_)->level < level_color[(_PL_)->ob->level].green) \
            { \
                (_PL_)->target_colr = NDI_GREY; \
            } \
            else \
            { \
                if ((_O_)->level < level_color[(_PL_)->ob->level].blue) \
                { \
                    (_PL_)->target_colr = NDI_GREEN; \
                } \
                else \
                { \
                    (_PL_)->target_colr = NDI_BLUE; \
                } \
            } \
        } \
        else \
        { \
            if ((_O_)->level >= level_color[(_PL_)->ob->level].purple) \
            { \
                (_PL_)->target_colr = NDI_PURPLE; \
            } \
            else if ((_O_)->level >= level_color[(_PL_)->ob->level].red) \
            { \
                (_PL_)->target_colr = NDI_RED; \
            } \
            else if ((_O_)->level >= level_color[(_PL_)->ob->level].orange) \
            { \
                (_PL_)->target_colr = NDI_ORANGE; \
            } \
            else \
            { \
                (_PL_)->target_colr = NDI_YELLOW; \
            } \
        } \
    } \
    (_PL_)->target_mode = (_M_); \
    send_target_command((_PL_)); \
    if ((_M_) == LOS_TARGET_ENEMY) \
    { \
        (_PL_)->ob->enemy = (_O_); \
        (_PL_)->ob->enemy_count = TAG((_O_)); \
    } \
    else \
    { \
        (_PL_)->ob->enemy = NULL; \
        (_PL_)->ob->enemy_count = 0; \
    }

extern void los_update(player_t *pl);
extern int  obj_in_line_of_sight(object_t *op, object_t *obj, rv_t *rv);
extern void los_find_target(player_t *pl, uint8 mode, sint16 start, sint16 stop, sint16 ox, sint16 oy);

#endif /* ifndef __LOS_H */
