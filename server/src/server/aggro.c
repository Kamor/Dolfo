/*
    Daimonin, the Massive Multiuser Online Role Playing Game
    Server Applicatiom

    Copyright (C) 2001-2005 Michael Toennies

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

/* aggro.c
 * In this module we check and maintain the "aggro/damage history" object.
 * In that object the server stores damage and "aggressive action"
 * information done to a object (normally a non player, living object).
 * This information is used to determinate their "aggro". The target of their
 * aggression is usually the target for attacks or other action.
 * Because a aggressive act is often and most times damage, the aggro history
 * is also used for DOT & AE damage to control the right DoT (damage over time)
 * to have a defined DPS (damage per second).
 */
#ifndef __AGGRO_C
#define __AGGRO_C
#endif

#include <global.h>

static uint32 exp_calc_tag=1; /* used to tag the player/group */

/*
 *    TODO: Quick access of this structures by a pointer initialized for players in fix_player and placed
 *  in the player struct.
 */

/*
 *    TODO: The code don't handle pet damage itself - its redirected to players.
 *  i played around a bit but i think its better to use a special "pet damage"
 *  history and adding it at the same time to the owner. Using 2 values:
 *  one is the "real" damage we use for exp, one is the "counted" exp we use for aggro.
 */

/* aggro_get_damage()
 * Get a damage object from target for damage source hitter.
 * This is used for AoE spells and other target synchronized damage dealers
 * Return: NULL or damage info object for hitter.
 */
object_t *aggro_get_damage(object_t *target, object_t *hitter)
{
    object_t *this,
           *next;

    /* damage objects are in a 2nd aggro_history object inside the base aggro history. */
    FOREACH_OBJECT_IN_OBJECT(this, target, next)
    {
        if (this->type == TYPE_AGGRO_HISTORY)
        {
            FOREACH_OBJECT_IN_OBJECT(this, this, next)
            {
                if (this->type == TYPE_AGGRO_HISTORY)
                {
                    FOREACH_OBJECT_IN_OBJECT(this, this, next) /* here are the damage infos */
                    {
                        if (this->type == TYPE_DAMAGE_INFO)
                        {
                            if (this->weight_limit == hitter->weight_limit)
                            {
                                return this;
                            }
                            /* because we have this sucker accessed...lets do some gc on the fly */
                            else if (this->damage_round_tag + DEFAULT_DMG_INVALID_TIME < ROUND_TAG)
                            {
                                remove_ob(this);
                            }
                        }
                    }

                    break;
                }
            }

            break;
        }
    }

    return this;
}

/*
 *
 */
object_t *aggro_insert_damage(object_t *target, object_t *hitter)
{
    object_t *tmp_t,
           *next,
           *tmp;

    /* find or insert base container */
    FOREACH_OBJECT_IN_OBJECT(tmp_t, target, next)
    {
        if(tmp_t->type == TYPE_AGGRO_HISTORY )
            break;
    }

    if(!tmp_t)
        tmp_t = insert_ob_in_ob(arch_to_object(archetype_global._aggro_history), target);

    /* find or insert 2nd damage info container */
    FOREACH_OBJECT_IN_OBJECT(tmp, tmp_t, next)
    {
        if(tmp->type == TYPE_AGGRO_HISTORY)
            break;
    }

    if(!tmp)
        tmp = insert_ob_in_ob(arch_to_object(archetype_global._aggro_history),  tmp_t);

    tmp = insert_ob_in_ob(arch_to_object(archetype_global._dmg_info), tmp);

    /* damage source is hitter and tick global_round_tag */
    tmp->weight_limit = hitter->weight_limit;
    /*LOG(llevNoLog,"add dmg: %x\n", hitter->weight_limit); */
    tmp->damage_round_tag = ROUND_TAG;

    return tmp;
}


/*
 * aggro_update_info()
 * This is the main setting function to update for a target the damage & aggro marker AFTER aggro and damage
 * is done from the hitter to the
 */
