-------------------------------------------------------------------------------
-- fanrir.lua -- TALK on NPC at castle_030a 15 2
--
-- Advisor Fanrir teaches new players the basics of game mechanics via
-- a series of introductory quests. He is not like other NPCs in that he is
-- aware that the player is playing a game.
-------------------------------------------------------------------------------
-- QUESTS
--
-- 1. "Examine Fanrir's Sack"
-- Requirements: None.
-- Purpose     : Teach game mechanics (movement and directions,
--               talking to NPCs, quests, containers, examining items,
--               inventory).
-- Scenario    : Fanrir's sandals are in the sack next to him.
-- Goal        : Open the sack.
-- Reward      : The sandals.
-- 2. "Find Fanrir's Lunch"
-- Requirements: None.
-- Purpose     : Teach game mechanics (exits, keys, traps, getting and
--               dropping items, hit points and taking damage, resting,
--               eating).
-- Scenario    : Fanrir's lunch is in one of the three chests in his larder.
-- Goal        : Get the lunch, take it to Fanrir.
-- Reward      : 3 food.
-- 3. "Mouse Hunt"
-- Requirements: None.
-- Purpose     : Teach game mechanics (money and coin type, mobs, melee combat,
--               gaining experience, using a light source).
-- Scenario    : Fanrir's larder is infested with mice.
-- Goal        : Kill mice until you have 2 tails, take them to Fanrir.
-- Reward      : 10c and level 5 req dam+0.3 ring.
-------------------------------------------------------------------------------
-- todo player can drop quest bread before finishing
-- this forces player to reset quest
-- droping bread needs a kind of quest reset for the chest or some other int logic

-- todo droping bread we explain the player how 2 cancel the quest and restart it
-- todo we must look, when player have a key, cancel quest and restart quest for keys
-- todo some fixes on rose item and get key text
-- todo give key please when player has bread?
-- todo money for buying keys is 1 copper todo


local npc = event.me
local player = event.activator

---------------------------------------
---------------------------------------
-------------------
-- give_key() gives player a key to Fanrir's food chests if he doesn't already
-- have one -- keys can be dropped, but disappear when dropped.
-------------------
local function count_keys()
    ---------
    -- Check how many keys player had ask for.
    ---------
	local keys=0
	
    for obj in obj_inventory(player) do
        if obj.type == game.TYPE_KEY and
           obj.slaying == "Find Fanrir's Lunch" then
			keys = keys + 1
        end
    end
	return keys
end

local function give_key()
    ---------
    -- Generate a key
    ---------
        local key = player:CreateObjectInside("key2", game.IDENTIFIED)

        assert(key, "Could not create key!")
        key.title = "to Fanrir's food chests"
        key.slaying = "Find Fanrir's Lunch"
        player:Write(npc.name .. " hands you a key.")
        player:Write("He says: Here's a key to unlock my food chests.")
end

---------------------------------------
---------------------------------------
require("interface_builder")

local ib = InterfaceBuilder()

---------------------------------------
---------------------------------------
require("quest_builder")

local qb = QuestBuilder()

-------------------
-------------------
qb:AddQuest("Examine Fanrir's Sack", game.QUEST_NORMAL)
qb:AddQuest("Find Fanrir's Lunch", game.QUEST_ITEM)
qb:AddQuest("Mouse Hunt", game.QUEST_KILLITEM)

local questnr = qb:Build(player)

---------------------------------------
-- The following handles things which we always want to check/do, whatever
-- player says to npc.
--------------------------------------
---------
-- "Find Fanrir's Lunch"
---------
--if qb:GetStatus(2) == game.QSTAT_ACTIVE then
--    give_key()
--end

---------------------------------------
-- DEFAULT: The player has tried an unrecognised topic.
---------------------------------------
-------------------
-- topic_default() handles unrecognised topics, giving a nice helpful 'try
-- different words' response.
-- Remember, the topic may be perfectly reasonable but just something this NPC
-- does not specifically respond to, too complex to parse (perhaps there is a
-- simpler way to say the same thing), or complete gobbledigook (maybe the
-- player did a typo). The default response should be valid in all three cases.
-------------------
local function topic_default()
    ib:SetTitle("What?")
    ib:SetMsg("I'm sorry. I didn't understand.\n")
    ib:AddMsg("\n|[The interface's parsing is extremely limited, so keep " ..
              "your input simple. Usually type only a noun (name of " ..
              "something), such as 'quest', or sometimes a verb (an action " ..
              "to do) and a noun, such as 'repair sword', to talk about a " ..
              "subject. But it may also be that this particular NPC simply " ..
              "does not know anything about that topic.]|\n")
    ib:AddMsg("\n`Hint -- Making yourself understood`\n")
    ib:AddMsg("\nIn fact, all NPCs should respond to the ~standard topics~ " ..
              "'hello', 'quest', 'background', and 'rumours', and many " ..
              "offer 'services' as well.\n")
    ib:AddMsg("\nAfter the initial '/talk hello' to start a " ..
              "conversation, it is also acceptable to stick to just the " ..
              "hypertext, icons, links, and buttons.")
