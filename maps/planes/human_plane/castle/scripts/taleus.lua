-- merc_taleus.lua
-- Teaches a single archery skill. Quest missing.
--

require("topic_list")
require("interface_builder")
require("quest_builder")

local qb        = QuestBuilder()
local pl        = event.activator
local me        = event.me
local ib        = InterfaceBuilder()
if not module_guildsLOADED then
    require("module/guilds")
end
local gstat = module_guildsGetStatus("Mercenary", pl)

local skill_bow = game:GetSkillNr("bow archery")
local skill_cbow = game:GetSkillNr("crossbow archery")
local skill_sling = game:GetSkillNr("sling archery")

local MAX = 110 -- todo need to get max from serverdefinition <--------------

local function questGoal(questnr)
    qb:AddQuestTarget(questnr, 1, 1, "goblin", "Hagvash"):
    AddQuestItem(1, "quest_object", "power_crystal.101", "Raiding Party Clue")
    pl:Sound(0, 0, 2, 0)
    pl:Write("You take the quest '".. qb:GetName(questnr) .."'.", game.COLOR_NAVY)
end

qb:AddQuest("Rats in Stonehaven?", game.QUEST_KILLITEM, nil, nil, nil, nil, 1, questGoal, nil)
local questnr = qb:Build(pl)
local QStatus = qb:GetStatus(1)

local function quest_reminder()
    ib:SetTitle(qb:GetName(questnr))
    ib:AddMsg("Haven't you taken care of it yet? Come back when you have!\n")
    ib:AddQuestChecklist(qb, questnr)
    ib:SetButton("Back", "hi") 
end
    
local function quest_icons1()
	ib:AddSelect("Learn Archery skill of Sling ", "quest complete 1", "sling_small.101", "Fastest, but hits lowest of the three")
	ib:AddSelect("Learn Archery skill of Bow ", "quest complete 2", "bow_short.101", "Balanced between fast and strong")
	ib:AddSelect("Learn Archery skill of Crossbow ", "quest complete 3", "crossbow_small.101", "Strongest, but slowest of the three")
end

local function quest_body1()
    ib:SetHeader("st_003", me)
    ib:SetTitle(qb:GetName(questnr))
    ib:SetMsg("We have had reports about strange noises from around one of the buildings on the east side of the town.\n\n")
    ib:SetMsg("The castle guards think it is rats and won't look into it. I must remain here in case my shipment arrives, and most other warriors are otherwise occupied. It would be great if you could investigate and take care of any problems.\n\n")
    ib:SetMsg("It seems most of the noises come from the first building on the eastern road. The woman who lives there will tell you more. Just follow this road south to the intersection by the main gates. Then go east and follow the road to the first house on the right hand side.")
    ib:SetDesc("Investigate the noises and handle any problems", 0, 0, 0, 0)
    quest_icons1()
end

-- changed logic taleus only gives quest to guildmembers
-- but he offers moneytraining to all
-- todo make a alternate quest reward (5 silver) for players who have bought a skill first
function topicDefault()
    ib:SetHeader("st_001", me)
    if gstat == game.GUILD_IN then -- or module_guildsPlayerGuildless(pl)
        if pl:FindSkill(skill_bow) ~= nil or pl:FindSkill(skill_cbow) ~= nil or pl:FindSkill(skill_sling) ~= nil or QStatus == game.QSTAT_DONE then
			ib:SetTitle("Greetings")
			ib:SetMsg("Hello guildmember " .. pl.name .. ".\n")
			ib:AddMsg("\n|** You salute each other **|\n")
			ib:AddMsg("\nYou need my ^services^?")
			ib:SetLHSButton("Services")
        elseif QStatus == game.QSTAT_NO then
            ib:SetTitle("Greetings")
            ib:SetMsg("\n\nHello, mercenary. I am Archery Commander Taleus.")
            ib:AddMsg("\n\nI have taken over from my mentor, ^Chereth^, since she lost her eyes.")
            ib:AddMsg("\n\nWell, I know a lot about ^archery^ and I ^train^ new recruits of the mercenary guild.")
			ib:SetLHSButton("Services") -- todo
        elseif QStatus == game.QSTAT_SOLVED then
            topicQuestComplete()
            return
        elseif QStatus == game.QSTAT_ACTIVE then
            quest_reminder()
		end
			
	elseif module_guildsPlayerGuildless(pl)  then
			ib:SetTitle("Hello")	
			ib:SetMsg("I am Archery Commander Taleus.")
			ib:AddMsg("\n\nI have taken over from my mentor, ^Chereth^, since she lost her eyes.")
            ib:AddMsg("\n\nWell, I know a lot about ^archery^ and I train new recruits of the mercenary guild.")
			ib:AddMsg("\n\nI have seen, you are guildless. Why you don't join our guild?\n")
			ib:AddMsg("\nYou can talk to Cashin, who is in the Guildhall southeast of this castle, about joining the mercenary guild.")
			if QStatus == game.QSTAT_NO then
				ib:AddMsg("\n\nI have also a quest for guild members and grant one free archery training lesson.\n")
			end
			ib:AddMsg("\n\nI you want, i can train you for money, see my ^services^?\n")
			ib:SetLHSButton("Services")
			
			

    else
		ib:SetTitle("Hello")
		ib:SetMsg("\nI am Archery Commander Taleus.\n")
		ib:AddMsg("\nYou need my ^services^?")
		ib:SetLHSButton("Services")
        --ib:SetTitle("Hello")
        --ib:SetMsg("I'm sorry, but I cannot teach you an archery skill due to your guild.")
        --ib:AddMsg(" You can either join the Mercenary guild or leave your current guild.")
    end
