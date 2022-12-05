local npc = event.me
local player = event.activator

require("interface_builder")
local ib = InterfaceBuilder()

function topicGreetings()
  ib:SetTitle("Greetings")
  ib:SetMsg("|^cheat^| |^cheat1^| |^cheat card^|  |^cheat homestone^| \n\n\n")
  ib:AddMsg("Psst. How did you find me?\n\nDon't tell the other npc's i am here and i can cheat you money and items.\n")
end

local function cheat()
  ib:SetMsg("Cheat money.")
  ib:SetCoins(0, 0, 0, 1)
  player:AddMoney(0, 0, 0, 1)
end

local function cheat1()
  ib:SetMsg("Cheat items.")
  
  player:CreateObjectInsideEx("sling_small", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("sstone", game.IDENTIFIED,100)

  player:CreateObjectInsideEx("bow_short", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("arrow", game.IDENTIFIED,100)

  player:CreateObjectInsideEx("crossbow_small", game.IDENTIFIED,1)
  player:CreateObjectInsideEx("bolt", game.IDENTIFIED,100)	
end

local function creditcard()
  ib:SetTitle("Cheat creditcard")
	ib:AddMsg("Here you have the prototyp of a creditcard!")
	ib:AddMsg("\n\nYou also need to go to a bank and open a bank account first! Later credit cards would be sold there.")
	ib:AddMsg("\n\nTo use this card, you also need to mark (M-Key) it before!")
	player:CreateObjectInsideEx("credit_card", game.IDENTIFIED, 1)
end

local function homestone()
  ib:SetTitle("Cheat homestone")
	ib:AddMsg("Here you have a homestone!")
	player:CreateObjectInsideEx("homestone", game.IDENTIFIED, 1)
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicGreetings)
tl:AddTopics("cheat", cheat)
tl:AddTopics("cheat1", cheat1)
tl:AddTopics("cheat card", creditcard)
tl:AddTopics("cheat homestone", homestone)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