end

---------------------------------------
-- GREETING: A standard topic. The player has said 'hello', etc.
---------------------------------------
-------------------
-- topic_greeting() handles this standard topic.
-- Fanrir's reason for existence is to help new players learn the basic
-- elements of gameplay, learn some (default) keys and commands, and offer a
-- little guidance though some introductory quests.
-------------------
local function topic_greeting()
    ib:SetHeader("st_001", npc)

    ---------
    -- It's the player's first time, so lets be welcoming.
    ---------
    if questnr == 1 and
       qb:GetStatus(1) == game.QSTAT_NO then
        ib:SetTitle("Welcome!")
        ib:SetMsg("Hello and welcome to Daimonin! My name is " ..
                  npc:GetName() .. ".\n")
        ib:AddMsg("\nThings can seem a bit confusing here, so I will teach " ..
                  "you some basics if you'd like. It is quite easy. You'll " ..
                  "be done in just a few minutes.\n")
        ib:AddMsg("\nI have a number of ^quests^ for you to do. You'll be " ..
                  "helping me and you'll be helping yourself.\n")
        ib:AddMsg("\n`Hint -- Talking to NPCs (1)`\n")
        ib:AddMsg("\n~NPCs~ are non-player characters, like me. I am an " ..
                  "NPC. As a rule NPCs are friendly to you, in that they " ..
                  "don't actually attack you. Of course, that doesn't mean " ..
                  "they all necessarily like you!\n")
        ib:AddMsg("\nMost NPCs have something to say. You can begin a " ..
                  "conversation by saying 'hello' to them. There are three " ..
                  "steps to talking to an NPC:\n")
        ib:AddMsg("\n|(1)| target an NPC -- do this by pressing ~S~ (this " ..
                  "will actually cycle through all visible NPCs so you " ..
                  "might need to repeat this step several times to get the " ..
                  "right one);\n")
        ib:AddMsg("\n|(2)| stand near to the NPC -- each NPC has it's own " ..
                  "sensing range (usually 6 squares distance) and he or " ..
                  "she must be in line of sight; and\n")
        ib:AddMsg("\n|(3)| talk to the NPC -- do this by pressing ~T~.\n")
        ib:AddMsg("\nI guess you did this to talk to me, so lets tell you " ..
                  "something you might not know.\n")
        ib:AddMsg("\n`Hint -- Talking to NPCs (2)`\n")
        ib:AddMsg("\nOnce you are talking to an NPC a window like this one " ..
                  "will show what he or she is saying. You will often see " ..
                  "highlighted text.\n")
        ib:AddMsg("\nGold text is used for various titles.\n")
        ib:AddMsg("\nGreen and yellow are just for emphasis. Think of " ..
                  "~green as italics~ and |yellow as bold|.\n")
        ib:AddMsg("\nThere is also hypertext which you can select and  " ..
                  "'say'. Doing so will take you further into the " ..
                  "conversation. Hypertext is not highlighted in the main " ..
                  "window, but is shown in a side-panel. There are three " ..
                  "ways to 'say' hypertext:\n")
        ib:AddMsg("\n|(1)| click on it with your mouse;\n")
        ib:AddMsg("\n|(2)| cycle through all available hypertext with " ..
                  "~TAB~ (and ~sTAB~ to go the other way) then press " ..
                  "~RETURN~ when you have the one you want (it will be " ..
                  "purple);\n")
        ib:AddMsg("\n|(3)| press ~BACKSPACE~ and type the word in.\n")
        ib:AddMsg("\nTABbing through hypertext or hovernig over it with " ..
                  "the mouse) will also reposition the viewable window so " ..
                  "that you can see the context of the word you have " ..
                  "selected.\n")
        ib:AddMsg("\nTry selecting and 'saying' the hypertext on this page.")
        ib:SetLHSButton("Quest")
    ---------
    -- OK, player knows the drill.
    ---------
    else
        ib:SetMsg("Hello again " .. player.name ..".\n")

        ---------
        -- Player has not done all the quests.
        ---------
        if questnr ~= 0 then
            if qb:GetStatus(questnr) == game.QSTAT_NO then
                ib:AddMsg("\nJust let me know when you're ready for your " ..
                          "next ^quest^.")
				ib:AddLink("I am ready.", "quest")
                ib:AddLink("Perhaps later.", "")
			  
                ib:SetLHSButton("Quest")
            elseif qb:GetStatus(questnr) == game.QSTAT_ACTIVE or
                   qb:GetStatus(questnr) == game.QSTAT_SOLVED then
                ib:AddMsg("\nHow's progress on that ^quest^?")
                ib:AddLink("I've done what you asked.", "quest complete")
                ib:AddLink("Slow. But I'm working on it.",
                           "quest incomplete")
						   
				if qb:GetStatus(2) == game.QSTAT_ACTIVE then
					-- we allow player to lie for keys, for fanrir easter egg
					ib:AddLink("I lost my key.", "key")
				end
            end
        ---------
        -- Player has done all the quests.
        ---------
        else
            if player:GetQuest("The Mercenary Guild Quest") == nil then
                ib:AddMsg("\nNow you should go and talk to Mercenary " ..
                          "Master Cashin inside the building down the road.\n")
            else
                ib:AddMsg("\nIt seems to me that you're ready to go out " ..
                          "into the world and seek adventure, " ..
                          player:GetName() .. ".\n")
				ib:AddMsg("\nOh! By the way, you might have to train a bit " ..
							"before you can use the ring I gave you.")
            end

            ib:AddMsg("\n\nThank you and good luck!")
        end
		ib:SetLHSButton("Quest") -- tricky to do left button intelligent -------------------------------------
    end
