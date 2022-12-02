local npc = event.me
player = event.activator -- we need this global for npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- these must be the correct arch names. we need this global ...
for_sale =
{
  {arch = "small_shield"},
  {arch = "shield_round"},
  {arch = "shield"},
  
  {arch = "gauntlets_leather"},
  {arch = "gauntlets_spiked"},
  
  {arch = "leather_spiked"},
  {arch = "leather_studded"},
  {arch = "leather_cuirass"},
  
  {arch = "low_boots"},
  {arch = "spiked_boots"},
  {arch = "high_boots"},
  {arch = "leggings_leather"}, 
  {arch = "fighting_boots"},
  {arch = "kicking_boots"},
  {arch = "combat_boots"},
  {arch = "fencing_boots"},
  
  {arch = "helm_horn"}
 
}

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("Welcome to my small shop.")
  ib:AddMsg("You want to see my stuff?")
  ib:AddLink("Sure.","shop")
  ib:AddLink("No, not now.","")
  ib:SetLHSButton("Shop")
end

require("/scripts/npc_shop.lua")

require("topic_list")
tl = TopicList()
tl:SetDefault(topic_greeting)
tl = shopAddTopics(tl)
tl:AddServices(nil, topic_shop)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
