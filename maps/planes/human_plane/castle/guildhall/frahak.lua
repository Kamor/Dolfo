-- Ogre Chief Frah'aks Letter Quest using template for a "item quest" script

me = event.me  -- we need this global for included included npc_shop script
pl = event.activator -- we need this global for included included npc_shop script

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- 1 must be the correct lua definitions for the skills
-- 2 must be valid icon for npc dialog
-- we need this global ...
to_train =
{
	{skill_name = "Find Traps", icon = "tspikes.101"},
	{skill_name = "Remove Trap", icon = "tspikes.101"},
}

require("quest_builder")

local msg       = string.lower(event.message)
local qb        = QuestBuilder()
--local skill     = game:GetSkillNr('find traps')

local function questGoal(questnr)
    qb:AddQuestItem(questnr, 1, "quest_object", "letter.101", "Frah'aks Letter")
    pl:Sound(0, 0, 2, 0)
    pl:Write("You take the quest '".. qb:GetName(questnr) .."'.", game.COLOR_NAVY)
end

local function questReward(questnr)
    pl:Sound(0, 0, 2, 0)
    pl:AcquireSkill(skill, game.LEARN)
end

-- quest names must be unique
qb:AddQuest("Ogre Chief Frah'aks Letter", game.QUEST_ITEM, nil, nil, nil,
            nil, 1, questGoal, questReward)

local questnr = qb:Build(pl)
local qstat   = qb:GetStatus(1)

local function topicDefault()
    if qstat < game.QSTAT_DONE and pl:FindSkill(skill) == nil then
        if qstat == game.QSTAT_NO then
            ib:SetHeader("st_001", me)
            ib:SetTitle("Ogre Chief Frah'ak")
            ib:AddMsg("\nYo shut up.\nYo grack zhal hihzuk alshzu...\nMe mighty ogre chief.\nMe ^warrior^ ,will destroy yo. They come.\nGuard and ^kobolds^ will die then.")          
        else
            ib:SetHeader("st_001", me)
            ib:SetTitle("Ogre Chief Frah'aks Letter Quest solved?")
            ib:AddMsg("You have the letter?")
            ib:AddLink("Finish Ogre Chief Frah'aks Letter Quest", "checkq1")
        end
    else
        ib:SetHeader("st_005", me)
        ib:SetTitle("Yo Want More Teaching")
        ib:AddMsg("\nAshahk! Yo want me teaching yo more trap skills?\nWill teach for money.\n")
        ib:AddLink("Yeah, show me your skills.", "training")
    end
end

-- quest body (added to player quest obj for quest list)
local function quest_icons1()
    ib:AddIcon("Find Traps Skill", "skill.101", " ") 
end

local function quest_body1()
    ib:SetMsg("Get Frah'aks Letter.")
    ib:SetDesc("Bring Frah'ak his note from kobolds", 0, 0, 0, 0)
end

-- start: accept or decline the quest
local function topStartQ1()
    if qstat ~= game.QSTAT_NO then
        topicDefault()
        return
    end
    ib:SetHeader("st_003", me)
    ib:SetTitle(qb:GetName(questnr))
    quest_body1()
    quest_icons1()
    ib:SetAccept(nil, "acceptq1") 
    ib:SetDecline(nil, "hi") 
end

-- accepted: start the quest
local function topAcceptQ1()
    if qstat ~= game.QSTAT_NO then
        topicDefault()
        return
    end
    ib:SetHeader("st_003", me)
    quest_body1()
    quest_icons1()
    qb:RegisterQuest(questnr, me, ib)
end

-- try to finish: check the quest
local function topCheckQ1()
    if qstat == game.QSTAT_NO then
        topicDefault()
        return
    end
    ib:SetHeader("st_003", me)
    ib:SetTitle("Ogre Chief Frah'aks Letter Quest")
    --ib:SetMsg("The quest status is: ".. qstat .."\n\n")
    if qstat ~= game.QSTAT_SOLVED then
        ib:AddMsg("\nNah, bring Frah'ak note from ^kobolds^ first!\n")
        ib:SetButton("Back", "hi") 
    else
        ib:AddMsg("\nAshahk! Yo bring me note!\nKobold chief bad time now, ha?\nNow me will teach you!\n")
        ib:SetDesc("here it is...", 0, 0, 0, 0)
        quest_icons1()
        ib:SetAccept(nil, "finishq1") 
        ib:SetDecline(nil, "hi") 
    end
end

-- done: finish quest and give reward
local function topFinishQ1()
    if qstat ~= game.QSTAT_SOLVED then
        topicDefault()
        return
    end
    qb:Finish(1)
    
    ib:SetTitle("Quest Completed")
    ib:SetMsg("Frah'ak teaches you an ancient skill.")
    ib:SetButton("Ok", "hi") 
end

local function topWarrior()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Warrior")
    ib:AddMsg("\nMe big chief. Me ogre destroy you.\nStomp on. Dragon kakka." )
    ib:SetButton("Back", "Hi") 
end

local function topKobolds()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Kolbolds")
    ib:AddMsg("\nKobolds traitors!\nGive gold for note, kobolds don't bring note to ogres.\nMe tell you: Kill kobold chief!\nMe will teach you find traps skill!\nShow me note i will teach you.\nKobolds in hole next room. Secret entry in wall." )
    ib:AddLink("Start the Ogre Chief Frah'aks Letter Quest", "startq1")
    ib:SetButton("Back", "Hi")
end

require("/scripts/npc_trainer.lua")

require("topic_list")
tl = TopicList()
-- tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("warrior", topWarrior)
tl:AddTopics("kobolds", topKobolds)

if qb:GetStatus(1) < game.QSTAT_DONE then
    tl:AddTopics("startq1", topStartQ1) 
    tl:AddTopics("acceptq1", topAcceptQ1) 
    tl:AddTopics("checkq1", topCheckQ1) 
    tl:AddTopics("finishq1", topFinishQ1) 
end

tl = trainerAddTopics(tl)
tl:AddServices(nil, topic_training)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))