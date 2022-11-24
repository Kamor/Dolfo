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

#ifdef NO_ERRNO_H
extern int      errno;
#else
#   include <errno.h>
#endif

#ifdef sequent
/* stoopid sequent includes don't do this like they should */
extern char    *sys_errlist[];
extern int      sys_nerr;
#endif

/*  peterm: rune_attack
 *
 * function handles those runes which detonate but do not cast spells.
 * Remember: at this point we KNOW the trap will hit the victim -
 * so no need for wce.
 * all we need is dam and a attack form
 */
void rune_attack(object_t *op, object_t *victim)
{
    int dam = op->stats.dam; /* save damage */

    /* lets first calc the damage - we use base dmg * level
     * For rune, the damage will *not* get additional
     * level range boni
     * we do here a more simple system like the normal monster damage.
     * with the hard set float we can control the damage a bit better.
     */
    op->stats.dam = (sint16) ((float) dam * (LEVEL_DAMAGE(op->level) * 0.925f));

    if (victim)
    {
        tag_t   tag = victim->count;
        if (!op->inv || op->inv->type != DISEASE)
        {
            damage_ob(victim, op->stats.dam, op, ENV_ATTACK_CHECK);
            if (!OBJECT_VALID(victim, tag))
            {
                op->stats.dam = dam;
                return;
            }
        }
        /*  if there's a disease in the needle, put it in the player */
        if (op->randomitems != NULL)
            create_treasure_list(op->randomitems, op, 0, op->level ? op->level : victim->map->difficulty, ART_CHANCE_UNSET, 0);
        if (op->inv && op->inv->type == DISEASE)
        {
            object_t *disease = op->inv;
            infect_object(victim, disease, 1);
            remove_ob(disease);
            move_check_off(disease, NULL, MOVE_FLAG_VANISHED);
        }
    }
    else
    {
        tag_t tag = op->count;

        hit_map(op, MSP_KNOWN(op));

        if (!OBJECT_VALID(op, tag))
        {
            return;
        }
    }

    op->stats.dam = dam;
}

/* check a object marked with
 * FLAG_IS_TRAPED has still a known
 * trap in it!
 */
void set_traped_flag(object_t *op)
{
    object_t *tmp,
           *next;
    int     flag;

    if (!op)
        return;

    /* player & monsters are not marked */
    if (op->type == PLAYER || op->type == MONSTER)
        return;

    flag = QUERY_FLAG(op, FLAG_IS_TRAPED);

    FOREACH_OBJECT_IN_OBJECT(tmp, op, next)
    {
        /* must be a rune AND visible */
        if (tmp->type == RUNE && tmp->stats.Cha <= 1)
        {
            SET_FLAG(op, FLAG_IS_TRAPED);
            if (!flag)
                goto set_traped_view;
            return;
        }
    }

    CLEAR_FLAG(op, FLAG_IS_TRAPED); /* clean */
    if (!flag)
        return;

    set_traped_view:
    OBJECT_UPDATE_UPD(op, UPD_FLAGS);
}

/*  This function generalizes attacks by runes/traps.  This ought to make
    it possible for runes to attack from the inventory,
    it'll spring the trap on the victim.  */

