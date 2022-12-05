--[[

these are the splitted uniform parts of quest logics, see quest_example.lua

you need to "include" npc_quest.lua in your script
example:
require("scripts/npc_quest.lua")

you need to "include" topics from npc_quest.lua in your script
use tl = questAddTopics(tl) after you included topiclist and initialized t1
example:
require("topic_list")
tl = TopicList()
tl = questAddTopics(tl)

you need global definitions and correct names for player, npc, ib, qb and questnr
you need at least one defined quest, if not QuestBuilder crashes, when trying to get the questnr
you need also the two functions quest_description and quest_reward, to prevent crashing
you can use multiply arrays to define quests, define at least one quest in quests array
example
quests =
{
  {name = "Example 1", type = game.QUEST_NORMAL}
}
for more complex quests see quest_example.lua

add quests to questbuilder with addQuests()
build your unique quest goal text in your script
you can target from there, quest, accept quest and finish quest
see quest_example.lua

]]

-- add the quests
function addQuests()
  for i, x in quests do
    qb:AddQuest(x.name, x.type, x.level, x.skillgroup)
  end
end

-- register quest, add quest items, quest targets and quest kill items
function registerQuest(questnr)
  quest_description(questnr) -- is there no better way to send description to quest builder?
  qb:RegisterQuest(questnr, mpc, ib)

  if quests[questnr].type == game.QUEST_ITEM then
    for i, x in quest_items do
      if (x.questnr==questnr) then
        qb:AddQuestItem(questnr, x.amount, x.type, x.icon, x.name)
      end
    end
  elseif quests[questnr].type == game.QUEST_KILL or quests[questnr].type == game.QUEST_KILLITEM then
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

-- quest logic to handle the quest from quest topic
-- with accept and finish quest logic and also shows quest description and status (qb:AddItemList)
function topicQuest()
  if questnr<0 then
    questnr=math.abs(questnr)
    ib:SetTitle("Quest "..quests[questnr].name)
    ib:SetMsg("\nYou need level "..quests[questnr].level)
    if quests[questnr].skillgroup~=nil then
						
      -- {"ITEM_SKILL_NO",          0},
      -- {"ITEM_SKILL_AGILITY",     SKILLGROUP_AGILITY + 1},
      -- {"ITEM_SKILL_PERSONALITY", SKILLGROUP_PERSONAL + 1},
      -- {"ITEM_SKILL_MENTAL",      SKILLGROUP_MENTAL + 1},
      -- {"ITEM_SKILL_PHYSICAL",    SKILLGROUP_PHYSIQUE + 1},
      -- {"ITEM_SKILL_MAGIC",       SKILLGROUP_MAGIC + 1},
      -- {"ITEM_SKILL_WISDOM",      SKILLGROUP_WISDOM + 1},
						
      -- wonderful a simple game:get_skillgroupname function would be nice 
      local sg=quests[questnr].skillgroup
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
    ib:SetMsg("\nThere are no more quests for you.\n")
    return
  end
  local qstat = qb:GetStatus(questnr)
  ib:SetTitle("Quest "..quests[questnr].name)
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
function topicAcceptQuest()
  if questnr<1 then
    topicQuest()
    return
  end

  local qstat = qb:GetStatus(questnr)
  if qstat == game.QSTAT_NO then
    -- sadly we have redundancy here, but registerQuest add an icon and when we jump to topicQuest we add this icon again 
    ib:SetTitle("Quest "..quests[questnr].name)
    registerQuest(questnr)
    qb:AddItemList(questnr, ib)    
    return
  end
  topicQuest()
end

-- finish quest
function topicFinishQuest()
  if questnr<1 then
    topicQuest()
    return
  end

  local qstat = qb:GetStatus(questnr)
  if qstat ~= game.QSTAT_SOLVED then
    topicQuest()
    return
  end

  ib:SetTitle("Quest "..quests[questnr].name) -- we keep this here? last chance before finish the quest to get it's name
  qb:Finish(questnr)
  -- TODO we want always jump to reward or only when its defined in our arrays?
  -- we can also do the jump from here or by questbuilder?
  quest_reward(questnr)
end

function questAddTopics(tlx) -- we need this global
  tlx:AddTopics("quest", topicQuest)
  tlx:AddTopics("accept quest", topicAcceptQuest)
  tlx:AddTopics("finish quest", topicFinishQuest)
  return tlx
end
