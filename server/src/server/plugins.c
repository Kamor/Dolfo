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

/*****************************************************************************/
/* First, the headers. We only include plugin.h, because all other includes  */
/* are done into it, and plugproto.h (which is used only by this file).      */
/*****************************************************************************/
#include <global.h>
#include <plugproto.h>

static int TriggerPlugin(shstr_t *pluginname, shstr_t *filename, shstr_t *options, int eventnr, object_t *me, object_t *activator, object_t *other, shstr_t *msg, int parm1, int parm2, int parm3, move_response *response);
void (*registerHooksFunc)(struct plugin_hooklist *hooks);

struct plugin_hooklist  hooklist    =
{
    /* FUNCTIONS */
    /* A */
    add_pet,
    add_quest_containers,
    add_quest_trigger,
    adjust_light_source,
    arch_to_object,
    /* B */
    buff_add,
    buff_remove,
    /* C */
    cast_spell,
    check_path,
    clear_mob_knowns,
    clone_object,
    command_combat,
    command_target,
    cost_string_from_value,
    create_financial_loot,
    create_instance_path_sh,
    create_mapdir_pathname,
    create_mempool,
    create_safe_path_sh,
    create_unique_path_sh,
    /* D */
    decrease_ob_nr,
    /* E */
    enter_map,
    enumerate_coins,
    esrv_send_or_del_item,
    esrv_update_item,
    examine_item,
    exp_adjust,
    /* F */
    find_animation,
    find_archetype,
    find_artifact,
    find_next_object,
    FindFace,
    fix_player,
    free_mempool,
    /* G */
    get_archetype,
    get_button_value,
    get_friendship,
    get_money_from_string,
    get_nrof_quest_item,
    get_poolchunk_array_real,
    get_tad,
    get_tad_offset_from_string,
    give_artifact_abilities,
    guild_get,
    guild_join,
    guild_leave,
    gui_npc,
    /* H */
    hashtable_clear,
    hashtable_delete,
    hashtable_erase,
    hashtable_find,
    hashtable_insert,
    hashtable_iterator,
    hashtable_iterator_next,
    hashtable_new,
    /* I */
    insert_ob_in_map,
    insert_ob_in_ob,
    is_player_inv,
    /* J */
    /* K */
    kill_object,
    /* L */
    learn_skill,
    load_object_str,
    locate_beacon,
    LOG,
    look_up_spell_name,
    lookup_skill_by_name,
#ifdef USE_CHANNELS
    lua_channel_message,
#endif
    /* M */
    map_check_in_memory,
    map_is_in_memory,
    map_player_link,
    map_player_unlink,
    map_save,
    map_transfer_apartment_items,
    material_repair_cost,
    material_repair_item,
    move_ob,
    msp_rebuild_slices_with,
    msp_rebuild_slices_without,
    /* N */
    ndi,
    ndi_map,
    nearest_pow_two_exp,
    normalize_path,
    normalize_path_direct,
    /* O */
    objectlink_delink,
    out_of_map,
    /* P */
    play_sound_map,
    play_sound_player_only,
    print_tad,
    /* Q */
    query_cost,
    query_money,
    query_name,
    quest_count_pending,
    quest_find_name,
    quest_get_active_status,
    /* R */
    ready_map_name,
    reload_behaviours,
    remove_ob,
    reset_instance_data,
    return_poolchunk_array_real,
    rv_get,
    /* S */
    set_personal_light,
    set_quest_status,
    shop_pay_amount,
    shstr_add_refcount,
    shstr_add_string,
    shstr_find,
    shstr_free,
    signal_connection,
    sparkly_create,
    spring_trap,
    strdup_local,
    sum_weight,
    /* T */
    thing_declare,
    thing_drop_to_floor,
    thing_pick_up,
    turn_off_light,
    turn_on_light,
    /* U */
    update_npc_knowledge,
    update_ob_speed,
    update_quest,
    unlearn_skill,
    /* V */
    /* W */
    /* X */
    /* Y */
    /* Z */

    /* GLOBAL VARIABLES */
    &animations,
    &archetype_global,
    behaviourclasses,
    brightness,
    coins_arch,
    exp_threshold,
    &global_instance_id,
    &new_faces,
    &pticks,
    &pticks_second,
    &settings,
    &shstr_cons,
    spells,
    &tadtick,
};

