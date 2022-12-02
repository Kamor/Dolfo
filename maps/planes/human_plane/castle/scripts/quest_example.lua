--[[
example for intelligent and understandable quest logic, easy to use for scripters
improved buttons, links, reduced redundancy
uniform parts of quest logics are splitted to npc_quest.lua
unique parts of quest logic must be set by scripters, like in this example
scripters needs to implement npc_quest.lua, define their quests, call addQuests and call questAddTopics
base vars and functions must be global to work with npc_quest.lua
more informations about implementing npc_quest.lua see there

scripters can define there quests very easy with arrays.
if scripters want more than the command interface to target quest logics, they need to connect this somehow intelligent
example to do this, you find in this script in function topicDefault (hello)
in this example hello text and quest intro text is also combined in one topic

so scripters must work
- introduction to npc
- introduction to quest
- quest button and side menu quest button
- short cut links in relation to quest status, to start or finish the quest
- sometimes it could be wise to split welcome from quest intro, then quest intro could be a separate topic
]]

player = event.activator  -- we need this global for npc_quest
npc = event.me

require("interface_builder")
ib = InterfaceBuilder()

require("quest_builder")
qb = QuestBuilder()
-- quest name, type, level, skillgroup, required , finalstep, repeats, questGoal, questReward, silent

-- ?looks like required can be a function, which is called when players don't have the requirements to do this quest
-- ?looks like repeats can be used to repeat quests, so far it only worked for me with nil or 1
-- ?also server hangs massiv when using >1 and finish quest.
-- finalstep>1 somehow can not finish the quests here
-- so for the moment finalstep and repeats are limited to default=1, you can also use nil
-- it's a pain, there are no working examples on public trunk and reading deeper in quest_builder.lua only reveals
--> that quest_builder.lua targets quest_manager.lua
--> that's a joke saying quest_manager.lua is deprecated, but using it as kernel for quest_builder.lua *lol*
-- questGoal can be a function, which is launched when quest is started (registered)
---> currently not implemented, we show quest goals in welcome/default topic
-- questReward can be a function, which is launched when quest is finished
---> currently not implemented, we hardjump to quest_reward, when quest is finished
quests =
{
  {name = "Example 1", type = game.QUEST_NORMAL},
  {name = "Example 2", type = game.QUEST_ITEM},
  {name = "Example 3", type = game.QUEST_KILL},
  {name = "Example 4", type = game.QUEST_KILLITEM},
  {name = "Example 5", type = game.QUEST_KILL},
  {name = "Example 6", type = game.QUEST_KILL},
  {name = "Example 7", type = game.QUEST_KILL},
  {name = "Example 8", type = game.QUEST_KILLITEM},
  {name = "Example 9", type = game.QUEST_KILLITEM},
  {name = "Example 10", type = game.QUEST_KILLITEM, level=5}, -- can't say if lua checks also for drained levels?
		
  -- !using skillgroup restriction
  -- wonderful, questbuilder uses different constants for skillgroups
  -- for the moment we best target skillgroup by +1, see daimonin_game.c
  -- {"ITEM_SKILL_NO",          0},
  -- {"ITEM_SKILL_AGILITY",     SKILLGROUP_AGILITY + 1},
  -- {"ITEM_SKILL_PERSONALITY", SKILLGROUP_PERSONAL + 1},
  -- {"ITEM_SKILL_MENTAL",      SKILLGROUP_MENTAL + 1},
  -- {"ITEM_SKILL_PHYSICAL",    SKILLGROUP_PHYSIQUE + 1},
  -- {"ITEM_SKILL_MAGIC",       SKILLGROUP_MAGIC + 1},
  -- {"ITEM_SKILL_WISDOM",      SKILLGROUP_WISDOM + 1},
  -- !if someone fix this mess in questbuilder in future, this must be fixed here to
  {name = "Example 11", type = game.QUEST_ITEM, level=2, skillgroup=game.SKILLGROUP_AGILITY+1}
}
-- you can assign more than 1 item to one quest
-- todo how to split these items not to be found in same places, quest steps? repeats?
quest_items =
{
  {questnr=2, amount=1, type = "quest_object", icon = "coppercoin.101", name = "quest example copper coin"},
  {questnr=11, amount=1, type = "quest_object", icon = "book.101", name = "Questus' book"},
  {questnr=11, amount=1, type = "quest_object", icon = "dagger.101", name = "Questus' dagger"},
  {questnr=11, amount=1, type = "quest_object", icon = "shirt.101", name = "Questus' shirt"},
  {questnr=11, amount=1, type = "quest_object", icon = "pants.101", name = "Questus' pants"}
}