object_t *aggro_update_info(object_t *target, object_t *hitter, object_t *hitter_owner, int dmg)
{
    object_t *history,
           *next,
           *aggro = NULL,
           *tmp;
    int skill_nr = 0;

    /* no legal hitter, no need for aggro.
     * TODO: perhaps we will add a kind of "neutral" damage
     * but atm a player will get full exp when killing a damaged target.
     * lets give the players some gifts.
     */

    if(hitter && hitter->chosen_skill)
        skill_nr = hitter->chosen_skill->stats.sp;
    else if(hitter_owner && hitter_owner->chosen_skill)
        skill_nr = hitter_owner->chosen_skill->stats.sp;

    /* debug...
    if(hitter && hitter->chosen_skill)
        ndi( NDI_UNIQUE, 0, hitter->type!=PLAYER?hitter_owner:hitter, "SKILL-hitter: %s (%d)",
        hitter->chosen_skill->name, hitter->chosen_skill->stats.sp);
    if(hitter_owner && hitter_owner->chosen_skill)
        ndi( NDI_UNIQUE, 0, hitter->type!=PLAYER?hitter_owner:hitter, "SKILL-howner: %s (%d)",
        hitter_owner->chosen_skill->name, hitter_owner->chosen_skill->stats.sp);
    */

    if(hitter_owner && (hitter_owner==hitter))
        hitter_owner = NULL;
    if(!hitter && !hitter_owner)
        return NULL;

    /* This is the interface to the AI system. At least for now =) */
    if(target->type == MONSTER)
    {
        /* Update hittee's friendship level towards hitter */
        object_t                 *root_hitter = hitter_owner ? hitter_owner : hitter;
        struct mob_known_obj   *enemy       = update_npc_knowledge(target, root_hitter, NULL, -dmg, 0);

        /* Attacking someone neutral always makes you an enemy (for now) */
        if (enemy && enemy->friendship > FRIENDSHIP_ATTACK && dmg > 0)
            enemy->friendship += FRIENDSHIP_ATTACK;
    }

    /* get or create a aggro history container */
    FOREACH_OBJECT_IN_OBJECT(history, target, next)
    {
        if(history->type == TYPE_AGGRO_HISTORY)
            break;
    }
    if(!history)
        history = insert_ob_in_ob(arch_to_object(archetype_global._aggro_history), target);

    if(hitter_owner)
        hitter=hitter_owner; /* TODO: change when we do pet/owner handling */

    if(hitter)
    {
        /* check for aggro history of this object_t */
        FOREACH_OBJECT_IN_OBJECT(aggro, history, next)
        {
            if(aggro->type == TYPE_DAMAGE_INFO)
            {
                if(aggro->enemy_count == hitter->count)
                    break;
                if(aggro->damage_round_tag+DEFAULT_DMG_INVALID_TIME < ROUND_TAG)
                    remove_ob(aggro);
            }
        }

        if(aggro)
        {
            aggro->stats.hp += dmg;
            /* TODO: set flags for some specials */
        }
        else
        {
            aggro = insert_ob_in_ob(arch_to_object(archetype_global._dmg_info), history);
            aggro->enemy_count = hitter->count;    /* tag so we can identify the hitter */
            aggro->enemy = hitter;    /* so we can find later this damage dealer */
            aggro->stats.hp = dmg;
            aggro->update_tag = ROUND_TAG; /* using this we can determinate who does first damage */
            if(hitter->type == PLAYER)
                aggro->last_sp = PLAYER;
        }
        aggro->damage_round_tag = ROUND_TAG; /* last time this hitter does some = right now */

        /* for players, we want to store the used skills */
        if(hitter->type == PLAYER)
        {
            FOREACH_OBJECT_IN_OBJECT(tmp, aggro, next)
            {
                if(tmp->type == TYPE_DAMAGE_INFO && tmp->last_heal == skill_nr)
                    break;
            }
            if(!tmp)
            {
                tmp = insert_ob_in_ob(arch_to_object(archetype_global._dmg_info), aggro);
                tmp->last_heal = skill_nr;
                tmp->stats.hp = dmg;
                tmp->last_sp = PLAYER;
            }
            else
                tmp->stats.hp += dmg;
        }
    }

    /* TODO - handle target_owner. Lets say we attack a pet - thats target.
     * Then we need to tell target_owner whats going on and mark hitter/hitter_object as
     * bad guy here.
     */

    /* TODO: init history container object with master aggro holder */

    return aggro;
}

