require("interface_builder")
local ib = InterfaceBuilder()
--local ib = ScriptContext:GetUtility("ib")

require("module/npc")
--local mnpc = ScriptContext:GetModule("npc")

local player = event.activator
-- local player = ScriptContext:GetObject("player") --> TODO how new system replace other event.activator ?
local npc = event.me
-- local npc = ScriptContext:GetObject("npc")

--> no need tor this, it's in module/npc and it's somewhere else too, so it's also there without loading module/npc
--> it also here tl:SetDefault(topic_default) ,this is redundance ;-)
--local function topic_default()
--    ib:SetTitle("What?")
--    ib:SetMsg("I'm sorry. I didn't understand.\n")
--    ib:AddMsg("\n|[The interface's parsing is extremely limited, so keep " ..
--              "your input simple. Usually type only a noun (name of " ..
--              "something), such as 'quest', or sometimes a verb (an action " ..
--              "to do) and a noun, such as 'repair sword', to talk about a " ..
--              "subject. But it may also be that this particular NPC simply " ..
--              "does not know anything about that topic.]|\n")
--    ib:AddMsg("\n`Hint -- Making yourself understood`\n")
--    ib:AddMsg("\nIn fact, all NPCs should respond to the ~standard topics~ " ..
--              "'hello', 'quest', 'background', and 'rumours', and many " ..
--              "offer 'services' as well.\n")
--   ib:AddMsg("\nAfter the initial '/talk hello' to start a " ..
--              "conversation, it is also acceptable to stick to just the " ..
--              "hypertext, icons, links, and buttons.")
--end
--function mnpc.extend.Greeting(
local function topic_greeting()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nHi\n")
	ib:AddLink("What are you doing here?", "diag1")
	ib:SetLHSButton("next","diag1")
end

local function topic_diag1()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nI don't know. I don't understand this game?\n\n|Can you help me?|\n")
	ib:AddLink("Sure.", "diag2")
	ib:SetLHSButton("next","diag2")
end

local function topic_diag2()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|What should i do?|\n")
	ib:AddLink("Speak to Advisor Fanrir first.", "diag3")
	ib:SetLHSButton("next","diag3")
end

local function topic_diag3()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|Who is Advisor Fanrir?|\n")
	ib:AddLink("Advisor Fanrir is before your noise.", "diag4")
	ib:SetLHSButton("next","diag4")
end

local function topic_diag4()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|You are Advisor Fanrir?|\n")
	ib:AddLink("No, there, the girl with the white hair and the purple dress.", "diag5")
	ib:SetLHSButton("next","diag5")
end

local function topic_diag5()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nAh, that's Advisor Fanrir.\n\n|Is this important? Why you tell me this?|\n")
	ib:AddLink("Because you ask me, what to do in this game.", "diag6")
	ib:SetLHSButton("next","diag6")
end

local function topic_diag6()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|Ah ok and what should i do in this game?|\n")
	ib:AddLink("Speak to this white haired purple dressed woman.", "diag7")
	ib:SetLHSButton("next","diag7")
end

local function topic_diag7()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|You mean Advisor Fanrir?|\n")
	ib:AddLink("Yes.", "diag8")
	ib:SetLHSButton("next","diag8")
end

local function topic_diag8()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|I know who is Advisor Fanrir.|\n")
	ib:AddLink("That's nice.", "diag9")
	ib:SetLHSButton("next","diag9")
end

local function topic_diag9()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nYeah cool, i know Advisor Fanrir.\n\n|Is this good for this game?|\n")
	ib:AddLink("Yes, now you can speak with Advisor Fanrir.", "diag10")
	ib:SetLHSButton("next","diag10")
end

local function topic_diag10()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n~Hello Advisor Fanrir!~\n")
	ib:AddLink("You need s and t her.", "diag11")
	ib:SetLHSButton("next","diag11")
end

local function topic_diag11()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|What?|\n")
	ib:AddLink("You need to press s till you selected her\nand then press t to talk.", "diag12")
	ib:SetLHSButton("next","diag12")
end

local function topic_diag12()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|What?| I want to play with mouse.\n")
	ib:AddLink("Then right click her and press the hello text bubble.", "diag13")
	ib:SetLHSButton("next","diag13")
end

local function topic_diag13()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nI can't.\n")
	ib:AddLink("Why?", "diag14")
	ib:SetLHSButton("next","diag14")
end

local function topic_diag14()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nI can't speak with you and her same time.\n\n|Can you speak with me and her same time?|\n")
	ib:AddLink("No.", "diag15")
	ib:SetLHSButton("next","diag15")
end

local function topic_diag15()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\n|Then, why you say this?| It's not possible.\n")
	ib:AddLink("... (speechless)", "diag16")
	ib:SetLHSButton("next","diag16")
end

local function topic_diag16()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nI made you speechless.\nI often made other players speechless.\n\n"..
			  "|Don't understand why? Did you know why?|\n")
	ib:AddLink("No, i really can't understand why you making other players speechless.", "diag17")
	ib:SetLHSButton("next","diag17")
end

local function topic_diag17()
    ib:SetHeader("st_002", npc)
    ib:SetTitle("Talking")
    ib:SetMsg("\nDon't worry. One day you will know it. Nice to help you.\n")
end

local function topic_next(counter)
	if counter==1 then topic_diag1()
	elseif counter==2 then topic_diag2()
	elseif counter==3 then topic_diag3()
	elseif counter==4 then topic_diag4()
	elseif counter==5 then topic_diag5()
	elseif counter==6 then topic_diag6()
	elseif counter==7 then topic_diag7()
	elseif counter==8 then topic_diag8()
	elseif counter==9 then topic_diag9()
	elseif counter==10 then topic_diag10()
	elseif counter==11then topic_diag11()
	elseif counter==12 then topic_diag12()
	elseif counter==13then topic_diag13()
	elseif counter==14then topic_diag14()
	elseif counter==15 then topic_diag15()
	elseif counter==16then topic_diag16()
	elseif counter==17then topic_diag17()
	end
end

require("topic_list")

local tl = TopicList()

--tl:SetDefault(topic_default)
tl:AddGreeting("what now%??", topic_greeting) -- this command is needed to launch the hi function ???
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
tl:AddTopics({ "diag11" }, topic_diag11)
tl:AddTopics({ "diag12" }, topic_diag12)
tl:AddTopics({ "diag13" }, topic_diag13)
tl:AddTopics({ "diag14" }, topic_diag14)
tl:AddTopics({ "diag15" }, topic_diag15)
tl:AddTopics({ "diag14" }, topic_diag14)
tl:AddTopics({ "diag16" }, topic_diag16)
tl:AddTopics({ "diag17" }, topic_diag17)
tl:AddTopics({ "next" }, topic_next)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        