require("topic_list")
require("interface_builder")

local pl = event.activator
local me = event.me
local ib = InterfaceBuilder()

require("quest_builder")
local qb = QuestBuilder()

qb:AddQuest("Chereth's Eye", game.QUEST_KILLITEM)
local questnr = qb:Build(pl)

-- Guild checks
local guild_tag = "Mercenary"
local guild_stat = game.GUILD_NO
local guild_force = nil
guild_force = pl:GetGuild(guild_tag)
if guild_force ~= nil then
    guild_stat = guild_force.subtype
end

-- Archery skill checks
local skills    = {"bow archery", "crossbow archery", "sling archery"}
local skill_bow = game:GetSkillNr(skills[1])
local skill_cbow = game:GetSkillNr(skills[2])
local skill_sling = game:GetSkillNr(skills[3])
local pl_skill = -1
if pl:FindSkill(skill_bow) ~= nil then
    pl_skill = 1
elseif pl:FindSkill(skill_cbow) ~= nil then
    pl_skill = 2
elseif pl:FindSkill(skill_sling) ~= nil then
    pl_skill = 3
end

local function topicDefault()
    local join = "join"
    ib:SetTitle("Greetings!")
    ib:SetMsg("\n\nWelcome to the Mercenary guild.")
    --ib:SetHeader("st_001", me)
    if guild_stat ~= game.GUILD_IN then
        if guild_stat == game.GUILD_OLD then
            join = "rejoin"
        end
        ib:AddMsg("\n\nAs you are not a member, you should talk to ^Cashin^ upstairs first. ")
        ib:AddMsg("He will tell you how you can "..join..".\n\n")
        ib:AddMsg("Then come back and I will have more for you.")
    else
        ib:AddMsg("\n\nI am Chereth, and I used to teach archery here before I was blinded in a fight with some devilish creatures.\n\n")
        ib:AddMsg("Now ^Taleus^ teaches those skills.")
        if pl_skill > 0 then
            ib:AddMsg("\n\nBut you don't need me to tell you that. I can see that you are ")
            ib:AddMsg("skilled in "..skills[pl_skill].." already.")
        end
        ib:AddMsg("\n\nThen of course there is my dear friend ^Jahrlen^, across the room there.")
				
				if questnr==1 then
					local qstat = qb:GetStatus(questnr)
					if qstat == game.QSTAT_NO then
						ib:AddMsg("\n\nPerhaps you can do for me a favour?")
						ib:AddLink("Favour?", "quest")
					else
						ib:AddMsg("\n\nYou have the spider eye?")
						ib:AddLink("Finish " ..qb:GetName(questnr).. " quest", "quest")
					end
				end
    end
end

local function topicQuest()
		if questnr==0 then
			topicDefault()
		end
    ib:SetButton("Back", "hi")
		
		if questnr==1 then
			ib:SetTitle(qb:GetName(questnr))
			local qstat = qb:GetStatus(questnr)
		  if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE  then
				ib:AddMsg("\nGo deeper in the old guild cellar and search for a big black spider. Bring me an eye of this spider.\n\nDon't ask me for what i need this! I am blind, perhaps this is reason, or perhaps it's because i am the supply chief!\n\nIf you bring me the eye, i give you some coppers." )

				ib:SetDesc("Bring Chereth his spider eye.", 0, 0, 0, 0)
				ib:SetCoins(50, 0, 0, 0)
			end
		
			if qstat == game.QSTAT_NO then
				ib:AddLink("Start the "..qb:GetName(questnr).." Quest", "accept quest")
				ib:SetAccept(nil, "accept quest")
				ib:SetDecline(nil, "hi")
			end
		
			if qstat == game.QSTAT_SOLVED then
				ib:AddMsg("\nNice, you have the eye!\n")
				ib:SetDesc("Here are your coppers.", 0, 0, 0, 0)
				ib:SetCoins(50, 0, 0, 0)
				ib:SetAccept(nil, "finish quest")
				ib:SetDecline(nil, "hi")
			end
    end
end	

local function topicAcceptQuest()
		if questnr==0 then
			topicDefault()
		end

		if questnr==1 then
			local qstat = qb:GetStatus(questnr)
			if qstat == game.QSTAT_NO then
				qb:RegisterQuest(questnr, me, ib)
				local target = qb:AddQuestTarget(questnr, 1, 1, "spider_widow", "Giant Black Widow")
        target:AddQuestItem(1, "quest_object", "eye.101", "Spider eye")
			end
		  topicQuest()
		end
end

local function topicFinishQuest()
		ib:SetTitle("Quest Completed")

		if questnr==0 then
			topicDefault()
		end

		if questnr==1 then
			local qstat = qb:GetStatus(questnr)
			if qstat ~= game.QSTAT_SOLVED then
        topicQuest()
        return
			end
			ib:SetTitle("Quest completed")
			ib:SetMsg("Chereth smiles and gives you 50 coppers.")
			ib:SetCoins(50, 0, 0, 0)
			pl:AddMoney(50, 0, 0, 0)
			qb:Finish(questnr)
		end
end

local function topCashin()
    -- ib:SetHeader("st_004", me)
    ib:SetTitle("Cashin")
    ib:SetMsg("\n\nCashin is the Guild Master.\n\n")
    if guild_stat == game.GUILD_NO then
        ib:AddMsg("If you take his quest, he will grant you membership.")
    else
        ib:AddMsg("He will probably let you rejoin our guild if you ask him.")
    end
    ib:SetButton("Back", "hi")
end

local function topTaleus()
    -- ib:SetHeader("st_004", me)
    ib:SetTitle("Taleus")
    ib:SetMsg("\n\nTaleus is our Archery Commander now. He took over archery training for me after I lost my sight.")
    ib:AddMsg("\n\nHe has become an expert in all of the skills, and will be an excellent teacher for you.")
    ib:AddMsg("\n\nHe is usually practicing on our archery ranges outside, but if he has ")
    ib:AddMsg("gone off for more supplies someone at the range will surely know where you can find him.") 
    ib:SetButton("Back", "hi")
end

local function topJahrlen()
    -- ib:SetHeader("st_004", me)
    ib:SetTitle("Jahrlen")
    ib:SetMsg("\n\nAh yes, dear old Jahrlen. If you looked at him you wouldn't guess he is over 200 years old, would you?")
    ib:AddMsg("\n\nHe has a highly developed sense of humour, which is why we were laughing when you ")
    ib:AddMsg("came down the stairs.")
    ib:AddMsg("\n\nHe is our resident mage, and will probably be able to teach you some useful magic if you talk to him.")
    ib:SetButton("Back", "hi")
end

local tl = TopicList()
tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("cashin", topCashin)
tl:AddTopics("taleus", topTaleus)
tl:AddTopics("jahrlen", topJahrlen)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
 