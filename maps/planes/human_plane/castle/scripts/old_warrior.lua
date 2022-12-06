local npc = event.me
player = event.activator -- we need this global for npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

groups =
{
  {name = "mail", type = 16},
  {name = "helm", type = 34}
}

-- these must be the correct arch names. we need this global ...
for_sale =
{
  {arch = "ring_mail"},
  {arch = "chain_mail"},
  {arch = "scale_armour"},
  {arch = "brigandine"},
  {arch = "lamellar_mail"},
  {arch = "scale_mail"},
  {arch = "splint_mail"},
  {arch = "banded_mail"},
  {arch = "chain_hauberk"},
  {arch = "bplate_mail"},
  {arch = "breastplate"},
  {arch = "plate_field"},
  {arch = "plate_full"},
  {arch = "plate_mail"},
  --{arch = "panoply_mail"},
  --{arch = "maximillian_mail"},
  --{arch = "dragon_plate_armour"},
  --{arch = "dragon_chain_armour"}
  
  {arch = "helm_horn"},
  {arch = "coif_scale"},
  {arch = "helm_iron"},
  {arch = "coif_chain"},
  {arch = "full_helmet"},
  {arch = "helm_bronze"},
  {arch = "helm_samurai"},
  {arch = "helm_field"},
  {arch = "helm_battle"},
  {arch = "helm_war"},
  {arch = "helm_plate"},
  {arch = "helm_guard"}
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
