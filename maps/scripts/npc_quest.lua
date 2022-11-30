
-- register quest, add quest items, quest targets and quest kill items
function RegisterQuest(questnr)
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

-- quest logic to handle the quest from quest topic
-- with accept and finish quest logic and also shows quest description and status (qb:AddItemList)
function topicQuest()
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
function topicAcceptQuest()
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
function topicFinishQuest()
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
  ib:SetMsg("You finished the quest.\n\n")
  qb:Finish(questnr)
  -- TODO we want always jump to reward or only when its defined in our arrays?
  -- we can also do the jump from here or by questbuilder?
  quest_reward(questnr)
end

function questAddTopics(tlx) -- we need this global
  tlx:AddTopics({"quest", "explain%s+quest"}, topicQuest)
  tlx:AddTopics("accept quest", topicAcceptQuest)
  tlx:AddTopics("finish quest", topicFinishQuest)
  return tlx
end