end

-- Fanrir now allows multiply key's, but show players, she is not stupid.
local function topic_key(cmd)
    ib:SetHeader("st_002", npc)
	
	if qb:GetStatus(2) == game.QSTAT_ACTIVE then
		local keys=count_keys()
		if keys== 0 then
			ib:SetMsg("Ok, no problem. That could happen. Here is a new key.")
			give_key()	
		elseif keys == 1 then
			ib:SetMsg("Really? You have a hole in your pocket? That's not good. But don't worry, I give you a new key.")
			give_key()
		elseif keys == 2 then
			ib:SetMsg("Do you think, i don't know about the 2 keys you have? But if you like, here is a third one.")
			give_key()
		else
		
			if cmd == nil then
				ib:SetMsg("You have " .. keys .. " keys now.")
				ib:AddLink("Please can you give me a key, the other keys are broken or don't work.", "key please") -- this joke don't showed full because of limited link chars -- TODO
				
			-- now players only get more keys if they use "key please" ...
	        elseif cmd == "please" then
				if keys<5 then
					ib:SetMsg("Hm? But if you like, here is another one.")
					give_key()
				else
					ib:SetMsg("You have " .. keys .. " keys, they can't be all broken!")		
					ib:AddLink("No please. I really need more keys. Please.", "key please please")		
				end
			elseif cmd == "please please" then
				if keys==5 then
					ib:SetMsg("Wow. You really have to much time! But ey. here is another one.")
					give_key()
				elseif keys==6 then
					ib:SetMsg("You like this? No problem, here is another one.")
					give_key()
				elseif keys==7 then
					ib:SetMsg("I hope you have fun, here is another one.")
					give_key()
				else
					ib:SetMsg("What do you want with " .. keys .. " keys?")		
					ib:AddLink("No please. Please I really need more keys. Please. Please", "key please please please")		
				end
			elseif cmd == "please please please" then	
				if keys==8 then
					ib:SetMsg("Ok. As you wish. Here is another one.")
					give_key()
				elseif keys==9 then
					ib:SetMsg("Sure. Why not. Here is another one.")
					give_key()
				elseif keys==10 then
					ib:SetMsg("Come on! Really? Here you have another key.")
					give_key()	
				else
					ib:SetMsg("Sorry no more keys for free! But you can buy more keys, if you need.")
					ib:AddLink("Yes, I want to buy keys.", "services")		
				end					
			else
				ib:SetMsg("What are you stuttering?")
			end

		end
		-- these links are for testing
		ib:AddMsg("\n\nTestlinks - will be removed later ...")
		ib:AddLink("I lost my key.", "key")
		ib:AddLink("I lost my key. Please", "key please")
		ib:AddLink("I lost my key. Please. Please", "key please please")
		ib:AddLink("I lost my key. Please. Please. Please", "key please please please")
	end
end

---------------------------------------
-- BACKGROUND: A standard topic.
---------------------------------------
-------------------
-------------------
local function topic_background()
    ib:SetHeader("st_002", npc)
    ib:SetMsg("~TODO~")
end

