-- template for intelligent and understandable quest logic, easy to use for scripters
-- improved buttons, links, reduced redundancy
-- todo definition of an array, must see if this makes thinhs easier or worse

-- welcome (default)
--> introduction to npc
--> introduction to quest
--> quest button and side menu quest button
--> shortcut link in relation to quest status, to start or finish the quest

-- quest
--> this is all in one, in relation to quest status it is accept quest, view quest status and finish quest
---> it targets accept quest and finish quest function

-- accept quest
---> register quest
-- finish quest
---> finish quest, reward

-- quest array todo must see if this can be more usefully, currently quest name comes from here
-- quest goal todo optimize
-- quest body, should be the description, scripters want to see in quest description on function quest here and in quest menu

require("topic_list")
local pl        = event.activator
local me        = event.me

require("interface_builder")
local ib = InterfaceBuilder()

require("quest_builder")
local qb        = QuestBuilder()
local quest =
{
 {name = "Lost Booze", type = game.QUEST_ITEM, level = 1, skill = game.ITEM_SKILL_NO}
}

local function questGoal(questnr)
 qb:AddQuestItem(questnr, 1, "quest_object", "booze.101", "Boozie")
 pl:Sound(0, 0, 2, 0)
 pl:Write("You take the quest '".. qb:GetName(questnr) .."'.", game.COLOR_NAVY)
end

local function quest_body1()
 ib:SetDesc("Bring meh boozie from meh 'ouse fer meh. Meh 'ouse is aroun' on the east part o' town.", 0, 4, 0, 0)
end

-- quest name, quest type, quest level, quest skill, ? , ?, 1, quest goal, ?
qb:AddQuest("Lost Booze", game.QUEST_ITEM, nil, nil, nil, nil, 1, questGoal, nil)
local questnr = qb:Build(pl)

local function topicDefault()
 ib:SetTitle("Cheers")
 if questnr==0 then
  ib:AddMsg("\nCarthson burps.\n")
  return
 end
 ib:SetMsg("|^Quest^|\n\n\n")
 if questnr==1 then
  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
   ib:AddMsg("'Lo der chap. Yer look like uh nice feller, so take yeh a set an' 'ave a bet o meh boozish... Jus' uh sec... whas tis? Someun tooked meh boozie! Why dat varmet, I swears, wen I git em--ah, oh. I mustuv lef meh boozun a 'ome in meh booze ches'. Would yeh be uh nice feller an' fetch meh boozit fo' some coin? ")
   ib:AddLink("Yessir", "accept quest")
  else
   ib:AddMsg("Yeh found meh boozie yet? I been awaitin 'ere fo' it.")
   ib:AddLink("Yup, here it is", "finish quest")
  end
  ib:SetLHSButton("Quest")
 end
end

local function topicQuest()
 if questnr==0 then
  topicDefault()
 end
	
 if questnr==1 then
  local qstat = qb:GetStatus(questnr)
  ib:SetTitle(quest[questnr].name)
  if qstat == game.QSTAT_ACTIVE or qstat == game.QSTAT_SOLVED then
   qb:AddItemList(questnr, ib)
  end
  if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
   quest_body1()
  end
  if qstat == game.QSTAT_NO then
   ib:SetAccept(nil, "accept quest")
   ib:SetDecline(nil, "hi")
  end
  if qstat == game.QSTAT_SOLVED then
   ib:SetDesc("Meh boozie! Teres meh boozun... Tankee much. ... 'Ere, ave yer coin.", 0, 4, 0, 0)
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
   quest_body1() -- is there no better way to send description to questbuilder? 
   qb:RegisterQuest(questnr, me, ib)
  end
  topicQuest()
 end
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
   ib:AddMsg("Meh Boozie? Yous gots meh boozie? Giver 'ere! Wha? No, you don' gots meh boozie... comere when yeh gots it.\n")
   ib:SetLHSButton("Quest")
   return
  end
  pl:Sound(0, 0, 2, 0)
  pl:AddMoneyEx(0,4,0,0)
  ib:SetMsg("Yeh ave yerself uh day o goodun!")
  qb:Finish(questnr)
 end
end

tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
