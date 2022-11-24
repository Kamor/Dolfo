local me= event.me
local pl = event.activator

require("interface_builder")
local ib = InterfaceBuilder()

local function teachSpell(spell)
    local skill = game:GetSkillNr("wizardry spells")
    if pl:FindSkill(skill) == nil then
        ib:AddMsg("\n\nFirst I will teach you the needed skill: ~wizardry spells~")
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
  ib:SetMsg("You want to learn some mage spells?")
	ib:AddLink("Sure.","Services")
	ib:AddLink("No, not now.","")
	ib:SetLHSButton("Services")
end

local function topic_services()
	ib:SetTitle("Learn Spells")
  ib:SetMsg("So, what do you want?")
	ib:AddLink("probe","learn probe")
	ib:AddLink("magic bullet","learn magic bullet")
	ib:AddLink("firestorm","learn firestorm")
	ib:AddLink("icestorm","learn icestorm")
	ib:AddLink("lightning","learn lightning")
	ib:AddLink("firebolt","learn firebolt")
	ib:AddLink("frostbolt","learn frostbolt")
	ib:AddLink("strength self", "learn strength self")
	ib:AddLink("identify", "learn identify")
end

local function topic_learn(spell)
	ib:SetHeader("st_005", me)
	ib:SetTitle("Learn " .. spell)
	ib:SetMsg("Ok, i teach you the " .. spell .. " spell.")
	ib:SetLHSButton("Back", "Services")
	if spell ~= nil then
    teachSpell(spell)
	end
end

require("topic_list")
local tl = TopicList()
tl:SetDefault(topic_greeting) -- we need no i don't understand you, if player talking nonsense we answer with hi
--tl:AddGreeting("what now%??", topic_greeting) -- todo why what now, when we overwrite title with ib:SetTitle later? but we currently need link to function here
tl:AddServices(nil, topic_services)
tl:AddTopics({"learn (probe)", "learn (magic bullet)", "learn (firestorm)" , "learn (icestorm)" , "learn (lightning)" , "learn (firebolt)", "learn (frostbolt)", "learn (strength self)", "learn (identify)"}, topic_learn)

ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
        