---------------------------------------
-- QUEST: A standard topic.
---------------------------------------
-------------------
-- This handles general quest enquiries.
-------------------
local function topic_quest()
    ib:SetHeader("st_003", npc)

    ---------
    -- Player has completed all quests.
    ---------
    if questnr == 0 then
        ib:SetMsg("I am sorry, but I have no more to teach you.\n")
        ib:AddMsg("\nYour next step should be to go and talk to Cashin of " ..
                  "the Mercenary Guild.\n")
        ib:AddMsg("\nYou'll find him in the first building just down this " ..
                  "road.")
    ---------
    -- Next quest is disallowed (ie, player does not meet level/skill req).
    ---------
    elseif questnr < 0 then
        ib:SetMsg("I have nothing for you at the moment.")
    ---------
    -- Player is eligible for a new quest.
    ---------
    else
        ib:SetTitle(qb:GetName(questnr))

        ---------
        -- "Examine Fanrir's Sack"
        ---------
        if questnr == 1 then
            ib:SetMsg("This is your first ~quest~!\n")
            ib:AddMsg("\nOpen the sack next to me and examine the sandals " ..
                      "inside. Then ~T~alk to me again.\n")
            ib:AddMsg("\n`Hint -- Accepting a quest`\n")
            ib:AddMsg("\nThere are three ways to accept a quest:\n")
            ib:AddMsg("\n|(1)| click the accept button below;\n")
            ib:AddMsg("\n|(2)| press the highlighted hotkey on that " ..
                      "button, here ~A~; or\n")
            ib:AddMsg("\n|(3)| press ~BACKSPACE~ and type 'accept quest'.\n")
            ib:AddMsg("\nWhere there is an accept button, there should " ..
                      "also be a corresponding decline button. The quest " ..
                      "can obviously be declined in similar fashion.")
        ---------
        -- "Find Fanrir's Lunch"
        ---------
        elseif questnr == 2 then
            ib:SetMsg("I don't know about you, but I'm a bit peckish now. " ..
                      "Just down the stairs over there to the west is my " ..
                      "larder. In it are three chests.\n")
            ib:AddMsg("\nPlease go down there and find some food.\n")
            ib:AddMsg("\nCareful though! I've been having problems with " ..
                      "mice down there recently. Filthy vermin!\n")
            --ib:AddMsg("\nTwo of the chests have traps in them. The food is " ..
            --          "in the third, but I can't remember which is which!\n")
			-- thats wrong all 3 chests are trapped
			ib:AddMsg("\nBe aware of trapped chests. " ..
                      "I can't remember in which chest i put my food!\n")
            ib:AddMsg("\nIf you see any of the mice just ignore them. They " ..
                      "won't attack you if you leave them alone... They " ..
                      "just eat my food! *grr*")
        ---------
        -- "Mouse Hunt"
        ---------
        elseif questnr == 3 then
            ib:SetMsg("This will teach you the basics of combat and " ..
                      "recovering your health afterwards.\n")
            ib:AddMsg("\nReturn to my larder. Remember those mice I told " ..
                      "you about? Well it seems they're learning to avoid " ..
                      "my traps.\n")
            ib:AddMsg("\nCould you kill some of the mice please? I'll pay " ..
                      "you ~10 copper coins~, and maybe something else, if you can bring" ..
                      "back two of their tails.\n")
            ib:AddMsg("\n`Hint -- The value of money`\n")
            ib:AddMsg("\nI'm sure you already know what money is. There " ..
                      "are four denominations of coins:\n")
            ib:AddMsg("\n|(1)| a ~copper~ coin is the lowest value. " ..
                      "Karamor down the road might sell you a torch for a " ..
                      "few of these. Or save them up because ~100~ are " ..
                      "worth:\n")
            ib:AddMsg("\n|(2)| a ~silver~ coin. You can probably pick up a "..
                      "beginner's sword for a couple of these. But if you " ..
                      "hang on to them until you have ~100~, that's worth:\n")
            ib:AddMsg("\n|(3)| a ~gold~ coin which just happens to bbe the " ..
                      "monthly rent on my larder. But I assume you won't " ..
                      "be renting a larder, so with the saving after " ..
                      "~1000~ months you'll have the equivalent of:\n")
            ib:AddMsg("\n|(4)| a ~mithril~ coin. I reckon you could buy a " ..
                      "building the size of the Mercenary's Guild with that!")
        end

        ---------
        -- Player is being offered a quest.
        ---------
        if qb:GetStatus(questnr) == game.QSTAT_NO then
            ib:SetAccept("Accept", "accept quest")
        end
    end
end