end

-- Player asks for quests
function topQuest()
    if QStatus == game.QSTAT_ACTIVE then
        quest_reminder()
    elseif QStatus == game.QSTAT_NO then 
        quest_body1()
        ib:SelectOff()
        ib:SetAccept(nil, "accept quest") 
        ib:SetDecline(nil, "hi") 
    else
        topicDefault()
        return
    end
end
-- accepted: start the quest
function topAccept()
    if QStatus == game.QSTAT_NO then
        quest_body1()
        ib:SelectOff()
        qb:RegisterQuest(questnr, me, ib)
   else
       topicDefault()
        return
    end
end

function topChereth()
    ib:SetHeader("st_004", me)
    ib:SetTitle("Chereth")
    ib:SetMsg("\n\nYes, Chereth lost her eyes in a major battle. When you find her, she can tell you more.")
	ib:AddMsg("\n\nNormally she is in the Guildhall southeast of this castle.")
    ib:SetButton("Back", "hi")
end

function topArchery()
    ib:SetHeader("st_003", me)
    ib:SetTitle("Archery Info")
    ib:SetMsg("\n\nYes, there are three archery skills:\n")
    ib:AddMsg("\nBow Archery is the most common, firing arrows.\n")
    ib:AddMsg("\nSling Archery allows fast firing stones with less damage.\n")
    ib:AddMsg("\nCrossbow Archery uses x-bows and bolts. Slow but powerful.\n")
	ib:SetButton("Back", "hi")
	
    --if gstat ~= game.GUILD_IN then -- and not module_guildsPlayerGuildless(pl)
    --    ib:AddMsg("\nI teach archery for money and train new recruits for the mercenary guild, but I can see that you're not a member.\n")
    --    ib:AddMsg("You should talk to Cashin, who is in the Guildhall southeast of this castle, about joining the mercenary guild.")
    --else
    --    ib:AddMsg("\nWell, there are the three different archery skills.\n\nI can teach you only ~*ONE*~ of them.\n")
    --    ib:AddMsg("You have to stick with it, so |choose wisely|.\n")
    --    ib:AddMsg("But before I teach you, I have a little ^quest^ for you.")
    --    ib:AddLink("Could you tell me what this 'quest' is?", "explain quest")
    -- end
    ib:SetButton("Back", "hi")
end
function topicQuestComplete(reward)
    if reward == nil then
		if QStatus == game.QSTAT_ACTIVE then
			quest_reminder()
		else	
			ib:AddMsg("Very well done! It was worse than I thought. I wonder where those kobolds came from--I thought we'd driven most of them out.\n\n")
			ib:AddMsg("I will take the crystal you found to Jahrlen, he might be able to understand it.\n\n")
			ib:AddMsg("Now you must choose your archery skill:")
			quest_icons1()
		end
	else
		if reward == "1" then
			ib:SetTitle("Sling archery")
			ib:SetMsg("Ah, you chose sling archery. An excellent choice!")
			pl:AcquireSkill(skill_sling, game.LEARN)
			pl:CreateObjectInsideEx("sling_small", game.IDENTIFIED,1)
			pl:CreateObjectInsideEx("sstone", game.IDENTIFIED,20)
		elseif reward == "2" then
			ib:SetTitle("Bow archery")
			ib:SetMsg("Ah, you chose bow archery. An excellent choice!")
			pl:AcquireSkill(skill_bow, game.LEARN)
			pl:CreateObjectInsideEx("bow_short", game.IDENTIFIED,1)
			pl:CreateObjectInsideEx("arrow", game.IDENTIFIED,20)
		elseif reward == "3" then
			ib:SetTitle("Crossbow archery")
			ib:SetMsg("Ah, you chose crossbow archery. An excellent choice!")
			pl:AcquireSkill(skill_cbow, game.LEARN)
			pl:CreateObjectInsideEx("crossbow_small", game.IDENTIFIED,1)
			pl:CreateObjectInsideEx("bolt", game.IDENTIFIED,20)
		end
		qb:Finish(1)
	end
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

