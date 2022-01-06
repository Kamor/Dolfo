-------------------------------------------------------------------------------
-- melee_weapon_skill.lua | Helper
--
-- Figure out which weapon skill the player has.
-- improved logic, first check for an equipped weapon and take this skill
-- with no equipped weapon, search skills, compare the exp, if equal choose by skillindex
-------------------------------------------------------------------------------
---------------------------------------
-- All helpers require a script context, so ensure the Utility is loaded.
-- example to use this script from outside
-- require("script_context")
-- require("helper/melee_weapon_skill")
-- local mws = ScriptContext:GetFunction("melee_weapon_skill")
-- local obj, mode = mws(player)
---------------------------------------
require("script_context")

---------------------------------------
-- Helpers define 1 function, nothing else (the function name is irrelevant).
---------------------------------------
local function f(player)
    assert(type(player) == "GameObject" and player.type == game.TYPE_PLAYER,
        "Arg #1 must be player GameObject!")
	
	local obj, mode	-- if we find skill, we put in in here
	-- check first if player has readied a weapon	
	local weapon = player:GetEquipment(game.EQUIP_WEAPON1)
	if weapon ~= nil then
		-- modula since Lua 5.2 we are under 5.2?
		local subtype = weapon.subtype - math.floor(weapon.subtype/4)*4
		if subtype==game.WEAP_1H_IMPACT then
			obj, mode = player:GetSkill("Impact Weapons")
		elseif subtype==game.WEAP_1H_SLASH then
			obj, mode = player:GetSkill("Slash Weapons")
		elseif subtype==game.WEAP_1H_PIERCE then	
			obj, mode = player:GetSkill("Pierce Weapons")
		elseif subtype==game.WEAP_1H_CLEAVE then
			obj, mode = player:GetSkill("Cleave Weapons")
		end
		
		if obj then	
			return obj, mode -- return skill from equipped weapon
		end
	end
	
	-- no weapon or no valid weapon skill found? we search skills now
	-- and compare the exp, if exp is equal it depends on the skillindex
	local sname = { "Cleave", "Impact", "Pierce", "Slash" }
    for _, v in ipairs(sname) do
        local temp_obj, temp_mode = player:GetSkill(v .. " Weapons")
        if temp_obj ~= nil then
			if obj == nil or temp_obj.experience>obj.experience then
				obj=temp_obj
				mode=temp_mode
			end
        end
    end
	
	if obj then
	  return obj, mode -- return highest skill
	end

    error(player:GetName() .. " has no wskill!")
end

---------------------------------------
-- Add the function to the script context under the key of the Helper script
-- name.
---------------------------------------
ScriptContext:AddFunction("melee_weapon_skill", f)