-- you can assign more than 1 target to one quest.
quest_targets =
{
  {questnr=3, chance=1, amount=1, arch="ant_red", name="Red Ant"},
  {questnr=4, chance=1, amount=1, arch="ant_red", name="Red Ant", id=1},
  {questnr=5, chance=1, amount=2, arch="ant_red", name="Red Ant"},
  {questnr=6, chance=2, amount=2, arch="ant_red", name="Red Ant"},
  {questnr=7, chance=1, amount=1, arch="ant_red", name="Red Ant"},
  {questnr=7, chance=1, amount=1, arch="ant_soldier", name="Black Ant"},
  {questnr=8, chance=1, amount=1, arch="ant_red", name="Red Ant", id=2},
  {questnr=8, chance=1, amount=1, arch="ant_soldier", name="Black Ant", id=3},
  {questnr=9, chance=1, amount=1, arch="ant_queen", name="Ant Queen", id=4},
  {questnr=10, chance=1, amount=1, arch="pudding_black", name="shivering black pudding", id=5}
}
-- you can assign 1 kill item to each target
quest_kill_items =
{
  {id=1, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=2, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=3, amount=1, type = "quest_object", icon = "eye_beholder.101", name = "Ant Eye"},
  {id=4, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=5, amount=1, type = "quest_object", icon = "pudding_black.111", name = "Black Pudding"}
}

-- descriptions todo is there a better way to handle quest descriptions?
function quest_description(questnr)
  if questnr == 1 then
    ib:SetDesc("Open the chest to the right.")
  elseif questnr == 2 then
    ib:SetDesc("Open the chest to the right. Bring me my copper coin.")
  elseif questnr == 3 then
    ib:SetDesc("Kill one red ant.")
  elseif questnr == 4 then
    ib:SetDesc("Kill a red ant. Bring me it's head.")
  elseif questnr == 5 then
    ib:SetDesc("Kill two red ants.")
  elseif questnr == 6 then
    ib:SetDesc("Kill two red ants. You need also luck to get the kill (50%).")
  elseif questnr == 7 then
    ib:SetDesc("Kill one red ant and one black ant.")
  elseif questnr == 8 then
    ib:SetDesc("Kill one red ant and one black ant. Get red ant head and black ant eye.")
  elseif questnr == 9 then
    -- you can also define a description with a reward
    ib:SetDesc("Kill the queen. Bring me her head.",10,0,0,0)
  elseif questnr == 10 then
    ib:SetDesc("Kill the shivering black pudding under guildhall. Search behind a secret wall, between these slugs and goblins. Bring me some pudding.")
  elseif questnr == 11 then
    ib:SetDesc("Bring Questus his shirt, pant, dagger and book from the chests to the left. Carefully they are trapped.")
  end
end

-- quest rewards
function quest_reward(questnr)
  if questnr==2 then
    ib:AddMsg("You can keep the coin!\n")
    ib:SetCoins(1, 0, 0, 0)
    player:AddMoneyEx(1, 0, 0, 0)
  elseif questnr==9 then
    ib:AddMsg("Here is your reward!\n")
    ib:SetCoins(10, 0, 0, 0)
    player:AddMoneyEx(10, 0, 0, 0)
  elseif questnr==10 then
    ib:AddMsg("Here is your reward! Don't ask me what is in these cookies. He smiles.\n")
    ib:AddIcon("Cookies", "cookie01.101", "|** Delicious cookies with a secret special ingredient**|", 5)
    local ob = game:CreateObject("cookie", game.IDENTIFIED, 5)
    if (ob~=nil) then
      player:PickUp(ob, nil, 5)
    end
  else
    ib:AddMsg("You finished this Quest!\n")
  end
end

require("scripts/npc_quest.lua")
addQuests()
questnr = qb:Build(player)

