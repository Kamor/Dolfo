require("topic_list")
require("interface_builder")

local MAX_NROF = 1000
local MAX_LEVEL = 110

local pl = event.activator
local me = event.me

local ib = InterfaceBuilder()
ib:SetHeader(me, me.name)

-- npc_shop can store 2 values in player data, nrof to adjust the amount to buy, level to adjust the level to buy
local ds = DataStore("npc_shop", pl)

local nrof = ds:Get("nrof")
if nrof == nil or nrof<1 then
	nrof = 1
	ds:Set("nrof", nrof)
	-- ds:WasChanged() -- looks like this is not needed
elseif nrof>MAX_NROF then
	nrof = MAX_NROF
	ds:Set("nrof", nrof)
	-- ds:WasChanged() -- looks like this is not needed
end
local level = ds:Get("level")
if level == nil or level<1 then
	level = 1
	ds:Set("level", level)
	-- ds:WasChanged() -- looks like this is not needed
elseif level>MAX_LEVEL then
	level = MAX_LEVEL
	ds:Set("level", level)
	-- ds:WasChanged() -- looks like this is not needed
end

--level = 5
--ds:Set("level", level) -- test adjustment

--nrof = 5
--ds:Set("nrof", nrof) -- test adjustment

--pl:Write(nrof, game.COLOR_YELLOW)
--pl:Write(level, game.COLOR_YELLOW)

-- these must be the arch names, using wrong archnames crashes the player
-- some generic arches get their title from hardcode, so we can't read title
--> for this arches we can overwrite title here
local for_sale =
{
	{arch = "drink_generic"},
	{arch = "scroll_min_heal", title = "of minor healing"},
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
	ib:SetMsg("You have " .. pl:ShowCost(pl:GetMoney()) .. ".")
	ib:AddMsg(" You carry " .. pl.carrying .. " (" .. pl:GetPlayerWeightLimit() ..") kg.") 
	for index, data in for_sale do
	  local ob = pl:CreateObject(data.arch, game.IDENTIFIED, 1) -- create 1 identified obj
	  
	  -- what happens with object when we don't use it? will it be removed from memory or must we handle this? <--------------------------
	  if (ob~=nil) then
	  	-- generic scrolls comes without title, currently we must get the informations from lua array
		if data.title ~= nil then
			ob.title = data.title
		end
		-- if we don't overwrite empty ob.title comes with ob.title=="", so no need to check there for nil
	
		-- make a rule for level adjustment
		-- 0 -> is always no level on this item
		-- 1 -> we must check, if there are wrong arch definitions, like my new food potions with level 1
		-- we can also go for spellcheck, all spell -1 have level 0 ? TODO
		if ob.level > 0 then
		   ob.level = level -- level comes now from datastore
		   ob.title = ob.title .. " (lvl ".. ob.level ..")"
		end
		-- pl:Write(ob.level, game.COLOR_YELLOW)
	  
		local icon = ob:GetInvFace()
		if icon == nil then
			icon = ob:GetFace()
		end
		if icon == nil then
			icon = "bug.101"
		end

		local value= ob.value*(ob.level+1)*nrof 
		ib:AddSelect(	"Buy " .. nrof .. " " .. ob.name .. " " .. ob.title,
						"buy " .. nrof .. " " .. index .. " " .. ob.level,
						icon ,
						pl:ShowCost(value) .. " - " .. ob.weight*nrof .. " kg",
						nrof	) 
	  end	  
	end
	
	ib:AddMsg(" ^nrof 1^ ^nrof 5^ ^level 1^ ^level 5^ ")
	
    pl:Interface(game.GUI_NPC_MODE_NPC, ib:Build())
end

local function topicBuy(nrof, index, level)
	-- pl:Write("Yeah!", game.COLOR_YELLOW)
	if index~=nil then
		local i = tonumber(index)
		--local text1 = "Index : " .. i
		--pl:Write(text1, game.COLOR_YELLOW)
		if i>0 then -- todo max array cap
			-- pl:Write(for_sale[i].arch, game.COLOR_YELLOW)
			
			-- nrof is nil or string, what happens on nrof = ""
			n = 1
			if nrof ~= nil then
				n = tonumber(nrof)
				if n < 1 then
					n = 1
				elseif n>MAX_NROF then
					n = MAX_NROF
				end
			end
			
			-- pl:Write(n, game.COLOR_YELLOW)
			
			local ob = pl:CreateObject(for_sale[i].arch, game.IDENTIFIED, n) -- create 1 identified obj
			if (ob~=nil) then
				local l = 0
				if level ~= nil then
					l = tonumber(level)
					if l < 0 then
						l = 0
					elseif l>MAX_LEVEL then
						l = MAX_LEVEL
					end
				end	
				ob.level = l
				
				-- calculation of price is value*(level+1)*n
				-- do we need check value? if someone is adjusting bad values in archetyp/artifactst
				if pl:PayAmount(ob.value*(ob.level+1)*n) == 1 then
						pl:PickUp(ob,nil,n)
				else
					pl:Write("You don't have enough money.!", game.COLOR_YELLOW)
				end			
			end
		end
	end
	topicDefault()
end

local function topicLevel(_level)
	if _level ~= nil then
		level = tonumber(_level)
		pl:Write(level, game.COLOR_YELLOW)
		ds:Set("level", level) -- calling topicDefault, restart the whole script, so we have check at script start for MIN and MAX
		-- ds:WasChanged() -- looks like this is not needed
	end
	topicDefault()
end

local function topicNrof(_nrof)
	if _nrof ~= nil then
		nrof = tonumber(_nrof)
		pl:Write(nrof, game.COLOR_YELLOW)
		ds:Set("nrof", tonumber(nrof)) -- calling topicDefault, restart the whole script, so we have check at script start for MIN and MAX
		-- ds:WasChanged() -- looks like this is not needed
	end
	topicDefault()
end

tl = TopicList()
tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("buy (%d) (%d) (%d)", topicBuy)
tl:AddTopics("nrof (%d)", topicNrof)
tl:AddTopics("level (%d)", topicLevel)
tl:CheckMessage(event)