CFPlugin                PlugList[PLUGIN_MAX];
int                     PlugNR      = 0;

int plugin_trigger_object_event(int eventnr, object_t *me, object_t *activator, object_t *other, shstr_t *msg, int parm1, int parm2, int parm3)
{
    object_t *this,
             *next;
    object_t *event = NULL;

    /* eventnr must be one of the object events. For troubleshooting, etc,
     * particular events can be disabled by commenting out the relevsnt clauses
     * below. When new events are added in plugin.h this test must also be
     * updated. */
    if (!(eventnr == PLUGIN_EVENT_OBJECT_APPLY ||
          eventnr == PLUGIN_EVENT_OBJECT_ATTACK ||
          eventnr == PLUGIN_EVENT_OBJECT_DEATH ||
          eventnr == PLUGIN_EVENT_OBJECT_DROP ||
          eventnr == PLUGIN_EVENT_OBJECT_PICKUP ||
          eventnr == PLUGIN_EVENT_OBJECT_SAY ||
          eventnr == PLUGIN_EVENT_OBJECT_STOP ||
          eventnr == PLUGIN_EVENT_OBJECT_TIME ||
          eventnr == PLUGIN_EVENT_OBJECT_THROW ||
          eventnr == PLUGIN_EVENT_OBJECT_TRIGGER ||
          eventnr == PLUGIN_EVENT_OBJECT_CLOSE ||
          eventnr == PLUGIN_EVENT_OBJECT_EXAMINE ||
          eventnr == PLUGIN_EVENT_OBJECT_TALK ||
          eventnr == PLUGIN_EVENT_OBJECT_KNOWLEDGE))
    {
        return 0;
    }

    /* Must be a me object with the appropriate event flag. */
    if (!me ||
        !(me->event_flags & PLUGIN_EVENT_FLAG(eventnr)))
    {
        return 0;
    }

    /* Currently just browse me->inv for the first appropriate event object. */
    /* TODO: This is a poor approach for several reasons. */
    FOREACH_OBJECT_IN_OBJECT(this, me, next)
    {
        if (this->type == TYPE_EVENT_OBJECT &&
            this->subtype == eventnr)
        {
            break;
        }
    }

    /* No appropriate event object? Always a problem. */
    if (!(event = this))
    {
        LOG(llevMapbug, "MAPBUG:: %s[%d] @ %s %d %d with %d event flag but no event object!\n",
            STRING_OBJ_NAME(me), TAG(me), STRING_MAP_PATH(me->map), me->x,
            me->y, eventnr);
        return 0;
    }

    /* Avoid double triggers and infinite loops. Currently only enabled for SAY
     * and TRIGGER events, as those are the ones we have experienced problems
     * with. */
    /* UPDATE: Used to test for a MULTIPLE_TRIGGERS flag before aborting.
     * However, this was only used in one case (a TRIGGER when updating a mob's
     * knowledge). So I just added a KNOWLEDGE event which inherently can be
     * triggered multiple times.
     *
     * -- Smacky 20160617 */
    if (eventnr == PLUGIN_EVENT_OBJECT_SAY ||
        eventnr == PLUGIN_EVENT_OBJECT_TRIGGER)
    {
        if (event->damage_round_tag == ROUND_TAG)
        {
            LOG(llevInfo, "ABORTING: Event called twice in the same tick!\n");
            return 0;
        }

        event->damage_round_tag = ROUND_TAG;
    }

    return TriggerPlugin(event->name, event->race, event->slaying, eventnr, me, activator, other, msg, parm1, parm2, parm3, NULL);
}

int plugin_trigger_behaviour_event(shstr_t * pluginname, shstr_t * filename, shstr_t * options, object_t * me, move_response * response)
{
    return TriggerPlugin(pluginname, filename, options, PLUGIN_EVENT_SPECIAL_BEHAVIOUR, me, NULL, NULL, NULL, 0, 0, 0, response);
}

int plugin_trigger_player_event(shstr_t *pluginname, shstr_t *filename, shstr_t *options, object_t *me, object_t *activator, object_t *other, shstr_t *msg, int parm1, int parm2)
{
    return TriggerPlugin(pluginname, filename, options, PLUGIN_EVENT_SPECIAL_PLAYER, me, activator, other, msg, parm1, parm2, 0, NULL);
}

