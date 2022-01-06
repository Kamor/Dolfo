-------------------------------------------------------------------------------
-- first_weapon.lua | helper script to give player a first weapon
--
-- Create and auto-equip a weapon of the appropriate type if player has not
-- already been given one by this script, has 0 exp, and has no weapon
-- equipped. So this is not foolproof, but should be good enough given the low
-- value of this starter weapon.
-------------------------------------------------------------------------------
local npc = event.me
local player = event.activator
local ds = DataStore("first_weapon", player)

if not ds:Get("weapon given") then
    ds:Set("weapon given", true)
    ds:WasChanged()
	
	if not player:GetEquipment(game.EQUIP_WEAPON1) then

		local weaponarch =
		{
			[game:GetSkillNr("slash weapons") ] = "shortsword",
			[game:GetSkillNr("impact weapons")] = "mstar_small",
			[game:GetSkillNr("cleave weapons")] = "axe_small",
			[game:GetSkillNr("pierce weapons")] = "dagger_large"
		}

		local weaponname
		for k, v in pairs(weaponarch) do
			local skill = player:FindSkill(k) 

			-- if we find one weapon skill with experience, player don't need anymore a first weapon
			-- but high level players can still get a weapon, if they levelled their char without using a weapon, so this could be improved by a level check?
			if skill then
				if skill.experience>0 then
					return
				else
					weaponname=v
					-- we continue searching and checking the exp from other weapon skills
					-- if we find other skills with zero exp, we take this skill
					-- so on multiply learned weaponskill with all 0 exp, script chooses the last skill for the weapon to give
					-- somehow slash is currently last skill, looks like pairs() is sorting the array
				end
			end
		end	
			
		if weaponname then
			local weapon = player:CreateObjectInside(weaponname, game.IDENTIFIED)
			assert(weapon, "Could not create weapon!")

			if npc.type == game.TYPE_MONSTER then
				player:Write(npc.name .. " puts a " .. weapon:GetName() ..
							 " in your hands.")
				player:Write(string.capitalize(gender_objective[npc:GetGender()]) ..
							 " says: Here, take this weapon.")
			end
			player:Apply(weapon, game.APPLY_ALWAYS)
		end
    end
end
