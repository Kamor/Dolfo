local me= event.me
local pl = event.activator

require("interface_builder")

local ib = InterfaceBuilder()

local skill_cleave = game:GetSkillNr("Cleave Weapons")
local skill_impact = game:GetSkillNr("Impact Weapons")
local skill_pierce = game:GetSkillNr("Pierce Weapons")
local skill_slash = game:GetSkillNr("Slash Weapons")

local MAX = 110 -- todo need to get max from serverdefinition <--------------

local function topic_default()
    ib:SetTitle("What?")
    ib:SetMsg("I'm sorry. I didn't understand.\n")
    ib:AddMsg("\n|[The interface's parsing is extremely limited, so keep " ..
              "your input simple. Usually type only a noun (name of " ..
              "something), such as 'quest', or sometimes a verb (an action " ..
              "to do) and a noun, such as 'repair sword', to talk about a " ..
              "subject. But it may also be that this particular NPC simply " ..
              "does not know anything about that topic.]|\n")
    ib:AddMsg("\n`Hint -- Making yourself understood`\n")
    ib:AddMsg("\nIn fact, all NPCs should respond to the ~standard topics~ " ..
              "'hello', 'quest', 'background', and 'rumours', and many " ..
              "offer 'services' as well.\n")
    ib:AddMsg("\nAfter the initial '/talk hello' to start a " ..
              "conversation, it is also acceptable to stick to just the " ..
              "hypertext, icons, links, and buttons.")
end

local function topic_greeting()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Welcome ...")
    ib:SetMsg("\n... on my presentation server.\n")
	ib:AddMsg("\nThese are the public accessable maps from www.daimonin.org. So they are the older maps.\n")
	ib:AddMsg("\nI made some changes and build my ideas here, so dev team or players can see it.\n")
	ib:AddMsg("\nThis is not a full game, it's the early maps from public trunk.\n")
	ib:AddMsg("\nI concentrate on early balancing and try to build new ideas.\n")
	ib:AddMsg("\nSee changes, so you know where to look for new or changed content.\n")
	ib:AddMsg("\nuse command ~/exp~ to see current server exp.")
	ib:AddLink("Changes")
	ib:AddLink("Cheat")
	ib:AddLink("Services")
	
	ib:SetLHSButton("Hello")
end

local function topic_changes()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Changes")
    ib:SetMsg("\nStupir NPC+stupir.lua : Smalltalk npc - |need a fix on long links, not fixable by lua only|\n")
	ib:AddMsg("\nfanrir.lua : improved key logic, easter egg, shop - |need some more cleaning|\n")
	ib:AddMsg("\nDolfo NPC+dolfo.lua : Presentation npc, ~player view clean, need code optimize~\n")
	ib:AddMsg("\ncashin.lua : fixed bug, reworked logic, ~player view nearly finished~\n")
	ib:AddMsg("\nchereth.lua : ~auto heal when hp<30~, heal and services command, very early state, |minor healing output error, not a lua fix|\n")
	ib:AddMsg("\nguildhall_guard.lua : Smalltalk and jailing guards, hint to secret kobold map.\n")
	ib:AddMsg("\ntaleus.lua : archery trainer, in work, needs some more fine tuning in logic, type cheat or cheat1\n")
	ib:SetLHSButton("Hello")
end

local function to_pay(skill)
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill)
		
		if sobj == nil then
			return 0 -- todo 0 return? or what should we do here, for error handling
		end
			
		local slevel = sobj.level + 1
		local costs = slevel * slevel * (50+slevel) * 3
		
		-- todo we want reduce the price by % depends on exp we have grinded for this level.
		local sexp = sobj.experience
		-- we need exp to next level, then we can calculate delta exp and the % reduce from this
		-- we have skill exp but can't access level array from here
		-- we need a new luafunction accessing new_levels
		-- then we can get next level exp from hooks->new_levels[slevel]
		
		-- npc_fee included here
		-- trainer % change depending to his level
		-- level 1 trainer wants 1% more
		-- level 10 trainer wants 10% more
		-- level 110 trainer wants 110% more
		costs = costs + costs / 100 * me.level
		
		return costs
end

-- this script is a mess, so much redundany, need optimize later
local function npc_fee(costs)
		costs = costs + costs / 100 * me.level
		return costs
end


