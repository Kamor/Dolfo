local npc = event.me
local player = event.activator

require("interface_builder")
local ib = InterfaceBuilder()

function topicGreetings()
  ib:SetTitle("Greetings")
  ib:SetMsg("|^cheat^| |^cheat1^|\n\n\n")
  ib:AddMsg("Psst. How did you find me?\n\nDon't tell the other npc's i am here and i can cheat you money and items.\n")
end

local function cheat()
  ib:SetHeader("st_005", npc)
  ib:SetMsg("Cheat money.")
  ib:SetCoins(0, 0, 0, 1)
  player:AddMoney(0, 0, 0, 1)
end

local function cheat1()
  ib:SetHeader("st_005", npc)
  ib:SetMsg("Cheat items.")
  
  player:CreateObjectInsideEx("sling_small", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("sstone", game.IDENTIFIED,100)

  player:CreateObjectInsideEx("bow_short", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("arrow", game.IDENTIFIED,100)

  player:CreateObjectInsideEx("crossbow_small", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("bolt", game.IDENTIFIED,100)	
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicGreetings)
tl:AddTopics({"cheat"}, cheat)
tl:AddTopics({"cheat1"}, cheat1)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
