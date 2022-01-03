-------------------------------------------------------------------------------
-- cashin.lua -- TALK on NPC at castle_030a 11 10
--
-- Guildmaster Cashin is the Guildmaster of the Mercenary Guild.
-- He is a serious man, especially about the guild.
--
-- But his personality must be balanced with his gameplay function as
-- new players' first real link (Fanrir doesn't count) to the gameworld.
--------------------------------------------------------------------------------
-- QUESTS
--
-- 1. "The Mercenary Guild Quest":
-- Requirements: None.
-- Purpose     : Intended as the first 'real' quest
--               (ie, Fanrir can safely be skipped).
-- Scenario    : The Ant Scout Leader in the guild cellar has Cashin's helm.
-- Goal        : Kill the ant and return the helm to Cashin.
-- Reward      : Merc Guild membership and Cashin's helm.
---------------------------------------------------------------------------------
-- SERVICES
---------------------------------------------------------------------------------
-- Join/Leave guild
---------------------------------------------------------------------------------

if not module_guildsLOADED then
    require("module/guilds")
end

require("module/npc")
local mnpc = ScriptContext:GetModule("npc")
local player = ScriptContext:GetObject("player")
local npc = ScriptContext:GetObject("npc")
local ib = ScriptContext:GetUtility("ib")
local qb = ScriptContext:GetUtility("qb")
local gstat = module_guildsGetStatus("Mercenary", player)

mnpc.locality = {"Guild Hall"}

-------------------
-------------------

local quest="The Mercenary Guild Quest"
qb:AddQuest(quest, game.QUEST_KILLITEM) --
local questnr = qb:Build(player)
-- questnr = 0 no quest
-- questnr < 0 quest not allowed
-- questnr >0 quest allowed
local queststatus = qb:GetStatus(questnr) -- GetStatus with call questnr=0 looks like save, check getstatus return for this scenario 

function mnpc.extend.Greeting()
    if gstat == game.GUILD_NO then
        ib:SetMsg("Greetings! I am ^" .. npc.name .. "^. Welcome to the " ..
                "^Mercenary Guild^ of ^Thraal^!\n")
        ib:AddMsg("\nOur troops are an important part of the imperial " ..
                "defence.\n")
		if queststatus == game.QSTAT_NO then
            ib:AddMsg("\nYou look like you have some potential. Do you " ..
                        "want to join our guild? You really would be welcome " ..
                        "in our ranks.")
            ib:AddLink("Yes, I would like to join the Mercenary Guild.",
                        "join")
            ib:AddLink("No thanks, maybe later.", "")
            ib:SetLHSButton("Join")
			-- thats wrong we need check status
        elseif queststatus == game.QSTAT_ACTIVE then
            ib:AddMsg("\nI hope that ^quest^ I gave you isn't proving too " ..
                        "difficult for you.")
            ib:SetLHSButton("Quest")
	    elseif queststatus == game.QSTAT_SOLVED then
            ib:AddMsg("\nGreat you brought back my helm back.")
			ib:AddLink("Finish quest and join the Mercenary Guild.", "Quest Complete")
			ib:SetLHSButton("Quest","Quest Complete")
        end	-- we can go for more like QSTAT_DONE, but chashin dialog is more focused on guild membership
    elseif gstat == game.GUILD_OLD then
        if math.random() >= 0.50 then
            ib:SetMsg("Ah! Look who's back?\n")
        else
            ib:SetMsg("Well if it isn't ~ex~-Guildmember " .. player.name .. ".\n")
        end

        ib:AddMsg("\nAre you ready to rejoin the ^guild^ yet? ^Thraal^ needs " ..
                  "all the defenders it can get nowadays.")
        ib:AddLink("Yes, I would like to rejoin the Mercenary Guild.", 
                   "rejoin")                                            -- Guild is not triggered to site topic, only guide is triggered
        ib:AddLink("Not now thanks, maybe later.", "")
        ib:SetLHSButton("Rejoin")
    else
        ib:SetMsg("Hello Guildmember " .. player.name .. ".\n")
        ib:AddMsg("\n|** You salute each other **|\n")

        ---------
        -- Player has completed all quests.
        ---------
        if questnr == 0 then
            ib:AddMsg("\nI have nothing more for you to do.\n")
            ib:AddMsg("\nThe ^guild^ values experience. Go and get some.")
            ib:SetLHSButton("Services")
        ---------
        -- Next quest is disallowed (ie, player does not meet level/skill req).
        ---------
        elseif questnr < 0 then -- normal player can't reach this logic
            ib:AddMsg("\nI have nothing for you to do at the moment.\n")
            ib:AddMsg("\nThe ^guild^ values experience. Come back later.")
            ib:SetLHSButton("Services")
        ---------
        -- Player is eligible for a quest.
        ---------
        else -- normal player can't reach this logic
            ib:AddMsg("\nAs a member you are eligible to take guild ^quests^.")
            ib:SetLHSButton("Quest")
        end
    end