-- this script is a mess, so much redundancy, need optimize later
local function npc_fee(costs)
		costs = costs + costs / 100 * me.level
		return costs
end



local function topic_services()
    ib:SetHeader("st_005", me)
    ib:SetMsg("In past i had orders only to train guild members. But guild changed this. I am allowed to train also non guild members.")
	ib:AddMsg("\n\nYou have " .. pl:ShowCost(pl:GetMoney()) .. "\n ")
	if pl:FindSkill(skill_bow) == nil then
		ib:AddSelect("Learn Archery skill of Bow - " .. pl:ShowCost(npc_fee(500)), "learn bow", "bow_short.101", "I can show you how to handle a bow.")
	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_bow)
		-- no need for nil check here, we checked it with FindSkill
		local slevel = sobj.level + 1 -- level to train
		-- no need for max level check here, eobj (skillgrouplevel can't be higher then 110)
		-- but we check for cleaner output logic
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_AGILITY)
	 			-- nil is error but we only say train more agility, somehow player has managed to kill his skillgroup
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Train Archery skill of Bow - " .. pl:ShowCost(to_pay(skill_bow)), "train bow", "bow_short.101", "Train to level " .. slevel .. ".")
				else
					ib:AddSelect("Train Archery skill of Bow - " .. pl:ShowCost(to_pay(skill_bow)), "services", "bow_short.101", "Train more agility first.")
				end
			else
				ib:AddSelect("Train Archery skill of Bow", "services", "bow_short.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Train Archery skill of Bow", "services", "bow_short.101", "Who in the world should train you? You are really a master!")	
		end
	end
	-- same logic now on crossbow
	if pl:FindSkill(skill_cbow) == nil then
		ib:AddSelect("Learn Archery skill of Crossbow - " .. pl:ShowCost(npc_fee(500)), "learn cbow", "crossbow_small.101", "I can show you how to handle a crossbow.") 
 	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_cbow)
		local slevel = sobj.level + 1
		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_AGILITY)
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Train Archery skill of Crossbow - " .. pl:ShowCost(to_pay(skill_cbow)), "train cbow", "crossbow_small.101", "Train to Level " .. slevel .. ".")
				else
					ib:AddSelect("Train Archery skill of Crossbow - " .. pl:ShowCost(to_pay(skill_cbow)), "services", "crossbow_small.101", "Train more agility first.")
				end
			else
				ib:AddSelect("Train Archery skill of Crossbow" , "services", "crossbow_small.101", "I can't train you anymore.")
			end
		else
			ib:AddSelect("Train Archery skill of Crossbow" , "services", "crossbow_small.101", "Who in the world should train you? You are really a master!")
		end	
	end
	-- and sling
	if pl:FindSkill(skill_sling) == nil then
		ib:AddSelect("Learn Archery skill of Sling - " .. pl:ShowCost(npc_fee(500)), "learn sling", "sling_small.101", "I can show you how to handle a sling.")
 	else
		local sobj = pl:GetSkill(game.TYPE_SKILL, skill_sling)
		local slevel = sobj.level + 1

		if (slevel<MAX) then
			if (slevel<=me.level) then
				local eobj = pl:GetSkill(game.TYPE_SKILLGROUP, game.SKILLGROUP_AGILITY)
				if eobj ~= nil and eobj.level >= slevel then
					ib:AddSelect("Train Archery skill of Sling - " .. pl:ShowCost(to_pay(skill_sling)), "train sling", "sling_small.101", "Train to Level " .. slevel .. ".")
				else
					ib:AddSelect("Train Archery skill of Sling - " .. pl:ShowCost(to_pay(skill_sling)), "services", "sling_small.101", "Train more agility first.")	
				end
			else
				ib:AddSelect("Train Archery skill of Sling", "services", "sling_small.101", "I can't train you anymore.")	
			end
		else
			ib:AddSelect("Train Archery skill of Sling", "services", "sling_small.101", "Who in the world should train you? You are really a master!")
		end		
	end
end