void spring_trap(object_t *trap, object_t *victim)
{
    object_t *env;
    tag_t   trap_tag    = trap->count;

    if (!trap->stats.hp ||      // prevents recursion
        (GET_GMASTER_MODE(victim) & GMASTER_MODE_SA))
    {
        return;
    }

    if (trap->subtype == SUBTYPE_TRAP_HALLOWED_GROUND && spring_hallowed_ground(victim, trap))
    {
        return;
    }

    /* Only entities can trigger runes that don't cast spells, as
    * doing direct damage to a non-entity doesn't work anyway.
    * Typical example is an arrow attacking a door.
    */
    if (victim->type != PLAYER &&
        victim->type != MONSTER &&
        !trap->stats.sp)
        return;

    /* Event trigger and quick exit */
    if (plugin_trigger_object_event(PLUGIN_EVENT_OBJECT_TRIGGER, trap, victim, NULL, NULL, 0, 0, 0))
    {
        return;
    }

    if (victim && victim->type == PLAYER && trap->msg)
        ndi(NDI_UNIQUE, 0, victim, "%s", trap->msg);

    /* Flash an image of the trap on the map so the poor sod
    *  knows what hit him.
    */

    for (env = trap; env->env != NULL; env = env->env)
        ;

		// this logic didn't flash, this stays till corpse is removed from map
		// we don't want to show it anymore, we have output on textbox, icon for trapped containers should only be seen
		// if we spot a trap and not 10 minutes later on triggered traps
    // if(trap->subtype != SUBTYPE_TRAP_NO_SHOW)
    //    trap_show(trap, env);
#if 0
    trap->type = MISC_OBJECT;  /* make the trap impotent */
    CLEAR_FLAG(trap, FLAG_FLY_ON);
    CLEAR_FLAG(trap, FLAG_WALK_ON);
    SHSTR_FREE(trap->msg);
    trap->stats.food = 20;  /* make it stick around until its spells are gone */
    SET_FLAG(trap, FLAG_IS_USED_UP);    /* ok, let the trap wear off */
    trap->speed = trap->speed_left = 1.0f;
    update_ob_speed(trap);
#endif

    if (!trap->stats.sp)
    {
        if (trap->race)
        {
            int soundtype = 0;
            int soundid;

            sscanf(trap->race, "%d", &soundtype);

            if (trap->slaying && soundtype)
            {
                if ((soundid = lookup_sound(soundtype - 1, trap->slaying)) >= 0)
                {
                    // Use victim here because trap may be in a container
                    play_sound_map(MSP_KNOWN(victim), soundid, soundtype - 1);
                }
            }
        }

        rune_attack(trap, victim);
        set_traped_flag(env);

        if (!OBJECT_VALID(trap, trap_tag))
        {
            return;
        }
    }
    else
    {
        /* This is necessary if the trap is inside something else */
        remove_ob(trap);
        move_check_off(trap, NULL, MOVE_FLAG_VANISHED);
        set_traped_flag(env);
        trap->x = victim->x;trap->y = victim->y;
        if (!insert_ob_in_map(trap, victim->map, trap, 0))
        {
            return;
        }

        cast_spell(trap, trap, trap->direction, trap->stats.sp - 1, 1, spellNormal, NULL);
    }

    if (trap->stats.hp > 0)
        trap->stats.hp--;

    if (trap->stats.hp == 0)
    {
        trap->type = MISC_OBJECT;  /* make the trap impotent */
        CLEAR_FLAG(trap, FLAG_FLY_ON);
        CLEAR_FLAG(trap, FLAG_WALK_ON);
        SHSTR_FREE(trap->msg);
        trap->stats.food = (trap->stats.food) ? trap->stats.food : 20;  /* make it stick around until its spells are gone */
        SET_FLAG(trap, FLAG_IS_USED_UP);
        trap->speed = trap->speed_left = 1.0f;
        update_ob_speed(trap);
    }
}

// functions uses 2 different rolls, one for see trap against hiddeness of trap, other for id trap against trap level
int trap_see(object_t *op, object_t *trap, int level)
{
	  if (level<1) return 0; // savetycheck
		int see_trap = 0, id_trap = 0;

		// did we see trap?
				ndi(NDI_UNIQUE, 0, op, "Dex : %d", op->stats.Dex);
						ndi(NDI_UNIQUE, 0, op, "level : %d", level);
								ndi(NDI_UNIQUE, 0, op, "Cha : %d", trap->stats.Cha);
										ndi(NDI_UNIQUE, 0, op, "level : %d", trap->level);
													ndi(NDI_UNIQUE, 0, op, "dam : %d", trap->stats.dam);

		int chance = max(min(50 + op->stats.Dex + level - trap->stats.Cha, 100),0);
    int roll = RANDOM_ROLL(0, 99);
		ndi(NDI_UNIQUE, 0, op, "find trap : roll %d < chance %d)?", roll, chance);

		if (trap->stats.Cha<=1
    		|| roll<chance)
		{
			see_trap=1;
		}

		if (see_trap)
		{
			// did we id trap?
			chance = max(min(50 + op->stats.Dex + level - trap->level, 100),0);
			roll = RANDOM_ROLL(0, 99);
			ndi(NDI_UNIQUE, 0, op, "id trap : roll %d < chance %d)?", roll, chance);

			if (roll<chance)
			{
				id_trap=1;
				if (!QUERY_FLAG(trap, FLAG_IDENTIFIED))
				{
					SET_FLAG(trap, FLAG_IDENTIFIED);
				}
			}

			// we only calculate exp and give messages to players
			if (op->type == PLAYER)
			{
				int trap_exp = MAX(1, trap->stats.Cha) * MAX(1, trap->stats.dam);
				if (id_trap)
				{
					trap_exp = trap_exp + MAX(1, trap->level) * MAX(1, trap->stats.dam);
				  ndi(NDI_UNIQUE, 0, op, "You spot a %s (lvl %d)!", trap->name, trap->level);
				}
				else
				{
					ndi(NDI_UNIQUE, 0, op, "You spot a trap, but can't identify it!");
				}
				exp_adjust(CONTR(op), SK_FIND_TRAPS, trap_exp, EXP_FLAG_CAP );
			}
      return 1; // trap seen
    }
    return 0; // trap not seen
}


/* I changed the use of this function... Now, this function does what the
 * name says: He get a trap and makes them visible and reinsert it.
 * This can be used from trigger but
 * also from detection sources - just be sure you set FLAG_IS_USED_UP in
 * your trigger functions as speed when you want start the auto destroy. MT-2003
 */