end

function mnpc.quest.Offer(qnr)
    ---------
    -- By converting qnr to qname we can then test for specific quest names
    -- which makes the hook robust if subsequently quests are added to (or
    -- removed from) within the current list (thus changing the nr sequence).
    ---------
	-- this logic has no clean npc reaction, if player finishes the quest but has joined guild before, but normal players can't do this
    if qnr ~= 0 then
        local qname = qb:GetName(qnr)
        if qname == "The Mercenary Guild Quest" then
            if qnr < 0 then
                ib:SetMsg("I have nothing for you at the moment.\n")
                ib:AddMsg("\nThe ^guild^ values experience. Come back later.")
            elseif queststatus == game.QSTAT_SOLVED then
				ib:AddMsg("\nGreat you brought back my helm back.")
				ib:AddLink("Finish quest and join the Mercenary Guild.", "Quest Complete")
				ib:AddLink("Can you tell me more about the guild?", "guild") 
			else
	            ib:SetMsg("Before you can join the ^guild^ I have a small " ..
                            "task for you.\n")
                ib:AddMsg("\nWe have had a problem with giant ants for " ..
                            "some time now. See this hole to the old guild " ..
                            "cellar beside me? One of those silly ants down " ..
                            "there has stolen my old helmet!\n")
                ib:AddMsg("\nEnter the hole and kill the ants! Don't " ..
                            "worry, they are weak.\n")
                ib:AddMsg("\nBring back the helmet and you can join the " ..
                            "guild.\n")
                ib:AddMsg("\nThere are four rooms in the cellar. I think the " ..
                            "Ant Scout Leader in the fourth room has my helmet.")
                ---------
                -- Lets give a hint to the newbs.
                ---------
                if player.level <= 2 then
                    ib:AddMsg("\n\n|Hint -- Close Combat|\n")
                    ib:AddMsg("\nHm, you don't look too experienced with " ..
                                "your weapon... Well, here is a tip from an " ..
                                "old wardog.\n")
                    ib:AddMsg("\nWhen you get down there, try fighting a " ..
                                "few ants in the first three rooms before " ..
                                "going for the Scout Leader. They won't " ..
                                "attack you until you attack them.\n")
                    ib:AddMsg("\nWhen you're ready, go for the Scout " ..
                                "Leader. He is a lot more aggressive and " ..
                                "will chase you on sight. Don't show him any " ..
                                "fear!\n")
                    ib:AddMsg("\nIf you run away like a coward he'll have " ..
                                "time to recover from his wounds, so it is " ..
                                "important that you keep on the offensive.\n")
                    ib:AddMsg("\nOh, and I wouldn't try going any deeper " ..
                                "into the nest just yet...")

                    if player:GetQuest("Find Fanrir's Lunch") then
                        ib:AddMsg("\n\nDon't forget what Fanrir told you " ..
                                    "about eating food to recover health.")
                    end
                end

		-- we have buttons accept and deline from outside <---------------------------------------------------- todo
		-- overwriting a button here is ugly, we override accept or decline
		-- after finished the quest, we have buttons quest and bye from outside
		-- quest button to restart this same dialog could be improved, must look deeper in new script context system 

            end
            return true
        end
    end
end

