-- lua has no good table count function, so we need to hardcode max length of our array or use this function
local function getTableSize(t)
    local count = 0
    for _, __ in pairs(t) do
        count = count + 1
    end
    return count
end

function topic_shop()
	ib:SetTitle("Shop")
  ib:SetMsg("So, what do you want?")
	
	for index, data in for_sale do
	  local ob = game:CreateObject(data.arch, game.IDENTIFIED, 1)
	  if (ob~=nil) then
		
		  local requirements=""
			
			-- int tmp_lev = (what->item_skill) ? pl->skillgroup_ptr[what->item_skill-1]->level : who->level;
			-- only to be save, todo do we need this?
			if ob.item_skill == nil then
			  ob.item_skill = 0
				pl:Write("debug : item_skill is nil ", game.COLOR_YELLOW)
			end
		
			local skill_group_name=""
			if ob.item_skill > 0 then
				local skill_group = pl:GetSkill(game.TYPE_SKILLGROUP, ob.item_skill-1)
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

			ib:AddSelect(	"Show me " .. ob.name .. requirements,	"show " .. index,	ob:GetFace() ,"price " .. pl:ShowCost(ob.value))
	  end
	end
end

local function topic_show(index)
	ib:SetTitle("Show item " .. index)
	ib:SetLHSButton("Back", "Services")

	if index~=nil then
		local i = tonumber(index)
		if i>0 and i <= getTableSize(for_sale) then
		  local ob = game:CreateObject(for_sale[i].arch, game.IDENTIFIED, 1)
	    if (ob~=nil) then
				ib:AddIcon(ob.name, ob:GetFace(), 'Condition: ~'.. ob.item_condition .. '~    Quality: ~' ..ob.item_quality .. '~')
				ib:AddLink("Buy " .. ob.name , "buy " .. index)
				return
			end
		end
	end	
	ib:SetMsg("This is not in my stock.")
end

local function topic_buy(index)
	ib:SetTitle("Buy item " .. index)
	ib:SetLHSButton("Back", "Services")

	if index~=nil then
		local i = tonumber(index)
		if i>0 and i <= getTableSize(for_sale) then
		  local ob = game:CreateObject(for_sale[i].arch, game.IDENTIFIED, 1) -- create 1 identified obj
	    if (ob~=nil) then
			
				if pl:PayAmount(ob.value) == 1 then
					ib:SetMsg("Nice that you found something.")
					pl:PickUp(ob,nil,1)
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
	tlx:AddTopics("shop", topic_shop)
  tlx:AddTopics("show (%d+)", topic_show)
  tlx:AddTopics("buy (%d+)", topic_buy)
  return tlx
end