

local function reforgeable(item)
    local types = 
    {
        [game.TYPE_ARROW] = true,
        [game.TYPE_BOW] = true,
        [game.TYPE_WEAPON] = true,
        [game.TYPE_ARMOUR] = true,
        [game.TYPE_SHIELD] = true,
        [game.TYPE_HELMET] = true,
        [game.TYPE_SHOULDER] = true,
        [game.TYPE_LEGS] = true,
        [game.TYPE_AMULET] = true,
        [game.TYPE_RING] = true,
        [game.TYPE_CLOAK] = true,
        [game.TYPE_BOOTS] = true,
        [game.TYPE_GLOVES] = true,
        [game.TYPE_BRACERS] = true,
        [game.TYPE_GIRDLE] = true,
    }

    if not types[item.type] then
        return false
    end

    if item.item_quality >= 85 then
        return false
    end

    return true
end

local function starts(str, check)
    return string.sub(str, 1, string.len(check)) == check
end

-- not the best logic, but to test a new idea we can overwrite quality to get the price for a specific quality
local function reforgeCost(item, quality)
   if (quality == nil) then
	    quality = item.item_quality
	 end
    -- These items shouldn't show up often.
    if quality < 40 then
        return 1000
    end

    local tier = 0

    if item.title ~= nil then
        if starts(item.title, "of inferior") then
            tier = 1
        elseif starts(item.title, "of lesser") then
            tier = 2
        elseif starts(item.title, "of small") then
            tier = 3
        elseif starts(item.title, "of medium") then
            tier = 4
        elseif starts(item.title, "of large") then
            tier = 5
        elseif starts(item.title, "of greater") then
            tier = 6
        elseif starts(item.title, "of superior") then
            tier = 7
        end
    end

    local clone = game:CreateObject(item:GetArchName())
    local level = clone.item_level
    if level == 0 then
        level = 1
    end

    if item.f_is_named then
        tier = 7
        level = item.item_level
    end

    local base = math.floor(quality / 10)
    local mod = math.pow(10, base - 4) / 6
    local cost = mod * math.pow(level / 10, 3) + mod * math.pow(tier, 3) + 15

    if item.type == game.TYPE_ARMOUR or item.type == game.TYPE_WEAPON then
        cost = cost * 2
    end

    if item.f_is_named then
        cost = cost * 2
    end

    return cost
end

local function getItemsToRepair()
    local items = {}
    local item = nil

    for i = 0, game.EQUIP_MAX - 1 do
        item = player:GetEquipment(i)

        if item ~= nil and item.item_quality > 0 and item.item_condition < 100 then
            table.insert(items, item)
        end
    end

    return items
end

local function repairCost(item)
    local cost = 5 + item:GetRepairCost()
    return cost
end

function topicSmithDefault()
    ib:AddMsg("I can repair or identify your equipment. I can also reforge it, retaining the magical properties " .. 
                "while improving the material quality.\n\n")
    ib:AddMsg("What would you like to do?")
				ib:AddLink("Identify items", "identify")
    ib:AddLink("Repair my equipment", "repair")
    ib:AddLink("Reforge my equipment", "reforge")
				ib:AddLink("Rebalance my weapon","Rebalance")			
end

