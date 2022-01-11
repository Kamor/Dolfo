# Dolfo
My presentation server for www.daimonin.org.
This is public trunk from https://sourceforge.net/p/daimonin/code/HEAD/tree/.
I show my changes and ideas here, if dev-team like it, they can grap it.

My changes:
- win32.h insert guard block

More npc's, upgrades or fixes
- dolfo.lua introduce to dolfo's world, news, gives credit card
- fanrir.lua improved key logic, new easter egg and shop
- cashin.lua reworked the interface flow and new topics
- stupir.lua fun/smalltalk script
- guildhall_guards.lua fun/jail, game hint script
- guard_default.lua simple basis script for castle guards doing some talking

New buyable skill system:
- daimonin_object.c removed SetSkill cap for indirect skills
- gimli.lua prototyp trainer for close weapon skills
- taleus.lua prototyp trainer for ranged weapon skills
- updated the logic to helper/melee_weapon_skill.lua
- updated the logic to helper/ranged_weapon_skill.lua
- improved scripts/first_weapon.lua

New food system:
- apply.c capped food to only eat one food same time, implemented % food
- item.c % food showed as % value
- lib/artifacts new potions + new % potions
- dev/testpotions.lua script
- scripts/alchemist.lua and new npc in guildhall_a_030a selling potions
- changed tavern food to %

New creditcard system
- added prototyp functionality in shop.c line line 297
- added a creditcard to lib/artifacts

Global exp system
- added/changed vars, changed exp calculation, added get/set commands
- -> see server/src/globalexpsystem.txt

Improved Pay logic
- logic now pays first with coppers, then silver, gold, mith
