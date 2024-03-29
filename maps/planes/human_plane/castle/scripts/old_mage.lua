local npc = event.me
player = event.activator -- we need this global for npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- these must be the correct arch names. we need this global ...
for_sale =
{
  {arch = "robe"},
  {arch = "hat_feather"},
  {arch = "shoulder_cloth"},
  {arch = "bracers_cloth"},
  {arch = "girdle_cloth"},
  {arch = "coat_mage"},
  {arch = "gloves_cloth"},
  {arch = "pants"},
  {arch = "sandals_cloth"},
  {arch = "mithril_ring"},
  {arch = "mithril_armour"},
  {arch = "mithril_coat"}
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
