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

#ifndef __SPROTO_H
#define __SPROTO_H

/* apply.c */
void                        move_apply(object_t *const trap, object_t *const victim, object_t *const originator, const uint16 flags);
int                         apply_object(object_t *op, object_t *tmp, int aflag);
int                         apply_equipment(object_t *who, object_t *op, int aflags);
void                        turn_on_light(object_t *op);
void                        turn_off_light(object_t *op);
void                        apply_light(object_t *who, object_t *op);
/* attack.c */
int                         attack_ob(object_t *op, object_t *hitter, object_t *hit_obj);
int                         damage_ob(object_t *op, int dam, object_t *hitter, attack_envmode_t env_attack);
sint32                      hit_map(object_t *hitter, msp_t *msp);
object_t                   *hit_with_arrow(object_t *op, object_t *victim);
void                        snare_player(object_t *op, object_t *hitter, int dam);
void                        poison_player(object_t *op, object_t *hitter, float dam);
void                        slow_player(object_t *op, object_t *hitter, int dam);
void                        fear_player(object_t *op, object_t *hitter, int dam);
void                        confuse_player(object_t *op, object_t *hitter, int ticks);
void                        blind_player(object_t *op, object_t *hitter, int dam);
void                        paralyze_player(object_t *op, object_t *hitter, int dam);
void                        remove_paralyze(object_t *op);
int                         is_aimed_missile(object_t *op);
int                         is_melee_range(object_t *hitter, object_t *enemy);
/* ban.c */
void                        load_ban_file(void);
void                        save_ban_file(void);
struct objectlink_t          *add_ban_entry(const char *account, const char *name, char *ip, int ticks, int ticks_left);
void                        remove_ban_entry(struct objectlink_t *entry);
int                         check_banned(NewSocket *ns, const char *account, const char *name, char *ip);
/* c_chat.c */
#ifndef USE_CHANNELS
int                         command_describe(object_t *op, char *params);
#endif
int                         command_say(object_t *op, char *params);
int                         command_gsay(object_t *op, char *params);
int                         command_shout(object_t *op, char *params);
int                         command_tell(object_t *op, char *params);
int                         command_nod(object_t *op, char *params);
int                         command_dance(object_t *op, char *params);
int                         command_kiss(object_t *op, char *params);
int                         command_bounce(object_t *op, char *params);
int                         command_smile(object_t *op, char *params);
int                         command_cackle(object_t *op, char *params);
int                         command_laugh(object_t *op, char *params);
int                         command_giggle(object_t *op, char *params);
int                         command_shake(object_t *op, char *params);
int                         command_puke(object_t *op, char *params);
int                         command_growl(object_t *op, char *params);
int                         command_scream(object_t *op, char *params);
int                         command_sigh(object_t *op, char *params);
int                         command_sulk(object_t *op, char *params);
int                         command_hug(object_t *op, char *params);
int                         command_cry(object_t *op, char *params);
int                         command_poke(object_t *op, char *params);
int                         command_accuse(object_t *op, char *params);
int                         command_grin(object_t *op, char *params);
int                         command_bow(object_t *op, char *params);
int                         command_clap(object_t *op, char *params);
int                         command_blush(object_t *op, char *params);
int                         command_burp(object_t *op, char *params);
int                         command_chuckle(object_t *op, char *params);
int                         command_cough(object_t *op, char *params);
int                         command_flip(object_t *op, char *params);
int                         command_frown(object_t *op, char *params);
int                         command_gasp(object_t *op, char *params);
int                         command_glare(object_t *op, char *params);
int                         command_groan(object_t *op, char *params);
int                         command_hiccup(object_t *op, char *params);
int                         command_lick(object_t *op, char *params);
int                         command_pout(object_t *op, char *params);
int                         command_shiver(object_t *op, char *params);
int                         command_shrug(object_t *op, char *params);
int                         command_slap(object_t *op, char *params);
int                         command_smirk(object_t *op, char *params);
int                         command_snap(object_t *op, char *params);
int                         command_sneeze(object_t *op, char *params);
int                         command_snicker(object_t *op, char *params);
int                         command_sniff(object_t *op, char *params);
int                         command_snore(object_t *op, char *params);
int                         command_spit(object_t *op, char *params);
int                         command_strut(object_t *op, char *params);
int                         command_thank(object_t *op, char *params);
int                         command_twiddle(object_t *op, char *params);
int                         command_wave(object_t *op, char *params);
int                         command_whistle(object_t *op, char *params);
int                         command_wink(object_t *op, char *params);
int                         command_yawn(object_t *op, char *params);
int                         command_beg(object_t *op, char *params);
int                         command_bleed(object_t *op, char *params);
int                         command_cringe(object_t *op, char *params);
int                         command_think(object_t *op, char *params);
int                         command_me(object_t *op, char *params);
/* c_misc.c */
int                         command_motd(object_t *op, char *params);
void                        malloc_info(object_t *op);
int                         command_who(object_t *op, char *params);
int                         command_malloc(object_t *op, char *params);
int                         command_mapinfo(object_t *op, char *params);
int                         command_mspinfo(object_t *op, char *params);
int                         command_time(object_t *op, char *params);
int                         command_dumpbelowfull(object_t *op, char *params);
int                         command_dumpbelow(object_t *op, char *params);
int                         command_dumpallobjects(object_t *op, char *params);
int                         command_dumpfriendlyobjects(object_t *op, char *params);
int                         command_dumpallarchetypes(object_t *op, char *params);
int                         command_dumpactivelist(object_t *op, char *params);
int                         command_setmaplight(object_t *op, char *params);
#if 0
int                         command_dumpmap(object_t *op, char *params);
int                         command_dumpallmaps(object_t *op, char *params);
#endif
int                         command_resting(object_t *op, char *params);
int                         command_help(object_t *op, char *params);
int                         command_privacy(object_t *op, char *params);
char                       *get_subdir(const char *name);
int                         command_stuck(object_t *op, char *params);
void                        command_exp(object_t *op, char *params);
/* c_move.c */
int                         command_push_object (object_t *op, char *params);
int                         command_turn_right (object_t *op, char *params);
int                         command_turn_left (object_t *op, char *params);
/* c_new.c */
int                         command_combat(object_t *op, char *params);
int                         command_target(object_t *op, char *params);
void                        command_face_request(char *params, int len, NewSocket *ns);
void                        generate_ext_title(player_t *pl);
/* c_object.c */
int                         command_uskill(object_t *pl, char *params);
int                         command_rskill(object_t *pl, char *params);
object_t                   *find_marked_object(object_t *op);
char                       *examine_monster(object_t *op, object_t *tmp, char *buf, int flag);
char                       *examine(object_t *op, object_t *tmp, int flag);
char                       *examine_item(object_t *op, object_t *item);
/* c_party.c */
/* c_range.c */
float                       fire_magic_tool(object_t *owner, object_t *op, int dir);
int                         command_cast_spell(object_t *op, char *params);
/* c_wiz.c */
int                         command_connections(object_t *op, char *params);
int                         command_kick(object_t *op, char *params);
int                         command_reboot(object_t *op, char *params);
int                         command_goto(object_t *op, char *params);
int                         command_create(object_t *op, char *params);
int                         command_generate(object_t *op, char *params);
int                         command_spawn(object_t *op, char *params);
int                         command_listarch(object_t *op, char *params);
int                         command_mutelevel(object_t *op, char *params);
int                         command_summon(object_t *op, char *params);
int                         command_teleport(object_t *op, char *params);
int                         command_inventory(object_t *op, char *params);
int                         command_dump(object_t *op, char *params);
int                         command_setskill(object_t *op, char *params);
int                         command_addexp(object_t *op, char *params);
int                         command_serverspeed(object_t *op, char *params);
int                         command_stats(object_t *op, char *params);
int                         command_setstat(object_t *op, char *params);
int                         command_resetmap(object_t *op, char *params);
int                         command_check_fd(object_t *op, char *params);
int                         command_mute(object_t *op, char *params);
int                         command_silence(object_t *op, char *params);
int                         command_ban(object_t *op, char *params);
int                         command_sa(object_t *op, char *params);
int                         command_mm(object_t *op, char *params);
int                         command_mw(object_t *op, char *params);
int                         command_gm(object_t *op, char *params);
int                         command_vol(object_t *op, char *params);
int                         command_gmasterlist(object_t *op, char *params);
int                         command_gmasterfile(object_t *op, char *params);
int                         command_listplugins(object_t *op, char *params);
int                         command_loadplugin(object_t *op, char *params);
int                         command_unloadplugin(object_t *op, char *params);
int                         command_ip(object_t *op, char *params);
int                         command_wizpass(object_t *op, char *params);
int                         command_matrix(object_t *op, char *params);
int                         command_stealth(object_t *op, char *params);
int                         command_invisibility(object_t *op, char *params);
int                         command_dm_dev(object_t *op, char *params);
int                         command_dm_light(object_t *op, char *params);
int                         command_password(object_t *op, char *params);
int                         command_set_global_exp(object_t *op, char *params);
/* commands.c */
void                        init_commands(void);
/* container.c */
int                         container_link(player_t *const pl, object_t *const sack);
int                         container_unlink(player_t *const pl, object_t *sack);
int                         container_trap(object_t *const op, object_t *const container);
/* disease.c */
int                         move_disease(object_t *disease);
int                         remove_symptoms(object_t *disease);
object_t                   *find_symptom(object_t *disease);
int                         check_infection(object_t *disease);
int                         infect_object(object_t *victim, object_t *disease, int force);
int                         do_symptoms(object_t *disease);
int                         grant_immunity(object_t *disease);
int                         move_symptom(object_t *symptom);
int                         check_physically_infect(object_t *victim, object_t *hitter);
object_t                   *find_disease(object_t *victim);
int                         cure_disease(object_t *sufferer, object_t *caster);
int                         reduce_symptoms(object_t *sufferer, int reduction);
/* egitem.c */
/* gmaster.c */
void                        remove_gmaster_list(player_t *pl);
int                         check_gmaster_file_entry(char *name, char *host, char *mode);
int                         load_gmaster_file(void);
void                        add_gmaster_file_entry(char *name, char *host, int mode_id);
void                        remove_gmaster_file_entry(objectlink_t *ol);
int                         check_gmaster_list(player_t *pl, int mode_id);
void                        set_gmaster_mode(player_t *pl, int mode_id);
void                        remove_gmaster_mode(player_t *pl);
int                         compare_gmaster_mode(int t, int p);
void                        write_gmaster_file(void);
void                        update_gmaster_file(void);
void                        free_gmaster_list(void);
/* gods.c */
/* init.c */
char                       *version_string(void);
void                        init(int argc, char **argv);
void                        compile_info(void);
void                        fatal_signal(int make_core, int close_sockets, uint8 status);
void                        init_library(void);
void                        free_strings(void);
void                        set_pticks_time(sint32 t);
void                        free_lists_and_tables(void);
/* ipcompare.c */
int                         parse_ip(const char * ip, unsigned char ip_terms[], int mask_pos[]);
int                         ip_compare(const char *ban_tmp, const char *ip_temp);
objectlink_t                 *find_players_on_ip(char *ipmask);
void                        free_iplist(objectlink_t *ip_list);
/* login.c */
int                         player_save(object_t *op);
addme_login_msg             player_load(NewSocket *ns, const char *name);
addme_login_msg             player_create(NewSocket *ns,player_t **pl_ret,char *name,int race,int gender,int skill_nr);
void                        player_addme_failed(NewSocket *ns, int error_msg);
/* main.c */
int                         get_new_instance_num(void);
void                        process_players1(map_t *map);
void                        process_players2(map_t *map);
void                        clean_tmp_files(int flag);
void                        cleanup_without_exit(void);
void                        dequeue_path_requests(void);
void                        do_specials(void);
void                        shutdown_agent(int timer, int ret, player_t *pl, char *reason);
int                         main(int argc, char **argv);
void                        process_events();
void                        iterate_main_loop();
/* mtrand.c */
void                        MTRand_init(const uint32 seed);
sint32                      MTRand_randComp(void);
/* npc_communicate.c */
void                        communicate(object_t *op, char *txt);
void                        talk_to_npc(player_t *pl, char *topic);
void                        gui_npc(object_t *who, uint8 mode, const char *text);
/* spawn_point.c */
void                        spawn_point(object_t *op);
void                        make_mob_homeless(object_t *mob);
void                        adjust_monster(object_t *monster);
objectlink_t                *add_linked_spawn(object_t *spawn);
void                        remove_linked_spawn_list(map_t *map);
void                        send_link_spawn_signal(object_t *spawn, object_t *target, int signal);
/* move.c */
/* pets.c */
/* player.c */
/* plugins.c */
/* rune.c */
void                        rune_attack(object_t *op, object_t *victim);
void                        set_traped_flag(object_t *op);
void                        spring_trap(object_t *trap, object_t *victim);
int                         trap_find(object_t *op, object_t *trap, int level);
int                         trap_show(object_t *trap, object_t *where);
int                         trap_disarm(object_t *disarmer, object_t *trap, int risk);
void                        trap_adjust(object_t *trap, int difficulty);
/* shop.c */
sint64                      query_cost(object_t *tmp, object_t *who, int flag);
char                       *cost_string_from_value(sint64 cost, int mode);
char                       *query_cost_string(object_t *tmp, object_t *who, int flag, int mode);
sint64                      query_money(object_t *where, moneyblock_t *money);
uint8                       player_is_in_shop(player_t *pl);
uint8                       shop_pay_amount(sint64 amount, object_t *op);
uint8                       shop_checkout(object_t *op, object_t *this);
void                        shop_return_unpaid(object_t *who, msp_t *msp);
int                         get_money_from_string(char *text, struct moneyblock_t *money);
int                         enumerate_coins(sint64 value, struct moneyblock_t *money);
object_t                   *create_financial_loot(moneyblock_t *money, object_t *who, uint8 mode);
/* skills.c */
int                         attack_melee_weapon(object_t *op, int dir, char *string);
int                         attack_hth(object_t *pl, int dir, char *string);
int                         skill_attack(object_t *tmp, object_t *pl, int dir, char *string);
int                         do_skill_attack(object_t *tmp, object_t *op, char *string);
int                         SK_level(object_t *op);
int                         find_traps(object_t *pl, int level);
int                         disarm_traps(object_t *op, int dir, int level);
int                         rage(object_t *op, int dir, int level);
/* skill_util.c */
void                        init_skills(void);
void                        validate_skills(player_t *pl);
int                         do_skill(object_t *op, int dir, char *string);
void                        dump_skills(void);
int                         lookup_skill_by_name(char *name);
int                         check_skill_to_apply(object_t *who, object_t *item);
int                         learn_skill(object_t *pl, int skillnr);
int                         unlearn_skill(object_t *op, int skillnr);
int                         use_skill(object_t *op, char *string);
sint8                       change_skill(object_t *who, sint16 nr);
void                        set_action_time(object_t *op, float ticks);
int                         check_skill_action_time(object_t *op, object_t *skill);
/* spell_effect.c */
void                        cast_mana_storm(object_t *op, int lvl);
void                        cast_magic_storm(object_t *op, object_t *tmp, int lvl);
int                         probe(object_t *op);
int                         cast_invisible(object_t *op, object_t *caster, int spell_type);
int                         perceive_self(object_t *op);
int                         cast_heal(object_t *op, int level, object_t *target, int spell_type);
int                         cast_hallowed_ground(object_t *op, int level, int spell_type);
int                         spring_hallowed_ground(object_t *op, object_t *trap);
int                         cast_change_attr(object_t *op, object_t *caster, object_t *target, int dir, int spell_type);
object_t                    *cure_what_ails_you(object_t *op, uint8 st1);
int                         fire_arch(object_t *op, object_t *caster, sint16 x, sint16 y, int dir, archetype_t *at, int type, int level, int magic);
void                        check_fired_arch(object_t *op);
void                        move_fired_arch(object_t *op);
/* spell_util.c */
void                        init_spells(void);
void                        dump_spells(void);
int                         insert_spell_effect(char *archname, map_t *m, int x, int y);
spell                      *find_spell(int spelltype);
int                         check_spell_known(object_t *op, int spell_type);
void                        do_learn_spell(object_t *op, int spell);
void                        do_forget_spell(object_t *op, int spell);
int                         cast_spell(object_t *op, object_t *caster, int dir, int type, int ability, SpellTypeFrom item,
                                       char *stringarg);
