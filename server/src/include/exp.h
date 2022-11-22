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

#ifndef __EXP_H
#define __EXP_H

// MAXLEVEL is defined in protocol.h -- it's 110
#define MAXMOBLEVEL 127 // must be >= MAXLEVEL

#define EXP_MAXEXP exp_threshold[MAXLEVEL]

/* The following seven values are categories (for players) by skill level.
 * These are provided to encourage a more consistent approach to when and what
 * players can do/have done to them. When a player's skill level <= the number
 * indicated (and implicitly > the category below), he is designated as that
 * category. */
#define EXP_NOOBLEV   4
#define EXP_VLLEV     9
#define EXP_LLEV     19
#define EXP_MLEV     49
#define EXP_HLEV     79
#define EXP_VHLEV   100
#define EXP_HEROLEV MAXLEVEL

/* EXP_FLAG_* are bits passed to exp_adjust() to fine tune aspects of its
 * behaviour. */
#define EXP_FLAG_CAP      (1 << 0) // cap exp gain on indirect skills
#define EXP_FLAG_NOSCRIPT (1 << 1) // do not run player script
#define EXP_FLAG_NO_BONUS (1 << 2) // avoid bonus calculation

/* EXP_ADJUST_CAP() caps the exp which a skill can gain or lose in a single
 * pass of exp_adjust(). */
#define EXP_ADJUST_CAP(_O_) \
    ((exp_threshold[(_O_)->level + 1] - exp_threshold[(_O_)->level]) / 4)

typedef struct _level_color
{
    int green;
    int blue;
    int yellow;
    int orange;
    int red;
    int purple;
} _level_color;

extern sint32       exp_threshold[MAXLEVEL + 2];
extern _level_color level_color[MAXMOBLEVEL + 1];

extern sint32           exp_adjust(player_t * pl, sint16 nr, sint32 exp, uint8 mask);
extern sint32           exp_death_penalty(player_t * pl);
extern float            calc_level_difference(int who_lvl, int op_lvl);
extern int              calc_skill_exp(object_t * who, object_t * op, float mod, int level, int *real);
extern int              exp_from_base_skill(player_t *pl, int base_exp, int sk);

#endif /* ifndef __EXP_H */
