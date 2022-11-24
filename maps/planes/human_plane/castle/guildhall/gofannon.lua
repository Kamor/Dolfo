------------------------------------------------------------------------------
-- gofannon.lua | Event
-- TALK on NPC at /planes/human_plane/castle/castle_020a 20 20
--
-- Gofannon is the Guild Hall smith, almoat certainly the first smith a new
-- player will meet. He considers himself to be a very big fish, and he is. He
-- does not consider GH to be sa very small pond, but it is.
--
-- QUESTS
--
-- 1. "How to Skin a Cat" 
-- Scenario: Gofannon needs some Wild Cat skins for an item he has been
--           commissioned to make by Chereth.
-- Type    : KILLITEM
-- Level   : Main 2
-- Required: -
-- Steps   : 1
-- Repeats : 1
-- Goal    : Kill Wild Cats (east end of GH) until you have 4 skins. Give them
--           to Gofannon.
-- Reward  : Gofannon will 'rebalance' your weapon (reduce ws),
-- Hidden  : No
------------------------------------------------------------------------------
require("module/npc")
local mnpc = ScriptContext:GetModule("npc")
mnpc.profession = "smith"
--[[local ]]player = ScriptContext:GetObject("player")
--[[local ]]npc = ScriptContext:GetObject("npc")
--[[local ]]ib = ScriptContext:GetUtility("ib")
local qb = ScriptContext:GetUtility("qb")
require("/scripts/smith.lua")
qb:AddQuest("How to Skin a Cat", game.QUEST_KILLITEM, 2)
qb:Build(player)

function mnpc.extend.Greeting()
    ib:SetMsg("Hello! My name is "..npc:GetName()..".\n\n")
    if mnpc.profession ~= nil then
        local profession = string.capitalize(mnpc.profession)
        local article = "a"
        if string.match(profession, "^[aeiouAEIOU]") ~= nil then
            article = "an"
        end
        ib:AddMsg(" I am "..article.." "..profession..".\n\n")
    end
    topicSmithDefault()
				ib:AddLink("Rebalance my weapon","Rebalance")
    local qnr = qb:GetQuestNr(true)
    if qnr > 0 then
        if qb:GetStatus(qnr) == game.QSTAT_NO then
            ib:AddMsg("I have a ^quest^ you may be interested in.")
        else
            ib:AddMsg("How's progress on that ^quest^?")
            ib:AddLink("I've done what you asked.", "quest complete")
            ib:AddLink("Slow. But I'm working on it.", "quest incomplete")
        end
    end
end

function mnpc.background.Topic(subject)
    if subject == "chereth" then
        ib:SetMsg("Chereth was the guild's master archer until she "..
            "unfortunately lost her sight in battle.\n\n")
        ib:AddMsg("Nowadays she usually hangs out downstairs in the guild.")
        return true
    end
end

function mnpc.quest.Offer(qnr)
    if qnr ~= 0 then
        local qname = qb:GetName(qnr)
        if qname == "How to Skin a Cat" then
            ib:SetMsg("It is just a small thing.\n\n")
            ib:AddMsg("I have been awaiting a shipment of leather for some "..
                "time, but it appears the trader has been delayed.\n\n")
            ib:AddMsg("I urgently need some feline skins for an item "..
                "ordered by ^Chereth^.\n\n")
            ib:AddMsg("Obviously I can't get mountain leopard skins as "..
                "ordered, but there are a few wild cats running around in "..
                "the forest patch east of here.\n\n")
            ib:AddMsg("Could you please see if you can catch a few? Lets "..
                "say three. Bring me their skins!\n\n")
            ib:AddMsg("Do that for me and I'll identify all your items once "..
                "for free.")
            return true
        end
    end
end

function mnpc.quest.Accept(qnr)
    local qname = qb:GetName(qnr)
    if qname == "How to Skin a Cat" then
        ib:SetMsg("|** Gofannon nods approvingly and almost knocks you "..
            "over with a comradely but overenthusiastic clap on the back. "..
            "**|\n\n")
        ib:AddMsg("I'm thinking I'll need three... no, better make it four "..
            "skins. Some of those cats are pretty scrawny and their fur is "..
            "dull and their skin is poor so not all are suitable.")
        ib:SetDesc("Go to the east end of Guild Hall and kill Wild Cats "..
            "until you have four skins.\n\n")
        ib:AddDesc("Bring these to Gofannon and in return he will "..
            "identify all your items.")
        qb:RegisterQuest(qnr, npc, ib)
        local target = qb:AddQuestTarget(qnr, 2, 4, "cat_black", "Wild Cat")
        target:AddQuestItem(4, "quest_object", "skin.101", "Wild Cat Skin")
        return true
    end
end

function mnpc.quest.Complete(qnr, reward)
    local qname = qb:GetName(qnr)
    if qname == "How to Skin a Cat" then
        ib:SetMsg("|** Gofannon stretches out the skins on some kind of "..
            "rack to dry near his forge. **|\n\n")
        ib:AddMsg("Many thanks! This will work well, and the chickens will "..
            "be safe.\n\n")
        ib:AddMsg("Now about your reward...\n\n")
								npc:CastSpell(player, game:GetSpellNr("identify"), 1, 0, "")
								return true -- this ends the quest
								-- TODO castspell sound and end quest sound together are very loud
								-- TODO transfer rebalance logic to smith.lua
    end
end

function topicSmithRebalance (confirm)
    local item = player:FindMarkedObject()

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
       return false
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
					  ib:AddMsg("This will cost you ".. player:ShowCost(costs) ..".\n\n")
					  ib:AddLink("You want me to do this?", "confirm rebalance")				
					end	
				end
end

mnpc:AddTopics("background", "(chereth)")
local tl = ScriptContext:GetUtility("tl")
tl = SmithAddTopics(tl)
tl:AddTopics("rebalance", topicSmithRebalance)
tl:AddTopics("(confirm) rebalance", topicSmithRebalance)

mnpc:ShowInterface()