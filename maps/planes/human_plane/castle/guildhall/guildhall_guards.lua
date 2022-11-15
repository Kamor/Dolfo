local npc = event.me
local player = event.activator

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
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("\nWhat do you want?\n")
	ib:AddLink("Can you help me?", "diag6")
	ib:AddLink("Where is the exit?", "diag9")
	ib:AddLink("I have to report a crime!", "diag1")
end

local function topic_imprison()
	player:Write("A strong force teleports you away.", 0)
	player:Sound(0, 0, game.SOUND_TELEPORT)
	player:SetPosition(game:ReadyMap("/planes/human_plane/castle/castle_030a"),4,4,game.OVERLAY_FIXED)
	ib:ShowSENTInce(0)
end

local function topic_diag1()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("What crime?")
	ib:AddLink("Clobbers boot collection is boring!", "diag5")
	ib:AddLink("People is not afk!", "diag3")
	ib:AddLink("Smacky has a level 1 twink and kills cats!", "diag2")
	ib:AddLink("I have insulted another player!", "imprison")
	-- todo some more crimes
end

local function topic_diag2()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("That's nonsense. Smacky would never do this. This is a conspiracy theory!")
  ib:AddLink("No, i am sure!", "diag4")
end

local function topic_diag3()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("People is always afk!")
  ib:AddLink("No, i am sure he is not afk!", "diag4")
end

local function topic_diag4()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("Stop this, or i must imprison you.")
  ib:AddLink("But that's true!", "imprison")
end

local function topic_diag5()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("Don't blame clobbers boot collection! It's the biggest boot collection on all known spheres.")
  ib:AddLink("That's not right. Somewhere far far away, there is this other girl ...", "imprison")
end

local function topic_diag6()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("I am a guard| Do you think i am a chat bot?")
  ib:AddLink("But you are doing nothing here!", "diag7")
end

local function topic_diag7()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("Better leave now, before i get angry!")
  ib:AddLink("Haha. You are a guard npc doing nothing!", "diag8")
end

local function topic_diag8()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("I guard this area. If enemy's come to this place i protect you.")
  ib:AddLink("lol", "imprison")
end

local function topic_diag9()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("\nYou are joking?\n")
  ib:AddLink("No, please help me! I need to find the exit!", "diag10")
end

local function topic_diag10()
  ib:SetHeader("st_002", npc)
  ib:SetTitle("Talking")
  ib:SetMsg("\nDon't anger me! You really want to see the exit?\n")
  ib:AddLink("Yes please!", "imprison")
end

require("topic_list")

local tl = TopicList()

tl:SetDefault(topic_default)
tl:AddGreeting("what now%??", topic_greeting)
tl:AddTopics({ "imprison" }, topic_imprison)

tl:AddTopics({ "diag1" }, topic_diag1)
tl:AddTopics({ "diag2" }, topic_diag2)
tl:AddTopics({ "diag3" }, topic_diag3)
tl:AddTopics({ "diag4" }, topic_diag4)
tl:AddTopics({ "diag5" }, topic_diag5)
tl:AddTopics({ "diag6" }, topic_diag6)
tl:AddTopics({ "diag7" }, topic_diag7)
tl:AddTopics({ "diag8" }, topic_diag8)
tl:AddTopics({ "diag9" }, topic_diag9)
tl:AddTopics({ "diag10" }, topic_diag10)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))