int plugin_trigger_help_event(shstr_t *pluginname, shstr_t *filename, shstr_t *options, object_t *me, shstr_t *msg)
{
    return TriggerPlugin(pluginname, filename, options, PLUGIN_EVENT_SPECIAL_HELP, me, me, NULL, msg, 0, 0, 0, NULL);
}

static int TriggerPlugin(shstr_t *pluginname, shstr_t *filename, shstr_t *options, int eventnr, object_t *me, object_t *activator, object_t *other, shstr_t *msg, int parm1, int parm2, int parm3, move_response *response)
{
    int       pluginnr;
    CFParm    CFP;
    int       returnvalue;

    /* pluginname is the name of the plugin so make sure it's valid. */
    if ((pluginnr = findPlugin(pluginname)) < 0)
    {
        object_t *env;

        for (env = me; env->env; env = env->env)
        {
        }

        LOG(llevMapbug, "MAPBUG:: %s[%d] @ %s %d %d references unknown plugin (%s)!\n",
            STRING_OBJ_NAME(me), TAG(me), STRING_MAP_PATH(env->map), env,
            env, pluginname);
        return 0;
    }

    /* This info is always good to see in the logs. */
    LOG(llevInfo, "Plugin:%s(%d) event:%d file:%s options:%s me:%s[%d] activator:%s[%d] other:%s[%d] text:%s p1:%d p2:%d p3:%d\n",
        pluginname, pluginnr, eventnr, filename, options,
        STRING_OBJ_NAME(me), TAG(me), STRING_OBJ_NAME(activator), TAG(activator), STRING_OBJ_NAME(other), TAG(other),
        STRING_SAFE(msg), parm1, parm2, parm3);

    /* TODO: This CFParm stuff is passe. I think it was being phased out (ie,
     * hooks-> structure as opposed to the clumsy wrappers interface) and this
     * should be continued. For now note that we have 4 more void * Values
     * availsble and/or 15 int Types.
     *
     * Smacky 20160614 */
    CFP.Value[0] = &eventnr;
    CFP.Value[1] = me;
    CFP.Value[2] = activator;
    CFP.Value[3] = other;
    CFP.Value[4] = (void *)msg;
    CFP.Value[5] = &parm1;
    CFP.Value[6] = &parm2;
    CFP.Value[7] = &parm3;
    CFP.Value[9] = (char *)filename;
    CFP.Value[10] = (char *)options;
    CFP.Value[11] = (move_response *)response;

#ifdef TIME_SCRIPTS
    TPR_START();
#endif
    returnvalue = PlugList[pluginnr].eventfunc(&CFP);
#ifdef TIME_SCRIPTS
    TPR_STOP("Script running time");
#endif

    /* TODO: we could really use a more efficient event interface */
    return returnvalue;
}

/*****************************************************************************/
/* Tries to find if a given command is handled by a plugin.                  */
/* Note that find_plugin_command is called *before* the internal commands are*/
/* checked, meaning that you can "overwrite" them.                           */
/*****************************************************************************/
int find_plugin_command(const char *cmd, object_t *op, CommArray_s *RTNCmd)
{
    CFParm              CmdParm;
    int                 i;
    char               *cmdchar = "command?";

    CmdParm.Value[0] = cmdchar;
    CmdParm.Value[1] = (char *) cmd;
    CmdParm.Value[2] = op;

    for (i = 0; i < PlugNR; i++)
    {
        if(PlugList[i].propfunc(&CmdParm, RTNCmd))
        {
            LOG(llevInfo, "RTNCMD: name %s, time %f\n", RTNCmd->name, RTNCmd->time);
            return 1;
        }
    }
    return 0;
}

/*****************************************************************************/
/* Displays a list of loaded plugins (keystrings and description) in the     */
/* game log window.                                                          */
/*****************************************************************************/
void displayPluginsList(object_t *op)
{
    char    buf[MEDIUM_BUF];
    int     i;

    ndi(NDI_UNIQUE, 0, op, "List of loaded plugins:");
    ndi(NDI_UNIQUE, 0, op, "-----------------------");
    for (i = 0; i < PlugNR; i++)
    {
        strcpy(buf, PlugList[i].id);
        strcat(buf, ", ");
        strcat(buf, PlugList[i].fullname);
        ndi(NDI_UNIQUE, 0, op, "%s", buf);
    }
}

