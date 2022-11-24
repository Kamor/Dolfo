local me= event.me
local pl = event.activator

require("interface_builder")
local ib = InterfaceBuilder()

local function teachSpell(spell)
    local skill = game:GetSkillNr("divine prayers")
    if pl:FindSkill(skill) == nil then
        ib:AddMsg("\n\nFirst I will teach you the needed skill: ~devine prayers~")
        pl:AcquireSkill(skill, game.LEARN)
    end
    local spellno = game:GetSpellNr(spell)
    if pl:DoKnowSpell(spellno) then
        ib:AddMsg("\n\nYou already know the spell '^"..spell.."^'!" )
    else
        pl:AcquireSpell(spellno, game.LEARN)
        ib:AddMsg("\n\n|** Merlin teaches you learn the spell '^"..spell.."^'! **|")
    end
end

local function topic_greeting()
  ib:SetTitle("Greetings")
  ib:SetMsg("You want to learn some priest spells?")
	ib:AddLink("Sure.","Services")
	ib:AddLink("No, not now.","")
	ib:SetLHSButton("Services")
end

local function topic_services()
	ib:SetTitle("Learn Spells")
  ib:SetMsg("So, what do you want?")
	ib:AddLink("cure poison","learn cure poison")
	ib:AddLink("minor healing","learn minor healing")
	ib:AddLink("restoration","learn restoration")
	ib:AddLink("remove depletion","learn remove depletion")
	ib:AddLink("remove death sickness","learn remove death sickness")
	ib:AddLink("remove slow","learn remove slow")
	ib:AddLink("remove fear","learn remove fear")
	ib:AddLink("remove snare","learn remove snare")
	ib:AddLink("remove confusion","learn remove confusion")
	ib:AddLink("remove paralysis","learn remove paralysis")
	ib:AddLink("sanctuary", "learn sanctuary")
end

local function topic_learn(spell)
	ib:SetTitle("Learn " .. spell)
	ib:SetMsg("Ok, i teach you the " .. spell .. " spell.")
	ib:SetLHSButton("Back", "Services")
	if spell ~= nil then
    teachSpell(spell)
	end
end

require("topic_list")
local tl = TopicList()

tl:SetDefault(topic_greeting)
tl:AddServices(nil, topic_services)
tl:AddTopics({"learn (cure poison)", "learn (minor healing)", "learn (restoration)", "learn (remove depletion)", "learn (sanctuary)" , "learn (remove slow)" , "learn (remove fear)", "learn (remove death sickness)", "learn (remove snare)" , "learn (remove confusion)", "learn (remove paralysis)",}, topic_learn)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        