static int give_default_guild_exp(player_t *pl, int base_exp)
{
    object_t *skill;
    int e1 = 0;
    int e2 = 0;
    int e3 = 0;

    if (pl->base_skill_group[0] >= 0 &&
        (skill = pl->highest_skill[pl->base_skill_group[0]]) &&
        skill->level > 1)
    {
        e1 = (int) ((float) base_exp * 0.55f);
        e1 = exp_from_base_skill(pl, e1, skill->stats.sp);
        exp_adjust(pl, skill->stats.sp, e1, EXP_FLAG_CAP);
    }

    if (pl->base_skill_group[1] >= 0 &&
        (skill = pl->highest_skill[pl->base_skill_group[1]]) &&
        skill->level > 1)
    {
        e2 = (int) ((float) base_exp * 0.30f);
        e2 = exp_from_base_skill(pl, e2, skill->stats.sp);
        exp_adjust(pl, skill->stats.sp, e2, EXP_FLAG_CAP);
    }

    if (pl->base_skill_group[2] >= 0 &&
        (skill = pl->highest_skill[pl->base_skill_group[2]]) &&
        skill->level > 1)
    {
        e3 = (int) ((float) base_exp * 0.15f);
        e3 = exp_from_base_skill(pl, e3, skill->stats.sp);
        exp_adjust(pl, skill->stats.sp, e3, EXP_FLAG_CAP);
    }

    if (e1 > 0 || e2 > 0 || e3 > 0)
    {
        ndi(NDI_UNIQUE | NDI_WHITE, 0, pl->ob, "You didn't fight this time.\nYou trained your default guild skills.");
    }

    return 1;
}

// we have a full history, where monsters save all hits
// we need to calc the max inflicted damage and can calc the % for each skill
// currently we use 2 loops to count first the total damage.
// for groups the 1 loop to calc max infliceted damage is outside in aggro_exp_group