/*****************************************************************************/
/* Searches in the loaded plugins list for a plugin with a keyname of id.    */
/* Returns the position of the plugin in the list if a matching one was found*/
/* or -1 if no correct plugin was detected.                                  */
/*****************************************************************************/
int findPlugin(const char *id)
{
    int i;
    for (i = 0; i < PlugNR; i++)
        if (!strcasecmp(id, PlugList[i].id))
            return i;
    return -1;
}

#ifdef WIN32
/*****************************************************************************/
/* WIN32 Plugins initialization. Browses the plugins directory and call      */
/* initOnePlugin for each file found.                                        */
/*****************************************************************************/
void initPlugins(void)
{
    struct dirent  *currentfile;
    DIR            *plugdir;
    int             n;
    char            buf[MEDIUM_BUF];
    char            buf2[MEDIUM_BUF];

    LOG(llevInfo, "Now initializing plugins\n");
    /* strcpy(buf,DATADIR); dlls should not part of DATADIR or LIBDOR */
    /* strcpy(buf,"./plugins/"); */
    strcpy(buf, PLUGINDIR"/");
    LOG(llevInfo, "Plugins directory is %s\n", buf);

    if (!(plugdir = opendir(buf)))
        return;

    n = 0;

    while ((currentfile = readdir(plugdir)))
    {
        if (strcmp(currentfile->d_name, ".."))
        {
            /* don't load dotfiles, CVS directory or all which has a .txt inside */
            if (currentfile->d_name[0] != '.' &&
                    !strstr(currentfile->d_name, ".txt") &&
                    strcmp(currentfile->d_name, "CVS"))
            {
                strcpy(buf2, buf);
                strcat(buf2, currentfile->d_name);
                LOG(llevInfo, "Registering plugin %s\n", currentfile->d_name);
                initOnePlugin(buf2);
            }
        }
    }
    closedir(plugdir);
}

/*****************************************************************************/
/* WIN32 Plugin initialization. Initializes a plugin known by its filename.  */
/* The initialization process has several stages:                            */
/* - Loading of the DLL itself;                                              */
/* - Basical plugin information request;                                     */
/* - CF-Plugin specific initialization tasks (call to initPlugin());         */
/* - Hook bindings;                                                          */
/*****************************************************************************/
void initOnePlugin(const char *pluginfile)
{
    int     i   = 0;
    HMODULE DLLInstance;
    void   *ptr = NULL;
    CFParm *HookParm;

    if ((DLLInstance = LoadLibrary(pluginfile)) == NULL)
    {
        LOG(llevBug, "BUG: Error while trying to load %s\n", pluginfile);
        return;
    }
    PlugList[PlugNR].libptr = DLLInstance;
    PlugList[PlugNR].initfunc = (f_plugin_init) (GetProcAddress(DLLInstance, "initPlugin"));
    if (PlugList[PlugNR].initfunc == NULL)
    {
        LOG(llevBug, "BUG: Plugin init error\n");
        FreeLibrary(ptr);
        return;
    }
    else
    {
        const char *name = "(unknown)", *version = "(unknown)";

        /* We must send the hooks first of all, so the plugin can use the LOG function */
        if ((registerHooksFunc = (void *) GetProcAddress(DLLInstance, "registerHooks")))
        {
            registerHooksFunc(&hooklist);
        }

        PlugList[PlugNR].initfunc(NULL, &name, &version);
        LOG(llevInfo, "Plugin name: %s, known as %s\n", version, name);
        PlugList[PlugNR].id = shstr_add_string(name);
        PlugList[PlugNR].fullname = shstr_add_string(version);
    }
    PlugList[PlugNR].removefunc = (f_plugin) (GetProcAddress(DLLInstance, "removePlugin"));
    PlugList[PlugNR].hookfunc = (f_plugin) (GetProcAddress(DLLInstance, "registerHook"));
    PlugList[PlugNR].eventfunc = (f_plugin_event) (GetProcAddress(DLLInstance, "triggerEvent"));
    PlugList[PlugNR].pinitfunc = (f_plugin) (GetProcAddress(DLLInstance, "postinitPlugin"));
    PlugList[PlugNR].propfunc = (f_plugin_prop) (GetProcAddress(DLLInstance, "getPluginProperty"));
    if (PlugList[PlugNR].pinitfunc == NULL)
    {
        LOG(llevBug, "BUG: Plugin postinit error\n");
        FreeLibrary(ptr);
        return;
    }

    if (PlugList[PlugNR].hookfunc == NULL)
    {
        LOG(llevBug, "BUG: Plugin hook error\n");
        FreeLibrary(ptr);
        return;
    }
    else
    {
        int j;
        i = 0;
        HookParm = (CFParm *) (calloc(1, sizeof(CFParm)));
        HookParm->Value[0] = (int *) (malloc(sizeof(int)));

        for (j = 0; j < NR_OF_HOOKS; j++)
        {
            memcpy(HookParm->Value[0], &j, sizeof(int));
            HookParm->Value[1] = HookList[j];

            PlugList[PlugNR].hookfunc(HookParm);
        }
        free(HookParm->Value[0]);
        free(HookParm);
    }
    if (PlugList[PlugNR].eventfunc == NULL)
    {
        LOG(llevBug, "BUG: Event plugin error\n");
        FreeLibrary(ptr);
        return;
    }
    PlugNR++;
    PlugList[PlugNR - 1].pinitfunc(NULL);
    LOG(llevInfo, "Done\n");
}

