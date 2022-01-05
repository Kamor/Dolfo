local me= event.me
local pl = event.activator

require("interface_builder")
local ib = InterfaceBuilder()

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

local function topic_greeting()
    -- ib:SetHeader("st_002", me)
    ib:SetTitle("Welcome to Dolfo's World!")
    ib:SetMsg("\nThis is my presentation server.\n")
	ib:AddMsg("\nThese are the public accessible maps from www.daimonin.org. So they are the older maps.\n")
	ib:AddMsg("\nI made some changes and build my ideas here, so dev team or players can see it.\n")
	ib:AddMsg("\nThis is not a full game, this are the early maps from public trunk.\n")
	ib:AddMsg("\nI concentrate on early balancing and try to build new ideas.\n")
	ib:AddMsg("\nSee news, so you know where to look for new or changed content.\n")
	ib:AddMsg("\nFor more informations about the coding go https://github.com/Kamor/Dolfo.\n")
	-- ib:AddMsg("\nuse command ~/exp~ to see current server exp.")
	ib:AddLink("News")
	ib:SetLHSButton("News")
end

local function topic_news()
    ib:SetHeader("st_002", me)
    ib:SetTitle("News")
	ib:AddLink("Weapon Trainers", "trainers")
	ib:AddLink("Credit cards", "creditcard")
	ib:AddLink("NPC's", "npcs")
	ib:AddLink("food system", "food")
	ib:SetLHSButton("Hello")
end

local function topic_trainers()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Weapon Trainers")
    ib:SetMsg("This is my idea of opening all four ph skills and all 3 archery skills and a kind of npc skill trainer systen.")
	ib:AddMsg("\n\nYou need to find a trainer npc, who can train you.\n\nTrainer npc's can train you up to their own level.")
	ib:AddMsg("\n\nHigher trainers want more fee for training than lower trainers.")
	ib:AddMsg("\n\nTrainers can only train up to your skill group level.")
	ib:AddMsg("\n\nSo you need to train your physique first before you can train weapon skills.")
	ib:AddMsg("\n\nSo you need to train your agility first before you can train archery skills.")
    ib:AddMsg("\n\nFor weapon skill go to Gimli by the guildhall entrance.")
	ib:AddMsg("\n\nFor archery skills go to Archery Commander Taleus in Stonehave Castle before the shop.\n")
	ib:SetLHSButton("Back","news")
end

local function topic_npcs()
    ib:SetHeader("st_002", me)
    ib:SetTitle("New and upgrades npc's")
	ib:AddMsg("\nDolfo NPC+dolfo.lua : Presentation npc, ~player view clean, need code optimize~\n")
    ib:SetMsg("\nStupir NPC+stupir.lua : Smalltalk npc - |need a fix on long links, not fixable by lua only|\n")
	ib:AddMsg("\nfanrir.lua : improved key logic, easter egg, shop - |need some more cleaning|\n")
	ib:AddMsg("\nGimli NPC+gimli.lua : weapon trainer npc, ~player view clean, need code optimize~\n")
	ib:AddMsg("\ncashin.lua : fixed bug, reworked logic, ~player view nearly finished~\n")
	ib:AddMsg("\nchereth.lua : ~auto heal when hp<30~, heal and services command, very early state, |minor healing output error, not a lua fix|\n")
	ib:AddMsg("\nguildhall_guard.lua : Smalltalk and jailing guards, hint to secret kobold map.\n")
	ib:AddMsg("\ntaleus.lua : archery trainer, in work, needs some more fine tuning in logic, type cheat or cheat1\n")
	ib:AddMsg("\nAriane NPC+ariane.lua : potion trader(guildhall basement), type cheat\n")
	ib:SetLHSButton("Back","news")
end

local function topic_food()
    ib:SetHeader("st_002", me)
    ib:SetTitle("New food system")
    ib:SetMsg("This is my idea of rebalancing the food system.")
	ib:AddMsg("\n\nThere are potions now, like food, but faster and with higher stats.")
	ib:AddMsg("\n\nThere is percent food now, refreshing your stats in relation to you max stats.")
	ib:AddMsg("\n\nFood is capped to eat only one piece at time.")
	ib:AddMsg("\n\nTavern food is percent food now.")
	ib:AddMsg("\n\nGo to Ariane in guild basement to buy new potions.")
	ib:AddMsg("\n\nUse cheat there to see also the higher potions.")
	ib:AddMsg("\n\nShe gives you also a test potion to reduce your stats to test the other potion.")
	ib:AddMsg("\n\nBeware this test potion can kill you!")
	ib:SetLHSButton("Back","news")
end

local function topic_creditcard()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Credit cards")
    ib:SetMsg("This is my idea of implementing a kind of credit card system to pay with.")
	ib:AddMsg("\n\nIt's a very early prototyp to test this feature.")
	ib:AddMsg("\n\nYou want a card?")
	ib:AddLink("Sure", "givecard")

	ib:SetLHSButton("Back","news")
end

local function topic_givecard()
    ib:SetHeader("st_002", me)
    ib:SetTitle("Credit cards")
    ib:SetMsg("Here you have one!")
	ib:AddMsg("\n\nAs is said this is a very early prototyp of this card!")
	ib:AddMsg("\n\nIf you need money, you can sell this card and i give you a new one. Or type cheat, if you need more money!")
	ib:AddMsg("\n\nYou also need to go to a bank and open a bank account first! Later credit cards would be sold there.")
	ib:AddMsg("\n\nTo use this card, you also need to mark (M-Key) it before!")

	local card = pl:CreateObjectInsideEx("credit_card", game.IDENTIFIED, 1)
	card.value=10000
    assert(card, "Could not create card!")

	ib:SetLHSButton("Back","news")
end


local function topic_cheat()
    ib:SetHeader("st_005", me)
	ib:SetMsg("Cheat money.")
    ib:SetCoins(0, 0, 0, 1)
	pl:AddMoney(0, 0, 0, 1)
	ib:SetLHSButton("Hello")
end

require("topic_list")

local tl = TopicList()

tl:SetDefault(topic_default)
tl:AddGreeting("what now%??", topic_greeting) -- todo why what now, when we overwrite title with ib:SetTitle later? but we currently need link to function here
tl:AddTopics({ "news" }, topic_news)
tl:AddTopics({ "trainers" }, topic_trainers)
tl:AddTopics({ "npcs" }, topic_npcs)
tl:AddTopics({ "food" }, topic_food)
tl:AddTopics({ "creditcard" }, topic_creditcard)
tl:AddTopics({ "givecard" }, topic_givecard)
tl:AddTopics({ "cheat" }, topic_cheat)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        