function mnpc.quest.Accept(qnr)
    local qname = qb:GetName(qnr)

    if qname == "The Mercenary Guild Quest" then
        ib:SetDesc("Enter the hole in the floor near Cashin in the " ..
                "Guild Hall Mercenary Guild.\n")
        ib:AddDesc("\nSlay the Ant Scout Leader and retrieve Cashin's " ..
                "helm.")

        qb:RegisterQuest(1, npc, ib)
        local target = qb:AddQuestTarget(1, 1, 1, "ant_red", "Ant Scout Leader")
        target:AddQuestItem(1, "quest_object", "helm_leather.101", "Cashin's helm")
        require("/scripts/first_weapon")
        return true
    end
end

function mnpc.quest.Complete(qnr)
    local qname = qb:GetName(qnr)
    if qname == "The Mercenary Guild Quest" then
        ib:SetTitle(qname)
        ib:SetMsg("Very well done! As promised, I will make you a " ..
                    "member of the ~Mercenary Guild~.\n")
        ib:AddMsg("\n|** " .. npc.name .. " enters your name in the " ..
                    "member roll **|\n")
        ib:AddMsg("\nWelcome, Guildmember " .. player.name .. "!\n")
        ib:AddMsg("\n|** " .. npc.name .. " looks fondly at his helmet " ..
                    "for a moment **|\n")
        ib:AddMsg("\nTell you what. You should also take this. You've " ..
                    "earned it.")
        ib:AddIcon("Membership of the ~Mercenary Guild~!", "guild_merc",
                    "", 0)
        ib:AddIcon("Cashin's old helm", "helm_leather.101",
                    "It will protect you well, as it did me.")

        qb:Finish(qnr, "Cashin's old helm")
        local helm = player:CreateObjectInside("helm_leather", game.IDENTIFIED)
        assert(helm, "Could not create helm!")
        helm.name = "Cashin's old helm"
        helm.f_is_named = 1
        if not player:GetEquipment(game.EQUIP_HELM) then
            player:Apply(helm, game.APPLY_ALWAYS)
        end
        module_guildsJoin("Mercenary", player)

        return true
    end
end

function mnpc.replace.Services()

    ib:SetMsg("I'm the guildmaster of our local branch of the ^Mercenary Guild^.")
    ib:AddMsg(" As part of the ^Thraal^ army corps, we are reclaiming this " ..
              "plane after the defeat of Moroch.")
    if gstat == game.GUILD_NO and questnr ~= 0 then
        ib:SetMsg("So you want to join the guild?\n")
        ib:AddMsg("\n|** " .. npc.name .. " gives you the once over " ..
                  "again **|\n")
        ib:AddMsg("\nWell with the ogre problem here in the Province and " ..
                  "Moroch causing untold trouble all over, a person of " ..
                  "your apparent calibre would be more than welcome.\n")
        ib:AddMsg("\nBut first I need you to prove your mettle by " ..
                  "completing a simple ^quest^.")
        ib:SetLHSButton("Quest")
		return
	end
		
    ib:AddMsg("\n\nWhat can I do for you?")

    if gstat == game.GUILD_NO or gstat == game.GUILD_OLD then -- it's only a fast implement here, logic could be better, we are missing guild not allowed check 
	   ib:AddLink("I want to join the Mercenary Guild.", "join") 
       ib:SetLHSButton("Join")		
    elseif gstat == game.GUILD_IN then
	   ib:AddLink("I want to leave the Mercenary Guild.", "leave")
       ib:SetLHSButton("Leave")
	end
    return true
end

