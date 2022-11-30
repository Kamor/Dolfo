-- template for intelligent and understandable quest logic, easy to use for scripters
-- improved buttons, links, reduced redundancy

-- welcome and quest intro (default)
--> introduction to npc
--> introduction to quest
--> quest button and side menu quest button
--> short cut links in relation to quest status, to start or finish the quest
--> sometimes it could be wise to split welcome from quest intro, then quest intro could be a separate topic

-- quest
--> this is all in one, in relation to quest status it is accept quest, view quest status and finish quest
---> it targets accept quest and finish quest function

-- accept quest
---> register quest
-- finish quest
---> finish quest, reward

local player = event.activator
local npc = event.me

require("interface_builder")
local ib = InterfaceBuilder()

require("quest_builder")
local qb = QuestBuilder()
-- quest name, type, level, skillgroup, ?required , ?finalstep, 1repeats, questGoal, questReward, silent
-- questGoal can be a function, which is launched when quest is started (registered)
---> we mostly do this automated here in relation to the arrays below
-- questReward can be a function, which is launched when quest is finished
local quest =
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
local quest_items =
{
  {questnr=2, amount=1, type = "quest_object", icon = "coppercoin.101", name = "quest example copper coin"},
  {questnr=11, amount=1, type = "quest_object", icon = "book.101", name = "Questus' book"},
  {questnr=11, amount=1, type = "quest_object", icon = "dagger.101", name = "Questus' dagger"},
  {questnr=11, amount=1, type = "quest_object", icon = "shirt.101", name = "Questus' shirt"},
  {questnr=11, amount=1, type = "quest_object", icon = "pants.101", name = "Questus' pants"}
}

-- you can assign more than 1 target to one quest.
local quest_targets =
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
local quest_kill_items =
{
  {id=1, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=2, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=3, amount=1, type = "quest_object", icon = "eye_beholder.101", name = "Ant Eye"},
  {id=4, amount=1, type = "quest_object", icon = "head_ant_queen.101", name = "Ant Head"},
  {id=5, amount=1, type = "quest_object", icon = "pudding_black.111", name = "Black Pudding"}
}

-- descriptions todo is there a better way to handle quest descriptions?
local function quest_description(questnr)
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
local function quest_reward(questnr)
  if questnr==2 then
    ib:AddMsg("You can keep the coin!\n")
    ib:SetCoins(1, 0, 0, 0)
    player:AddMoneyEx(1, 0, 0, 0)
  end
		
  if questnr==9 then
    ib:AddMsg("Here is your reward!\n")
    ib:SetCoins(10, 0, 0, 0)
    player:AddMoneyEx(10, 0, 0, 0)
  end
		
  if questnr==10 then
    ib:AddMsg("Here is your reward! Don't ask me what is in these cookies. He smiles.\n")
    ib:AddIcon("Cookies", "cookie01.101", "|** Delicious cookies with a secret special ingredient**|", 5)
    local ob = game:CreateObject("cookie", game.IDENTIFIED, 5)
    if (ob~=nil) then
      player:PickUp(ob, nil, 5)
    end
  end

end

-- add the quests
for i, x in quest do
  qb:AddQuest(x.name, x.type, x.level, x.skillgroup)
end

-- register quest, add quest items, quest targets and quest kill items
local function RegisterQuest(questnr)
  quest_description(questnr) -- is there no better way to send description to quest builder?
  qb:RegisterQuest(questnr, mpc, ib)

  if quest[questnr].type == game.QUEST_ITEM then
    for i, x in quest_items do
      if (x.questnr==questnr) then
        qb:AddQuestItem(questnr, x.amount, x.type, x.icon, x.name)
      end
    end
  elseif quest[questnr].type == game.QUEST_KILL or quest[questnr].type == game.QUEST_KILLITEM then
    for i, x in quest_targets do
      if (x.questnr==questnr) then
        local target=qb:AddQuestTarget(questnr, x.chance, x.amount, x.arch, x.name)
        if (x.id ~= nil) then
          for j, y in quest_kill_items do
            if (x.id==y.id) then
              target:AddQuestItem(y.amount, y.type, y.icon, y.name)
            end
          end
        end
      end
    end
  end
