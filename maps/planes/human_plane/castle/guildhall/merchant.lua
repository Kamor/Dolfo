local me = event.me
pl = event.activator -- we need this global for included included npc_shop script

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- these must be the correct arch names. we need this global ...
for_sale =
{
	{arch = "shield_round"},
	{arch = "shield"},
	{arch = "gauntlets_leather"},
	{arch = "gauntlets_spiked"},
	{arch = "leather_cuirass"},
	{arch = "ring_mail"}
}

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("Welcome to my small shop.")
	ib:AddMsg("You want to see my stuff?")
	ib:AddLink("Sure.","Services")
	ib:AddLink("No, not now.","")
	ib:SetLHSButton("Services")
end

require("/scripts/npc_shop.lua")

require("topic_list")
tl = TopicList()
tl:SetDefault(topic_greeting)
tl = shopAddTopics(tl)
tl:AddServices(nil, topic_shop)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