-- button left all topics go back to "guild" topic and only "guild" topic goes to services
function mnpc.background.Topic(subject)
    subject = subject or ""
    if subject == "thraal" then
        ib:SetTitle("The Army of Thraal")
        ib:SetMsg("The Thraal started off as a ragtag band of humans, simply trying " ..
                  "desperately to survive. In recent years, however, we've turned " ..
                  "our focus to taking back our homes and destroying Moroch.")
        
        -- TODO: Whenever races are reorganized more sensibly, ogres probably won't be "giants"
        if player.race ~= "giant" then
            ib:AddMsg("\n\nWe've managed to get help from other races, even a handful " ..
                      "of ogres. I'm not quite sure I fully trust them yet, though.")
        end
		ib:AddMsg(" This is how our ^guild^ started.")
		ib:AddLink("Can you tell me more about the guild?", "guild") -- AddLink don't support ^^ feature, so for a good click-nterface we need a ^guild^ in msg
		
    elseif subject == "guild" then -- or string.find(subject, "^mercenar") ~= nil then -- <- what's this "^mercenar" ???
        ib:SetTitle("The Mercenary Guild")
        ib:SetMsg("As part of the ^Thraal^ army corps, we are reclaiming this " ..
                  "plane after the defeat of Moroch.")
        ib:AddMsg("\n\nWe're only a small outpost of the Guild. Myself, ^Chereth^, " ..
                  ", ^Jahrlen^ and ^Taleus^ assigned here, but we get plenty of help " ..
                  "from the locals.")
		ib:AddMsg("\n\nWe are focused on ^physique^ and ^agility^, but we have also mercenarys in our guild using ^spells^.")
		ib:AddMsg("\n\n|^" .. npc.name .. "^ looks at you.|") -- so player can click the story about chashin again

		if questnr ~= 0 then
			ib:AddLink("Can you tell me more about the quest?", "quest")
		elseif gstat == game.GUILD_OLD then
    		ib:AddLink("I would like to rejoin the Mercenary Guild.", "rejoin") 
		elseif gstat == game.GUILD_IN then
		    ib:AddLink("I would like to leave the Mercenary Guild.", "leave") 
		end 
				  
    elseif subject == "chereth" then
        ib:SetTitle("Commander Chereth")
        ib:SetMsg("Former Archery Commander Chereth is now our Supply " ..
                 "Chief.\n")
        ib:AddMsg("\nAlthough she was seriously wounded in battle she " ..
                  "is still a great expert in archery.")
		ib:AddMsg("\n\nChereth is still very important for our ^guild^. You find her downstairs.")
		ib:AddLink("Can you tell me more about the guild?", "guild")

    elseif subject == "jahrlen" then
        ib:SetTitle("Chronomancer Jahrlen")
        ib:SetMsg("Jahrlen is our ^guild^ mage.\n")
        ib:AddMsg("\nOf course, normally we don't have a guild mage, " ..
                  "since we are mercenaries. But we are at war here " ..
                  "and he was assigned to us, so we treat him as one " ..
                  "of our own.\n")
        ib:AddMsg("\nIn fact he is a high level chronomancer and we " ..
                  "are honored to have him help us.\n")
        ib:AddMsg("\nHe is normally downstairs in the strategy room.")
		ib:AddLink("Can you tell me more about the guild?", "guild")
    elseif subject == "taleus" then
        ib:SetTitle("Commander Taleus")
        ib:SetMsg("Commander Taleus has taken over for Commander ^Chereth^ as " ..
                  "our local archery instructor for our ^guild^. He'll never tell you this, but " ..
                  "he's still a bit fresh. He'll get you started with the basics, though.")
        ib:AddMsg("\n\nHe's currently on an assignment in Stonehaven, helping a " ..
                  "citizen there. If you're looking for training, he should be " ..
                  "able to help you get started.")
		ib:AddLink("Can you tell me more about the guild?", "guild")
	elseif subject == "physique" then
        ib:SetTitle("Physique")
        ib:SetMsg("Physique (PH) is the way to handle close weapons. You can train your PH-Skill " ..
                  "by fighting with close weapons. You also need PH-Skill to wear better gear.")
		ib:AddMsg(" PH is our ^guild^ primary goal.")
		ib:AddLink("Can you tell me more about the guild?", "guild")
	elseif subject == "agility" then
        ib:SetTitle("Agility")
        ib:SetMsg("Agility is the way to handle range weapons. You can train your Agility-Skill " ..
                  "by fighting with range weapons. Agility is our ^guild^ secondary goal.")
		ib:AddMsg(" Ask ^Chereth^ or ^Taleus^ about archery")
		ib:AddLink("Can you tell me more about the guild?", "guild")
	elseif subject == "spells" then
        ib:SetTitle("Spells")
        ib:SetMsg("Yes, our ^guild^ members also know, how to use some spells. We also have some priests and mages in our ranks.")
		ib:AddMsg("You better ask ^Chereth^ or ^Jahrlen^ about this. You find them downstairs.")
		ib:AddLink("Can you tell me more about the guild?", "guild")
    else -- we have no logic here for other subject, this goes all to about Cashin here ???
        ib:SetTitle(npc:GetName())
        ib:SetMsg("I joined the ^guild^ 15 years ago when I came of age. My village " ..
                  "was on the verge of destruction by a large battalion of demons, " ..
                  "so I fled with nowhere to go. When I encountered a man from the " ..
                  "guild, he helped me get my feet on the ground and get started in " ..
                  "the guild.")
		ib:AddLink("Can you tell me more about the guild?", "guild")
    end
	ib:SetLHSButton("Hello")

    return true
