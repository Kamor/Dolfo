-- script using npc_quest.lua, for more information see quest_example.lua and npc_quest.lua
player = event.activator
npc = event.me

require("interface_builder")
ib = InterfaceBuilder()

require("quest_builder")
qb = QuestBuilder()

quests =
{
  {name = "Lost Booze", type = game.QUEST_ITEM} -- we change this to level 2 requirement?
}

quest_items =
{
  {questnr=1, amount=1, type = "quest_object", icon = "booze.101", name = "Boozie"}
}

-- descriptions
function quest_description(questnr)
  if questnr == 1 then
    ib:SetDesc("Bring meh boozie from meh 'ouse fer meh. Meh 'ouse is aroun' on the east part o' town.", 0, 4, 0, 0)
  end
end

-- quest rewards
function quest_reward(questnr)
  if questnr==1 then
    ib:AddMsg("Meh boozie! Teres meh boozun... Tankee much. ... 'Ere, ave yer coin.")
    ib:SetCoins(4, 0, 0, 0)
    player:AddMoneyEx(0, 4, 0, 0)
  end
end

require("/scripts/npc_quest.lua")
addQuests()
questnr = qb:Build(player)

function topicGreetings()
  ib:SetTitle("Cheers")
  if questnr==0 then
    ib:SetMsg("Yeh ave yerself uh day o goodun!")
    ib:AddMsg("\n\n|**Carthson burps.**|\n")
    return
  end
  ib:SetMsg("|^Quest^|\n\n\n")
  ib:SetLHSButton("Quest")
  if questnr==1 then
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      ib:AddMsg("'Lo der chap. Yer look like uh nice feller, so take yeh a set an' 'ave a bet o meh boozish... Jus' uh sec... whas tis? Someun tooked meh boozie! Why dat varmet, I swears, wen I git em--ah, oh. I mustuv lef meh boozun a 'ome in meh booze ches'. Would yeh be uh nice feller an' fetch meh boozit fo' some coin? ")
      ib:AddLink("Start quest "..quests[questnr].name, "accept quest")
    else
      ib:AddMsg("Yeh found meh boozie yet? I been awaitin 'ere fo' it.")
      ib:AddLink("Finish quest "..quests[questnr].name, "finish quest")
    end
  end
end

-- this was the original answer when player lied
-- ib:AddMsg("Meh Boozie? Yous gots meh boozie? Giver 'ere! Wha? No, you don' gots meh boozie... comere when yeh gots it.\n")
-- this was the original changed description when player solved the quest, before finish quest
-- ib:SetDesc("Meh boozie! Teres meh boozun... Tankee much. ... 'Ere, ave yer coin.", 0, 4, 0, 0)
-- this was the original answer when player finished the quest
-- ib:SetMsg("Yeh ave yerself uh day o goodun!")

require("topic_list")
tl = TopicList()
tl:SetDefault(topicGreetings)
tl = questAddTopics(tl)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