-------------------
-- topic_questAccept() handles player accepting a quest.
-- Note that it is allowed to accept a quest even if it has not been offered
-- yet.
-- This is by design (all right, also it just worked out that way ;)) as it
-- enables players who have been through it all before (for example they're
-- playing with an alt) to simply cut to the chase with '/talk accept quest'.
-------------------
local function topic_questAccept()
    ib:SetHeader("st_003", npc)

    ---------
    -- Player has completed all quests or next quest is disallowed (ie, player
    -- does not meet level/skill req).
    ---------
    if questnr <= 0 then
        return topic_quest()
    end

    ---------
    -- Player is in the middle of a quest already.
    ---------
    if qb:GetStatus(questnr) ~= game.QSTAT_NO then
        ib:SetMsg("Whoah! Slow down there. You haven't even told me how " ..
                  "you got on with the previous ^quest^.")
    ---------
    -- Player is eligible for a quest.
    ---------
    else
        ib:SetTitle(qb:GetName(questnr))

        ---------
        -- "Examine Fanrir's Sack"
        ---------
        if questnr == 1 then
            ib:SetMsg("`Hint -- Moving`\n")
            ib:AddMsg("\nSo first you need to walk over to the sack. " ..
                      "Daimonin uses the eight compass directions, north, " ..
                      "northeast, east, and so on. North is diagonally up " ..
                      "and right on your screen. So from your starting " ..
                      "position the water is to the north and " .. npc.name ..
                      " is to the west.\n")
            ib:AddMsg("\nMovement is done with the ~numberpad~, usually on " ..
                      "the right side of your keyboard: ~9~ is north, ~6~ " ..
                      "is northeast, ~3~ is east, and so on round to ~8~ " ..
                      "which is northwest.\n")
            ib:AddMsg("\n`Hint -- Opening a container`\n")
            ib:AddMsg("\nA ~container~ is a chest, sack or bag, a shelf " ..
                      "on a wall, a bookcase or a desk, or a similar " ..
                      "object. There are three steps to opening a " ..
                      "container:\n")
            ib:AddMsg("\n|(1)| stand on top of the container;\n")
            ib:AddMsg("\n|(2)| move the blue cursor over it -- move the " ..
                      "cursor with the ~CURSOR KEYS~ (the ones with arrows " ..
                      "on them); and\n")
            ib:AddMsg("\n|(3)| apply the container -- do this by pressing " ..
                      "~A~.\n")
            ib:AddMsg("\nWhen you have opened the container you can " ..
                      "examine the items inside -- move the cursor over " ..
                      "them as before and press ~E~.")
            ib:SetDesc("Open the sack next to Fanrir (~A~pply it) and " ..
                       "~E~xamine the sandals which are inside.\n")
            ib:AddDesc("\nThen ~T~alk to him again.")
            qb:RegisterQuest(1, npc, ib)
        ---------
        -- "Find Fanrir's Lunch"
        ---------
        elseif questnr == 2 then
            ib:SetMsg("`Hint -- Using an exit`\n")
            ib:AddMsg("\nAn ~exit~ is a set of stairs, a rope ladder, a " ..
                      "hole in the ground, a teleporter, or a similar " ..
                      "object. Just as with opening a container, which you " ..
                      "mastered in the previous quest, there are three " ..
                      "steps to using an exit:\n")
            ib:AddMsg("\n|(1)| stand on top of the exit;\n")
            ib:AddMsg("\n|(2)| move the cursor over it with the ~CURSOR " ..
                      "KEYS~; and\n")
            ib:AddMsg("\n|(3)| apply the exit with ~A~.\n")
            ib:AddMsg("\nWhen you use an exit successfully you will be " ..
                      "moved to a new map.\n")
            ib:AddMsg("\n`Hint -- Keys`\n")
            ib:AddMsg("\nSometimes chests and doors will be locked. In " ..
                      "this case, the *cough* only way you will open them " ..
                      "is if you have the right key.\n")
            ib:AddMsg("\nAll my food chests are locked. Although that " ..
                      "doesn't seem to keep the mice out... But you'll " ..
                      "need to use the key I have given you.\n")
            ib:AddMsg("\nUsing a key is automatic. Just try to open the " ..
                      "chest and if the key is in your inventory, you'll " ..
                      "unlock it first. They'll automatically lock again " ..
                      "when closed, so don't lose the key!\n")
            ib:AddMsg("\n(Actually, if you do just talk to me again and " ..
                      "I'll give you a new one.)\n")
            ib:AddMsg("\n`Hint -- Trapped chests`\n")
            ib:AddMsg("\nIf you're unlucky enough to open a ~trapped~ " ..
                      "chest, the trap will be sprung immediately and you " ..
                      "will take some damage.\n")
            ib:AddMsg("\nIf your hitpoints ever reach zero you will die " ..
                      "(but don't worry, these traps aren't that strong).")
            ib:SetDesc("The stairs just west of Fanrir lead to his larder.\n")
            ib:AddDesc("\nGo down them (~A~pply them) and find his lunch " ..
                       "in one of the chests down there.\n")
            ib:AddDesc("\nLook out for mouse traps!")
            qb:RegisterQuest(2, npc, ib)
            qb:AddQuestItem(2, 1, "quest_object", "bread02.101", "Fanrir's lunch")
            give_key()
        ---------
        -- "Mouse Hunt"
        ---------
        elseif questnr == 3 then
            ib:SetMsg("`Hint -- Attacking mobs`\n")
            ib:AddMsg("\n'~Mobs~' is just another word for monsters and " ..
                      "monsters are anything alive (or later on, undead " ..
                      "too!) that is your enemy. So mice are monsters are " ..
                      "mobs.\n")
            ib:AddMsg("\nOK, now that introductions are over, the " ..
                      "important question: how do you attack them? There " ..
                      "are three steps to attacking a mob:\n")
            ib:AddMsg("\n|(1)| turn on combat mode -- do this by pressing " ..
                      "~C~ (pressing it a second time turns combat mode " ..
                      "off again -- you can see which mode you are in by " ..
                      "looking at the icon in the lower left part of the " ..
                      "screen);\n")
            ib:AddMsg("\n|(2)| target a mob -- do this by pressing ~X~ " ..
                      "(this will actually cycle through all visible mobs " ..
                      "so you might need to repeat this step several times " ..
                      "to get the right one); and\n")
            ib:AddMsg("\n|(3)| stand next to the mob and you will " ..
                      "automatically attack him.\n")
            ib:AddMsg("\nDon't forget, most mobs fight back!")
            ib:SetDesc("Return to Fanrir's larder and exterminate some of " ..
                       "the mice down there for him.\n")
            ib:AddDesc("\nBring back two mouse tails to prove your valour.")
            ib:SetCoins(10, 0, 0, 0)
            qb:RegisterQuest(3, npc, ib)
            local target = qb:AddQuestTarget(3, 2, 2, "mouse", "Mouse")
            target:AddQuestItem(2, "quest_object", "tail_rat.101", "Mouse tail")
            require("/scripts/first_weapon")
        end
    end