end

local questnr = qb:Build(player)
-- hello (default), here are mostly the unique npc's dialogs
-- some npc's want to have a kind of "can you do something for me?"
-- so here we need a split between hello and quest story - TODO - must also see, where we go back with buttons? to hi?
-- but except this split, hello should build a sitelink topic to quest
-- and should have shortcuts to fast except and fast finish quests.
-- an optional dialog like "I have not finished your quest" is useless and not worth the work for the scripters.
-- also the check if player has lied or not, who cares. Only useless blowing up code.
local function topicDefault()
  ib:SetTitle("Hello")
   if questnr<0 then
    ib:SetMsg("|^Quest^|\n\n\n") -- show quest topic in side menu
    ib:AddMsg("Sorry, i think you need some more training for the next quest.")
    return
  end
  if questnr==0 then
    ib:AddMsg("\nI have no more quests for you.\n")
    return
  end
  ib:SetMsg("|^Quest^|\n\n\n") -- show quest topic in side menu
  ib:SetLHSButton("Quest")
  ib:AddMsg("|Quest "..quest[questnr].name.."|\n\n")

  local qstat = qb:GetStatus(questnr)
  if questnr==1 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Open a chest - game.QUEST_NORMAL\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("- quest trigger QUEST_NORMAL\n")
      ib:AddMsg("- name = questname\n")
      ib:AddMsg("- quest step (last_heal) = 1\n")
    end
  end

  if questnr==2 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Open a chest - game.QUEST_ITEM\n\n")
      ib:AddMsg("Without a defined quest item, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("With a defined quest item, this quest is nearly same to first example, except we get a quest item.\n\n")
      ib:AddMsg("Triggered by this near chest:\n")
      ib:AddMsg("With quest trigger QUEST_ITEM\n")
      ib:AddMsg("With name = questname\n")
      ib:AddMsg("And quest step (last_heal) = 1\n")
    end
  end

  if questnr==3 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill red ant - game.QUEST_KILL\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Triggered by killing the QuestTarget(s).\n")
      ib:AddMsg("This example is kill 1 red ant.\n")
    end
  end

  if questnr==4 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill read ant - game.QUEST_KILLITEM\n\n")
      ib:AddMsg("Without a defined QuestTarget, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("Without a defined QuestItem, this quest is always SOLVED when started.\n\n")
      ib:AddMsg("With a defined QuestItem, this quest is nearly same to the KILL quest, except we get items on kill.\n\n")
      ib:AddMsg("Triggered by killing the target(s).\n\n")
      ib:AddMsg("This is kill one red ant, get it's heat.\n\n")
    end
  end
		
  if questnr==5 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill two read ants.\n\n")
    end
  end
		
  if questnr==6 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill two read ants.\n\nYou need also luck to get the kill. In this example 50%.\n\n")
      ib:AddMsg("This would make more sense on QUEST_KILLITEM quests, but also work on QUEST_KILL quests.\n\n")
    end
  end
		
  if questnr==7 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill one red ant and one black ant.\n\n")
    end
  end
		
  if questnr==8 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill one red ant and one black ant.\n\nGet red ant head and black ant eye.\n\n")
    end
  end
		
  if questnr==9 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Kill the queen. Bring me here head.\n\n")
      ib:AddMsg("If you do this, I give you some coppers.\n\n")
      -- ib:SetCoins(10, 0, 0, 0) -- looks ugly, with a text link behind, to close
      -- we show the coopers, when player is accepting quest and he can also look in quest list
    end
  end
		
  if questnr==10 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Oh yeah, you look strong enough to help me.\n\n")
      ib:AddMsg("Under guildhall between all these slugs and gobs, there is a hidden area, with a black pudding.\n\n")
      ib:AddMsg("Find the pudding, kill it and bring it to me.\n\n")
      ib:AddMsg("|This example is restricted to at least level 5 in main level.|\n\n")
    end
  end
		
  if questnr==11 then
    if qstat == game.QSTAT_NO then
      ib:AddMsg("Can you bring me my shirt, pant, dagger and book?\n\n")
      ib:AddMsg("They are in these chests left of us.\n\n")
      ib:AddMsg("But someone is trolling and trapped these chests.\n\n")
      ib:AddMsg("Perhaps you ask Frah'ak in jail for some trap training?\n\n")
      ib:AddMsg("|This example is restricted to at least level 2 in agility.|\n\n")
    end
  end

  -- shortcut links to start and finish a quest
  if qstat == game.QSTAT_NO then
    ib:AddLink("Start quest "..quest[questnr].name, "accept quest")
  else
    ib:AddLink("Finish quest "..quest[questnr].name, "finish quest")
  end
end

-- quest logic to handle the quest from quest topic
-- with accept and finish quest logic and also shows quest description and status (qb:AddItemList)
local function topicQuest()
  if questnr<0 then
    questnr=math.abs(questnr)
    ib:SetTitle("Quest "..quest[questnr].name)
    ib:AddMsg("\nYou need level "..quest[questnr].level)
    if quest[questnr].skillgroup~=nil then
						
      -- {"ITEM_SKILL_NO",          0},
      -- {"ITEM_SKILL_AGILITY",     SKILLGROUP_AGILITY + 1},
      -- {"ITEM_SKILL_PERSONALITY", SKILLGROUP_PERSONAL + 1},
      -- {"ITEM_SKILL_MENTAL",      SKILLGROUP_MENTAL + 1},
      -- {"ITEM_SKILL_PHYSICAL",    SKILLGROUP_PHYSIQUE + 1},
      -- {"ITEM_SKILL_MAGIC",       SKILLGROUP_MAGIC + 1},
      -- {"ITEM_SKILL_WISDOM",      SKILLGROUP_WISDOM + 1},
						
      -- wonderful a simple game:get_skillgroupname function would be nice, also this incompatible of questbuilder sucks 
      local sg=quest[questnr].skillgroup
      if sg==game.SKILLGROUP_AGILITY+1 then
        ib:AddMsg(" in agility")
      elseif sg==game.SKILLGROUP_PERSONAL+1 then
        ib:AddMsg(" in personal")
      elseif sg==game.SKILLGROUP_MENTAL+1 then
        ib:AddMsg(" in mental")
      elseif sg==game.SKILLGROUP_PHYSIQUE+1 then
        ib:AddMsg(" in physique")
      elseif sg==game.SKILLGROUP_MAGIC+1 then
        ib:AddMsg(" in magic")				
      elseif sg==game.SKILLGROUP_WISDOM+1 then
        ib:AddMsg(" in wisdom")
      else
        ib:AddMsg(" in skillgroup (questbuilder) ".. sg)
      end
    end
    ib:AddMsg(" for this quest.\n")
    return
  end
  if questnr==0 then
    topicDefault()
    return
  end
  local qstat = qb:GetStatus(questnr)
  ib:SetTitle("Quest "..quest[questnr].name)
  if qstat == game.QSTAT_ACTIVE or qstat == game.QSTAT_SOLVED then
    qb:AddItemList(questnr, ib)
  end
  if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
    quest_description(questnr)
  end
  if qstat == game.QSTAT_NO then
    ib:SetAccept("Accept", "accept quest")
    ib:SetDecline(nil, "hi")
  end
  if qstat == game.QSTAT_SOLVED then
    quest_description(questnr)
    ib:SetAccept("Finish", "finish quest")
    ib:SetDecline(nil, "hi")
  end
end

-- accept quest
local function topicAcceptQuest()
  if questnr<1 then
    topicDefault()
    return
  end

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    RegisterQuest(questnr)
  end
  topicQuest()
end

-- finish quest
local function topicFinishQuest()
  if questnr<1 then
    topicDefault()
    return
  end

  local qstat = qb:GetStatus(questnr)
  if qstat ~= game.QSTAT_SOLVED then
    topicQuest()
    return
  end

  ib:SetTitle("Quest "..quest[questnr].name)
  --player:Sound(0, 0, 2, 0) -- we have quest finish sound, we want also a second sound?
  ib:SetMsg("You finished the quest.\n\n")
  qb:Finish(questnr)
  -- TODO we want always jump to reward or only when its defined in our arrays?
  -- we can also do the jump from here or by questbuilder?
  quest_reward(questnr)
end

require("topic_list")
tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
