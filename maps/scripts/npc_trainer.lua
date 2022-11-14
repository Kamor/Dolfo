local MAX = 110 -- TODO if there is a way to get max player level from serverdefinitions, this would be better here

-- lua has no good table count function, so we need to hardcode max length of our array or use this function
local function getTableSize(t)
    local count = 0
    for _, __ in pairs(t) do
        count = count + 1
    end
    return count
end

local function npc_fee(costs)
		-- trainer % change depending to his level
		-- level 1 trainer wants 1% more
		-- level 10 trainer wants 10% more
		-- level 110 trainer wants 110% more
		costs = costs + costs / 100 * me.level
		return costs
end

local function to_pay(skill)
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill)
		
		if sobj == nil then
			return 0 -- we simply return 0 here, only to avoid crash, normaly the checks for the skills are outside this function
		end
			
		local slevel = sobj.level + 1
		-- local costs = slevel * slevel * (50+slevel) * 3
		local costs = 100 * slevel * slevel * 2
		
		-- hybrid training - reduce the costs in relation to your present grinded exp.
		-- !!! this part need the GetExpThreshold function in daimonin_object.c
		-- without this function hybrid training didn't work
		local exp_threshold = pl:GetExpThreshold(slevel);
		if (exp_threshold) then
			local sexp = sobj.experience
			local exp_delta=exp_threshold - sexp
			if (exp_delta>0) then
			  local exp_percent=100*exp_delta/exp_threshold
			  costs = costs/100*exp_percent
			end
		end
		return costs
end

function topic_training() -- we need this global
	ib:SetTitle("Training")
  ib:SetMsg("So, what do you want?")
  ib:AddMsg("\n\nYou have " .. pl:ShowCost(pl:GetMoney()) .. ".")
	
	for index, data in to_train do
	  local skill_number=game:GetSkillNr(data.skill_name)
		if skill_number==-1 then
			ib:AddMsg("\n\nERROR : There is no definition for " .. data.skill_name .."!")
		else
			local skill = pl:GetSkill(game.TYPE_SKILL, skill_number)
	    if skill == nil then
		    ib:AddSelect("Learn " .. data.skill_name .. " - " .. pl:ShowCost(npc_fee(500)) , "learn " .. index, data.icon, "I can teach you ".. data.skill_name)
	    else
		    local slevel = skill.level + 1 -- level to train
			  if (slevel<MAX) then
			    if (slevel<=me.level) then
				    local skill_group = pl:GetSkill(game.TYPE_SKILLGROUP, skill.magic) -- in skill.magic we find the skill group
				    if skill_group ~= nil then
					    if skill_group.level >= slevel then
						    ib:AddSelect("Train " .. data.skill_name .. " Level " .. slevel .. " - " .. pl:ShowCost(npc_fee(to_pay(skill_number))) , "train " .. index, data.icon, "I can train you in ".. data.skill_name)
				      else
						    ib:AddSelect("Train " .. data.skill_name .. " Level " .. slevel .. " - " .. pl:ShowCost(npc_fee(to_pay(skill_number))) , "services" , data.icon, "Train first more " .. skill_group.name)
				      end
					  else
						  ib:AddMsg("\n\nERROR : Somehow you managed to get " .. data.skill_name .." skill without a skillgroup !")
					  end
			    else
				    ib:AddSelect("Train " .. data.skill_name .. " Level " .. slevel, "services", data.icon, "I can't train you anymore.")
			    end
		    else
			    ib:AddSelect("Train " .. data.skill_name, "services", data.icon, "Who in the world should train you? You are really a master!")	
		    end
			end
		end
	end
	ib:SetLHSButton("Back" , "Training")
end

local function topic_learn(index)
	ib:SetTitle("Learn skill " .. index)
	ib:SetLHSButton("Back" , "Training")

	if index~=nil then
		local i = tonumber(index)
		if i>0 and i <= getTableSize(to_train) then
	    local skill_number=game:GetSkillNr(to_train[i].skill_name)
			if skill_number~=-1 then
  			if pl:GetSkill(game.TYPE_SKILL, skill_number) ~= nil then
	  		  ib:SetMsg("\nYou like to troll me?.\nYou already know this skill.")
		  	  return
		  	end

			  if pl:PayAmount(npc_fee(500)) == 1 then -- 5 silver + npc fee
			    pl:AcquireSkill(skill_number, game.LEARN)
				  ib:SetMsg("You learned " .. to_train[i].skill_name)
			  else
			    ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money.")
			  end
			  return
			end
		end
  end
	ib:SetMsg("I don't know this skill.")
end

local function topic_train(index)
	ib:SetTitle("Train skill " .. index)
	ib:SetLHSButton("Back" , "Training")

	if index~=nil then
		local i = tonumber(index)
		if i>0 and i <= getTableSize(to_train) then
	    local skill_number=game:GetSkillNr(to_train[i].skill_name)
			if skill_number~=-1 then
  			skill = pl:GetSkill(game.TYPE_SKILL, skill_number)
	  		if skill == nil then
		  	  ib:SetMsg("\nYou like to troll me?.\nYou better learn this skill first, before try to train it.")
			    return
			  end
			
			  local slevel = skill.level + 1
	      if (slevel>me.level) then
		      ib:SetMsg("I can't train you anymore.")
		      return
	      end
	      if (slevel>MAX) then
		      ib:SetMsg("Who in the world should train you? You are really a master!")
		      return	
	      end
			
			  local skill_group = pl:GetSkill(game.TYPE_SKILLGROUP, skill.magic) -- in skill.magic we find the skill group
			
			  if skill_group ~= nil then
			    if skill_group.level >= slevel then
				    if pl:PayAmount(npc_fee(to_pay(skill_number))) == 1 then
			        pl:SetSkill(game.TYPE_SKILL, skill_number, slevel, 0)
			        ib:SetMsg("You trained " .. skill.name)
		        else
			        ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		        end
				    return
			    else
				    ib:SetMsg("Train first more " .. skill_group.name)
			    end
			    return
			  else
				  ib:SetMsg("\n\nERROR : Somehow you managed to get " .. data.skill_name .." skill without a skillgroup !")
        end
        return
			end
	  end
	end
	ib:SetMsg("I don't know this skill.")
end

function trainerAddTopics(tlx) -- we need this global
	tlx:AddTopics("training", topic_training)
	tlx:AddTopics("learn (%d+)", topic_learn)
  tlx:AddTopics("train (%d+)", topic_train)
  return tlx
end