-- in this example hello and default is same
-- for a good flow on links, sitelinks and buttons
-- we have build in
--> a sitelink topic to quest *1
--> shortcuts to fast except and fast finish quest. *2
--> a left click button to quest. *3
function topicDefault()
  ib:SetTitle("Hello")
   if questnr<0 then
    ib:SetMsg("|^Quest^|\n\n\n") -- *1 show quest topic in side menu
    ib:AddMsg("Sorry, i think you need some more training for the next quest.")
    return
  end
  if questnr==0 then
    ib:AddMsg("\nI have no more quests for you.\n")
    return
  end
  ib:SetMsg("|^Quest^|\n\n\n") -- *1 show quest topic in side menu 
  ib:SetLHSButton("Quest") -- *3
  ib:AddMsg("|Quest "..quests[questnr].name.."|\n\n")

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    if questnr==1 then
      ib:AddMsg("Open a chest - game.QUEST_NORMAL\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("- quest trigger QUEST_NORMAL\n")
      ib:AddMsg("- name = questname\n")
      ib:AddMsg("- quest step (last_heal) = 1\n")
    elseif questnr==2 then
      ib:AddMsg("Open a chest - game.QUEST_ITEM\n\n")
      ib:AddMsg("Without a defined quest item, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("With a defined quest item, this quest is nearly same to first example, except we get a quest item.\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("With quest trigger QUEST_ITEM\n")
      ib:AddMsg("With name = questname\n")
      ib:AddMsg("And quest step (last_heal) = 1\n")
    elseif questnr==3 then
      ib:AddMsg("Kill red ant - game.QUEST_KILL\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Triggered by killing the QuestTarget(s).\n")
      ib:AddMsg("This example is kill 1 red ant.\n")
    elseif questnr==4 then
      ib:AddMsg("Kill read ant - game.QUEST_KILLITEM\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Without a defined QuestItem, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("With a defined QuestItem, this quest is nearly same to the KILL quest, except we get items on kill.\n\n")
      ib:AddMsg("Triggered by killing the target(s).\n\n")
      ib:AddMsg("This is kill one red ant, get it's heat.\n\n")
    elseif questnr==5 then
      ib:AddMsg("Kill two read ants.\n\n")
    elseif questnr==6 then
      ib:AddMsg("Kill two read ants.\n\nYou need also luck to get the kill. In this example 50%.\n\n")
      ib:AddMsg("This would make more sense on QUEST_KILLITEM quests, but also work on QUEST_KILL quests.\n\n")
    elseif questnr==7 then
      ib:AddMsg("Kill one red ant and one black ant.\n\n")
    elseif questnr==8 then
      ib:AddMsg("Kill one red ant and one black ant.\n\nGet red ant head and black ant eye.\n\n")
    elseif questnr==9 then
      ib:AddMsg("Kill the queen. Bring me her head.\n\n")
      ib:AddMsg("If you do this, I give you some coppers.\n\n")
      -- ib:SetCoins(10, 0, 0, 0) -- looks ugly, with a text link behind, to close
      -- we show the coopers, when player is accepting quest and he can also look in quest list
    elseif questnr==10 then
      ib:AddMsg("Oh yeah, you look strong enough to help me.\n\n")
      ib:AddMsg("Under guildhall between all these slugs and gobs, there is a hidden area, with a black pudding.\n\n")
      ib:AddMsg("Find the pudding, kill it and bring it to me.\n\n")
      ib:AddMsg("|This example is restricted to at least level 5 in main level.|\n\n")
    elseif questnr==11 then
      ib:AddMsg("Can you bring me my shirt, pant, dagger and book?\n\n")
      ib:AddMsg("They are in these chests left of us.\n\n")
      ib:AddMsg("But someone is trolling and trapped these chests.\n\n")
      ib:AddMsg("Perhaps you ask Frah'ak in jail for some trap training?\n\n")
      ib:AddMsg("|This example is restricted to at least level 2 in agility.|\n\n")
    end

    -- * 2shortcut links to start and finish a quest
    ib:AddLink("Start quest "..quests[questnr].name, "accept quest")
  else
    ib:AddLink("Finish quest "..quests[questnr].name, "finish quest")
  end
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicDefault) -- we use default, for what the name default stands, not to show an error dialog
tl = questAddTopics(tl)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
