npc = event.me  -- we need this global for npc_trainer
player = event.activator -- we need this global for npc_trainer

require("interface_builder")
ib = InterfaceBuilder() -- we need this global ...

-- 1 must be the correct lua definitions for the skills
-- 2 must be valid icon for npc dialog
-- we need this global ...
to_train =
{
  {skill_name = "Cleave Weapons", icon = "axe_small.101"},
  {skill_name = "Impact Weapons", icon = "club.101"},
  {skill_name = "Pierce Weapons", icon = "dagger.101"},
  {skill_name = "Slash Weapons", icon = "shortsword.101"},
  {skill_name = "Throwing", icon = "t_dagger.101"}
}

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("Ey, you want some weapon training?")
  ib:AddMsg("\n\nI know a lot about weapons.\n")
  ib:AddMsg("\n\nI can train you to handle cleave, impact, pierce, slash weapons and can show you, how to become better in this skills.\n")
  ib:AddMsg("\n\nI also know some basics about throwing weapon skills, if you need some training there.\n")
  ib:AddLink("Sure.","training") -- or "Training"
  ib:AddLink("No, not now.","")
  ib:SetLHSButton("Training","training") -- or ib:SetLHSButton("Training")
end

require("/scripts/npc_trainer.lua")

require("topic_list")
tl = TopicList()
tl:SetDefault(topic_greeting)
tl = trainerAddTopics(tl)
tl:AddServices(nil, topic_training)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        
