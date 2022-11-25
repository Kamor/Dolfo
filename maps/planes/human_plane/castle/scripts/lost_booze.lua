-- template for a "item quest" script
require("topic_list")
require("quest_builder")
require("interface_builder")

local pl        = event.activator
local me        = event.me
local msg       = string.lower(event.message)
local qb        = QuestBuilder()

local q_name_1  = "Lost Booze"
local q_step_1  = 0
local q_level_1  = 4
local q_skill_1  = game.ITEM_SKILL_NO

local ib = InterfaceBuilder()

local function questGoal(questnr)
    qb:AddQuestItem(questnr, 1, "quest_object", "booze.101", "Boozie")
    pl:Sound(0, 0, 2, 0)
    pl:Write("You take the quest '".. qb:GetName(questnr) .."'.", game.COLOR_NAVY)
end

-- qb:AddQuest("Lost Booze", game.QUEST_ITEM, q_level_1, q_skill_1, nil, nil, 1, questGoal, nil)
qb:AddQuest("Lost Booze", game.QUEST_ITEM, nil, nil, nil, nil, 1, questGoal, nil)
local questnr = qb:Build(pl)
local q_stat_1 = qb:GetStatus(1)

local function topicDefault()
  ib:SetTitle("Cheers")
		if questnr==0 then
    ib:AddMsg("\nCarthson burps.\n")
				-- pl:Write(me.name .." has nothing to say.", game.COLOR_NAVY)
		  -- pl:Interface(game.GUI_NPC_MODE_NO)
			 return
		end
		
		if questnr==1 then
				ib:SetMsg("|^Quest^|\n\n\n")
		 	local qstat = qb:GetStatus(questnr)
		 	if qstat == game.QSTAT_NO then
						ib:AddMsg("'Lo der chap. Yer look like uh nice feller, so take yeh a set an' 'ave a bet o meh boozish... Jus' uh sec... whas tis? Someun tooked meh boozie! Why dat varmet, I swears, wen I git em--ah, oh. I mustuv lef meh boozun a 'ome in meh booze ches'. Would yeh be uh nice feller an' fetch meh boozit fo' some coin? ")
						ib:AddLink("Yessir", "quest")
				else
			   ib:AddMsg("Yeh found meh boozie yet? I been awaitin 'ere fo' it.")
			   ib:AddLink("Yup, here it is", "finish quest")
		  end
				ib:SetLHSButton("Quest")
		end		
end

-- quest body (added to player quest obj for quest list)
local function quest_icons1()
	ib:AddIcon("Silver", "silvercoin.101", "A Silver Coin") 
end
local function quest_body1()
	ib:SetMsg("Bring meh boozie from meh 'ouse fer meh. Meh 'ouse is aroun' on the east part o' town.")
	ib:SetDesc("Rememer, brin' meh ~Boozie~ from meh booze ches'.", 0, 4, 0, 0)
end

local function topicQuest()
		if questnr==0 then
			topicDefault()
		end
		if questnr==1 then
				local qstat = qb:GetStatus(questnr)
		  if qstat == game.QSTAT_NO or qstat == game.QSTAT_ACTIVE then
						ib:SetTitle(q_name_1)
						quest_body1()
				end
				if qstat == game.QSTAT_NO then
				ib:SetAccept(nil, "accept quest")
				ib:SetDecline(nil, "hi")
				end
				if qstat == game.QSTAT_SOLVED then
						ib:AddMsg("Meh boozie! Teres meh boozun... Tankee much.\n")
						ib:SetDesc("'Ere, ave yer coin.", 0, 4, 0, 0)
						ib:SetAccept(nil, "finish quest")
						ib:SetDecline(nil, "hi")
				end
		end
end

local function topicAcceptQuest()
		if questnr==0 then
			topicDefault()
		end

		if questnr==1 then
			local qstat = qb:GetStatus(questnr)
			if qstat == game.QSTAT_NO then
				 qb:RegisterQuest(questnr, me, ib)
			end
		 topicQuest()
		end
end

local function topicFinishQuest()
		if questnr==0 then
		 	topicDefault()
		end
		
		if questnr==1 then
		 	local qstat = qb:GetStatus(questnr)
		 	if qstat ~= game.QSTAT_SOLVED then
						ib:SetMsg("|^Quest^|\n\n\n")
						ib:AddMsg("Meh Boozie? Yous gots meh boozie? Giver 'ere! Wha? No, you don' gots meh boozie... comere when yeh gots it.\n")
						ib:SetLHSButton("Quest")
      return
			 end
				pl:Sound(0, 0, 2, 0)
				pl:AddMoneyEx(0,4,0,0)
				ib:SetTitle("Lost Booze")
				ib:SetMsg("Yeh ave yerself uh day o goodun!")
				qb:Finish(questnr)
		end
end

tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics({"quest", "explain%s+quest"}, topicQuest)
tl:AddTopics("accept quest", topicAcceptQuest)
tl:AddTopics("finish quest", topicFinishQuest)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))