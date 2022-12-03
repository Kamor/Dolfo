--
-- Karamor the Merchant. Lvl: no restriction. Kill Thieves beneath the guildhall wilderness. Reward: Lvl 5 leggings.
--

player = event.activator
npc = event.me

require("topic_list")
require("interface_builder")
ib = InterfaceBuilder()

require("quest_builder")
qb = QuestBuilder()

quests =
{
  {name = "Merchant's Trouble", type = game.QUEST_ITEM} -- normaly this quest would be better QUEST_KILLITEM
}

quest_items =
{
  {questnr=1, amount=1, type = "quest_object", icon = "rubbish.101", name = "Karamor's Stolen Items"}
}

function quest_description(questnr)
  if questnr == 1 then
    ib:SetDesc("Find whoever is stealing from the merchant, kill them, and bring back the loot. I will reward you with something from my private collection.")
    ib:AddIcon("Leather Leggins", "leggings_leather.101", "Well-made high leather boots")
  end
end

function quest_reward(questnr)
  if questnr==1 then
    ib:AddMsg("Here, please take those boots as a reward:")
    ib:AddIcon("Leather Leggins", "leggings_leather.101", "Well-made high leather boots")
    player:CreateObjectInsideEx("leggings_leather", 1, 1)
  end
end

require("scripts/npc_quest.lua")
addQuests()
questnr = qb:Build(player)

ib:SetHeader(npc, npc.name .. " the merchant")

function topicDefault()
  ib:SetTitle("Greetings")
  ib:SetMsg("Hello! I am " .. npc.name .. ", the owner of this ^shop^.\n\n")

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    ib:AddMsg("I'm having trouble with repeated break-ins. Could you help me, perhaps?\n\n")
    ib:AddLink("I'm interested", "explain quest")
  elseif qstat == game.QSTAT_ACTIVE then
    ib:AddMsg("So you still haven't found my items. I wonder if they are gone forever...")
    ib:AddLink("Explain Quest", "explain quest")
  elseif qstat == game.QSTAT_SOLVED then
    ib:AddMsg("Thank the Tabernacle! I never thought I'd see those things again\n\n")
    ib:AddLink("Finish Quest", "finish quest")
  end
end

function topicExplainQuest()
  if questnr==0 then
    ib:AddMsg("\nI have no more quests for you.\n")
    return
  end

  local qstat = qb:GetStatus(questnr)
  ib:SetTitle("Quest "..quests[questnr].name)

  ib:SetMsg("|^Quest^|\n\n\n")
  ib:SetLHSButton("Quest")
  ib:AddMsg("I have a big problem with break-ins.\n\n")
  ib:AddMsg("Someone has been breaking in and stealing equipment from my store for several nights now. Nobody has seen the thieves, but the guards have found hobgoblin tracks outside.\n\n")
  ib:AddMsg("If this keeps up I'll have to close down the shop! I need someone to track down the thieves, recover what has been stolen, and make sure they never do it again.\n\n")
  ib:AddMsg("The guards suspect hobgoblins, but they wouldn't be smart enough to sneak in here repeatedly. I even had ^Jahrlen^ put a warding spell on the shop, but it didn't help at all. If they are hobgoblins, they are really well organized.\n\n")
  ib:AddMsg("After the last break-in I followed the tracks into the eastern forest. The tracks just end right there near the northeastern corner of the guild hall compound. I couldn't figure out if they went over or under the wall.\n\n")
  ib:AddMsg("Be careful, though. These aren't any amateur shoplifters")
  if qstat == game.QSTAT_NO then
  ib:AddLink("Start quest "..quests[questnr].name, "accept quest")
  end
end

local function topicShop()
  ib:SetTitle("Shop")
  ib:SetMsg("\n\nYes, I'm trying to earn my living by supplying newcomers with high-quality equipment at premium prices.")
  ib:AddMsg("\n\nIf you want more specialized equipment you have to go to Stonehaven. But that can be a long and dangerous journey for the unexperienced.")
  ib:AddMsg("\n\nYou can enter the shop through the teleporter over there.")
  ib:AddMsg("\n\nIf you find something you like, just pick it up and exit through the teleporter on the other side. If you have enough money, you will be let through back automatically.")
  ib:AddMsg("\n\nIn case that you have something to sell, just drop it on the floor inside the fenced off area.")
end

local function topicJahrlen()
  ib:SetTitle("Jahrlen")
  ib:SetMsg("\n\nDon't you know him? He's the head mage of the mercenary guild. I guess he's better at offensive spells than he is at warding spells, otherwise I'd still have all of my inventory left.")
  ib:AddMsg("\n\nAnyhow, if you want to see him, I guess he is down in the guild.")
end

tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics("jahrlen", topicJahrlen) 
tl:AddTopics("shop", topicShop) 
tl = questAddTopics(tl)
tl:AddTopics("explain quest", topicExplainQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