local function topic_services()
	ib:SetHeader("st_005", me)
    ib:SetMsg("This is my idea of opening all four ph skills and all 3 archery skills.")
    ib:AddMsg(" For archery skills got to Archery Commander Taleus.\n")
	ib:AddMsg("\nYou need to find a npc who can train you. For example i can train all 4 weapon skills up to level ".. me.level ..".\n")
	ib:AddMsg("\nBut you need to train also your physique skill first to this level.")
    ib:AddMsg(" You have " .. pl:ShowCost(pl:GetMoney()) .. ".")
	
	if pl:FindSkill(skill_cleave) == nil then
		ib:AddSelect("Learn cleave weapon - " .. pl:ShowCost(npc_fee(500)) , "learn cleave", "axe_small.101", "I can show you how to handle a cleave weapon.")
	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_cleave)
		-- no need for nil check here, we checked it with FindSkill
		local slevel = sobj.level + 1 -- level to train
		-- no need for max level check here, eobj (skillgrouplevel can't be higher then 110)
		-- but we check for cleaner output logic
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_PHYSIQUE)
	 			-- nil is error but we only say train more agility, somehow player has managed to kill his skillgroup
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Learn cleave weapon - " .. pl:ShowCost(to_pay(skill_cleave)), "train cleave", "axe_small.101", "Train to level " .. slevel .. ".")
				else
					ib:AddSelect("Learn cleave weapon - " .. pl:ShowCost(to_pay(skill_cleave)), "services", "axe_small.101", "Train more physique first.")
				end
			else
				ib:AddSelect("Learn cleave weapon", "services", "axe_small.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Learn cleave weapon", "services", "axe_small.101", "Who in the world should train you? You are really a master!")	
		end
	end
	
	if pl:FindSkill(skill_impact) == nil then
		ib:AddSelect("Learn impact weapon - " .. pl:ShowCost(npc_fee(500)), "learn impact", "club.101", "I can show you how to handle an impact weapon.")
	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_impact)
		-- no need for nil check here, we checked it with FindSkill
		local slevel = sobj.level + 1 -- level to train
		-- no need for max level check here, eobj (skillgrouplevel can't be higher then 110)
		-- but we check for cleaner output logic
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_PHYSIQUE)
	 			-- nil is error but we only say train more agility, somehow player has managed to kill his skillgroup
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Learn impact weapon - " .. pl:ShowCost(to_pay(skill_impact)), "train impact", "club.101", "Train to level " .. slevel .. ".")
				else
					ib:AddSelect("Learn impact weapon - " .. pl:ShowCost(to_pay(skill_impact)), "services", "club.101", "Train more physique first.")
				end
			else
				ib:AddSelect("Learn impact weapon", "services", "club.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Learn impact weapon", "services", "club.101", "Who in the world should train you? You are really a master!")	
		end
	end
	
    if pl:FindSkill(skill_pierce) == nil then
		ib:AddSelect("Learn pierce weapon - " .. pl:ShowCost(npc_fee(500)), "learn pierce", "dagger.101", "I can show you how to handle a pierce weapon.")
	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_pierce)
		-- no need for nil check here, we checked it with FindSkill
		local slevel = sobj.level + 1 -- level to train
		-- no need for max level check here, eobj (skillgrouplevel can't be higher then 110)
		-- but we check for cleaner output logic
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_PHYSIQUE)
	 			-- nil is error but we only say train more agility, somehow player has managed to kill his skillgroup
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Learn pierce weapon - " .. pl:ShowCost(to_pay(skill_pierce)), "train pierce", "dagger.101", "Train to level " .. slevel .. ".")
				else
					ib:AddSelect("Learn pierce weapon - " .. pl:ShowCost(to_pay(skill_pierce)), "services", "dagger.101", "Train more physique first.")
				end
			else
				ib:AddSelect("Learn pierce weapon", "services", "dagger.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Learn pierce weapon", "services", "dagger.101", "Who in the world should train you? You are really a master!")	
		end
	end
	
	if pl:FindSkill(skill_slash) == nil then
		ib:AddSelect("Learn slash weapon - " .. pl:ShowCost(npc_fee(500)), "learn slash", "dagger.101", "I can show you how to handle a slash weapon.")
	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_slash)
		-- no need for nil check here, we checked it with FindSkill
		local slevel = sobj.level + 1 -- level to train
		-- no need for max level check here, eobj (skillgrouplevel can't be higher then 110)
		-- but we check for cleaner output logic
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_PHYSIQUE)
	 			-- nil is error but we only say train more agility, somehow player has managed to kill his skillgroup
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Learn slash weapon - " .. pl:ShowCost(to_pay(skill_slash)), "train slash", "shortsword.101", "Train to level " .. slevel .. ".")
				else
					ib:AddSelect("Learn slash weapon - " .. pl:ShowCost(to_pay(skill_slash)), "services", "shortsword.101", "Train more physique first.")
				end
			else
				ib:AddSelect("Learn slash weapon", "services", "shortsword.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Learn slash weapon", "services", "shortsword.101", "Who in the world should train you? You are really a master!")	
		end
	end
	