end

-------------------
-- topic_questDecline() handles player declining a quest.
-- Note that it is allowed to decline a quest even if it has not been offered
-- yet.
-- This is by design (all right, also it just worked out that way ;)) as it
-- enables players who have been through it all before (for example they're
-- playing with an alt) to simply cut to the chase with '/talk decline quest'.
-------------------
local function topic_questDecline()
    ib:SetHeader("st_003", npc)

    ---------
    -- Player has completed all quests or next quest is disallowed (ie, player
    -- does not meet level/skill req).
    ---------
    if questnr <= 0 then
        return topic_quest()
    end

    ---------
    -- Player is in the middle of a quest already.
    ---------
    if qb:GetStatus(questnr) ~= game.QSTAT_NO then
        ib:SetMsg("Whoah! Slow down there. You haven't even told me how you " ..
                  "got on with the previous ^quest^.")
    ---------
    -- Refusal accepted.
    ---------
    else
        ib:SetMsg("That's a shame. Oh well, your choice.\n")
        ib:AddMsg("\n`Hint -- Declining a quest`\n")
        ib:AddMsg("\nDon't worry though, declining a ^quest^ is never " ..
                  "permanent. Next time you talk to the NPC, as long as " ..
                  "you still meet the requirements, you'll be offered the " ..
                  "quest again.")
    end
end