int                         fire_bolt(object_t *op, object_t *caster, int dir, int type, int magic);
int                         cast_cone(object_t *op, object_t *caster, int dir, int strength, int spell_type,
                                      archetype_t *spell_arch, int level, int magic);
void                        check_cone_push(object_t *op);
void                        cone_drop(object_t *op);
void                        move_cone(object_t *op);
void                        forklightning(object_t *op, object_t *tmp);
int                         reflwall(msp_t *msp, object_t *sp_op);
void                        move_bolt(object_t *op);
void                        explode_object(object_t *op);
void                        drain_rod_charge(object_t *rod);
void                        fix_rod_speed(object_t *rod);
int                         find_target_for_spell(object_t *op, object_t *item, object_t **target, int dir, uint32 flags);
int                         can_see_monsterP(map_t *m, int x, int y, int dir);
int                         SP_level_strength_adjust(object_t *op, object_t *caster, int spell_type);
int                         SP_level_spellpoint_cost(object_t *op, object_t *caster, int spell_type);
int                         SP_lvl_dam_adjust(int level, int spell_type, int base_dam);
void                        SP_stat_adjust(object_t *caster, int spell, float *value);
int                         look_up_spell_by_name(object_t *op, const char *spname);
/* stats.c */
void                        stats_event(stats_event_type type, ...);
/* time.c */
/* pathfinder.c */
/* swamp.c */
extern object_t *sparkly_create(archetype_t *at, object_t *who, sint16 t, sint16 nr, uint8 stype);
extern void      sparkly_move(object_t *effect);

int command_level(object_t *op, char *params);

#endif /* ifndef __SPROTO_H */
