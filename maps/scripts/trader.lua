require("topic_list")
require("interface_builder")

local pl = event.activator
local me = event.me

local ib = InterfaceBuilder()
ib:SetHeader(me, me.name)

-- these must be the arch names, using wrong archnames crashes the player
-- some generic arches get their title from hardcode, so we can't read title
--> for this arches we can overwrite title here
local for_sale =
{   {arch = "scroll_min_heal", title = "of minor healing"},
    {arch = "scroll_firestorm", title = "of firestorm"},
	--"scroll_firestorm",
	--"scroll_icestorm",
	--"scroll_min_heal",
	--"scroll_str_self",
	--"scroll_identify",
	--"scroll_dmagic",
	--"scroll_dcurse",
	--"scroll_rem_curse",
	--"scroll_rem_damn,",
	 {arch =  "scroll_cause_lwound", title = "of cause light wounds"},
	 {arch = "potion_minor_healing"},
	 {arch = "potion_minor_mana"},
	 {arch = "potion_minor_grace"}
}

local function topicDefault()
    ib:SetTitle("Trader")
	ib:SetMsg("\n\nYou have " .. pl:ShowCost(pl:GetMoney()) .. "\n ")
	
	for what, data in for_sale do
	  local ob = pl:CreateObject(data.arch, game.IDENTIFIED, 1) -- create 1 identified obj
	  
	  -- what happens with object when we don't use it? will it be removed from memory or must we handle this? <--------------------------
	  if (ob~=nil) then
	  	-- generic scrolls comes without title, currently we must get the informations from lua array
		if data.title ~= nil then
			ob.title = data.title
		end
	  
		local icon = ob:GetInvFace()
		ib:AddSelect("Buy  1 " .. ob.name .. " " .. ob.title, "buy " .. what , icon , pl:ShowCost(ob.value) .. "index:" .. what)
	  end	  
	end
    pl:Interface(game.GUI_NPC_MODE_NPC, ib:Build())
end

local function topicBuy(index)
	-- pl:Write("Yeah!", game.COLOR_YELLOW)
	if index~=nil then
		local i= tonumber(index)
		local text1 = "Index : " .. i
		--pl:Write(text1, game.COLOR_YELLOW)
		if i>0 then -- todo max array cap
			-- pl:Write(for_sale[i].arch, game.COLOR_YELLOW)
			
			local ob = pl:CreateObject(for_sale[i].arch, game.IDENTIFIED, 1) -- create 1 identified obj
			if (ob~=nil) then
				pl:PickUp(ob)
			end
		end
	end
	topicDefault()
end

tl = TopicList()
tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("buy (%d)", topicBuy) 
tl:CheckMessage(event)