-------------------
-- topic_questComplete() handles player claiming to have solved the current
-- quest.
-- Remember that just because player says something doesn't make it so.
-- Always check the quest status.
-------------------
local function topic_questComplete()
    ib:SetHeader("st_003", npc)

    ---------
    -- Player has completed all quests or next quest is disallowed (ie, player
    -- does not meet level/skill req).
    ---------
    if questnr <= 0 then
        return topic_quest()
    end

    ---------
    -- Player lied! Told you... ;)
    ---------
    if qb:GetStatus(questnr) ~= game.QSTAT_SOLVED then
        ib:SetMsg("|** " .. npc.name .. " looks you up and down **|\n")

        ---------
        -- Quest not even accepted yet.
        ---------
        if qb:GetStatus(questnr) == game.QSTAT_NO then
            ib:AddMsg("\nI fail to see how you've managed that as I have " ..
                      "yet to give you the ^quest^.")
        ---------
        -- Quest is active.
        ---------
        elseif qb:GetStatus(questnr) == game.QSTAT_ACTIVE then
            ib:AddMsg("\n")
            qb:AddItemList(questnr, ib)
            ib:AddMsg("\n\nNo, you have yet to do everything I asked of you.\n")
            ib:AddMsg("\n`Hint -- The quest list`\n")
            ib:AddMsg("\nYou can check your progress on any quest in " ..
                      "progress in your ^quest^ list.\n")
            ib:AddMsg("\nJust press ~Q~ or click the ~Quest~ button in the " ..
                      "top right corner of the screen to open the quest " ..
                      "list.\n")
        end
    ---------
    -- Quest really is complete.
    ---------
    else
        ---------
        -- "Examine Fanrir's Sack"
        ---------
        if questnr == 1 then
            ib:SetMsg("Well done! I'll tell you what. As a reward for a " ..
                      "job well done I'm going to give you those sandals.\n")
            ib:AddMsg("\n|** " .. npc.name .. " gives you the sandals **|\n")
            ib:AddMsg("\n`Hint -- Your inventory`\n")
            ib:AddMsg("\nYour ~inventory~ is every item you carry.\n")
            ib:AddMsg("\nTo open your inventory, and see what you've got, " ..
                      "hold down ~SHIFT~. This will open a window in the " ..
                      "lower left corner of the screen.\n")
            ib:AddMsg("\nWhile ~SHIFT~ is held down you can move the blue " ..
                      "cursor around in this window with the ~CURSOR KEYS~ " ..
                      "to highlight individual items. Release ~SHIFT~ to " ..
                      "close your inventory again.\n")
            ib:AddMsg("\nAs you would expect, the total weight of your " ..
                      "inventory has an effect on your speed -- basically, " ..
                      "the more you carry, the slower you become.\n")
            ib:AddMsg("\nYour carry limit and the total weight of your " ..
                      "inventory are shown in the same area as the " ..
                      "inventory window (so they are only visible when it " ..
                      "is closed).\n")
            ib:AddMsg("\n`Hint -- Wearing/removing equipment`\n")
            ib:AddMsg("\nDuring your adventures you will find various " ..
                      "objects which you can wear or wield, such as " ..
                      "weapons, rings, magic devices, armour, clothing, " ..
                      "headwear, and footwear like my sandals. " ..
                      "Collectively these objects are known as " ..
                      "~equipment~. There are three steps to " ..
                      "wearing/removing equipment:\n")
            ib:AddMsg("\n|(1)| The object must be in your inventory, so " ..
                      "open your inventory with ~SHIFT~;\n")
            ib:AddMsg("\n|(2)| move the cursor over the object with the " ..
                      "~CURSOR KEYS~; and\n")
            ib:AddMsg("\n|(3)| apply it with ~A~.\n")
            ib:AddIcon("Fanrir's sandals", "sandals.101",
                       "|[Fanrir's sandals are now in your inventory]|")
            qb:Finish(1, "Fanrir's sandals")
            local sandals = player:CreateObjectInsideEx("sandals", game.IDENTIFIED)
            assert(sandals, "Could not create sandals!")
            sandals.name = "Fanrir's sandals"
            sandals.f_is_named = 1
            if not player:GetEquipment(game.EQUIP_BOOTS) then
                player:Apply(sandals, game.APPLY_ALWAYS)
            end
        ---------
        -- "Find Fanrir's Lunch"
        ---------
        elseif questnr == 2 then
            ib:SetMsg("Congratulations! Sorry again for the rocks but I'm " ..
                      "sure it was worth it.")
            ib:AddIcon("Bread", "bread02.101",
                       "|** Fanrir offers you some of his lunch as a " ..
                       "reward **|", 4)
            qb:Finish(2, "part of Fanrir's lunch")
            local bread = player:CreateObjectInsideEx("bread1", game.IDENTIFIED, 4)
            assert(bread, "Could not create bread!")
        ---------
        -- "Mouse Hunt"
        ---------
        elseif questnr == 3 then
            ib:Sound(true)
            ib:SetMsg("Many thanks! That should slow them down a bit.\n")
            ib:AddMsg("\nWell, that deserves a reward. Here you go. And I think " ..
                "you also deserve this ring. It has a ~level requirement~ of level 5," ..
                "which you can see by highlighting it in your inventory. " ..
                "This means you will have to be level 5 before you can use that " ..
                "ring, mind you.")
            ib:SetCoins(10, 0, 0, 0)
            ib:AddIcon("Fanrir's Ring", "ring_tin.101", "Fanrir has also given you a ring", 1)
            qb:Finish(3, "10 copper coins")
            player:AddMoney(10, 0, 0, 0)
            local ring = player:CreateObjectInsideEx("ring_generic", game.IDENTIFIED)
            assert(ring, "Could not create ring!")
            ring.name = "Fanrir's Ring"
            ring.damage = 3
            ring.item_level = 5
            ring:MakeEgo(game.EGOITEM_MODE_UNBOUND)
        end

        if questnr ~= 0 then
            if qb:GetQuestNr() > questnr and
               qb:GetStatus(questnr + 1) == game.QSTAT_NO then
                ib:SetLHSButton("Quest")
            elseif qb:GetQuestNr() == questnr then
                ib:SetLHSButton("What now?")
            end
        end
    end
