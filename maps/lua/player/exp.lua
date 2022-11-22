-------------------------------------------------------------------------------
-- /lua/player/exp.lua | Special event
-- Triggered when a player gains/loses exp.
-------------------------------------------------------------------------------
---------------------------------------
-- These are the event members useful to this script.
---------------------------------------
local player = event.me -- the player whose exp changed
local skill = event.activator -- the skill affected
local skillgroup = event.other -- the skillgroup affected

local ds = DataStore("110_list")

-------------------
-- There are 8 possibly relevant integers which the script knows about. This is
-- too many to be passed as event.parameterNm so instead we pass them as a
-- string in event.options.
--
-- The 8 are:
--   bexp: bonus exp which some players might get sometimes (ie via an amulet);
--   dlev: drained levels;
--   sexp: exp gained/lost in skill;
--   slev: lev gained/lost in skill;
--   gexp: exp gained/lost in skillgroup;
--   glev: lev gained/lost in skillgroup;
--   mexp: exp gained/lost in main;
--   mlev: lev gained/lost in main;
--
-- Apart from the first two, these are relative values. The absolute current
-- values are held in .experience and .level of skill, skillgroup, and player.
--
-- player.level already has dlev subtracted.
-------------------
local _, _, bexp, dlev, sexp, slev, gexp, glev, mexp, mlev = string.find(event.options,
    "(-?%d+) (%d+) (%-?%d+) (%-?%d+) (%-?%d+) (%-?%d+) (%-?%d+) (%-?%d+)")
bexp = tonumber(bexp)
dlev = tonumber(dlev)
sexp = tonumber(sexp)
slev = tonumber(slev)
gexp = tonumber(gexp)
glev = tonumber(glev)
mexp = tonumber(mexp)
mlev = tonumber(mlev)

---------------------------------------
-- Give the player some info. This is the main purpose of this script, as the
-- server gives no explicit info. However, remember that various numbers in the
-- client will change anyway. So it's nice to emphasise bigger points with text
-- or other effects but lets not go nuts with redundant info.
--
-- To this end, I make no mention of skillgroup, main exp, or drained levels.
---------------------------------------
local message
if skill.last_eat == game.INDIRECT then
    message = "~"..tostring(math.abs(sexp)).."~ " 
    if bexp > 0 then
        message = message.."("..tostring(sexp-bexp).."+"..tostring(bexp)..") "
				else
						if bexp < 0 then
							 message = message.."(" ..tostring(sexp-bexp)..tostring(bexp)..") "	
						end		
    end
    message = message.."experience " 
else
    message = "~"..tostring(math.abs(slev)).."~ level"
    if math.abs(slev) > 1 then
        message = message.."s " 
    else
        message = message.." " 
    end
end
if slev ~= 0 then
    message = message.."(and are now level ~"..tostring(skill.level).."~) "
end
if sexp > 0 then
    player:Write("You gain "..message.."in "..skill:GetName().."!", game.NDI_COLR_ORANGE)
elseif sexp < 0 then
    player:Write("You lose "..message.."in "..skill:GetName().."!", game.NDI_COLR_PURPLE)
end

local maxlevel_rank = 0

if slev ~= 0 then
    if mlev > 0 then

        -- Keep track of who makes it to 110
        if player.level + dlev == 110 and not ds:Get(player.name) then
            ds:Set(player.name, true)
            local standings = ds:Get("standings")

            if standings == nil then
                standings = {}
            end

            table.insert(standings, player.name)
            maxlevel_rank = table.getn(standings)
            ds:Set("standings", standings)
            DataStore.Save("110_list")
        end

        player:Write("Your main level increases to ~"..tostring(player.level + dlev).."~!", game.NDI_COLR_ORANGE + game.NDI_FLAG_VIM)
        player:AddSparkly("level_up", 2.5)
        player:Sound(player.x, player.y, game.SOUND_LEVEL_UP, game.SOUNDTYPE_NORMAL)
    elseif mlev < 0 then
        player:Write("Your main level decreases to ~"..tostring(player.level + dlev).."~!", game.NDI_COLR_PURPLE)
    end
end

if mlev <= 0 then
    return
end

---------------------------------------
-- Below here the event is announced to the world, so respect privacy.
---------------------------------------
if player:GetPrivacy() == true then
    return
end

---------------------------------------
-- Announce on the PA channel.
---------------------------------------
if slev ~= 0 then
    player:ChannelMsg("PA", "levelled up to level " .. player.level + dlev .. "!", game.CHANNEL_MODE_EMOTE)

    if maxlevel_rank > 0 then
        player:ChannelMsg("PA", "is the |#" .. maxlevel_rank .. "| player to reach max level!", game.CHANNEL_MODE_EMOTE)
    end
end
