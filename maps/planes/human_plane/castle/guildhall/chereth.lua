player = event.activator
npc = event.me

require("interface_builder")
ib = InterfaceBuilder()

-- Guild checks
-- guild_tag is name like "Mercenary"
-- guild_stats -- game.GUILD_NO -- game.GUILD_IN -- game.GUILD_OLD
local function get_guild_stat(guild_tag)
  local guild_force = player:GetGuild(guild_tag)
  if guild_force ~= nil then
    return guild_force.subtype
  end
  return game.GUILD_NO
end
local guild_stat = get_guild_stat("Mercenary")

-- Archery skill checks
local skills = {"bow archery", "crossbow archery", "sling archery"}
local skill_bow = game:GetSkillNr(skills[1])
local skill_cbow = game:GetSkillNr(skills[2])
local skill_sling = game:GetSkillNr(skills[3])
local pl_skill = -1
if player:FindSkill(skill_bow) ~= nil then -- deprecated, also redundant logic todo
  pl_skill = 1
elseif player:FindSkill(skill_cbow) ~= nil then
  pl_skill = 2
elseif player:FindSkill(skill_sling) ~= nil then
  pl_skill = 3
end

require("quest_builder")
qb = QuestBuilder()
quests =
{
  {name = "Chereth's Eye", type = game.QUEST_KILLITEM}
}

quest_targets =
{
  {questnr=1, chance=1, amount=1, arch="spider_widow", name="Giant Black Widow", id=1}
}

quest_kill_items =
{
  {id=1, amount=1, type = "quest_object", icon = "eye.101", name = "Spider eye"}
}

function quest_description(questnr)
  if questnr == 1 then
  ib:SetDesc("Go deeper in the old guild cellar and search for a big black spider. Bring Chereth an eye of this spider.", 50, 0, 0, 0) -- ib:SetCoins(50, 0, 0, 0)
  end
end

function quest_reward(questnr)
  if questnr==1 then
    ib:SetMsg("Chereth smiles and gives you 50 coppers.")
    ib:SetCoins(50, 0, 0, 0)
    player:AddMoney(50, 0, 0, 0)
  end
end

require("scripts/npc_quest.lua")
addQuests()
questnr = qb:Build(player)

function topicDefault() 
  -- someone spend to much time to build a logic for different text outputs in relation to guild and skills
  -- 99% of players don't care about this and it makes the scripts only more complicated and unreadable
  -- also quest restrictions to "guildmembers only" in this early game stage is only confusing the players.
  ib:SetTitle("Greetings!")
				
  ib:AddMsg("\n\nWelcome to the Mercenary guild.")
  local join = "join"
  if guild_stat ~= game.GUILD_IN then
    if guild_stat == game.GUILD_OLD then
      join = "rejoin"
    end
    ib:AddMsg("\n\nAs you are not a member, you should talk to ^Cashin^ upstairs first. ")
    ib:AddMsg("He will tell you how you can "..join..".\n\n")
    ib:AddMsg("Then come back and I will have more for you.")
    else
    ib:AddMsg("\n\nI am Chereth, and I used to teach archery here before I was blinded in a fight with some devilish creatures.\n\n")
    ib:AddMsg("Now ^Taleus^ teaches those skills.")
    if pl_skill > 0 then
      ib:AddMsg("\n\nBut you don't need me to tell you that. I can see that you are ")
      ib:AddMsg("skilled in "..skills[pl_skill].." already.")
    end
    ib:AddMsg("\n\nThen of course there is my dear friend ^Jahrlen^, across the room there.")
								
    -- todo clean this mess above
    if questnr==0 then
      return
    end

    if questnr==1 then
      local qstat = qb:GetStatus(questnr)
      if qstat == game.QSTAT_NO then
        ib:AddMsg("\n\nPerhaps you can do for me a favour?")
        ib:AddLink("Favour?", "favour")
      else
        ib:AddMsg("\n\nYou have the spider eye?")
        ib:AddLink("Finish " ..qb:GetName(questnr).. " quest", "finish quest")
      end
    end
  end
end
-- todo not the best logic, but this is how a hello/quest split could look somehow
-- or make it all in one function?
local function topicSpiderEye()
  if questnr==1 then
    ib:SetTitle(qb:GetName(questnr))
    ib:SetMsg("|^Quest^|\n\n\n")
    local qstat = qb:GetStatus(questnr)
    if qstat == game.QSTAT_NO then
      ib:AddMsg("\nGo deeper in the old guild cellar and search for a big black spider. Bring me an eye of this spider.\n\nDon't ask me for what i need this! I am blind, perhaps this is reason, or perhaps it's because i am the supply chief!\n\nIf you bring me the eye, i give you some coppers." )  
      ib:AddLink("Start quest "..quests[questnr].name, "accept quest")
    else
      ib:AddMsg("You have the spider eye?")
      ib:AddLink("Finish quest "..quests[questnr].name, "finish quest")
    end
    return
  end
  topicDefault() 
end	

local function topCashin()
  ib:SetTitle("Cashin")
  ib:SetMsg("\n\nCashin is our Guild Master.\n\n")
  ib:AddMsg("Speak to him, if you want join or leave the mercenaries.")
  ib:SetButton("Back", "hi")
end

local function topTaleus()
  ib:SetTitle("Taleus")
  ib:SetMsg("\n\nTaleus is our Archery Commander now. He took over archery training for me after I lost my sight.")
  ib:AddMsg("\n\nHe has become an expert in all of the skills, and will be an excellent teacher for you.")
  ib:AddMsg("\n\nHe is usually practicing on our archery ranges outside, but if he has ")
  ib:AddMsg("gone off for more supplies someone at the range will surely know where you can find him.") 
  ib:SetButton("Back", "hi")
end

local function topJahrlen()
  ib:SetTitle("Jahrlen")
  ib:SetMsg("\n\nAh yes, dear old Jahrlen. If you looked at him you wouldn't guess he is over 200 years old, would you?")
  ib:AddMsg("\n\nHe has a highly developed sense of humour, which is why we were laughing when you ")
  ib:AddMsg("came down the stairs.")
  ib:AddMsg("\n\nHe is our resident mage, and will probably be able to teach you some useful magic if you talk to him.")
  ib:SetButton("Back", "hi")
end

require("topic_list")
local tl = TopicList()
tl:SetDefault(topicDefault)
tl:AddTopics("cashin", topCashin)
tl:AddTopics("taleus", topTaleus)
tl:AddTopics("jahrlen", topJahrlen)
tl:AddTopics({"favour", "spidereye"}, topicSpiderEye)
tl = questAddTopics(tl)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))