// aggro is history of highest_hitter
static inline int aggro_exp_single(object_t *victim, object_t *aggro, int total_damage)
{
	object_t *hitter = aggro->enemy;
	player_t *pl = CONTR(hitter);

	ndi(NDI_UNIQUE | NDI_RED, 0, pl->ob, "function : aggro_exp_single");
	ndi(NDI_UNIQUE | NDI_BLUE, 0, pl->ob, "TOTAL group damage = %d", total_damage);

	// check kill quests for active players, check for passiv group players is in aggro_exp_group
	if(pl->quests_type_kill && pl->quests_type_kill->inv)
		check_kill_quest_event(hitter, victim);

	object_t *index,
		*next;

		// we only need to count for solo players, in group we get the total_damage from aggro_exp_group
	if (total_damage<0) // perhaps we should use -1 here, but how could a group get the kill with 0 damage?
	{
		// loop the full aggro object / exp_calc_obj
		total_damage = 0; // we count full damage on enemy
		FOREACH_OBJECT_IN_OBJECT(index, aggro, next)
		{
			if(!pl->skill_ptr[index->last_heal]) // savety check, target got hit with a skill which doesn't exists, can this happen?
				continue;
			total_damage+=index->stats.hp; // inflicted damage with this skill
		}
	}

	ndi(NDI_UNIQUE | NDI_YELLOW, 0, pl->ob, "TOTAL damage player or group on enemy %d", total_damage);

	int given_exp = 0;
	// savety check if total is 0, we don't need loop, also don't want division by zero errors
	if (total_damage>0)
	{
		// now we can calculate the % for EACH skill
		FOREACH_OBJECT_IN_OBJECT(index, aggro, next)
		{
			if(!pl->skill_ptr[index->last_heal]) // savety check, target got hit with a skill which doesn't exists, can this happen?
				continue;

			ndi(NDI_UNIQUE | NDI_WHITE, 0, pl->ob, "skill number %d", index->last_heal);
			ndi(NDI_UNIQUE | NDI_YELLOW, 0, pl->ob, "skill damage %d", index->stats.hp);

			// here we can insert our server_exp_factor, but this is ugly here
			// server_exp_factor and level adjustment should be in create process of monster, not here TODO
			float factor = 1.00f / total_damage * index->stats.hp;
			ndi(NDI_UNIQUE | NDI_PURPLE, 0, pl->ob, "skill factor %f", factor);

			// we can also call this function with factor here instead using factor in exp_adjust call
			int exp = calc_skill_exp(pl->ob, victim, 1.0f, pl->skill_ptr[index->last_heal]->level, NULL); //&exp

			exp=(int)(factor*exp);
			ndi(NDI_UNIQUE | NDI_GREEN, 0, pl->ob, "exp for this skill %d", exp);

			// calculate base exp bonus (guilds). if there is one for this skill
			exp = exp_from_base_skill(pl, exp, index->last_heal);
			ndi(NDI_UNIQUE | NDI_GREEN, 0, pl->ob, "exp with base bonus %d", exp);

			given_exp+=exp_adjust(pl, index->last_heal, (int)(exp), EXP_FLAG_CAP);
		}
	}

	// if *all* possible skill exp has been zero because mob was to low - drop a message
	if (given_exp==0)
	{
		ndi(NDI_UNIQUE | NDI_GREY, 0, pl->ob, "Your enemy was too low for exp.");
	}
	return given_exp;
}

// clone of in_group_exp_range, comparing only two objects.
// so it's enough to compare a player if he is in range to target
// don't need additionally check if he is in range to groupmembers with highest aggro
static inline int in_range(object_t *victim, object_t *member)
{
  int i;
  map_t *tmp_map, *map = member->map;

  /* we do 2 tests: Is member on the map or a DIRECT attached map
  * from victim or hitter? If not - no exp.
  */

  /* some sanity checks... */
  if(QUERY_FLAG(victim,FLAG_REMOVED) )
  {
    LOG(llevDebug,"in_group_exp_range(): victim %s is removed!\n", STRING_OBJ_NAME(victim));
    return 0;
  }

  if(QUERY_FLAG(member,FLAG_REMOVED) ) /* secure... */
  {
    LOG(llevDebug,"in_group_exp_range(): member %s is removed!\n", STRING_OBJ_NAME(member));
    return 0;
  }

  /* quick check the easiest cases */
  if (map == victim->map)
  {
#ifdef DEBUG_AGGRO
    LOG(llevNoLog,"->%s on same map as victim\n", STRING_OBJ_NAME(member));
#endif
    return 1;
  }


  for (tmp_map = victim->map, i = 0; i < TILING_DIRECTION_NROF; i++)
  {
    if (tmp_map->tiling.tile_map[i] == map)
    {
#ifdef DEBUG_AGGRO
      LOG(llevNoLog,"->%s on attached map from victim!\n", STRING_OBJ_NAME(member));
#endif
      return 1;
    }
  }

#ifdef DEBUG_AGGRO
  LOG(llevNoLog,"->%s is out of range!\n", STRING_OBJ_NAME(member));
#endif

  /* don't give this group member quest items from victim */
  CONTR(member)->group_mask |= GROUP_BIT_NOQUEST;
  return 0;
}
/* calc exp for a group
 * CONTR() will access here always players
 */

