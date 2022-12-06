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

#ifndef __PLUGIN_H
#define __PLUGIN_H

/*****************************************************************************/
/* Daimonin  plugin support - (C) 2001 by Yann Chachkoff.                    */
/* This code is placed under the GPL.                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Headers needed.                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* This one does not exist under Win32.                                      */
/*****************************************************************************/
#ifndef WIN32
#include <dlfcn.h>
#endif

#undef MODULEAPI
#ifdef WIN32
#ifdef LUA_PLUGIN_EXPORTS
#define MODULEAPI __declspec(dllexport)
#else
#define MODULEAPI __declspec(dllimport)
#endif
#else
#define MODULEAPI
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif


/*****************************************************************************/
/* This one does not exist under Win32.                                      */
/*****************************************************************************/
#ifndef WIN32
#include <dirent.h>
#endif

#define PLUGIN_MAX 32
#define PLUGIN_NAME "Lua"

/* There are max 32 events (actually 31 as 0 means none). Within this number, 1
 * and above are object events and 31 and below are special events. Between
 * these (15-26 at time of writing) are free numbers which may be allocated as
 * previously indicated in future.
 *
 * Object events are the familiar event types (APPLY, TALK, etc) which mappers
 * may attach to various game objects to trigger an arbitrary script in
 * response to some defined circumstance. Note that because the numbers are
 * referred to by maps, the order below must be preserved (TODO: well it is
 * possible to make some changes with a little care -- and such will probably
 * be done in future -- but it's not entirely trivial).
 *
 * TODO: KNOWLEDGE is likely to become  part of a special event in future.
 *
 * Special events are inherently more difficult to generically define.
 *
 * HELP events are special events in response to the /help command.
 *
 * PLAYER events are triggered when a player has reached some milestone or
 * other (such as death or completing a quest). Unlike object events, the
 * 'type' depends on context, and therefore which fixed script is run (see
 * below) and with which parameters.
 *
 * BEHAVIOUR events are triggered by the AI system. (TODO: More)
 *
 * DETACHED events are in fact not even real event numbers, in the sense that no
 * object actually has this event. Rather, every tick the server triggers such
 * an event which does no more than browse through the list of scripts which
 * have yielded on a previous tick, continuing those as appropriate. TODO:
 * Remove -- This is just a 'convenience' to fit in with the ecidting plugin
 * system.
 *
 * QUEST events are not even real 'plugin' events. They are not associated with
 * a plugin or script but simply mark a container as carrying a quest trigger.
 * */
#define PLUGIN_EVENT_NONE              0  // no event (exists only to reserve the "0")
#define PLUGIN_EVENT_OBJECT_APPLY      1  // object applied/unapplied
#define PLUGIN_EVENT_OBJECT_ATTACK     2  // monster attacked
#define PLUGIN_EVENT_OBJECT_DEATH      3  // monster killad
#define PLUGIN_EVENT_OBJECT_DROP       4  // object dropped on the floor
#define PLUGIN_EVENT_OBJECT_PICKUP     5  // object picked up
#define PLUGIN_EVENT_OBJECT_SAY        6  // object in range of a say
#define PLUGIN_EVENT_OBJECT_STOP       7  // thrown object stopped
#define PLUGIN_EVENT_OBJECT_TIME       8  // DO NOT USE
#define PLUGIN_EVENT_OBJECT_THROW      9  // object is thrown
#define PLUGIN_EVENT_OBJECT_TRIGGER    10 // button pushed, lever pulled, etc
#define PLUGIN_EVENT_OBJECT_CLOSE      11 // container closed
#define PLUGIN_EVENT_OBJECT_EXAMINE    12 // object was examined
#define PLUGIN_EVENT_OBJECT_TALK       13 // talk event for npc interface system
#define PLUGIN_EVENT_OBJECT_KNOWLEDGE  14 // monster gains knowledge of object
/* 15 is free. */
/* 16 is free. */
/* 17 is free. */
/* 18 is free. */
/* 19 is free. */
/* 20 is free. */
/* 21 is free. */
/* 22 is free. */
/* 23 is free. */
/* 24 is free. */
/* 25 is free. */
/* 26 is free. */
#define PLUGIN_EVENT_SPECIAL_HELP      27
#define PLUGIN_EVENT_SPECIAL_PLAYER    28
#define PLUGIN_EVENT_SPECIAL_BEHAVIOUR 29
#define PLUGIN_EVENT_SPECIAL_QUEST     30
#define PLUGIN_EVENT_SPECIAL_DETACHED  31

