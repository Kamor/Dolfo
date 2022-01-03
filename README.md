# Dolfo
My presentation server for www.daimonin.org.
This is public trunk from https://sourceforge.net/p/daimonin/code/HEAD/tree/.
I show my changes and ideas here, if dev-team like it, they can grap it.

My changes:
- win32.h insert guard block

More npc's, upgrades or fixes
- fanrir.lua improved key logic, new easter egg and shop
- cashin.lua reworked the interface flow and new topics
- stupir.lua fun/smalltalk scipt (guildhall_a_030)
- guildhall_guards.lua fun/jail, game hint script
- guard_default.lua simple basis script for castle guards doing some talking

New buyable skill system:
- daimonin_object.c removed SetSkill cap for indirect skills
- dolfo.lua prototyp trainer for close weapon skills (on map castle_030a)
- talues.lua prototyp trainer for ranged weapon skills

New food system:
- apply.c capped food to only eat one food same time
- lib/artifacts new potions
- scripts/alchemist.lua and new npc in guildhall_a_030a selling potions