// aggro history
// object_t *hitter = aggro->enemy; // object with the highest aggro
// player_t *pl = CONTR(hitter); // player with the highest aggro
// object_t *leader = pl->group_leader; // leader object of group
// for(tmp=leader;tmp;tmp=CONTR(tmp)->group_next) // loop all group members
// pl->exp_calc_obj // aggro history limited to this player
static inline int aggro_exp_group(object_t *victim, object_t *aggro)
{
	object_t *leader = CONTR(aggro->enemy)->group_leader;
	object_t *high = leader, *tmp;

	// TODO we need to count total damage on target, also from non group members
	// but first make it "easier" and "only" count damage from all group members
	int total_damage = 0; // we count full damage from our group on enemy
	for(tmp=leader;tmp;tmp=CONTR(tmp)->group_next)
	{
		player_t *pl = CONTR(tmp);
		object_t *index, *next;

		FOREACH_OBJECT_IN_OBJECT(index, pl->exp_calc_obj, next) // we need the tag check?
		{
			if(!pl->skill_ptr[index->last_heal]) // savety check, target got hit with a skill which doesn't exists, can this happen?
				continue;
			total_damage+=index->stats.hp; // inflicted damage with this skill
		}
	}

	// now loop again and call aggro_exp_single for each group member
	for(tmp=leader;tmp;tmp=CONTR(tmp)->group_next)
	{
		player_t *pl = CONTR(tmp);
		if(pl->exp_calc_tag == exp_calc_tag) // this check prevents from crash? or players on same maps do this?
		{
			aggro_exp_single(victim, pl->exp_calc_obj, total_damage);
		}
		else
		{
		  ndi(NDI_UNIQUE | NDI_YELLOW, 0, pl->ob, "PASSIV PLAYER");
			if (in_range(victim, pl->ob))
			{
				ndi(NDI_UNIQUE | NDI_YELLOW, 0, pl->ob, "DISTANCE CHECK");
				// check kill quests
				if(pl->quests_type_kill && pl->quests_type_kill->inv)
					check_kill_quest_event(pl->ob, victim);
			}
			// we can still do this here, if we found no better solutions for our healers behind the lines
			// give_default_guild_exp(pl, (int)((float)exp * 0.75f));
		}
	}
	return 1;
}

/*
 *  Analyze all aggro info in this object and give player exp basing on this info.
 *    Well, if we ever merge libcross.a with the server we should merge all in one exp.c module
 *  If slayer is != NULL we use it to determinate we have kill steal or a NPC kill.
 *  We decide here what we will do in that cases.
 *  Return: The corpse owner (NULL: There is no owner, target was to low, NPC kill...)
 */