end

-------------------
-- topic_questIncomplete() handles player claiming to have not solved the
-- current quest.
-- Remember that just because player says something doesn't make it so.
-- Always check the quest status.
-------------------
local function topic_questIncomplete()
    ib:SetHeader("st_003", npc)

    ---------
    -- Player has completed all quests or next quest is disallowed (ie, player
    -- does not meet level/skill req).
    ---------
    if questnr <= 0 then
        return topic_quest()
    end

    ---------
    -- Player's lied!
    ---------
    if qb:GetStatus(questnr) ~= game.QSTAT_ACTIVE then
        ib:SetMsg("|** " .. npc.name .. " looks at you quizzically **|\n")

        ---------
        -- Quest not even accepted yet.
        ---------
        if qb:GetStatus(questnr) == game.QSTAT_NO then
            ib:AddMsg("\nI fail to see how you've managed that as I have " ..
                      "yet to give you the ^quest^.")
        ---------
        -- Quest is solved.
        ---------
        elseif qb:GetStatus(questnr) == game.QSTAT_SOLVED then
            ib:SetTitle(qb:GetName(questnr))
            ib:AddMsg("\n")
            qb:AddItemList(questnr, ib)
            ib:AddMsg("\nUm, you might want to rethink your answer for this ^quest^ ...")
        end
    ---------
    -- Player really is as useless as he claims...
    ---------
    else
        ib:SetTitle(qb:GetName(questnr))
        ib:SetMsg("Never mind, I am patient. Keep on trying your ^quest^.")
    end
end

---------------------------------------
-- RUMOURS: A standard topic.
---------------------------------------
-------------------
-- topic_rumours() handles this standard topic.
-------------------
local function topic_rumours()
    ib:SetHeader("st_004", npc)
    ib:SetMsg("~TODO~")
end

---------------------------------------
-- SERVICES: A standard topic.
---------------------------------------
-------------------
-- topic_services() handles this standard topic.
-------------------
local function topic_services()
    ib:SetHeader("st_005", npc)
	local keys=count_keys() -- +1000 -- if you want to see exploding costs
	
    ib:SetMsg("Due to high demand for my keys, i started business on selling keys. You have ".. keys .. " keys.\n")
	ib:AddMsg("\nYou have " .. player:ShowCost(player:GetMoney()) .. "\n")
	ib:AddMsg("\nIf you ask why my keys get more expensive when buying them, this is supply and demand rule.\n")
	ib:AddMsg("\nBut above 10 keys you gain a free rose.\n") -- todo
	
	local price=10+keys*keys*keys
	ib:AddSelect("Buy key", "buy", "key2.111", player:ShowCost(price))
end

local function topic_buy()

    ib:SetHeader("st_005", npc)
	local keys=count_keys()
	local price=10+keys*keys*keys
	
	price = 1
	
	if player:PayAmount(price) == 1 then
		give_key()
	    ib:SetMsg("Here you have it.\n")
		ib:AddIcon("Key", "key2.111", "high quality, unbreakable.", 1)
		-- todo icon
		if keys>9 then
			    ib:AddMsg("And a rose. |Fanrir smiles at you.|\n")
				
				ib:AddIcon("red rose", "redrose.101", "without plant fertilizers", 1)
				local rose = player:CreateObjectInsideEx("redrose", game.IDENTIFIED, 1)
				assert(rose, "Could not create rose!")
				

		end
		
	end

	ib:SetLHSButton("Back","services")
end

---------------------------------------
-- Sort out all the tl stuff.
---------------------------------------
require("topic_list")

local tl = TopicList()

tl:SetDefault(topic_default)
tl:AddGreeting("what now%??", topic_greeting)
tl:AddBackground(nil, topic_background)
tl:AddQuest(nil, topic_quest)
tl:AddTopics("accept quest", topic_questAccept)
tl:AddTopics("decline quest", topic_questDecline)
tl:AddTopics("quest complete", topic_questComplete)
tl:AddTopics("quest incomplete", topic_questIncomplete)
tl:AddTopics({"key", "key (please)", "key (please please)", "key (please please please)"}, topic_key)
tl:AddTopics("buy", topic_buy)
tl:AddRumours(nil, topic_rumours)
tl:AddServices(nil, topic_services)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