/*****************************************************************************/
/* Removes one plugin from memory. The plugin is identified by its keyname.  */
/*****************************************************************************/
void removeOnePlugin(const char *id)
{
    int plid;
    int j;

    /* what that warning means MT-2005 */
    /* LOG(llevDebug, "Warning - removeOnePlugin non-canon under Win32\n"); */
    plid = findPlugin(id);
    if (plid < 0)
        return;
    if (PlugList[plid].removefunc != NULL)
        PlugList[plid].removefunc(NULL);

    SHSTR_FREE(PlugList[plid].id);
    SHSTR_FREE(PlugList[plid].fullname);

    /* We unload the library... */
    FreeLibrary(PlugList[plid].libptr);
    /* Then we copy the rest on the list back one position */
    PlugNR--;
    if (plid == 31)
        return;
    for (j = plid + 1; j < 32; j++)
    {
        PlugList[j - 1] = PlugList[j];
    }
}

#else

#ifndef HAVE_SCANDIR

extern int alphasort(struct dirent **a, struct dirent **b);
#endif

/*****************************************************************************/
/* UNIX Plugins initialization. Browses the plugins directory and call       */
/* initOnePlugin for each file found.                                        */
/*****************************************************************************/
void initPlugins(void)
{
    struct dirent  **namelist   = NULL;
    int             n;
    char            buf[MEDIUM_BUF];
    char            buf2[MEDIUM_BUF];

    LOG(llevInfo, "Initializing plugins :\n");
    /*        strcpy(buf,DATADIR);
            strcat(buf,"/../plugins/");*/
    strcpy(buf, PLUGINDIR"/");
    LOG(llevInfo, "Plugins directory is %s\n", buf);
    n = scandir(buf, &namelist, 0, alphasort);
    if (n < 0)
        LOG(llevBug, "BUG: plugins.c: scandir...\n");
    else
        while (n--)
        {
            if (strcmp(namelist[n]->d_name, ".."))
            {
                /* don't load dorfiles, CVS directory or all which has a .txt inside */
                if (namelist[n]->d_name[0] != '.'
                 && !strstr(namelist[n]->d_name, ".txt")
                 && strcmp(namelist[n]->d_name, "CVS"))
                {
                    strcpy(buf2, buf);
                    strcat(buf2, namelist[n]->d_name);
                    LOG(llevInfo, " -> Loading plugin : %s\n", namelist[n]->d_name);
                    initOnePlugin(buf2);
                }
            }
            if (namelist[n] != NULL)
                free(namelist[n]);
        }
    if (namelist != NULL)
        free(namelist);
}