#define PLUGIN_EVENT_FLAG(_N_) (1 << ((_N_) - 1))

/* PLUGIN_SCRIPT_PLAYER_* are the various scripts run by a PLAYER special
 * event. */
#define PLUGIN_SCRIPT_PLAYER_LOGIN   "/lua/player/login.lua"
#define PLUGIN_SCRIPT_PLAYER_QUEST   "/lua/player/quest.lua"
#define PLUGIN_SCRIPT_PLAYER_ONEDROP "/lua/player/onedrop.lua"
#define PLUGIN_SCRIPT_PLAYER_LEVELUP "/lua/player/levelup.lua"
#define PLUGIN_SCRIPT_PLAYER_EXP     "/lua/player/exp.lua"
#define PLUGIN_SCRIPT_PLAYER_DEATH   "/lua/player/death.lua"
#define PLUGIN_SCRIPT_PLAYER_LOGOUT  "/lua/player/logout.lua"

/* PLUGIN_SCRIPT_HELP is the base script run by the HELP special event. */
#define PLUGIN_SCRIPT_HELP  "/lua/help/help.lua"

/*****************************************************************************/
/* Hook codes. A hook is a function pointer passed from the server to the    */
/* plugin, so the plugin can call a server/crosslib functionality. Some may  */
/* call them "callbacks", although I don't like that term, which is too      */
/* closely bound to C and pointers.                                          */
/* I didn't add comments for all those hooks, but it should be quite easy to */
/* find out to what function they are pointing at. Also consult the plugins.c*/
/* source file in the server subdirectory to see the hook "wrappers".        */
/*****************************************************************************/

/* Gecko 2005-05-14: This old hook system is being phased out, use the
 * plugin_hooklist struct below instead */
enum
{
    HOOK_SENDCUSTOMCOMMAND,
    HOOK_CREATEOBJECT,
    HOOK_CMDRSKILL,
    HOOK_BECOMEFOLLOWER,
    HOOK_COMMUNICATE,
    HOOK_FINDPLAYER,
    HOOK_MANUALAPPLY,
    HOOK_FINDMARKEDOBJECT,
    HOOK_LEARNSPELL,
    HOOK_CHECKFORSPELL,
    HOOK_LOADOBJECT,
    HOOK_DUMPOBJECT,
    HOOK_DETERMINEGOD,
    HOOK_FINDGOD,

    NR_OF_HOOKS
};


/*****************************************************************************/
/* CFParm is the data type used to pass informations between the server and  */
/* the plugins. Using CFParm allows a greater flexibility, at the cost of a  */
/* "manual" function parameters handling and the need of "wrapper" functions.*/
/* Each CFParm can contain up to 15 different values, stored as (void *).    */
/*****************************************************************************/
typedef struct _CFParm
{
    const int     Type[15];   /* Currently unused, but may prove useful later.     */
    void   *Value[15];  /* The values contained in the CFParm structure.     */
} CFParm;


/*****************************************************************************/
/* Generic plugin function prototype. All hook functions follow this.        */
/*****************************************************************************/
typedef CFParm*(*f_plugin)    (CFParm *PParm);

typedef int (*f_plugin_event) (CFParm *PParm);
typedef int (*f_plugin_prop)  (CFParm *PParm, CommArray_s *RTNCmd);
typedef int (*f_plugin_init)  (CFParm *PParm, const char **name, const char **version);


/*****************************************************************************/
/* CFPlugin contains all pertinent informations about one plugin. The server */
/* maintains a list of CFPlugins in memory. Note that the library pointer is */
/* a (void *) in general, but a HMODULE under Win32, due to the specific DLL */
/* management.                                                               */
/*****************************************************************************/
#ifndef WIN32
#define LIBPTRTYPE void*
#else
#define LIBPTRTYPE HMODULE
#endif
typedef struct _CFPlugin
{
    f_plugin_event    eventfunc;          /* Event Handler function            */
    f_plugin_init     initfunc;           /* Plugin Initialization function.   */
    f_plugin          pinitfunc;          /* Plugin Post-Init. function.       */
    f_plugin          removefunc;         /* Plugin Closing function.          */
    f_plugin          hookfunc;           /* Plugin CF-funct. hooker function  */
    f_plugin_prop     propfunc;           /* Plugin getProperty function       */
    LIBPTRTYPE        libptr;             /* Pointer to the plugin library     */
    shstr_t             *id;                /* Plugin identification string      */
    shstr_t             *fullname;          /* Plugin full name                  */
} CFPlugin;

