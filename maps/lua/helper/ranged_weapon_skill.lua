-------------------------------------------------------------------------------
-- ranged_weapon_skill.lua | Helper
--
-- Figure out which bow skill the player has.
-- improved logic, first check for an equipped weapon and take this skill
-- with no equipped weapon, search skills, compare the exp, if equal choose by skill index
-------------------------------------------------------------------------------
---------------------------------------
-- All helpers require a script context, so ensure the Utility is loaded.
-- example to use this script from outside
-- require("script_context")
-- require("helper/ranged_weapon_skill")
-- local rws = ScriptContext:GetFunction("ranged_weapon_skill")
-- local obj, mode = rws(player)
---------------------------------------
require("script_context")

---------------------------------------
-- Helpers define 1 function, nothing else (the function name is irrelevant).
---------------------------------------
local function f(player)
    assert(type(player) == "GameObject" and player.type == game.TYPE_PLAYER,
        "Arg #1 must be player GameObject!")
	
	local obj, mode	-- if we find skill, we put in in here
	-- check first if player has readied a bow	
	local bow = player:GetEquipment(game.EQUIP_BOW)
	if bow ~= nil then
		if bow.subtype==game.RANGE_WEAP_BOW then
			obj, mode = player:GetSkill("Bow Archery")
		elseif bow.subtype==game.RANGE_WEAP_XBOWS then
			obj, mode = player:GetSkill("Crossbow Archery")
		elseif bow.subtype==game.RANGE_WEAP_SLINGS then	
			obj, mode = player:GetSkill("Sling Archery")
		elseif bow.subtype==game.RANGE_WEAP_FIREARMS then	
			--obj, mode = player:GetSkill("xxx")
			error(player:GetName() .. " range firearms archery not implemented!")			
		end
		
		if obj then	
			return obj, mode -- return skill from equipped weapon
		end
	end
	
	-- no bow or no valid bow skill found? we search skills now
	-- and compare the exp, if exp is equal it depends on the skill index
    local sname = { "Bow", "Crossbow", "Sling" }
    for _, v in ipairs(sname) do
        local temp_obj, temp_mode = player:GetSkill(v .. " Archery")
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
ScriptContext:AddFunction("ranged_weapon_skill", f)