/*****************************************************************************/
/* Removes one plugin from memory. The plugin is identified by its keyname.  */
/*****************************************************************************/
void removeOnePlugin(const char *id)
{
    int plid;
    int j;
    plid = findPlugin(id);
    if (plid < 0)
        return;
    if (PlugList[plid].removefunc != NULL)
        PlugList[plid].removefunc(NULL);

    SHSTR_FREE(PlugList[plid].id);
    SHSTR_FREE(PlugList[plid].fullname);

    /* We unload the library... */
    dlclose(PlugList[plid].libptr);
    /* Then we copy the rest on the list back one position */
    PlugNR--;
    if (plid == 31)
        return;
    LOG(llevInfo, "plid=%i, PlugNR=%i\n", plid, PlugNR);
    for (j = plid + 1; j < 32; j++)
    {
        PlugList[j - 1] = PlugList[j];
    }
}

/*****************************************************************************/
/* UNIX Plugin initialization. Initializes a plugin known by its filename.   */
/* The initialization process has several stages:                            */
/* - Loading of the DLL itself;                                              */
/* - Basical plugin information request;                                     */
/* - CF-Plugin specific initialization tasks (call to initPlugin());         */
/* - Hook bindings;                                                          */
/*****************************************************************************/
void initOnePlugin(const char *pluginfile)
{
    int     i   = 0;
    void   *ptr = NULL;
    CFParm *HookParm;

    if ((ptr = dlopen(pluginfile, RTLD_NOW | RTLD_GLOBAL)) == NULL)
    {
        LOG(llevInfo, "Plugin error: %s\n", dlerror());
        return;
    }
    PlugList[PlugNR].libptr = ptr;
    PlugList[PlugNR].initfunc = (f_plugin_init) (dlsym(ptr, "initPlugin"));
    if (PlugList[PlugNR].initfunc == NULL)
    {
        LOG(llevInfo, "Plugin init error: %s\n", dlerror());
    }
    else
    {
        const char *name = "(unknown)", *version = "(unknown)";

        /* We must send the hooks first of all, so the plugin can use the LOG function */
        if ((registerHooksFunc = dlsym(ptr, "registerHooks")))
        {
            registerHooksFunc(&hooklist);
        }

        PlugList[PlugNR].initfunc(NULL, &name, &version);
        LOG(llevInfo, "    Plugin %s loaded under the name of %s\n", version, name);
        PlugList[PlugNR].id = shstr_add_string(name);
        PlugList[PlugNR].fullname = shstr_add_string(version);
    }
    PlugList[PlugNR].removefunc = (f_plugin) (dlsym(ptr, "removePlugin"));
    PlugList[PlugNR].hookfunc = (f_plugin) (dlsym(ptr, "registerHook"));
    PlugList[PlugNR].eventfunc = (f_plugin_event) (dlsym(ptr, "triggerEvent"));
    PlugList[PlugNR].pinitfunc = (f_plugin) (dlsym(ptr, "postinitPlugin"));
    PlugList[PlugNR].propfunc = (f_plugin_prop) (dlsym(ptr, "getPluginProperty"));
    LOG(llevInfo, "Done\n");
    if (PlugList[PlugNR].pinitfunc == NULL)
    {
        LOG(llevInfo, "Plugin postinit error: %s\n", dlerror());
    }

    if (PlugList[PlugNR].hookfunc == NULL)
    {
        LOG(llevInfo, "Plugin hook error: %s\n", dlerror());
    }
    else
    {
        int j;
        i = 0;
        HookParm = (CFParm *) (malloc(sizeof(CFParm)));
        HookParm->Value[0] = (int *) (malloc(sizeof(int)));

        for (j = 0; j < NR_OF_HOOKS; j++)
        {
            memcpy(HookParm->Value[0], &j, sizeof(int));
            HookParm->Value[1] = HookList[j];
            PlugList[PlugNR].hookfunc(HookParm);
        }
        free(HookParm->Value[0]);
        free(HookParm);
    }

    if (PlugList[PlugNR].eventfunc == NULL)
    {
        LOG(llevBug, "BUG: Event plugin error %s\n", dlerror());
    }
    PlugNR++;
    PlugList[PlugNR - 1].pinitfunc(NULL);
    LOG(llevInfo, "[Done]\n");
}
#endif /*WIN32*/