-- i separated this, cause of code blowing up, but 1 click interface logic is normally better then 2 click
-- setskill is blocking level up on indirect skills ... need a recompile
-- also think of level drain logic later ???
local function topic_train(skill)
	if skill == nil then
		-- this is jump in for mercenary questt
	
		ib:SetMsg("So you want train some archery?\n")
		ib:AddMsg("\n|" .. me.name .." is smiling at you.|\n")
		ib:AddMsg("\nBut before I teach you, I have a little ^quest^ for you.")
		ib:AddLink("Could you tell me what this 'quest' is?", "explain quest")
		ib:SetLHSButton("Quest")
	
		return -- todo?
	end
	ib:SetHeader("st_005", me)
	
	-- we really need to optimize logic later ... so much redundance
	local sobj = 0
	if skill=="bow" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_bow)
	elseif skill=="cbow" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_cbow)
	elseif skill=="sling" then
		sobj = pl:GetSkill(game.TYPE_SKILL, skill_sling)

	end
	
	local sobj = pl:GetSkill(game.TYPE_SKILL, skill)
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
	
	if skill=="bow" then
		if pl:PayAmount(to_pay(skill_bow)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_bow, slevel, 0)
			ib:SetMsg("You trained bow archery.")	
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	elseif skill=="cbow" then
		if pl:PayAmount(to_pay(skill_cbow)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_cbow, slevel, 0)
			ib:SetMsg("You trained crossbow archery.")
		else
			ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
		end
	elseif skill=="sling" then
		if pl:PayAmount(to_pay(skill_sling)) == 1 then
			pl:SetSkill(game.TYPE_SKILL, skill_sling, slevel, 0)
			ib:SetMsg("You trained sling archery.")
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
	
	if pl:PayAmount(npc_fee(500)) == 1 then -- 5 silver + npc_fee
		if skill=="bow" then
			pl:AcquireSkill(skill_bow, game.LEARN)
			ib:SetMsg("You learned bow archery.")	
		elseif skill=="cbow" then
			pl:AcquireSkill(skill_cbow, game.LEARN)
			ib:SetMsg("You learned crossbow archery.")
		elseif skill=="sling" then
			pl:AcquireSkill(skill_sling, game.LEARN)
			ib:SetMsg("You learned sling archery.")
		else
			ib:SetMsg("I can't teach you " .. skill .. " skill!") -- todo?
		end	
	else
		ib:SetMsg("\nYou don't have enough money.\nCome back if you have the money." )	
	end
	ib:SetLHSButton("Back","services")
end

-- this is only for testing, must removed later
local function cheat()
    ib:SetHeader("st_005", me)
	ib:SetMsg("Cheat money.")
    ib:SetCoins(0, 0, 0, 1)
	pl:AddMoney(0, 0, 0, 1)
end

local function cheat1()
    ib:SetHeader("st_005", me)
	ib:SetMsg("Cheat items.")
	
	pl:CreateObjectInsideEx("sling_small", game.IDENTIFIED,1)
	pl:CreateObjectInsideEx("sstone", game.IDENTIFIED,100)
	
	pl:CreateObjectInsideEx("bow_short", game.IDENTIFIED,1)
	pl:CreateObjectInsideEx("arrow", game.IDENTIFIED,100)
	
	pl:CreateObjectInsideEx("crossbow_small", game.IDENTIFIED,1)
	pl:CreateObjectInsideEx("bolt", game.IDENTIFIED,100)	
end

tl = TopicList()
tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)

if gstat == game.GUILD_IN or module_guildsPlayerGuildless(pl) then
    if qb:GetStatus(1) < game.QSTAT_DONE then
        tl:AddTopics({"quest", "explain%s+quest"}, topQuest)
        tl:AddTopics({"accept", "accept%s+quest"}, topAccept) 
        if qb:GetStatus(1) == game.QSTAT_SOLVED then
            tl:AddTopics({"complete", "quest%s+complete"}, topicQuestComplete) 
            tl:AddTopics({"complete", "quest%s+complete%s*#?(%d*)"}, topicQuestComplete)
        end
    end
end

tl:AddTopics({"cheat"}, cheat)
tl:AddTopics({"cheat1"}, cheat1)

tl:AddTopics({"archery"}, topArchery)
tl:AddTopics({"chereth", "rumors", "rumours", "rumor", "rumour"}, topChereth)

tl:AddTopics({"learn (sling)", "learn (bow)", "learn (cbow)" }, topic_learn)
tl:AddTopics({"train", "train (sling)", "train (bow)", "train (cbow)" }, topic_train)

tl:AddServices(nil, topic_services)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
