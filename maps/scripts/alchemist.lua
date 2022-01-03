require("topic_list")
require("interface_builder")

local pl = event.activator
local me = event.me

local ib = InterfaceBuilder()
ib:SetHeader(me, me.name)

-- this logic limits the way a command could build, because command must be same than var name
-- we can't use "buy 5 potion of minor healing"
local for_sale = 
{
    healing = {title = "healing", price = 100, icon = "potion_red.101", arch = "potion_minor_healing", weight = 250},
    mana = {title = "mana",    price = 100, icon = "potioncol.101", arch = "potion_minor_mana", weight = 250},
    grace =  {title = "grace",   price = 100, icon = "potion_green.101", arch = "potion_minor_grace", weight = 250}
}

local function topicDefault()
    ib:SetTitle("Alchemist")
    ib:SetMsg("Hello! I am " .. me.name ..".")
    ib:AddMsg(" I sell the best potions here in this area.")
    ib:AddMsg("\nDo you need some?")
	--ib:AddMsg("\n" .. pl.carrying .. " - " .. pl:GetPlayerWeightLimit()) 
    for what, data in for_sale do
        --ib:AddLink("Buy  1 "..data.title.. " for "..pl:ShowCost(data.price), "buy 1 "..data.title)
		ib:AddSelect("Buy  1 "..data.title, "buy 1 "..data.title, data.icon, pl:ShowCost(data.price))
        --ib:AddLink("Buy 10 "..data.title.. " for "..pl:ShowCost(data.price*10), "buy 10 "..data.title)
		ib:AddSelect("Buy  10 "..data.title, "buy 10 "..data.title, data.icon, pl:ShowCost(data.price*10),10)
        --ib:AddLink("Buy 25 "..data.title.. " for "..pl:ShowCost(data.price*25), "buy 25 "..data.title)
		--ib:AddSelect("Buy  25 "..data.title, "buy 25 "..data.title, data.icon, pl:ShowCost(data.price*25),25)
    end
    pl:Interface(game.GUI_NPC_MODE_NPC, ib:Build())
end

local function topicBuy(nrof, what)
    local data = for_sale[what]
    if data == nil then
        ib:SetTitle("Sorry, we don't have that")
        ib:SetMsg("Sorry, but we don't serve that here. Can I get you " ..
                  "something else?")
				  
		-- ib:AddMsg("\n" .. what)
		-- ib:AddMsg("\n" .. for_sale[what])	
		
        ib:SetDecline("No")
        ib:SetAccept("Yes", "hello")
    else 
        if nrof == nil then 
            nrof="1" 
        end
        if nrof <= "50" then
            ib:SetTitle("Here is my offer")
            ib:SetMsg(nrof .. " " .. data.title .. " will cost you ~" ..
                      pl:ShowCost(data.price * nrof) .. "~.")
            ib:AddMsg("\nYou have ~" .. pl:ShowCost(pl:GetMoney()) .. "~.")
            if nrof == "1" then
                ib:AddMsg("\nYou want to buy it?")
            else
                ib:AddMsg("\nYou want to buy them?")
            end
            ib:AddIcon(data.title, data.icon, "", tonumber(nrof))
            ib:SetAccept(nil, "confirm "..nrof.." "..data.title) 
            ib:SetDecline(nil, "hi")
        else
            ib:SetTitle("Tavern")
            ib:SetMsg("Sorry but we aren't a wholesaler, you can only buy " ..
                      "small amounts.")
        end
    end
    pl:Interface(game.GUI_NPC_MODE_NPC, ib:Build())
end

local function topicConfirm(nrof, what)
    ib:SetTitle("Buying some Stuff")
    local data = for_sale[what]
    if nrof == nil then
        nrof = "1"
    end
    if data == nil then
        ib:SetTitle("Sorry, we don't have that")
        ib:SetMsg("Sorry, but we don't serve that here. Can I get you " ..
                  "something else?")
        ib:SetDecline("No")
        ib:SetAccept("Yes", "hello")
    elseif nrof > "50" then
        ib:SetMsg("Sorry, but we aren't a wholesaler, you can only buy " ..
                  "small amounts.")
    elseif pl.carrying + (nrof * data.weight) >= pl:GetPlayerWeightLimit() then
        ib:SetMsg("Can you explain me, how you would carry all that stuff, " ..
                  "without collapsing?")
    elseif pl:PayAmount(data.price * nrof) == 1 then
        ib:SetMsg("|** " .. me.name .. " takes your money **|")
        ib:AddMsg("\nOk, here is your " .. data.title .. "!")
        tmp = pl:CreateObjectInsideEx(data.arch, 1, nrof) 
        ib:AddIcon(tmp.name, tmp:GetFace(), "", tonumber(nrof))
    else
        ib:SetMsg("You don't have enough money!")
    end
		--ib:AddMsg("" .. pl.carrying .. " - " .. nrof * data.weight .. " - " .. pl:GetPlayerWeightLimit() .."")
	
    ib:SetButton("Back", "Hi") 
    pl:Interface(game.GUI_NPC_MODE_NPC, ib:Build())
end

tl = TopicList()
tl:AddGreeting(nil, topicDefault)
tl:SetDefault(topicDefault)
tl:AddTopics("buy%s+(%d*)%s*(.*)", topicBuy) 
tl:AddTopics("confirm%s+(%d*)%s*(.*)", topicConfirm) 
tl:CheckMessage(event)