object_t *aggro_calculate_exp(object_t *victim, object_t *slayer)
{
    object_t *tmp,
           *tmp3,
           *history,
           *next,
           *highest_hitter = NULL;
    int ret, total_dmg=0,total_dmg_all=0, highest_dmg;

    /* slayer is not a player (if the kill hitter was a pet, slayer was set to owner) */
//    if(slayer && slayer->type != PLAYER)
//        return;

    /* TODO: don't give exp for player pets - lets add a "iam a pet" flag here later */

    FOREACH_OBJECT_IN_OBJECT(history, victim, next)
    {
        if(history->type == TYPE_AGGRO_HISTORY )
            break;
    }
    if(!history)
        return NULL;

    /* count damage, ignore non player damage.
     * we are fair - if helped us a NPC, we ignored
     * their damage here.
    */
    exp_calc_tag++; /* increase tag counter */

    /* TODO: lets look we have a kill steal here. */
    if(slayer)
    {
    }

    /* lets sort out every illegal damage which would count */
    FOREACH_OBJECT_IN_OBJECT(tmp, history, next)
    {
        total_dmg_all += tmp->stats.hp;
        /* remove illegal enemy pointer and/or non player dmg */
        if(tmp->type != TYPE_DAMAGE_INFO || tmp->damage_round_tag+DEFAULT_DMG_INVALID_TIME < ROUND_TAG ||
                          !tmp->enemy || tmp->enemy->count!=tmp->enemy_count /*|| tmp->enemy->type!=PLAYER*/)
            remove_ob(tmp);
        else
            total_dmg += tmp->stats.hp;
    }
#ifdef DEBUG_AGGRO
    LOG(llevNoLog,"%s (%d) KILLED (%d). All dmg: %d  - player dmg: %d \n", STRING_OBJ_NAME(victim), victim->count, history->stats.hp, total_dmg_all, total_dmg);
#endif
    highest_dmg = -1;
    /* now run through the dmg left and give all their share of the exp */
    while(history->inv)
    {
        tmp = history->inv;
#ifdef DEBUG_AGGRO
        LOG(llevNoLog,"--> %s [%s] (%x)--> dmg done: %d\n", STRING_OBJ_NAME(tmp->enemy),tmp->last_sp == PLAYER?"player":"non player",tmp->enemy->count, tmp->stats.hp );
#endif
        if(tmp->enemy && tmp->enemy->type == PLAYER) /* player? */
        {
            if(CONTR(tmp->enemy)->group_id <= 0) /* single player? */
            {
                if(tmp->stats.hp > highest_dmg)
                {
                    highest_dmg = tmp->stats.hp;
                    highest_hitter = tmp;
                }
                remove_ob(tmp);
            }
            else /* group */
            {
                sint32 g_id = CONTR(tmp->enemy)->group_id;
                sint32 g_dmg = tmp->stats.hp;

                /* i *love* our object system */
                CONTR(tmp->enemy)->exp_calc_tag = exp_calc_tag; /* marks this group member as active for this exp gain */
                CONTR(tmp->enemy)->exp_calc_obj = tmp; /* thats the related aggro/dmg history object as shortcut */
                remove_ob(tmp);

                /* count grp dmg at once.
                 * we can safely remove the group members - we still can
                 * access them over the group links
                 */
                FOREACH_OBJECT_IN_OBJECT(tmp3, history, next)
                {
                    if(CONTR(tmp3->enemy) && CONTR(tmp3->enemy)->group_id == g_id)
                    {
                        g_dmg+=tmp3->stats.hp;
                        CONTR(tmp3->enemy)->exp_calc_tag = exp_calc_tag;
                        CONTR(tmp3->enemy)->exp_calc_obj = tmp3;
                        remove_ob(tmp3);
                    }
                }

                if(g_dmg > highest_dmg)
                {
                    highest_dmg = g_dmg;
                    highest_hitter = tmp;
                }
            }
        }
        else /* non player */
        {
            if(tmp->stats.hp > highest_dmg)
            {
                highest_dmg = tmp->stats.hp;
                highest_hitter = tmp;
            }
            remove_ob(tmp);
        }
    }
    if(!highest_hitter) /* funny situation: A shot arrow to C, B killed A, arrow killed C - slayer A is dead = no highest_hitter */
        return NULL;

#ifdef DEBUG_AGGRO
    LOG(llevNoLog," -> highest_hitter: %s ", STRING_OBJ_NAME(highest_hitter->enemy));
#endif

    /* we have a winner... highest_hitter is now a non player, single player or a group */
    if(!highest_hitter->enemy || highest_hitter->enemy->type!=PLAYER) /* NPC kill - no exp, no loot */
    {
#ifdef DEBUG_AGGRO
        LOG(llevNoLog,"--> NPC kill.\nend.\n");
#endif
        return highest_hitter->enemy;
    }

    if(CONTR(highest_hitter->enemy)->group_id <= 0)
        ret = aggro_exp_single(victim, highest_hitter, -1);
    else
        ret = aggro_exp_group(victim, highest_hitter);
#ifdef DEBUG_AGGRO
    LOG(llevNoLog,"end.\n");
#endif

    /* be sure not to drop items */
    if(ret == 0)
        SET_FLAG(victim, FLAG_NO_DROP);

    return highest_hitter->enemy; /* used to create the corpse bounty */
}
