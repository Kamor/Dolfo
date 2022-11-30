npc = event.me  -- we need this global for npc_trainer
player = event.activator -- we need this global for npc_trainer and npc_shop

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- these must be the correct arch names. we need this global ...
for_sale =
{
  {arch = "helmet_ag"},
  {arch = "bracers_ag"},
  {arch = "armour_ag"},
  {arch = "belt_ag"},
  {arch = "pants_ag"},
  {arch = "boots_ag"}
}

-- 1 must be the correct lua definitions for the skills
-- 2 must be valid icon for npc dialog
-- we need this global ...
to_train =
{
  {skill_name = "Punching", icon = "gauntlets_chain.101"}
}

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("Welcome to my small shop.")
  ib:AddMsg("You want to see my stuff?")
  ib:AddMsg("\n\nOr should i train you in punching skill?")
  ib:AddLink("Sure, show me what you sell." , "shop")
  ib:AddLink("Yeah, train me in punching." , "training")
end

require("/scripts/npc_shop.lua")
require("/scripts/npc_trainer.lua")

require("topic_list")
tl = TopicList()
tl:SetDefault(topic_greeting)
tl = shopAddTopics(tl)
tl = trainerAddTopics(tl)
tl:AddServices(nil, topic_shop)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