int trap_show(object_t *trap, object_t *where)
{
    object_t *env;

    if (where == NULL || trap == NULL)
    {
        return 0;
    }

    /* Because we don't want unhide traps by using "see invisible" (because
     * traps are not invisibile - they are hidden) and the normal "see hidden"
     * we put them in a sys_object ... truely invisible for the client.
     * There are only 2 ways to make then visible - fire them by moving/apply them
     * or use a "see/unhide traps" skill/spell.
     * For that, we set sys_object 0 and layer 4. Don't forget to set the player
     * because level 0 is really a special thing.
     */
    env = trap->env;
    /* FIXME: Not entirely sure we really need do a full remove/insert any
     * more, but certainly careful msp_set_slices()/update_ob() OR
     * esrv_send_item(), depending on if trap->map or trap->env, is needed.
     *
     *
     * -- Smacky 20130228 */
    remove_ob(trap); /* we must remove and reinsert it.. */
    CLEAR_FLAG(trap, FLAG_SYS_OBJECT);
    CLEAR_MULTI_FLAG(trap, FLAG_IS_INVISIBLE);
    trap->stats.Cha = 1; // make it visible

    trap->layer = MSP_SLAYER_ITEMB;

    if (trap->arch != NULL && trap->arch->clone.layer != 0)
    {
        trap->layer = trap->arch->clone.layer;
    }

    if (env && env->type != PLAYER && env->type != MONSTER && env->type != TYPE_DOOR && !QUERY_FLAG(env, FLAG_NO_PASS))
    {
        SET_FLAG(env, FLAG_IS_TRAPED);
        OBJECT_UPDATE_UPD(env, UPD_FLAGS);
        insert_ob_in_ob(trap, env);
    }
    else
        insert_ob_in_map(trap, where->map, NULL, 0);
    return 1;
}

int trap_disarm(object_t *op, object_t *trap, int level)
{
	  if (level<1) return 0; // savetycheck
		
						ndi(NDI_UNIQUE, 0, op, "Dex : %d", op->stats.Dex);
						ndi(NDI_UNIQUE, 0, op, "level : %d", level);
								ndi(NDI_UNIQUE, 0, op, "Cha : %d", trap->stats.Cha);
										ndi(NDI_UNIQUE, 0, op, "level : %d", trap->level);
														ndi(NDI_UNIQUE, 0, op, "dam : %d", trap->stats.dam);

		if (trap->stats.Cha <= 1)
    {
			// todo this should be in show trap, this is redundant here. but to be save let keep it in here for the moment
      /*if (QUERY_FLAG(trap, FLAG_SYS_OBJECT) ||
          QUERY_FLAG(trap, FLAG_IS_INVISIBLE))
      {
        trap_show(trap, trap);
      }*/

			int id_trap = QUERY_FLAG(trap, FLAG_IDENTIFIED);

			int chance = max(min(id_trap?50:0 + op->stats.Dex + level - trap->level, 100),0);
			int roll = RANDOM_ROLL(0, 99);
			ndi(NDI_UNIQUE, 0, op, "disarm trap : roll %d < chance %d)?", roll, chance);

			if (roll<chance)
			{
				int trap_exp = MAX(1, trap->level) * MAX(1, trap->stats.dam);
				if (!id_trap)
				  trap_exp = trap_exp*2; // we grant double exp, if player managed to disarm an unidentified trap

			  ndi(NDI_UNIQUE, 0, op, "You successfuly remove the %s (lvl %d)!", trap->name, trap->level);
				
				object_t *env = trap->env;
        remove_ob(trap);
        move_check_off(trap, NULL, MOVE_FLAG_VANISHED); // todo do we need this? when we use remove_ob with or without this?
         // set_traped_flag(env);
				CLEAR_FLAG(env, FLAG_IS_TRAPED);
				exp_adjust(CONTR(op), SK_REMOVE_TRAP, trap_exp, EXP_FLAG_CAP );
				return 1;
			}
			ndi(NDI_UNIQUE, 0, op, "You fail to remove the %s (lvl %d).", trap->name, trap->level);
			CLEAR_FLAG(trap->env, FLAG_IS_TRAPED);
			spring_trap(trap, op);
		}
	return 0;
}

// trap adjusting (trap level and trap hiddenness) by map difficulty +/-20%
void trap_adjust(object_t *trap, int difficulty)
{
    if (difficulty < 1)
        difficulty = 1;

    int off = difficulty / 5;

    trap->level = RANDOM_ROLL(difficulty - off, difficulty + off);
    // if (trap->level < 1) trap->level = 1; // above formula can not reach <1

    // trap hiddenness (stats.Cha)
    trap->stats.Cha = RANDOM_ROLL(difficulty - off, difficulty + off);
    // if (trap->stats.Cha < 1) trap->stats.Cha = 1; // above formula can not reach <1
}