void removePlugins(void)
{
    if (PlugNR)
    {
        int i;
        shstr_t *ids[32];

        for (i = 0; i != 32; ++i)
            ids[i] = NULL;

            LOG(llevInfo, "Unloading plugins:\n");
        for (i = 0; i != PlugNR; ++i)
            ids[i] = PlugList[i].id;

        for (i = 0; i != 32; ++i)
            if (ids[i] != NULL)
                removeOnePlugin(ids[i]);
    }
}
/* TODO: remove all this old style plugin functions and move them to hooks->
 * This will also remove this ugly mallocs here . MT-11.2005
 */

/*****************************************************************************/
/* command_rskill wrapper.                                                   */
/*****************************************************************************/
/* 0 - player;                                                               */
/* 1 - parameters.                                                           */
/*****************************************************************************/
CFParm * CFWCmdRSkill(CFParm *PParm)
{
    static int  val;
    CFParm     *CFP;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = command_rskill((object_t *) (PParm->Value[0]), (char *) (PParm->Value[1]));
    CFP->Value[0] = (void *) (&val);
    return CFP;
}

/*****************************************************************************/
/* become_follower wrapper.                                                  */
/*****************************************************************************/
/* 0 - object to change;                                                     */
/* 1 - new god object.                                                       */
/*****************************************************************************/
CFParm * CFWBecomeFollower(CFParm *PParm)
{
    become_follower((object_t *) (PParm->Value[0]), (object_t *) (PParm->Value[1]));
    return NULL;
}

/*****************************************************************************/
/* find_player wrapper.                                                      */
/*****************************************************************************/
/* 0 - name of the player to find.                                           */
/*****************************************************************************/
CFParm * CFWFindPlayer(CFParm *PParm)
{
    player_t *pl;
    CFParm *CFP;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    pl = find_player((char *) (PParm->Value[0]));
    CFP->Value[0] = (void *) (pl);
    return CFP;
}

/*****************************************************************************/
/* apply_object wrapper.                                                     */
/*****************************************************************************/
/* 0 - object applying;                                                      */
/* 1 - object to apply;                                                      */
/* 2 - apply flags.                                                          */
/*****************************************************************************/
CFParm * CFWManualApply(CFParm *PParm)
{
    CFParm     *CFP;
    static int  val;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = apply_object((object_t *) (PParm->Value[0]), (object_t *) (PParm->Value[1]), *(int *) (PParm->Value[2]));
    CFP->Value[0] = &val;
    return CFP;
}

/*****************************************************************************/
/* check_spell_known wrapper.                                                */
/*****************************************************************************/
/* 0 - object to check;                                                      */
/* 1 - spell index to search.                                                */
/* return: 0: op has not this skill; 1: op has this skill                    */
/*****************************************************************************/
CFParm * CFWCheckSpellKnown(CFParm *PParm)
{
    CFParm     *CFP;
    static int  val;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = check_spell_known((object_t *) (PParm->Value[0]), *(int *) (PParm->Value[1]));
    CFP->Value[0] = &val;
    return CFP;
}

/*****************************************************************************/
/* do_learn_spell wrapper.                                                   */
/*****************************************************************************/
/* 0 - object to affect;                                                     */
/* 1 - spell index to learn;                                                 */
/* 2 - mode 0:learn , 1:unlearn                                              */
/*****************************************************************************/
CFParm * CFWDoLearnSpell(CFParm *PParm)
{
    /* if mode = 1, unlearn - if mode =0 learn */
    if (*(int *) (PParm->Value[2]))
    {
        do_forget_spell((object_t *) (PParm->Value[0]), *(int *) (PParm->Value[1]));
    }
    else
    {
        do_learn_spell((object_t *) (PParm->Value[0]), *(int *) (PParm->Value[1]));
    }
    return NULL;
}

/*****************************************************************************/
/* determine_god wrapper.                                                    */
/*****************************************************************************/
/* 0 - object to determine the god of.                                       */
/*****************************************************************************/
CFParm * CFWDetermineGod(CFParm *PParm)
{
    CFParm     *CFP;
    const char *val;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = determine_god((object_t *) (PParm->Value[0]));
    CFP->Value[0] = (void *) (val);
    return CFP;
}

