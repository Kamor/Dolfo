-- template for intelligent and understandable quest logic, easy to use for scripters
-- improved buttons, links, reduced redundancy

-- welcome and quest intro (default)
--> introduction to npc
--> introduction to quest
--> quest button and side menu quest button
--> short cut links in relation to quest status, to start or finish the quest
--> sometimes it could be wise to split welcome from quest intro, then quest intro could be a separate topic

-- quest
--> this is all in one, in relation to quest status it is accept quest, view quest status and finish quest
---> it targets accept quest and finish quest function

-- accept quest
---> register quest
-- finish quest
---> finish quest, reward

local pl = event.activator
local me = event.me

require("interface_builder")
local ib = InterfaceBuilder()

require("quest_builder")
local qb        = QuestBuilder()
-- quest name, quest type, quest level, quest skill, ? , ?, 1, quest goal, ?
local quest =
{
  {name = "Quest Example 1", type = game.QUEST_NORMAL},
  {name = "Quest Example 2", type = game.QUEST_ITEM},
  {name = "Quest Example 3", type = game.QUEST_KILL},
  {name = "Quest Example 4", type = game.QUEST_KILLITEM},
  {name = "5 - Kill 2 red ants", type = game.QUEST_KILL},
  {name = "6 - Kill 2 red ants, Chance 50%", type = game.QUEST_KILL},
  {name = "7 - Kill red ant and black ant", type = game.QUEST_KILL},
  {name = "8 - Get ant head and ant eye", type = game.QUEST_KILLITEM},
  {name = "9 - Kill queen, get queen head", type = game.QUEST_KILLITEM}
}
-- you can assign more than 1 item to one quest? todo test this
local quest_items =
{
  {questnr=2, amount=1, type = "quest_object", icon = "coppercoin.101", name = "quest example copper coin"}
}
-- you can assign more than 1 target to one quest.
local quest_targets =
{
  {questnr=3, chance=1, amount=1, arch="ant_red", name="Red Ant"},
  {questnr=4, chance=1, amount=1, arch="ant_red", name="Red Ant", id=1},
  {questnr=5, chance=1, amount=2, arch="ant_red", name="Red Ant"},
  {questnr=6, chance=2, amount=2, arch="ant_red", name="Red Ant"},
  {questnr=7, chance=1, amount=1, arch="ant_red", name="Red Ant"},
  {questnr=7, chance=1, amount=1, arch="ant_soldier", name="Black Ant"},
  {questnr=8, chance=1, amount=1, arch="ant_red", name="Red Ant", id=2},
  {questnr=8, chance=1, amount=1, arch="ant_soldier", name="Black Ant", id=3},
  {questnr=9, chance=1, amount=1, arch="ant_queen", name="Ant Queen", id=4}
}
-- you can assign 1 kill item to each target
local quest_kill_items =
{
  {id=1, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=2, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=3, amount=1, type = "quest_object", icon = "eye_beholder.101", name = "Ant Eye"},
  {id=4, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"}
}

-- you can also define a description with a reward
local function quest_description(questnr)
  if questnr == 1 then
    ib:SetDesc("Open the chest to the right.")
  elseif questnr == 2 then
    ib:SetDesc("Open the chest to the right. Bring me my copper coin.")
  elseif questnr == 3 then
    ib:SetDesc("Kill one red ant.")
  elseif questnr == 4 then
    ib:SetDesc("Kill a red ant. Bring me it's head.")
  elseif questnr == 5 then
    ib:SetDesc("Kill two red ant.")
  elseif questnr == 6 then
    ib:SetDesc("Kill two red ant. You need also luck to get the kill.")
  elseif questnr == 7 then
    ib:SetDesc("Kill one red ant and one black ant.")
  elseif questnr == 8 then
    ib:SetDesc("Get red ant head and black ant eye.")
  elseif questnr == 9 then
    ib:SetDesc("Kill the queen. Bring me here head.",10,0,0,0)
  end
end

for i, x in quest do
  qb:AddQuest(x.name, x.type)
end

local function RegisterQuest(questnr)
  quest_description(questnr) -- is there no better way to send description to quest builder?
  qb:RegisterQuest(questnr, me, ib)

  if quest[questnr].type == game.QUEST_ITEM then
    for i, x in quest_items do
      if (x.questnr==questnr) then
        qb:AddQuestItem(questnr, x.amount, x.type, x.icon, x.name)
      end
    end
  elseif quest[questnr].type == game.QUEST_KILL or quest[questnr].type == game.QUEST_KILLITEM then
    for i, x in quest_targets do
      if (x.questnr==questnr) then
        local target=qb:AddQuestTarget(questnr, x.chance, x.amount, x.arch, x.name)
        if (x.id ~= nil) then
          for j, y in quest_kill_items do
            if (x.id==y.id) then
              target:AddQuestItem(y.amount, y.type, y.icon, y.name)
            end
          end
        end
      end
    end
  end
end

local questnr = qb:Build(pl)
-- hello (default), here are mostly the unique npc's dialogs
-- some npc's want to have a kind of "can you do something for me?"
-- so here we need a split between hello and quest story - TODO - must also see, where we go back with buttons? to hi?
-- but except this split, hello should build a sitelink topic to quest
-- and should have shortcuts to fast except and fast finish quests.
-- an optional dialog like "I have not finished your quest" is useless and not worth the work for the scripters.
-- also the check if player has lied or not, who cares. Only useless blowing up code.
local function topicDefault()
  ib:SetTitle("Hello")
  if questnr<1 then
    ib:AddMsg("\nI have no more quests for you.\n") -- questnr 0 is no more quest, questnr<0 is quests are somehow forbidden
    return
  end
  ib:SetMsg("|^Quest^|\n\n\n") -- show quest topic in side menu
  ib:SetLHSButton("Quest")

  local qstat = qb:GetStatus(questnr)
  if questnr==1 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example 1 - Open a chest - game.QUEST_NORMAL\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("- quest trigger QUEST_NORMAL\n")
      ib:AddMsg("- name = questname\n")
      ib:AddMsg("- quest step (last_heal) = 1\n")
      ib:AddLink("Start Quest Example 1", "accept quest") -- shortcut start quest
    else
      ib:AddMsg("Have you opened the chest?")
      ib:AddLink("Yes, i have.", "finish quest") -- shortcut finish quest
    end
  end

  if questnr==2 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example 2 - Open a chest - game.QUEST_ITEM\n\n")
      ib:AddMsg("Without a defined quest item, this quest is always SOLVED when started.")
      ib:AddMsg(" With a defined quest item, this quest is nearly same to first example, except we get a quest item.\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("With quest trigger QUEST_ITEM\n")
      ib:AddMsg("With name = questname\n")
      ib:AddMsg("And quest step (last_heal) = 1\n")
      ib:AddLink("Start Quest Example 2", "accept quest")
    else
      ib:AddMsg("Have you opened the chest?")
      ib:AddLink("Yes, i have.", "finish quest")
    end
  end

  if questnr==3 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example 3 - Kill red ant - game.QUEST_KILL\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Triggered by killing the QuestTarget(s).\n")
      ib:AddMsg("This example is kill 1 red ant.\n")
      ib:AddLink("Start Quest Example 3", "accept quest")
    else
      ib:AddMsg("Have you killed the ant?")
      ib:AddLink("Yes, i have.", "finish quest")
    end
  end

  if questnr==4 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example 4 - Kill read ant - game.QUEST_KILLITEM\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Without a defined QuestItem, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("With a defined QuestItem, this quest is nearly same to the KILL quest, except we get items on kill.\n\n")
      ib:AddMsg("Triggered by killing the target(s).\n")
      ib:AddMsg("This is kill one red ant, get their heat.\n\n")
      ib:AddLink("Start Quest Example 4", "accept quest")
    else
      ib:AddMsg("Have you found a ant head?")
      ib:AddLink("Yes, i have.", "finish quest")
    end
  end

  if questnr>4 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example ".. questnr.."\n\n")
      ib:AddLink("Start "..quest[questnr].name, "accept quest")
    else
      ib:AddMsg("Have you finished the quest?")
      ib:AddLink("Finish "..quest[questnr].name, "finish quest")
    end
  end
end

-- quest logic to handle the quest from quest topic
-- with accept and finish quest logic and also shows quest description and status (qb:AddItemList)
local function topicQuest()
  if questnr<1 then
    topicDefault()
    return
  end
  local qstat = qb:GetStatus(questnr)
  ib:SetTitle(quest[questnr].name)
  if qstat == game.QSTAT_ACTIVE or qstat == game.QSTAT_SOLVED then
    qb:AddItemList(questnr, ib)
  end
  if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
    quest_description(questnr)
  end
  if qstat == game.QSTAT_NO then
    ib:SetAccept("Accept", "accept quest")
    ib:SetDecline(nil, "hi")
  end
  if qstat == game.QSTAT_SOLVED then
    quest_description(questnr)
    ib:SetAccept("Finish", "finish quest")
    ib:SetDecline(nil, "hi")
  end
end

-- accept quest
local function topicAcceptQuest()
  if questnr<1 then
    topicDefault()
    return
  end

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    RegisterQuest(questnr)
  end
  topicQuest()
end

-- finish quest todo, more dynamic, split unique parts from equal parts?
local function topicFinishQuest()
  if questnr<1 then
    topicDefault()
    return
  end
		
  ib:SetTitle(quest[questnr].name)
		local qstat = qb:GetStatus(questnr)
  if qstat ~= game.QSTAT_SOLVED then
    topicQuest()
    return
  end

  --pl:Sound(0, 0, 2, 0) -- we have quest finish sound, we want also a second sound?

  if questnr==1 then
    ib:SetMsg("You finished quest example 1!")
    qb:Finish(questnr)
  end
	
  if questnr==2 then
    ib:SetMsg("You finished quest example 2!\n")
    ib:SetMsg("You can keep the coin!\n")
    ib:SetCoins(1, 0, 0, 0)
    pl:AddMoneyEx(1,0,0,0)
    qb:Finish(questnr)
  end
	
  if questnr==3 then
    ib:SetMsg("You finished quest example 3!\n")
    qb:Finish(questnr)
  end
	
  if questnr==4 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      topicQuest()
    end
    ib:SetMsg("You finished quest example 4!\n")
    qb:Finish(questnr)
  end
		
  if questnr>4 then
    ib:SetMsg("You finished quest example "..questnr..".\n\n"..quest[questnr].name.."!\n")
    qb:Finish(questnr)
  end
		
  if questnr==9 then
		  ib:SetMsg("Here is your reward!\n")
    ib:SetCoins(10, 0, 0, 0)
    pl:AddMoneyEx(10,0,0,0)
  end
		
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