end
mnpc:AddTopics("background", "(guild)")
mnpc:AddTopics("background", "(thraal)")
mnpc:AddTopics("background", "(chereth)")
mnpc:AddTopics("background", "(jahrlen)")
mnpc:AddTopics("background", "(taleus)")
mnpc:AddTopics("background", "(physique)")
mnpc:AddTopics("background", "(agility)")
mnpc:AddTopics("background", "(spells)")
mnpc:AddTopics("background", "(mercenary guild)")
-- mnpc:AddTopics("background", "(mercenary?i?e?'?s?%s?.*)") -- this works, but what is this ???
mnpc:AddTopics("background", "(" .. npc:GetName() .. ")")

--tl:AddTopics({ "(chereth)", "(jahrlen)", "(taleus)" }, topic_backgroundMembers)

-------------------
-------------------
local function topic_servicesJoin()
    ib:SetHeader("st_005", npc)
    ---------
    -- Player is not a guildmember and never has been (means he can not have
    -- done the first quest).
    ---------
	if gstat == game.GUILD_NO and questnr ~= 0 then
        ib:SetMsg("So you want to join the ^guild^?\n")
        ib:AddMsg("\n|** " .. npc.name .. " gives you the once over " ..
                  "again **|\n")
        ib:AddMsg("\nWell with the ogre problem here in the Province and " ..
                  "Moroch causing untold trouble all over, a person of " ..
                  "your apparent calibre would be more than welcome.\n")
        ib:AddMsg("\nBut first I need you to prove your mettle by " ..
                  "completing a simple ^quest^.")
    else
        ---------
        -- Player has already completed the quest, but somehow isn't in the guild.
        ---------
        if questnr == 0 and gstat == game.GUILD_NO then
            ib:SetMsg("Well, I must say that I am still very grateful for your services.")
            ib:AddMsg("\nI'm not sure why you aren't in the guild, but you're welcome to join us!")
            module_guildsJoin("Mercenary", player)
        ---------
        -- Player used to be a guildmember but left.
        ---------
        elseif gstat == game.GUILD_OLD then
            ib:SetMsg("|** " .. npc.name .. " frowns, but then his face " ..
                      "relaxes **|\n")
            ib:AddMsg("\nWell times are tough and good mercenaries don't " ..
                      "grow on trees, so no benefit in recriminations.\n")
            ib:AddMsg("\nWelcome back, Guildmember " .. player.name .. "!")
            module_guildsJoin("Mercenary", player)
        ---------
        -- Player is currently a guildmember.
        ---------
        else
            ib:SetMsg("|** " .. npc.name .. " looks down the member roll " ..
                      "**|\n")
            ib:AddMsg("\nErr... I admire your enthusiasm but you already " ..
                      "joined the guild.")
        end
    end
	if questnr > 0 then
		ib:SetLHSButton("Quest")
    else
		ib:SetLHSButton("Services")
    end
end

-------------------
-------------------
local function topic_servicesLeave()
    ib:SetHeader("st_005", npc)
    ib:SetMsg("|** " .. npc.name .. " looks down the member roll **|\n")

    if gstat ~= game.GUILD_IN then
        ib:AddMsg("\nAs you aren't a guildmember, I can't say I'm sorry " ..
                  "to lose you.")
    elseif module_guildsLeave("Mercenary", player) then
        ib:AddMsg("\nSorry to lose you, Guildmember " .. player.name .. ".")
    end

	-- normal player can't be in guild without finishing the quest, but we let this for cheating gm's or for future changes
    if questnr > 0 then
        ib:SetLHSButton("Quest")
    else
        ib:SetLHSButton("Services")
    end
end

local tl = ScriptContext:GetUtility("tl")
tl:AddTopics({ "join", "rejoin", "join guild", "rejoin guild" },
             topic_servicesJoin)
tl:AddTopics({ "leave", "leave guild"}, topic_servicesLeave)

mnpc:ShowInterface()
