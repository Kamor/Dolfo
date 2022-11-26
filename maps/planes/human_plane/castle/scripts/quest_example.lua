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

local pl        = event.activator
local me        = event.me

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
  {name = "Quest Example 4", type = game.QUEST_KILLITEM}
}
-- you can assign more than 1 item to one quest? todo test this
local quest_items =
{
  {questnr=2, amount=1, type = "quest_object", icon = "coppercoin.101", name = "quest example copper coin"}
}
-- you can assign more than 1 target to one quest? todo test this
local quest_targets =
{
  {questnr=3, amount=1, chance=1, arch="ant_red", name="Red Ant"},
  {questnr=4, amount=1, chance=1, arch="ant_red", name="Red Ant", id=1}
}
-- you can assign more than 1 kill item to a target? todo test this
local quest_kill_items =
{
  {id=1, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"}
}

local function quest_description(questnr)
  if questnr == 1 then
    ib:SetDesc("Open the chest to the right.")
  elseif questnr == 2 then
    ib:SetDesc("Open the chest to the right. Bring me my copper coin.")
  elseif questnr == 3 then
    ib:SetDesc("Kill one red ant.")
  elseif questnr == 4 then
    ib:SetDesc("Kill a red ant. Bring me it's head.")
  end
end

for i, x in quest do
  qb:AddQuest(x.name, x.type)
end

local function addQuestItem(questnr)
-- we want a check here for quest type? and what happens, when we put 2 items in here?
  for i, x in quest_item do
    if (x.questnr==questnr) then
      qb:AddQuestItem(questnr, x.amount, x.type, x.icon, x.name)
    end
  end
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
        local target=qb:AddQuestTarget(questnr, x.amount, x.chance, x.arch, x.name)
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
-- also the check if player has lied or not, who cares. only useless blowing up code
local function topicDefault()
  ib:SetTitle("Hello")
  if questnr<1 then
    ib:AddMsg("\nNo more quests.\n") -- questnr 0 is no more quest, questnr<0 is quests are somehow forbidden
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
      ib:AddLink("Start Quest Example 1", "accept quest") -- shortcut to start the quest
    else
      ib:AddMsg("Have you opened the chest?")
      ib:AddLink("Yes, i have.", "finish quest") -- shortcut to finish the quest, if player lies, this must be handled there
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
      ib:AddLink("Start Quest Example 2", "accept quest") -- shortcut to start the quest
    else
      ib:AddMsg("Have you opened the chest?")
      ib:AddLink("Yes, i have.", "finish quest") -- shortcut to finish the quest, if player lie, this must be handled there
    end
  end

  if questnr==3 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Quest Example 3 - Kill red ant - game.QUEST_KILL\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Triggered by killing the QuestTarget(s).\n")
      ib:AddMsg("This example is kill 1 red ant.\n")
      ib:AddLink("Start Quest Example 3", "accept quest") -- shortcut to start the quest
    else
      ib:AddMsg("Have you killed the ant?")
      ib:AddLink("Yes, i have.", "finish quest") -- shortcut to finish the quest, if player lie, this must be handled there
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
      ib:AddLink("Start Quest Example 4", "accept quest") -- shortcut to start the quest
    else
      ib:AddMsg("Have you found a ant head?")
      ib:AddLink("Yes, i have.", "finish quest") -- shortcut to finish the quest, if player lie, this must be handled there
    end
  end
end

-- short quest logic to handle the quest from quest topic
-- with accept and finish quest logic and also shows quest description and status (qb:AddItemList)
local function topicQuest()
  if questnr<1 then
    topicDefault()
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

local function topicAcceptQuest()
  if questnr<1 then
    topicDefault()
  end

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    RegisterQuest(questnr)
  end
  topicQuest()
end

local function topicFinishQuest()
  if questnr==0 then
    topicDefault()
  end
  ib:SetTitle(quest[questnr].name)

  if questnr==1 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      ib:SetMsg("|^Quest^|\n\n\n")
      ib:AddMsg("You need to open the chest first.\n")
      return
    end
    pl:Sound(0, 0, 2, 0)
    ib:SetMsg("You finished quest example 1!")
    qb:Finish(questnr)
  end
	
  if questnr==2 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      ib:SetMsg("|^Quest^|\n\n\n")
      ib:AddMsg("Bring me the quest example copper coin.\n")
      return
    end
    pl:Sound(0, 0, 2, 0)
    ib:SetMsg("You finished quest example 2!\n")
    ib:SetMsg("You can keep the coin!\n")
    ib:SetCoins(1, 0, 0, 0)
    pl:AddMoneyEx(1,0,0,0)
    qb:Finish(questnr)
  end
	
  if questnr==3 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      ib:SetMsg("|^Quest^|\n\n\n")
      ib:AddMsg("Kill one red ant.\n")
      return
    end
    pl:Sound(0, 0, 2, 0)
    ib:SetMsg("You finished quest example 3!\n")
    qb:Finish(questnr)
  end
	
  if questnr==4 then
    local qstat = qb:GetStatus(questnr)
    if qstat ~= game.QSTAT_SOLVED then
      ib:SetMsg("|^Quest^|\n\n\n")
      ib:AddMsg("Kill a red ant. Bring me it's head.\n")
      return
    end
    pl:Sound(0, 0, 2, 0)
    ib:SetMsg("You finished quest example 4!\n")
    qb:Finish(questnr)
  end
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