function topicSmithRepair()
    local flag = false
    ib:SetTitle("Repair my Equipment")
    ib:SetMsg("Let me check your equipment...\nPerhaps an item needs a fix.\nI will tell you how much each will cost.\n")
				ib:AddMsg("\nYou have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".")
    local o = player:FindMarkedObject()
    if o ~= nil and o.item_quality > 0 and o.item_condition < 100 then
        local linkstring = "~*M*~ " .. o:GetName()

        if string.len(linkstring) >= 48 then
            linkstring = string.sub(linkstring, 1, 41) .. "..."
        end

        linkstring = linkstring .. " (".. o.item_condition .. "\%): " ..
                     player:ShowCost(repairCost(o), game.COSTSTRING_SHORT)
        ib:AddLink(linkstring, "itemfix ".. -1)
        flag = true
    end

    local items = getItemsToRepair()

    for i = 1, table.getn(items) do
        local linkstring = items[i]:GetName()

        if string.len(linkstring) >= 48 then
            linkstring = string.sub(linkstring, 1, 41) .. "..."
        end

        linkstring = linkstring .. " (".. items[i].item_condition .. "\%): " ..
                     player:ShowCost(repairCost(items[i]), game.COSTSTRING_SHORT)
        ib:AddLink(linkstring, "itemfix ".. i)
        flag = true
    end

    if flag == false then
        ib:AddMsg("\n\n~Your equipment doesn't need any repair~")
    else
        ib:AddLink("Repair all my equipment", "itemfix all")
    end
    ib:SetButton("Back", "hi") 
end

function topicSmithItemFix(what)
    local num = tonumber(what)
    local items = {}
    local o

    if what=="-1" then 
        o = player:FindMarkedObject()
    elseif what == "all" then
        items = getItemsToRepair()
    elseif num ~= nil then
        o = getItemsToRepair()[num]
    end

    if table.getn(items) > 0 then
        ib:SetTitle("Repairing")

        local cost = 0
        for i = 1, table.getn(items) do
            cost = cost + repairCost(items[i])
        end

        ib:SetMsg("\n\nRepairing all of your equipment will cost you " .. player:ShowCost(cost, game.COSTSTRING_SHORT) .. ".\n\n")
        ib:AddMsg("You have " ..
                      player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".\n\n")
        ib:AddMsg("Should I repair your equipment now?")

        ib:SetAccept("Repair", "fix all")
        ib:SetDecline(nil, "repair")
    elseif o == nil then
        topicSmithRepair()
    else
        ib:SetTitle("Repairing")
        if o.item_quality > 0 and o.item_condition < 100 then
            ib:SetMsg("Will cost you " ..
                      player:ShowCost(repairCost(o), game.COSTSTRING_SHORT) ..
                      ".\n\n")
            ib:AddMsg("You have " ..
                      player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".\n\n")
            ib:AddMsg("Should I repair it now?")

            local icontitle = o:GetName()

            if string.len(icontitle) >= 48 then
                icontitle = string.sub(icontitle, 1, 41) .. "..."
            end

            ib:AddIcon(icontitle, o:GetFace(), 'Condition: ~'.. o.item_condition .. '\%~')
            ib:SetAccept("Repair", "fix " .. what)
            ib:SetDecline(nil, "repair") 
        else
            ib:SetMsg("The item doesn't need any repair.")
            ib:SetButton("Back", "repair") 
        end
    end
end

function topicSmithFix(what)
    local num = tonumber(what)
    local items = {}
    local o

    if what == "-1" then 
        o = player:FindMarkedObject()
    elseif what == "all" then
        items = getItemsToRepair()
    else
        if num == nil then
            topicSmithRepair()
            return
        end

        o = getItemsToRepair()[num]
    end

    ib:SetTitle("Pay and Repair")

    if what == "all" and table.getn(items) > 0 then
        local cost = 0
        for i = 1, table.getn(items) do
            cost = cost + repairCost(items[i])
        end

        if player:PayAmount(cost) == 1 then
            for i = 1, table.getn(items) do
                items[i]:Repair()
                ib:SetMsg("\n\n~** ".. npc.name .." takes your money and your items **~\n")
                ib:AddMsg("~** after some time he returns **~\n\n")

                ib:AddMsg("Here is your repaired equipment! It took quite the beating; perhaps you should take better care of it!")
                ib:AddMsg("\n\nIs there anything else I can help you with?")
            end
        else
            ib:SetMsg("\n\nYou don't have enough money! Maybe I could ^repair^ only a few of your items for a little less?")
        end
        ib:SetButton("Back", "hello") 
    elseif what == "all" then
        ib:SetMsg("\n\nYou don't have any items in need of repair.")
        ib:SetButton("Back", "hello") 
    elseif o == nil then
        ib:SetMsg("Hm, where is the item??")
        ib:SetButton("Back", "repair") 
    else
        if o.item_quality > 0 and o.item_condition < 100 then
            local qua = o.item_quality
            if player:PayAmount(repairCost(o)) == 1 then
                o:Repair()
                ib:SetMsg("~** ".. npc.name .." takes your money and the item **~\n")
                ib:AddMsg("~** after some time he returns **~\n\n")
                if o.item_quality < qua then
                    ib:AddMsg("Here is your repaired equipment!\nSadly it has lost a bit in quality!\nAnything else I can do for you?\n")
                else
                    ib:AddMsg("Here is your repaired equipment!\nThank you. Anything else I can do for you?\n")
                end
            else
                ib:SetMsg("You don't have enough money!")
            end
        else
            ib:SetMsg("The item doesn't need any repair.")
        end

        local icontitle = o:GetName()

        if string.len(icontitle) >= 48 then
            icontitle = string.sub(icontitle, 1, 41) .. "..."
        end

        ib:AddIcon(icontitle, o:GetFace(), 'Condition: ~'.. o.item_condition .. '\%~')

        ib:SetButton("Back", "repair") 
    end
end

function topicSmithIdentify()
    ib:SetTitle("Item Identification")
    ib:SetMsg("Lets see what i can do for you.\nI can ~identify~ a single item or all.\nI can ~detect magic~ or ~detect curse~.\nRember you must mark the single item first.\n\n")
    ib:AddMsg(".You have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".\n\n") 
    ib:AddMsg("What would you like to do?\n")
    local o = player:FindMarkedObject()
    if o ~= nil then
        if o.f_identified ~= true then
            local linkstring = "~*M*~ Identify " .. o:GetName()

            if string.len(linkstring) >= 48 then
                linkstring = string.sub(linkstring, 1, 41) .. "..."
            end

            ib:AddLink(linkstring .. ": 150c", "detect single")
        else
            ib:AddMsg("\n~*M*~ Your marked item is already identified.")
        end
    end
    ib:AddLink("Identify all for 5s", "detect all")
    ib:AddLink("Detect magic for 50c", "detect magic")
    ib:AddLink("Detect curse for 50c", "detect curse")
    ib:SetButton("Back", "hi") 
end

function topicSmithDetect(what)
    ib:SetTitle("It will cost you")
    if what=="magic" then
        ib:SetMsg("I can cast ~Detect Magic~ for 50c")
    elseif what == "all" then
        ib:SetMsg("I can ~Identify all~ for 5s")
    elseif what == "curse" then
        ib:SetMsg("I can cast ~Detect Curse~ for 50c")
    else
        ib:SetMsg("I can ~Identify One Item~ for 150c")
    end
    ib:AddMsg(" coins.\n\nYou have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".\n\nYou want me to do it now?")
    ib:SetAccept(nil, "cast " .. what) 
    ib:SetDecline(nil, "identify") 
end

function topicSmithCast(what)
    local sum, spell, o
    if what == "magic" then
        sum = 50
        spell = "detect magic"
    elseif what == "curse" then
        sum = 50
        spell = "detect curse"
    elseif what == "all" then
        sum = 500
    else
        sum = 150
        o = player:FindMarkedObject(); 
        if o == nil then
            topicSmithIdentify()
            return
        end
    end
    ib:SetTitle("Identification...")
    if player:PayAmount(sum) == 0 then
        ib:SetMsg("You don't have enough money!")
    else
        ib:SetMsg("|**" .. npc.name .. " takes your money **|\n\n")
        if sum == 50 then
            npc:CastSpell(player, game:GetSpellNr(spell), 1, 0, "")
            ib:AddMsg("Done!")
        else
												-- npc:CastSpell(player, game:GetSpellNr("identify"), 1, 0, "")
								
								
            local list = npc:IdentifyItem(player, o) -- deprecated function
            if list == nil then
                ib:AddMsg("I could not identify anything!")
            else
																-- this didn't work
                --for _, v in ipairs(list) do
                --    player:Write(v:GetName().." has been identified!")
                --end
                ib:AddMsg("Done!")
            end
        end
    end
    ib:SetButton("Back", "identify") 
end

function topicSmithReforge (quality) -- (confirm, quality)
    local item = player:FindMarkedObject()

    ib:SetTitle("Reforge equipment")

    if quality ~= nil then
        if item then
            if reforgeable(item) then
  							-- savetycheck against illegal command use range
								if tonumber(quality) <= item.item_quality or tonumber(quality)>85 then
										 ib:SetMsg("I can't reforge your " .. item.name .. " to quality " .. quality .. "!")
								else
										total_costs=0
										for qualityX=item.item_quality+1,quality,1
										do
												total_costs = total_costs + reforgeCost(item, qualityX)
										end
						
										if player:PayAmount(total_costs) == 1 then
												ib:SetMsg("\n\n|** " .. npc:GetName() .. " takes your ~" .. item.name .. "~ and begins to work on it. **|")
												item.item_quality = quality -- item.item_quality + 1
												if item.f_applied then
														player:Fix()
												end
												ib:AddMsg("\n\nHere you are! Your " .. item.name .. " is now at ~" .. item.item_quality .. "%~ quality.")
										else
												ib:SetMsg("You don't have enough money!")
										end
								end
            else
                ib:SetMsg("I can't reforge that item, I'm afraid.")
            end
        else
            ib:SetMsg("Which item did you want me to reforge? You haven't given me an item!")
        end
    else
        ib:SetMsg("I can reforge your equipment, retaining the magical properties while improving material quality.")

        if item then
            if reforgeable(item) then
								ib:AddMsg("\n\nI can reforge ~" .. item:GetName(player) .. "~ to raise its quality to a specific quality up to 85.")
								ib:AddMsg(" You have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".")
								total_costs=0
								for qualityX=item.item_quality+1,85,1
								do
									total_costs = total_costs + reforgeCost(item, qualityX)
									ib:AddLink("Reforge to quality " .. qualityX .. " for ".. player:ShowCost(total_costs, game.COSTSTRING_SHORT), "reforge " .. qualityX)
								end
                --ib:AddMsg("\n\nI can reforge ~" .. item:GetName(player) .. "~ to raise its quality by ~1%~ for a price of ~" -- .. player:ShowCost(reforgeCost(item), game.COSTSTRING_SHORT) .. "~.")
                --ib:AddLink("Reforge my " .. item.name, "confirm reforge")
            else
                ib:AddMsg("\n\nI can't reforge that item, I'm afraid.")
            end
        else
            ib:AddMsg("\n\nWhich item would you like me to reforge? ~M~ark the item in your inventory.")
        end
    end
end

function topicSmithRebalance (confirm)
    -- local item = player:FindMarkedObject()
    ib:SetTitle("Rebalance weapon")
				local weapon = player:GetEquipment(game.EQUIP_WEAPON1)
    if weapon == nil then
     ib:AddMsg("Please apply the weapon you want me to rebalance "..
         "for you, then talk to me again.")
				return false
				else
     ib:AddMsg("|** You relinquish your weapon to Gofannon and he "..
       "examines it closely...")
					-- No rerebalancing!
     local arch = game:LoadObject("arch " .. weapon:GetArchName() .. "\nend")
     if weapon.weapon_speed < arch.weapon_speed then
       ib:AddMsg(" But then hands it back. **|\n\n")
       ib:AddMsg("This weapon has been worked on before. The "..
           "materials only have so much tolerance. Try a "..
           "different one.\n\n")
       ib:AddMsg("Please apply the weapon you want me to "..
           "rebalance for you, then talk to me again.")
     end
					ib:AddMsg(" **|\n\n")
					-- this is only a very fast formula to define the costs, we need better formula here TODO
					local costs=arch.value*100
					if (confirm) then
							if player:PayAmount(1) == 1 then -- total_costs
							  -- not cleanest logic, we overwrite above examine message here
									ib:SetMsg("\n\n|** Gofannon takes your ~" .. arch.name .. "~ and begins to work on it. **|")
								 ib:AddMsg("\n\nHere you are! Your " .. arch.name .. " is now faster.")
									ib:AddMsg("\n\n|** He tosses it back to you and you catch it "..
           "easily in your fighting hand. **|")
          -- Eligible weapon arches have ws 1.85:2.65. Take 0.25 off.
          weapon.weapon_speed = weapon.weapon_speed - 0.25
          player:Fix()
							else
									ib:SetMsg("You don't have enough money!")
							end
					else
 				  ib:AddMsg("Hmm... Yes, I can do something about that...\n\n")
					  ib:AddMsg("This will cost you ".. player:ShowCost(costs) ..".\n")
							ib:AddMsg("\nYou have " .. player:ShowCost(player:GetMoney(), game.COSTSTRING_SHORT) .. ".")
					  ib:AddLink("You want me to do this?", "confirm rebalance")				
					end	
				end
end

function SmithAddTopics(tl)
    tl:AddTopics("detect (.*)", topicSmithDetect) 
    tl:AddTopics("cast (.*)", topicSmithCast) 
    tl:AddTopics("repair", topicSmithRepair) 
    tl:AddTopics("itemfix (.*)", topicSmithItemFix) 
    tl:AddTopics("fix (.*)", topicSmithFix)
    tl:AddTopics("identify", topicSmithIdentify)
    tl:AddTopics("reforge", topicSmithReforge)
				tl:AddTopics("reforge (.*)", topicSmithReforge)
				tl:AddTopics("rebalance", topicSmithRebalance)
    tl:AddTopics("(confirm) rebalance", topicSmithRebalance)
    return tl
end
