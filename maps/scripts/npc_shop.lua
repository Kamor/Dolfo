-- lua has no good table count function, so we need to hardcode max length of our array or use this function
local function getTableSize(t)
    local count = 0
    for _, __ in pairs(t) do
        count = count + 1
    end
    return count
end

local function calcValue(ob)
  -- to be consistent we use the c function, perhaps we want calc later the npc level?
  local value=player:GetItemCost(ob, game.COST_TRUE)
  -- type is one of game.COST_TRUE, game.COST_BUY or game.COST_SELL 
  return value
end

-- groups is array, group_to_show is group index to show, group is user command
function topic_shop(group) -- we need this global
  local group_to_show=0 -- lua starts array indexes with 1, so 0 is invalid index, or no index
  ib:SetTitle("Shop")
  -- when user has not choose a group he want to see, check if we have a groups array, then show groups
  if (group==nil) then
    if (groups~=nil) then
      ib:SetMsg("What do you want to see?")
      for index, data in groups do
        ib:AddLink("Show " .. data.name , "shop " .. data.name)
      end
      return
    end
  else		
    -- user has send a group command, validate this and find the index
    for index, data in groups do
      if (data.name==group) then
        group_to_show=index
        break;
      end
    end
    -- invalid group command, we handle like no group
  end

  ib:SetMsg("What do you want to see? You have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".")

  for index, data in for_sale do
    local ob = game:CreateObject(data.arch, game.IDENTIFIED, 1)
    if (ob~=nil) then
      -- lua has no continue and i don't want to use to complex one line comparisons, so i use a flag here
      local show_item=true
      if group_to_show~=0 then
        if groups[group_to_show].type~=nil and groups[group_to_show].type~=ob.type then
          show_item=false
        elseif groups[group_to_show].subtype~=nil and groups[group_to_show].subtype~=ob.subtype then
          show_item=false
        end
      end

      if show_item==true then
        local requirements=""
			
        -- c -- int tmp_lev = (what->item_skill) ? pl->skillgroup_ptr[what->item_skill-1]->level : who->level;
        -- only to be save, todo do we need this?
        if ob.item_skill == nil then
          ob.item_skill = 0
          player:Write("debug : item_skill is nil ", game.COLOR_YELLOW)
        end
		
        local skill_group_name=""
        if ob.item_skill > 0 then
          local skill_group = player:GetSkill(game.TYPE_SKILLGROUP, ob.item_skill-1)
          if skill_group ~= nil then
            skill_group_name=skill_group.name
          end
        end
			
        if ob.item_level>0 then
          requirements=" (level " .. ob.item_level
          if skill_group_name~="" then
            requirements=requirements .. " " .. skill_group_name
          end
          requirements=requirements .. ")"
        end
        ib:AddSelect(	"Show me " .. ob.name .. requirements,	"show " .. index,	ob:GetFace() ,"price " .. player:ShowCost(calcValue(ob)))
      end
    end
  end
		
  -- if we are in "group to show" mode we set a button back to "no group shop"
  -- sadly the "back" button from topic_show now always targets the group choice logic
  if (group_to_show>0) then
    ib:SetLHSButton("Back", "shop")	
  end
end

local function topic_show(index)
  ib:SetTitle("Show item " .. index)
  ib:SetLHSButton("Back", "shop")

  if index~=nil then
    local i = tonumber(index)
      if i>0 and i <= getTableSize(for_sale) then
        local ob = game:CreateObject(for_sale[i].arch, game.IDENTIFIED, 1)
        if (ob~=nil) then
        
        local result = player:ExamineItem(ob)
        ib:AddMsg(result)
        ib:AddLink("Buy " .. ob.name , "buy " .. index)
        return
      end
    end
  end	
  ib:SetMsg("This is not in my stock.")
end

local function topic_buy(index)
  ib:SetTitle("Buy item " .. index)
  ib:SetLHSButton("Back", "shop")

  if index~=nil then
  local i = tonumber(index)
    if i>0 and i <= getTableSize(for_sale) then
      local ob = game:CreateObject(for_sale[i].arch, game.IDENTIFIED, 1) -- create 1 identified obj
      if (ob~=nil) then

        if player:PayAmount(calcValue(ob)) == 1 then
          ib:SetMsg("Nice that you found something.")
          player:PickUp(ob,nil,1)
        else
          ib:SetMsg("Sorry, come back if you have the money for it.")				
        end
        return
      end
    end
  end	
  ib:SetMsg("This is not in my stock.")
end

function shopAddTopics(tlx) -- we need this global
  tlx:AddTopics({"shop", "shop (.*)"}, topic_shop)
  tlx:AddTopics("show (%d+)", topic_show)
  tlx:AddTopics("buy (%d+)", topic_buy)
  return tlx
end