/* Test of a new, more efficient hook system */
/* The new way to use hooks is better, because
 * - we don't need to copy arguments to and from the CFParm structs
 * - we can choose to call the hooked function directly instead of through a wrapper
 * - we can still use wrappers if we want to
 * - we get type safety and a cleaner code
 */
struct plugin_hooklist
{
    /* FUNCTIONS */
    /* A */
    int (*add_pet)(object_t *, object_t *, int);
    void (*add_quest_containers)(object_t *);
    void (*add_quest_trigger)(object_t *, object_t *);
    void (*adjust_light_source)(msp_t *, int);
    object_t *(*arch_to_object)(archetype_t *);
    /* B */
    int (*buff_add)(object_t *, object_t *, short);
    int  (*buff_remove)(object_t *, char *, uint32);
    /* C */
    int (*cast_spell)(object_t *, object_t *, int, int, int, SpellTypeFrom,
                      char *);
    sint8 (*check_path)(const char *, uint8);
    void (*clear_mob_knowns)(object_t *, struct mob_known_obj **, hashtable_t *);
    object_t *(*clone_object)(object_t *, uint8, uint8);
    int (*command_combat)(object_t *, char *);
    int (*command_target)(object_t *, char *);
    char *(*cost_string_from_value)(sint64, int);
    object_t *(*create_financial_loot)(moneyblock_t *, object_t *, uint8);
    shstr_t *(*create_instance_path_sh)(player_t *, shstr_t *, uint32);
    char *(*create_mapdir_pathname)(const char *);
    struct mempool *(*create_mempool)(const char *, uint32, uint32,
        uint32, chunk_initialisator, chunk_deinitialisator,
        chunk_constructor, chunk_destructor);
    shstr_t *(*create_safe_path_sh)(const char *);
    shstr_t *(*create_unique_path_sh)(shstr_t *, shstr_t *);
    /* D */
    object_t *(*decrease_ob_nr)(object_t *, uint32);
    /* E */
    sint8 (*enter_map)(object_t *, msp_t *, object_t *, uint8, uint32);
    int (*enumerate_coins)(sint64, struct moneyblock_t *);
    void (*esrv_send_or_del_item)(object_t *);
    void (*esrv_update_item)(uint16, object_t *);
    char *(*examine_item)(object_t *, object_t *);
    sint32 (*exp_adjust)(player_t *, sint16, sint32, uint8);
    /* F */
    int (*find_animation)(char *);
    archetype_t *(*find_archetype)(const char *);
    artifact_t *(*find_artifact)(const char *);
    object_t *(*find_next_object)(object_t *, uint8, uint8, object_t *);
    int (*FindFace)(const char *, int);
#ifdef DEBUG_FIX_PLAYER
    void (*fix_player)(object_t *, char *);
#else
    void (*fix_player)(object_t *);
#endif
    void (*free_mempool)(struct mempool *);
    /* G */
    object_t *(*get_archetype)(const char *);
    int (*get_button_value)(object_t *);
    int (*get_friendship)(object_t *, object_t *);
    int (*get_money_from_string)(char *, struct moneyblock_t *);
    uint32 (*get_nrof_quest_item)(const object_t *, const char *,
                                  const char *, const char *);
    void *(*get_poolchunk_array_real)(struct mempool *, uint32);
    void (*get_tad)(timeanddate_t *, sint32);
    sint32 (*get_tad_offset_from_string)(const char *);
    void (*give_artifact_abilities)(object_t *, artifact_t *);
    object_t *(*guild_get)(player_t *, char *);
    object_t *(*guild_join)(player_t *, char *, int, int, int, int, int, int);
    void (*guild_leave)(player_t *);
    void (*gui_npc)(object_t *, uint8, const char *);
    /* H */
    void (*hashtable_clear)(hashtable_t *);
    void (*hashtable_delete)(hashtable_t *);
    int (*hashtable_erase)(hashtable_t *const, const hashtable_const_key_t);
    hashtable_value_t (*hashtable_find)(const hashtable_t *const,
                                        const hashtable_const_key_t);
    int (*hashtable_insert)(hashtable_t *const, const hashtable_const_key_t,
                            const hashtable_value_t);
    hashtable_iterator_t (*hashtable_iterator)(const hashtable_t *const);
    hashtable_iterator_t (*hashtable_iterator_next)(const hashtable_t *const,
                                                    hashtable_iterator_t);
    hashtable_t *(*hashtable_new)(hashtable_size_t (*hash_func)(const hashtable_const_key_t),
                                int (*equals_func)(const hashtable_const_key_t,
                                const hashtable_const_key_t),
                                hashtable_const_key_t, hashtable_const_key_t,
                                hashtable_size_t);
    /* I */
    object_t *(*insert_ob_in_map)(object_t * const, map_t *, object_t *const,
                                const int);
    object_t *(*insert_ob_in_ob)(object_t *, object_t *);
    object_t *(*is_player_inv)(object_t *);
    /* J */
    /* K */
    object_t *(*kill_object)(object_t *, object_t *, const char *, const char *);
    /* L */
    int (*learn_skill)(object_t *, int);
    object_t *(*load_object_str)(char *);
    object_t *(*locate_beacon)(shstr_t *);
    sint8 (*LOG)(log_t, char *, ...) DAI_GNUC_PRINTF(2, 3);
    int (*look_up_spell_name)(const char *);
    int (*lookup_skill_by_name)(char *);
#ifdef USE_CHANNELS
    int (*lua_channel_message)(char *, const char *, char *, int);
#endif
    /* M */
    void (*map_check_in_memory)(map_t *);
    map_t *(*map_is_in_memory)(shstr_t *);
    void (*map_player_link)(map_t *, sint16, sint16, uint8);
    uint16 (*map_player_unlink)(map_t *, shstr_t *);
    map_t *(*map_save)(map_t *);
    void (*map_transfer_apartment_items)(map_t *, map_t *, sint16, sint16);
    sint64 (*material_repair_cost)(object_t *, object_t *);
    void (*material_repair_item)(object_t *, uint8);
    sint8 (*move_ob)(object_t *, sint8, object_t *);
    void (*msp_rebuild_slices_with)(msp_t *, object_t *);
    void (*msp_rebuild_slices_without)(msp_t *, object_t *);
    /* N */
    void (*ndi)(const int, const int, const object_t *const, const char *const, ...) DAI_GNUC_PRINTF(4, 5);
    void (*ndi_map)(const int, msp_t *, const int, const object_t *const, const object_t *const, const char *const, ...) DAI_GNUC_PRINTF(6, 7);
    uint32 (*nearest_pow_two_exp)(uint32);
    char *(*normalize_path)(const char *, const char *, char *);
    char *(*normalize_path_direct)(const char *, const char *, char *);
    /* O */
    objectlink_t *(*objectlink_delink)(objectlink_t **, objectlink_t **, objectlink_t *);
    map_t *(*out_of_map)(map_t *, sint16 *, sint16 *);
    /* P */
    void (*play_sound_map)(msp_t *, int, int);
    void (*play_sound_player_only)(player_t *, int, int, int, int);
    char *(*print_tad)(timeanddate_t *, int);
    /* Q */
    sint64 (*query_cost)(object_t *, object_t *, int);
    sint64 (*query_money)(object_t *, moneyblock_t *);
    char *(*query_name)(object_t *, object_t *, uint32, uint8);
    int (*quest_count_pending)(const object_t *);
    object_t *(*quest_find_name)(const object_t *, const char *);
    int (*quest_get_active_status)(player_t *, object_t *);
    /* R */
    map_t *(*ready_map_name)(shstr_t *, shstr_t *, uint32, shstr_t *);
    void  (*reload_behaviours)(object_t *);
    void (*remove_ob)(object_t *);
    void (*reset_instance_data)(player_t *pl);
    void (*return_poolchunk_array_real)(void *, uint32, struct mempool *);
    sint8 (*rv_get)(object_t *, msp_t *, object_t *, msp_t *, rv_t *, uint8);
    /* S */
    void (*set_personal_light)(player_t *, int);
    void (*set_quest_status)(object_t *, int, int);
    uint8 (*shop_pay_amount)(sint64, object_t *);
    shstr_t *(*shstr_add_refcount)(const char *);
    shstr_t *(*shstr_add_string)(const char *);
    shstr_t *(*shstr_find)(const char *);
    void (*shstr_free)(const char *);
    void (*signal_connection)(object_t *, object_t *, object_t *, map_t *);
    object_t *(*sparkly_create)(archetype_t *, object_t *, sint16, sint16, uint8);
    void (*spring_trap)(object_t *, object_t *);
    char *(*strdup_local)(const char *);
    sint32 (*sum_weight)(object_t *);
    /* T */
    sint32 (*thing_declare)(object_t *, object_t *, object_t *, sint32, uint32, objectlink_t **);
    object_t *(*thing_drop_to_floor)(object_t *, object_t *, uint32);
    object_t *(*thing_pick_up)(object_t *, object_t *, object_t *, uint32);
    void (*turn_off_light)(object_t *);
    void (*turn_on_light)(object_t *);
    /* U */
    struct mob_known_obj *(*update_npc_knowledge)(object_t *, object_t *, rv_t *, int, int);
    void (*update_ob_speed)(object_t *);
    int (*update_quest)(object_t *, uint8, object_t *, char *, char *);
    int (*unlearn_skill)(object_t *, int);
    /* V */
    /* W */
    /* X */
    /* Y */
    /* Z */