end

-- i separated this, cause of code blowing up, but 1 click interface logic is normally better then 2 click
-- setskill is blocking level up on indirect skills ... need a recompile
-- also think of level drain logic later ???
local function topic_train(skill)
	ib:SetHeader("st_005", me)
	
	-- we really need to optimize logic later ... so much redundance
	local sobj = nil
	if skill=="cleave" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_cleave)
	elseif skill=="impact" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_impact)
	elseif skill=="pierce" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_pierce)
	elseif skill=="slash" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_slash)
	end
		
	if sobj == nil then
		return -- todo?
	end
	local slevel = sobj.level + 1
	
	if (slevel>=me.level) then
		ib:SetMsg("I can't train you anymore.")
		ib:SetLHSButton("Back","services")
		return
	end
	if (slevel>=MAX) then
		ib:SetMsg("Who in the world should train you? You are really a master!")
		ib:SetLHSButton("Back","services")
		return	
	end
	
	if skill=="cleave" then
		if pl:PayAmount(to_pay(skill_cleave)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_cleave, slevel, 0)
			ib:SetMsg("You trained cleave weapons.")	
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	elseif skill=="impact" then
		if pl:PayAmount(to_pay(skill_impact)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_impact, slevel, 0)
			ib:SetMsg("You trained impact weapons.")
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	elseif skill=="pierce" then
		if pl:PayAmount(to_pay(skill_pierce)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_pierce, slevel, 0)
			ib:SetMsg("You trained pierce weapons.")
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	elseif skill=="slash" then
		if pl:PayAmount(to_pay(skill_slash)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_slash, slevel, 0)
			ib:SetMsg("You trained slash weapons.")
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	else
		ib:SetMsg("I can't train your " .. skill .. " skill!") -- todo?
	end
	ib:SetLHSButton("Back","services")
end

local function topic_learn(skill)
	if skill == nil then
		return -- todo?
	end
    ib:SetHeader("st_005", me)
	
	if pl:PayAmount(npc_fee(500)) == 1 then -- 5 silver + npc fee
		if skill=="cleave" then
			pl:AcquireSkill(skill_cleave, game.LEARN)
			ib:SetMsg("You learned cleave weapons.")	
		elseif skill=="impact" then
			pl:AcquireSkill(skill_impact, game.LEARN)
			ib:SetMsg("You learned impact weapons.")
		elseif skill=="pierce" then
			pl:AcquireSkill(skill_pierce, game.LEARN)
			ib:SetMsg("You learned pierce weapon.")
		elseif skill=="slash" then
			pl:AcquireSkill(skill_slash, game.LEARN)
			ib:SetMsg("You learned slash weapon.")
		else
			ib:SetMsg("I can't teach you " .. skill .. " skill! But thanks for the money.") -- todo?
		end	
	else
		ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
	end
	ib:SetLHSButton("Back","services")
end


local function topic_cheat()
    ib:SetHeader("st_005", me)
	ib:SetMsg("Cheat money.")
    ib:SetCoins(0, 0, 0, 1)
	pl:AddMoney(0, 0, 0, 1)
	ib:SetLHSButton("Hello")
end


require("topic_list")

local tl = TopicList()

tl:SetDefault(topic_default)
tl:AddGreeting("what now%??", topic_greeting) -- todo why what now, when we overwrite title with ib:SetTitle later? but we currently need link to function here
tl:AddTopics({ "changes" }, topic_changes)
tl:AddTopics({ "cheat" }, topic_cheat)
tl:AddServices(nil, topic_services)

tl:AddTopics({"learn (cleave)", "learn (impact)", "learn (pierce)", "learn (slash)" }, topic_learn)
tl:AddTopics({"train (cleave)", "train (impact)", "train (pierce)", "train (slash)" }, topic_train)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        