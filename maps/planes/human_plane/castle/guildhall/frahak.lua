npc = event.me  -- we need this global for npc_trainer
player = event.activator -- we need this global for npc_trainer
-- todo update this to quest example logic

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
local qb = QuestBuilder()

local function questGoal(questnr)
  qb:AddQuestItem(questnr, 1, "quest_object", "letter.101", "Frah'aks Letter")
  player:Sound(0, 0, 2, 0)
  player:Write("You take the quest '".. qb:GetName(questnr) .."'.", game.COLOR_NAVY)
end

local function questReward(questnr)
  player:Sound(0, 0, 2, 0)
  local skill = game:GetSkillNr('find traps')
  player:AcquireSkill(skill, game.LEARN)
end

-- quest names must be unique
qb:AddQuest("Ogre Chief Frah'aks Letter", game.QUEST_ITEM, nil, nil, nil, nil, 1, questGoal, questReward)
qb:AddQuest("Frah'aks Mushrooms", game.QUEST_KILLITEM)
qb:AddQuest("Kill Kobold Shaman", game.QUEST_KILLITEM)

local questnr = qb:Build(player) -- qb:Build crashes when their are non quest added before
-- same for GetStatus when it targets a non existing quest number
-- looks like quest builder is a crash machine addquest without the functions leads to crashes when register quest

local function topicDefault()

  if questnr==0 then
    ib:SetTitle("Yo Want More Teaching")
    ib:AddMsg("\nAshahk! Yo want me teaching yo more trap skills?\nWill teach for money.\n")
    ib:AddLink("Yeah, show me your skills.", "training")
    return
  end

  if questnr==1 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      ib:SetTitle("Ogre Chief Frah'ak")
      ib:AddMsg("\nYo shut up.\nYo grack zhal hihzuk alshzu...\nMe mighty ogre chief.\nMe ^warrior^ ,will destroy yo. They come.\nGuard and ^kobolds^ will die then.")
      ib:AddLink("Kobolds?", "quest")
    else
      ib:SetTitle("Ogre Chief Frah'aks Letter Quest solved?")
      ib:AddMsg("You have the letter?")
      ib:AddLink("Finish Ogre Chief Frah'aks Letter Quest", "quest")
    end
  end
		
  if questnr==2 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      ib:SetTitle("Frah'aks Mushrooms")
      ib:AddMsg("\n\nDown there is shitty shaman.You kill him!\n\nDo this for mighty ogre chief Frah'ak. But first bring me some mushrooms. Frah'ak needs mushrooms. You help Frah'ak, me help you!")
      ib:AddLink("Mushrooms?", "quest")					
    else
      ib:SetTitle("Frah'aks Mushrooms")
      ib:AddMsg("You have mushrooms?")
      ib:AddLink("Finish Frah'aks Mushrooms Quest", "quest")
    end
  end

  if questnr==3 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      ib:SetTitle("Kill Kobold Shaman")
      ib:AddMsg("\n\nDown there is shitty shaman.You kill him!\n\nDo this for mighty ogre chief Frah'ak. And Frah'ka teach you Remove Traps skill!")
      ib:AddLink("Kill Shaman", "quest")					
    else
      ib:SetTitle("Kill Kobold Shaman")
      ib:AddMsg("You have killed nasty shaman?")
      ib:AddLink("Finish Frah'aks Shaman Quest", "quest")
    end
  end
end