    /* GLOBAL VARIABLES */
    Animations **animations;
    struct _archetype_global *archetype_global;
    struct behaviourclass_decl *behaviourclasses;
    uint16 *brightness;
    archetype_t **coins_arch;
    sint32 *exp_threshold;
    long *global_instance_id;
    New_Face **new_faces;
    uint32 *pticks;
    uint32 *pticks_second;
    settings_t *settings;
    struct shstr_constants *shstr_cons;
    spell *spells;
    uint16 *tadtick;
};

/*****************************************************************************/
/* Exportable functions. Any plugin should define all those.                 */
/* initPlugin        is called when the plugin initialization process starts.*/
/* removePlugin      is called before the plugin gets unloaded from memory.  */
/* getPluginProperty is currently unused.                                    */
/* registerHook      is used to transmit hook pointers from server to plugin.*/
/* triggerEvent      is called whenever an event occurs.                     */
/*****************************************************************************/
/*
extern MODULEAPI CFParm    *initPlugin(CFParm *PParm);
extern MODULEAPI CFParm    *removePlugin(CFParm *PParm);
extern MODULEAPI CFParm    *getPluginProperty(CFParm *PParm);
extern MODULEAPI CFParm    *registerHook(CFParm *PParm);
extern MODULEAPI CFParm    *triggerEvent(CFParm *PParm);
*/

