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

#ifndef __DAIMONIN_GAME_H
#define __DAIMONIN_GAME_H

/* First the required header files - only the CF module interface and Lua */
#include <plugin_lua.h>

/* Game methods */
static int  Game_CreateObject(lua_State *L);
static int  Game_EnumerateCoins(lua_State *L);
static int  Game_FindPlayer(lua_State *L);
static int  Game_GetSpellNr(lua_State *L);
static int  Game_GetSkillNr(lua_State *L);
static int  Game_GetExpThreshold(lua_State *L);
static int  Game_GetTimeAndDate(lua_State *L);
static int  Game_IsValid(lua_State *L);
static int  Game_LoadObject(lua_State *L);
static int  Game_LocateBeacon(lua_State *L);
static int  Game_Log(lua_State *L);
static int  Game_PrintTimeAndDate(lua_State *L);
static int  Game_ReadyMap(lua_State *L);
static int  Game_ServerTickDiff(lua_State *L);
static int  Game_UpgradeApartment(lua_State *L);
static int  Game_Write(lua_State *L);

#endif /* ifndef__DAIMONIN_GAME_H */