/*****************************************************************************/
/* find_god wrapper.                                                         */
/*****************************************************************************/
/* 0 - Name of the god to search for.                                        */
/*****************************************************************************/
CFParm * CFWFindGod(CFParm *PParm)
{
    CFParm *CFP;
    object_t *val;
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = find_god((char *) (PParm->Value[0]));
    CFP->Value[0] = (void *) (val);
    return CFP;
}

/*****************************************************************************/
/* dump_me wrapper.                                                          */
/*****************************************************************************/
/* 0 - object to dump;                                                       */
/* JRG 13-May-2009 added size parameter to dump_me                           */
/*****************************************************************************/
CFParm * CFWDumpObject(CFParm *PParm)
{
    CFParm *CFP;
    char   *val;
    /*    object* ob; not used */
    val = (char *) (malloc(sizeof(char) * 10240));
    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    dump_me((object_t *) (PParm->Value[0]), val, 10240);
    CFP->Value[0] = (void *) (val);
    return CFP;
}

/*****************************************************************************/
/* load_object_str wrapper.                                                  */
/*****************************************************************************/
/* 0 - object dump string to load.                                           */
/*****************************************************************************/
CFParm * CFWLoadObject(CFParm *PParm)
{
    CFParm *CFP;
    object_t *val;

    CFP = (CFParm *) (malloc(sizeof(CFParm)));
    val = load_object_str((char *) (PParm->Value[0]));
    LOG(llevDebug, "CFWLoadObject: %s\n", STRING_OBJ_NAME(val));
    CFP->Value[0] = (void *) (val);
    return CFP;
}

CFParm * CFWSendCustomCommand(CFParm *PParm)
{
    send_plugin_custom_message((object_t *) (PParm->Value[0]), (char *) (PParm->Value[1]));
    return NULL;
}


/*****************************************************************************/
/* communicate wrapper.                                                      */
/*****************************************************************************/
/* 0 - object                                                                */
/* 1 - string                                                                */
/*****************************************************************************/
CFParm * CFWCommunicate(CFParm *PParm)
{
    /*char buf[MEDIUM_BUF];*/
    object_t *op      = (object_t *) PParm->Value[0];
    char   *string  = (char *) PParm->Value[1];
    if ((!op) || (!string))
        return NULL;

    communicate(op, string);
    return NULL;
}

/*****************************************************************************/
/* find_marked_object .                                                      */
/*****************************************************************************/
/* 0 - object                                                                */
/*****************************************************************************/
/* return: object or NULL                                                    */
/*****************************************************************************/
CFParm * CFWFindMarkedObject(CFParm *PParm)
{
    static CFParm   CFP;

    object_t         *op  = (object_t *) PParm->Value[0];
    if (op)
        op = find_marked_object(op);

    CFP.Value[0] = (void *) op;

    return(&CFP);
}

/*****************************************************************************/
/* create_object wrapper.                                                    */
/*****************************************************************************/
/* 0 - archetype                                                             */
/* 1 - map;                                                                  */
/* 2 - x;                                                                    */
/* 3 - y;                                                                    */
/*****************************************************************************/
CFParm * CFWCreateObject(CFParm *PParm)
{
    CFParm         *CFP;
    archetype_t      *arch;
    object_t         *newobj;

    CFP = (CFParm *)calloc(1, sizeof(CFParm));

    CFP->Value[0] = NULL;

    if (!(arch = find_archetype((char *) (PParm->Value[0]))))
        return(CFP);

    if (arch->clone.type == PLAYER)
        return(CFP);

    if (!(newobj = arch_to_object(arch)))
        return(CFP);

    newobj->x = *(int *) (PParm->Value[2]);
    newobj->y = *(int *) (PParm->Value[3]);

    if(newobj->type == MONSTER)
        fix_monster(newobj);

    newobj = insert_ob_in_map(newobj, (map_t *) (PParm->Value[1]), NULL, 0);

    CFP->Value[0] = newobj;
    return (CFP);
}

/*****************************************************************************/
/* GROS: The following one is used to allow a plugin to send a generic cmd to*/
/* a player. Of course, the client need to know the command to be able to    */
/* manage it !                                                               */
/*****************************************************************************/
void send_plugin_custom_message(object_t *pl, char *buf)
{
    if (pl && CONTR(pl))
    {
        cs_cmd_generic(buf, strlen(buf), &(CONTR(pl)->socket));
    }
}