extern CFPlugin             PlugList[PLUGIN_MAX];
extern int                  PlugNR;

extern int       plugin_trigger_object_event(int eventnr, object_t * me, object_t * activator, object_t * other, shstr_t * msg, int parm1, int parm2, int parm3);
extern int       plugin_trigger_behaviour_event(shstr_t * pluginname, shstr_t * filename, shstr_t * options, object_t * me, move_response * response);
extern int       plugin_trigger_player_event(shstr_t *pluginname, shstr_t *filename, shstr_t *options, object_t *me, object_t *activator, object_t *other, shstr_t *msg, int parm1, int parm2);
extern int       plugin_trigger_help_event(shstr_t *pluginname, shstr_t *filename, shstr_t *options, object_t *me, shstr_t *msg);
extern int       find_plugin_command(const char *cmd, object_t * op, CommArray_s * ret);
extern void      displayPluginsList(object_t * op);
extern int       findPlugin(const char *id);
extern void      initPlugins(void);
extern void      removeOnePlugin(const char *id);
extern void      initOnePlugin(const char *pluginfile);
extern void      removePlugins(void);
extern CFParm * CFWCmdRSkill(CFParm * PParm);
extern CFParm * CFWBecomeFollower(CFParm * PParm);
extern CFParm * CFWFindPlayer(CFParm * PParm);
extern CFParm * CFWManualApply(CFParm * PParm);
extern CFParm * CFWCheckSpellKnown(CFParm * PParm);
extern CFParm * CFWDoLearnSpell(CFParm * PParm);
extern CFParm * CFWDoLearnSkill(CFParm * PParm);
extern CFParm * CFWUpdateSpeed(CFParm * PParm);
extern CFParm * CFWDetermineGod(CFParm * PParm);
extern CFParm * CFWFindGod(CFParm * PParm);
extern CFParm * CFWDumpObject(CFParm * PParm);
extern CFParm * CFWLoadObject(CFParm * PParm);
extern CFParm * CFWSendCustomCommand(CFParm * PParm);
extern CFParm * CFWCommunicate(CFParm * PParm);
extern CFParm * CFWFindMarkedObject(CFParm * PParm);
extern CFParm * CFWIdentifyObject(CFParm * PParm);
extern CFParm * CFWTeleportObject(CFParm * PParm);
extern CFParm * CFWCreateObject(CFParm * PParm);
extern CFParm * CFMapSave(CFParm * PParm);
extern CFParm * CFMapDelete(CFParm * PParm);
extern CFParm * CFInterface(CFParm * PParm);
extern void      send_plugin_custom_message(object_t * pl, char *buf);

#endif /* ifndef __PLUGIN_H */