local function topicQuest()
  if questnr==0 then
    topicDefault()
  end
  ib:SetButton("Back", "hi")

  if questnr==1 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE  then
      ib:SetTitle(qb:GetName(questnr))
      ib:AddMsg("\nKobolds traitors!\nGive gold for note, kobolds don't bring note to ogres.\nMe tell you: Kill kobold chief!\nMe will teach you find traps skill!\nShow me note i will teach you.\nKobolds in hole next room. Secret entry in wall." )

      ib:AddMsg("\n\nGet Frah'aks Letter.")
      ib:SetDesc("Bring Frah'ak his note from kobolds", 0, 0, 0, 0)
      -- todo alternate +exp reward
      -- if player:FindSkill(skill) == nil then
      ib:AddIcon("Find Traps Skill", "skill.101", " ") 
    end

    if qstat == game.QSTAT_NO then
      ib:AddLink("Start the Ogre Chief Frah'aks Letter Quest", "accept quest")
      ib:SetAccept(nil, "accept quest") 
      ib:SetDecline(nil, "hi")
    end

    if qstat == game.QSTAT_SOLVED then
      ib:AddMsg("\nAshahk! Yo bring me note!\nKobold chief bad time now, ha?\nNow me will teach you!\n")
      ib:SetDesc("here it is...", 0, 0, 0, 0)
      ib:AddIcon("Find Traps Skill", "skill.101", " ")
      ib:SetAccept(nil, "finish quest")
      ib:SetDecline(nil, "hi")
    end
    -- todo this don't trigger here, we need this in topic kobolds
    if qstat == game.QSTAT_DONE then
      ib:AddMsg("\nHaha, you slaying kobolds was fun.\nFrah'ak laughing the hole day!\n")
    end
  end

  if questnr==2 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
      ib:SetTitle("Frah'aks Mushrooms")
      ib:AddMsg("\nGo down, search cauldron with stinky kobold soup, go hidden hole, kobolds are sneaky!\n\nYou kill big mushrooms! Bring mushrooms to Frah'ak. Frah'aks gives you mighty amulet. Best amulet in world!")
      ib:SetDesc("Best amulet in world! From mighty Frah'ak. Ogre chief!", 0, 0, 0, 0)
      ib:AddIcon("Frah'aks Amulet", "amulet.101", "")
    end

    if qstat == game.QSTAT_NO then
      ib:AddLink("Start Frah'aks Mushrooms Quest", "accept quest")
      ib:SetAccept(nil, "accept quest") 
      ib:SetDecline(nil, "hi")
    end

    if qstat == game.QSTAT_SOLVED then
      ib:AddMsg("\nAshahk! Yo bring me mushrooms!\nKobold shaman bad time now, ha?\nNow me will give you mighty amulet!\n")
      ib:SetDesc("here it is...", 0, 0, 0, 0)
      ib:AddIcon("Frah'aks Amulet", "amulet.101", "")
      ib:SetAccept(nil, "finish quest")
      ib:SetDecline(nil, "hi")
    end
    if qstat == game.QSTAT_DONE then
      -- todo this don't trigger here, we need this in topic mushrooms
      ib:AddMsg("\nHaha, you slaying mushrooms was fun.\nFrah'ak laughing the hole day now from delicious mushrooms!\n")
    end
  end

  if questnr==3 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
      ib:SetTitle("Kill Shaman Quest")
      ib:AddMsg("\nGo down. Kill shaman. You kill him! Bring me his tooth\n\nDo this for mighty ogre chief Frah'ak. And Frah'ka teach you Remove Traps skill!")
      ib:AddMsg("\n\nGet tooth  from shaman.")
      ib:SetDesc("Bring Frah'ak tooth from shaman", 0, 0, 0, 0)
      -- todo alternate +exp reward
      -- if player:FindSkill(skill) == nil then
      ib:AddIcon("Remove Traps Skill", "skill.101", " ") 
    end

    if qstat == game.QSTAT_NO then
      ib:AddLink("Start Kill Shaman quest for Frah'ak", "accept quest")
      ib:SetAccept(nil, "accept quest") 
      ib:SetDecline(nil, "hi")
    end
	
    if qstat == game.QSTAT_SOLVED then
      ib:AddMsg("\nAshahk! Yo bring me tooth from shaman!\nKobold shaman bad time now, ha?\nNow Frah'ka teach you Remove Traps skill!\n")
      ib:SetDesc("here it is...", 0, 0, 0, 0)
      ib:AddIcon("Remove Traps Skill", "skill.101", " ") 
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
      qb:RegisterQuest(questnr, npc, ib)
    end
    topicQuest()
  end

  if questnr==2 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      qb:RegisterQuest(questnr, npc, ib)
      local target = qb:AddQuestTarget(questnr, 2, 2, "fungus", "Giant Fungus")
      target:AddQuestItem(2, "quest_object", "plant_mushroom1.101", "Mushroom")
    end
    topicQuest()
  end

  if questnr==3 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      qb:RegisterQuest(questnr, npc, ib)
      local target = qb:AddQuestTarget(questnr, 1, 1, "kobold_shaman", "Kobold Shaman")
      target:AddQuestItem(1, "quest_object", "tooth.101", "Shaman tooth")
    end
  topicQuest()
  end
end

local function topicFinishQuest()
  if questnr==0 then
    topicDefault()
  end

  if questnr==1 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      topicQuest()
      return
    end
    qb:Finish(questnr)
    ib:SetTitle("Quest Completed")
    ib:SetMsg("Frah'ak teaches you an ancient skill.")	
		end

  if questnr==2 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      topicQuest()
      return
    end
    qb:Finish(questnr)
    ib:SetTitle("Quest Completed")
    ib:SetMsg("Frah'ak gives you an amulet.")

    local amulet = game:LoadObject("arch amulet_generic\nidentified 1\nname Frah'ak Amulet\nresist_weaponmagic 20\nitem_level 5\nend")
    assert(amulet, "Could not create amulet!")
    amulet:MakeEgo(game.EGOITEM_MODE_UNBOUND)
    amulet:InsertInside(player)
  end

  if questnr==3 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      topicQuest()
      return
    end
    qb:Finish(questnr)
    player:Sound(0, 0, 2, 0)
    local skill = game:GetSkillNr('remove trap')
    player:AcquireSkill(skill, game.LEARN)
    ib:SetTitle("Quest Completed")
    ib:SetMsg("Frah'ak teaches you an ancient skill.")	
  end
end

local function topWarrior()
  ib:SetTitle("Warrior")
  ib:AddMsg("\nMe big chief. Me ogre destroy you.\nStomp on. Dragon kakka." )
  ib:SetButton("Back", "Hi") 
end

require("/scripts/npc_trainer.lua")
require("topic_list")
tl = TopicList()
-- tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("warrior", topWarrior)
tl:AddTopics("kobolds", topicQuest)

tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)

tl = trainerAddTopics(tl)
tl:AddServices(nil, topic_training)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
