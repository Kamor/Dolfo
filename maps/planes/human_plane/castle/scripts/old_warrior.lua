local npc = event.me
player = event.activator -- we need this global for npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

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
  {arch = "plate_mail"}
  --{arch = "panoply_mail"},
  --{arch = "maximillian_mail"},
  --{arch = "dragon_plate_armour"},
  --{arch = "dragon_chain_armour"}
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
