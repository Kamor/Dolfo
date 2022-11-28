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

/* This Table is sorted in 64er blocks of the same base material, defined in
materialtype. Entrys, used for random selections should start from down of
a table section. Unique materials should start from above the 64 block down.
The M_RANDOM_xx value will always counted from down. */

/* this IS extrem ugly - i will move it ASAP to a data file, which can be used
* from editor too!
*/
material_real_struct    material_real[NROFMATERIALS *NROFMATERIALS_REAL + 1]    =
{
    /* undefined Material - for stuff we don't need material information about */
    {"", 100,100,       0,0,0,      M_NONE,         RACE_TYPE_NONE},

        /* PAPERS */
    {"paper ",         90,80,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,81,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,82,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,83,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,84,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,85,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,86,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,87,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"paper ",         90,88,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"rice paper ",    90,89,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"parchment ",    100,90,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"parchment ",    100,91,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"parchment ",    100,92,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"vellum ",       100,93,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"demon vellum ", 100,94,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"papyrus ",      100,95,       0,0,0,      M_PAPER,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* IRON (= Metal) (65) */
    {"iron ",                   100,70,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"hardened iron ",           95,72,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"forged iron ",             90,74,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"black iron ",              80,76,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"shear iron ",              70,78,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"steel ",                   60,80,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"hardened steel ",          50,82,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"forged steel ",            45,84,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"shear steel ",             40,86,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"diamant steel ",           40,88,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"darksteel ",               35,90,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"forged darksteel ",        30,92,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"silksteel ",               25,94,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"forged silksteel ",        25,96,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"meteoric steel ",          20,98,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"forged meteoric steel ",   20,100,   0,0,0,    M_IRON,    RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* Crystals/breakable/glass (129) */
    {"glass ",      100,80,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 129 */
    {"zircon ",     100,80,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 130 */
    {"pearl ",       75,83,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 131 */
    {"emerald ",     75,85,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 132 */
    {"sapphire ",    50,92,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 133 */
    {"ruby ",        30,93,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 134 */
    {"diamond ",     10,95,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 135 */
    {"jasper ",      75,80,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 136 */
    {"jade ",        75,80,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 137 */
    {"aquamarine ",  80,80,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 138 */
    {"opal ",        20,90,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 139 */
    {"amethyst ",    25,88,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 140 */
    {"amber ",       30,88,       0,0,0,    M_GLASS, RACE_TYPE_NONE}, /* 141 */
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* LEATHER (193) */
    {"soft leather ",         100,40,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"hardened leather ",      95,42,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"stone leather ",         90,44,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"dark leather ",          80,48,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"silk leather ",          70,50,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"wyvern leather ",        60,52,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"ankheg leather ",        50,54,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"griffon leather ",       45,56,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"hellcat leather ",       40,58,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"gargoyle leather ",      35,60,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"diamant leather ",       35,62,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"chimera leather ",       30,64,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"manticore leather ",     25,66,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"cockatrice leather ",    20,68,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"basilisk leather ",      20,70,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"dragon leather ",        20,72,    0,0,0,    M_LEATHER,   RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* WOOD (257) */
    {"pine ",        100,40,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"spruce ",       95,42,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"cedar ",        90,44,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"ash ",          85,46,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"yew ",          80,48,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"birch ",        75,50,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"hickory ",      70,52,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"cherry ",       65,54,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"walnut ",       60,56,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"maple ",        55,58,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"elm ",          50,60,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"red elm ",      45,62,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"rosewood ",     40,64,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"pernambuco ",   35,66,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"white oak ",    30,68,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"red oak ",      20,70,       0,0,0,       M_WOOD,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* ORGANIC (321) */
    {"organic ",       100,80,       0,0,0,      M_ORGANIC,       RACE_TYPE_NONE}, /* 321 used for misc organics */
    {"dragon ",      50,96,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE}, /* 322 */
    {"chitin "              , 50,45,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE}, /* 323 */
    {"scale "               , 50,55,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 324 */
    {"white dragonscale "   , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 325 */
    {"blue dragonscale "    , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 326 */
    {"red dragonscale "     , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 327 */
    {"yellow dragonscale "  , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 328 */
    {"grey dragonscale "    , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 329 */
    {"black dragonscale "   , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 330 */
    {"orange dragonscale "  , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 331 */
    {"green dragonscale "   , 10,65,       0,0,0,      M_ORGANIC,         RACE_TYPE_NONE},/* 332 */
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* STONE (385) */
    {"flint ",      100,40,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"greenstone ",  95,43,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"basalt ",      90,46,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"limestone ",   85,49,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"shale ",       80,52,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"gypsum ",      75,55,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"marble ",      70,58,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"granite ",     60,61,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"quartz ",      50,64,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"obsidian ",    40,67,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"brimstone ",   30,70,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"bloodstone ",  20,73,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"pyrite ",      20,76,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"lignite ",     20,79,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"cinnabar ",    20,82,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"phosphate ",   20,85,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"galena ",      20,95,       0,0,0,      M_STONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* CLOTH (449) */
    {"wool ",          100,10,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"linen ",          90,12,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"rat hair ",       85,14,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"dog hair ",       80,16,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"cat hair ",       75,18,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"mugwump hair ",   70,20,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"wildcat hair ",   65,22,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"wolf hair ",      60,24,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"horse hair ",     55,26,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"giant hair ",     50,28,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"minotaur hair ",  40,30,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"worm silk ",      30,32,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"spider silk ",    25,34,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"angel hair ",     20,36,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"unicorn hair ",   15,38,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"elven hair ",     10,40,       0,0,0,      M_CLOTH,       RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* ADAMANT (= magic metals) (513) */
    {"magic silk ",  1,65,       0,0,0,      M_ADAMANT,         RACE_TYPE_NONE},
    {"mithril ",     1,65,       0,0,0,      M_ADAMANT,         RACE_TYPE_NONE},
    {"adamant ",     10,65,      0,0,0,      M_ADAMANT,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* Liquid (577) */
    /* some like ice... the kind of liquid/potions in game don't depend
    * or even handle the liquid base type*/
    {"",       100,80,       0,0,0,      M_LIQUID,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* Soft Metal (641) */
    {"tin ",         100,80,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 641 */
    {"brass ",       100,80,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 642 */
    {"copper ",      100,80,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 643 */
    {"bronze ",      100,80,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 644 */
    {"silver ",      50, 90,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 645 */
    {"gold ",        20, 95,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 646 */
    {"platinum ",    10, 99,       0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 647 */
    {"lead ",        100, 80,      0,0,0,   M_SOFT_METAL,   RACE_TYPE_NONE}, /* 648 */
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* Bone (705) */
    {"",            100,40,       0,0,0,      M_BONE,         RACE_TYPE_NONE}, /* for misc bones*/
    {"human ",      100,43,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"elven ",      100,46,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"dwarven ",    100,49,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"rat ",        100,52,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"cat ",        100,55,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"dog ",        100,58,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"kobold ",     100,61,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"ogre ",       100,64,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"orc ",        100,67,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"gnoll ",      100,70,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"giant ",      100,73,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"troll ",      100,76,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"minotaur ",   100,79,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"dragon ",     100,82,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"demon ",      100,85,       0,0,0,      M_NONE,         RACE_TYPE_NONE},

    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    /* Ice (769) */
    /* not sure about the sense to put here different elements in...*/
    {"",         100,80,       0,0,0,      M_ICE,         RACE_TYPE_NONE}, /* water */
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
    {"", 0,0,       0,0,0,      M_NONE,         RACE_TYPE_NONE},
};

materialtype            material[NROFMATERIALS]                             =
{
    /*              P  M  F  E  C  C  A  D  W  G  P S P T F  C D D C C G H B  I *
    *               H  A  I  L  O  O  C  R  E  H  O L A U E  A E E H O O O L  N *
    *               Y  G  R  E  L  N  I  A  A  O  I O R R A  N P A A U D L I  T *
    *               S  I  E  C  D  F  D  I  P  S  S W A N R  C L T O N   Y N  R *
    *               I  C     T     U     N  O  T  O   L      E E H S T P   D  N */
    {"paper",     {15,10,17, 9, 5, 7,13, 0,20,15, 0,0,0,0,0,10,0,0,0,0,0,0,0,0}},
    {"metal",     { 2,12, 3,12, 2,10, 7, 0,20,15, 0,0,0,0,0,10,0,0,0,0,0,0,0,0}},
    {"crystal",   {14,11, 8, 3,10, 5, 1, 0,20,15, 0,0,0,0,0, 0,0,0,0,0,0,0,0,0}},
    {"leather",   { 5,10,10, 3, 3,10,10, 0,20,15, 0,0,0,0,0,12,0,0,0,0,0,0,0,0}},
    {"wood",      {10,11,13, 2, 2,10, 9, 0,20,15, 0,0,0,0,0,12,0,0,0,0,0,0,0,0}},
    {"organics",  { 3,12, 9,11, 3,10, 9, 0,20,15, 0,0,0,0,0, 0,0,0,0,0,0,0,0,0}},
    {"stone",     { 2, 5, 2, 2, 2, 2, 1, 0,20,15, 0,0,0,0,0, 5,0,0,0,0,0,0,0,0}},
    {"cloth",     {14,11,13, 4, 4, 5,10, 0,20,15, 0,0,0,0,0, 5,0,0,0,0,0,0,0,0}},
    {"magic material",    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0,0,0,0, 0,0,0,0,0,0,0,0,0}},
    {"liquid",    { 0, 8, 9, 6,17, 0,15, 0,20,15,12,0,0,0,0,11,0,0,0,0,0,0,0,0}},
    {"soft metal",{ 6,12, 6,14, 2,10, 1, 0,20,15, 0,0,0,0,0,10,0,0,0,0,0,0,0,0}},
    {"bone",      {10, 9, 4, 5, 3,10,10, 0,20,15, 0,0,0,0,0, 2,0,0,0,0,0,0,0,0}},
    {"ice",       {14,11,16, 5, 0, 5, 6, 0,20,15, 0,0,0,0,0, 7,0,0,0,0,0,0,0,0}}
};


// num chances to damage more than one item is not used here
// instead we use it to send dmg (damage) from outside
// we use this num to overwrite chance, ugly TODO
// imunities are deactivated, only check for weapon is in base
// see comments in material_attack_damageOLD
// droping items to the ground on condition 0 is also deactivated
// such code, mostly never used by players is nice to have as a kind of secret unknown game behaviour
// but also blows up the code, i deactivated it for the moment here, it is also bugged code, see below
// the imunity system is not really good implemented in old code, see there, i have removed it here
void material_attack_damage(object_t *op, int num, int chance, int base)
{
  object_t *item;
  player_t *pl;
  int slot;
  if(op->type != PLAYER || !(pl = CONTR(op))) return;

		//ndi(NDI_UNIQUE, 0, op, "DAMAGE = %d", num);

		// thats tricky now, more damage hitter has, more risk should be to damage gear
		// we don't want each hit break our gear, so we need random
		// original idea was 2%+xacid+xmagicweapon against 100 roll
		// my idea is against quality roll
		// my idea is to increase the chance by damage
		// but 1 damage should have a chance to damage gear somehow
		// thinking of a troll hitting 1000 on a quality 1 item
		// this would be a 1000 roll against 1 ?
		// this means quality is better adjusted in higher ranges 1-1000 to compensation incoming damage?
		// but for the moment, lets keep quality in range 1-100 and use a *10 modificator for it
		// so one roll is rnd%damage, where the other is rnd%quality*10
		// this means a quality 100 have a 1000 roll, where a hitting 1 damage ant has a 1 roll
		// so its really hard for such a weak ant to make item_damage on such a high quality item

		// todo this is ugly and only temporary
		chance=num;

		// if he have HIT_FLAG_WEAPON, we pick the weapon slot, not a random slot
		if(base & HIT_FLAG_WEAPON)
		{
			slot=PLAYER_EQUIP_WEAPON1; // weaponslot
		}
		else
		{
    slot = RANDOM() % PLAYER_EQUIP_MAX; // get a random slot
    // todo slots could have different chances, like body slot gets the most hits?
    // perhaps also a forced slot from outside, like aiming on head?
		}

		if(!(item = pl->equipment[slot])) return; // very easy, if there is no item in this slot, do nothing

	// now we roll the chance(damage) from outside against the quality,
  if (!item->item_quality) return; // todo launch error
	if(RANDOM()%chance < (RANDOM()%10*item->item_quality)) return;

	item->item_condition--;
	// first make it slow, only decrease 1, we have also damage from outside (acid, magic weapons)
	// we can also try more damage in relation to the rolls above?
	// i temporary removed the different handling of item stacks

	if (item->item_condition<0) item->item_condition=0;

	ndi(NDI_UNIQUE, 0, op, "%s %s damaged!",
    query_name(item, op, ARTICLE_DEFINITE, 0),
    (item->nrof > 1) ? "are" : "is");
  OBJECT_UPDATE_UPD(item, UPD_QUALITY);

	FIX_PLAYER(op ,"material_attack_damage - end");
}

// Old code to keep the code syntax and logics alive
void material_attack_damageOLD(object_t *op, int num, int chance, int base)
{
    object_t *item;
    player_t *pl;
    int r, i, flag_fix = 0;

    if(op->type != PLAYER || !(pl = CONTR(op)))
        return;

    /*ndi(NDI_UNIQUE, 0, op, "num: %d chance:%d base:%d", num,chance,base);*/
    flag_fix = 0;
    while(num--)
    {

        if(chance < (RANDOM()%100))
            continue;

        r = RANDOM() % PLAYER_EQUIP_MAX; /* get a random slot */

        /* get the first slot with something in using random start slot */
        for(i=0;i<PLAYER_EQUIP_MAX;i++)
        {
            if((item = pl->equipment[(r+i)%PLAYER_EQUIP_MAX]))
            {
                /* only damage weapons? */
                if((base & HIT_FLAG_WEAPON) && item->type != WEAPON)
                    continue;
                break;
            }
        }

        if(i==PLAYER_EQUIP_MAX) /* nothing there we can damage */
        {
            if(flag_fix)
                FIX_PLAYER(op ,"material_attack_damage - leave");
            return;
        }


        if(base & MATERIAL_BASE_SPECIAL)
        {
            /* if we miss a single immunity, special will do damage.
             * all 4 immunites = item can't be damaged
             */
            if( QUERY_FLAG(op, FLAG_PROOF_PHYSICAL) &&
                    QUERY_FLAG(op, FLAG_PROOF_MAGICAL) &&
                    QUERY_FLAG(op, FLAG_PROOF_ELEMENTAL) &&
                    QUERY_FLAG(op, FLAG_PROOF_SPHERICAL))
                continue;
        }
        else
        {
          // what is this? we check immunity from players to say items are immune too?
          // also we have hard coded definitions of setting this MATERIAL_BASE_X outside ...
          // ... in damage object, where we set specific this in HitPlayerAttacktype
          // so it makes no sense to use this definitions here.
          // also there are only query_flag targeting this flags in code
          // nowhere we set these flags in code, except in loader
          // so we have always no set flags and always hardcoded material_base settings
          // before calling this function
            if(base & MATERIAL_BASE_PHYSICAL && !QUERY_FLAG(op, FLAG_PROOF_PHYSICAL))
                goto mnaterial_dmg_jmp;
            if(base & MATERIAL_BASE_ELEMENTAL && !QUERY_FLAG(op, FLAG_PROOF_ELEMENTAL))
                goto mnaterial_dmg_jmp;
            if(base & MATERIAL_BASE_MAGICAL && !QUERY_FLAG(op, FLAG_PROOF_MAGICAL))
                goto mnaterial_dmg_jmp;
            if(base & MATERIAL_BASE_SPHERICAL && !QUERY_FLAG(op, FLAG_PROOF_SPHERICAL))
                goto mnaterial_dmg_jmp;

            continue; /* no missing immunity found - item has resisted */
        }
        mnaterial_dmg_jmp:
        if (item->item_condition > 0)
        {
            if (item->type == ARROW)
            {
                int lost = (int)(0.5f / item->item_quality * item->nrof + RANDOM_ROLL(-5, 5));
                char *waswere = "were";

                if (lost <= 0)
                {
                    lost = 1;
                    waswere = "was";
                }

                ndi(NDI_UNIQUE, 0, op, "%d of your %s %s destroyed!", lost, query_name(item, op, ARTICLE_NONE, 0), waswere);
                decrease_ob_nr(item, lost);
            }
            else
            {
                /* TODO: Variable damage. Several ideas occur to me but this needs
                 * specific thought.
                 *
                 * -- Smacky 20151109 */
                item->item_condition--;
            }
        }

        // this is strange, this should be something like if ... and item_condition==0
        // so i think this don't work
        if (!item->item_condition)
        {
            /* If the player is daft enough to let an item get broken in this way,
             * it falls off -- but this is not an easy way to beat a curse! */
            if (!QUERY_FLAG(item, FLAG_CURSED) &&
                !QUERY_FLAG(item, FLAG_DAMNED))
            {
                ndi(NDI_UNIQUE, 0, op, "%s %s is broken and falls to the floor!",
                    query_name(item, op, ARTICLE_DEFINITE, 0),
                    (item->nrof > 1) ? "are" : "is");
                (void)thing_drop_to_floor(op, item, item->nrof);
            }
            else
            {
                ndi(NDI_UNIQUE, 0, op, "%s %s is broken but the curse holds!",
                    query_name(item, op, ARTICLE_DEFINITE, 0),
                    (item->nrof > 1) ? "are" : "is");
                OBJECT_UPDATE_UPD(item, UPD_QUALITY);
            }
        }
        else
        {
            if (item->type != ARROW)
            {
                ndi(NDI_UNIQUE, 0, op, "%s %s damaged!",
                    query_name(item, op, ARTICLE_DEFINITE, 0),
                    (item->nrof > 1) ? "are" : "is");
                OBJECT_UPDATE_UPD(item, UPD_QUALITY);
            }
        }

        flag_fix = 1;
    }

    if(flag_fix)
        FIX_PLAYER(op ,"material_attack_damage - end");
}

/* repair costs for item - owner is owner of that item */
sint64 material_repair_cost(object_t *item, object_t *owner)
{
    double tmp;
    sint64 costs=0;

    if(item->value && item->item_quality && item->item_condition < 100)
    {
        sint64 value = item->value;
        /* this is a problem.. we assume, that costs (as 64 bit value) will be covered
         * by tmp as double. This will work fine if costs is not insane high - what should
         * not be. If we have here problems, then we need to split this calc in a 64 bit one
         * with high values and small one
         */
        if(value < item->item_quality)
            value = (sint64) item->item_quality;

        tmp = (double) value / (double)item->item_quality; /* how much cost is 1 point of quality */
        tmp *= (double)(100 - item->item_condition); /* number of condition we miss */
        costs = (sint64) tmp;
    }

    return costs;
}

/* repair the item */
void material_repair_item(object_t *item, uint8 amount)
{
    if (amount < 0)
    {
        item->item_condition = 100;
    }
    else
    {
        item->item_condition += amount;
        if (item->item_condition > 100)
        {
            item->item_condition = 100;
        }
    }
}
