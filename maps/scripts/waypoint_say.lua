-- waypoints_say.lua using magic ear
-- you can use it with magic ear alone, or also with other object and a magic ear
-- put in a magic ear in a map, give it a name, it's name is the waypoint name
-- additional put in an object, you need same name here as from magic ear
-- ear need a lua event, trigger this by say and connect it to this script
-- object need a lua event, trigger this by apply and connect it to this script
-- both events can target only one initialize SENTInce, all links in SENTInce non /help non /say leads to a tell command
-- and then server is searching nearest npc and send command to this npc lua event

local pl = event.activator
local me = event.me
local waypoint_name = me.name

-- apply event has no message, so check first if we have a message
if event.message ~= nil and event.message ~= "" then
  -- then we can check if say event starts with waypoint, we don't want our magic ear to react on other say message
	-- think of two player's speaking local to each other and each time SENTInce popup here. 
	if string.sub(event.message, 1, 8) ~= "waypoint" then
	 return
	end
end

-- Mark this as our single action for this tick
-- So server didn't additionally action on our waypoint like "I don't now how to apply an obelisk."
event.returnvalue = 1

--if name==null or if name==nil what's better?
if waypoint_name==nil or waypoint_name=="" then
	pl:Write("Error: Waypoint has no name!")
	return
end

local waypoint_map	= me.map
if waypoint_map==nil then
  -- this should normally not happen, do we need this check?
	pl:Write("Error: Waypoint has no pointer to map! ")
	return
end

-- path is real path -- orig_path is file path
-- we need to check if a waypoint is on a instanced map_path /dynamic generated map
-- if yes we can't store this waypoint
-- TODO perhaps we can allow a one direction only waypoint on such maps
-- but first lets launch an error here

-- TODO do we also need a savety check here for path==nil or path==""

if waypoint_map.path~=waypoint_map.orig_path then
	pl:Write("Error: Waypoint could be in a player instanced map!")
	return
end

local waypoint_x=me.x
local waypoint_y=me.y
-- we don't check for valid x,y here

local ds = DataStore("waypoints_global")
local waypoints = ds:Get("waypoints_global")

-- we have no waypoints?, we declare an empty waypoint array
if not waypoints then
	waypoints = {}
end

-- we need to check, if current waypoint is stored in waypoints_global
local found=false
for what, data in waypoints do
	if data.waypoint == waypoint_name then
		-- we also check if waypoint is still at the same map at same x and y position
		if data.x~=waypoint_x or data.y~=waypoint_y or data.map_path~=waypoint_map.path then
			-- update the array
			data.x=waypoint_x
			data.y=waypoint_y
			data.map_path=waypoint_map.path
			-- update the DataStore
			ds:Set("waypoints_global", waypoints)
		end
		found=true
		break -- found waypoint in global list
	end
end
-- if waypoint not found we insert it
if found==false then
	-- waypoint is not in list, we need to write new waypoint to DataStore
	local waypoint_to_insert = {waypoint = waypoint_name, x = waypoint_x, y = waypoint_y, map_path = waypoint_map.path}
	table.insert(waypoints, waypoint_to_insert)
	ds:Set("waypoints_global", waypoints)
end

--now check the player waypoint list, player waypoint list needs only to store the waypoint name
local ds_player = DataStore("waypoints_player", pl)
local waypoints_player = ds_player:Get("waypoints_player")

if not waypoints_player then
	waypoints_player = {}
end

-- check if current waypoint is stored in waypoints_player
local found=false
for what, data in waypoints_player do
	if data.waypoint == waypoint_name then
		found=true
		break -- found waypoint in player list
	end
end

-- if waypoint not found we insert it
if found==false then
	pl:Write("Found new waypoint " .. waypoint_name)
	pl:Sound(0, 0, game.SOUND_LEARN_SPELL) -- TODO give player a better sound
	-- waypoint is not in list, we need to write new waypoint to DataStore
	local waypoint_to_insert = {waypoint = waypoint_name}
	table.insert(waypoints_player, waypoint_to_insert)
	ds_player:Set("waypoints_player", waypoints_player)
end

require("interface_builder")
local ib = InterfaceBuilder()

local function teleport_to_waypoint(waypoint_target)
-- if this function is called we need to make save that a probably old SENTInce dialog would be closed
	ib:ShowSENTInce(0)
-- we need to do all checks here again, because player can also use teleport command with wrong paramater by typing the command
  if waypoint_target~=nil then
		-- search first if player have the waypoint
		for whatx, datax in waypoints_player do
			-- to be compatible with waypoint_talk and also to allow players type lowercase commands 
			if string.lower(datax.waypoint) == string.lower(waypoint_target) then
			  -- when player knows the waypoint
				-- check if server knows it too and have data for it
				for what, data in waypoints do
					-- waypoint names can have uppercase letters, topicList parameters are somehow lowercase, when they come here 
					if string.lower(data.waypoint) == string.lower(waypoint_target) then
						pl:Write("A strong force teleports you away.", 0)
						pl:Sound(0, 0, game.SOUND_TELEPORT)
						-- OVERLAY_FIXED on waypoints we want to force the teleport on exactly this waypoint position, this looks best.
						-- OVERLAY_RANDOM when fixed make trouble we can also try to teleport to the next free positions, but this looks more ugly to teleport near a free waypoint and not on it
						pl:SetPosition(game:ReadyMap(data.map_path),data.x,data.y,game.OVERLAY_FIXED) -- OVERLAY_RANDOM -- OVERLAY_FIXED
						return
					end -- if string ..
				end -- for what ..
				-- when we are here, player knows waypoint, but server not, this should not happen, but better launch an error here
				pl:Write("Error : Global waypoint storage didn't know a waypoint " .. waypoint_target)
			  return
			end -- if string..
		end -- for what ..
		pl:Write("You didn't know a waypoint " .. waypoint_target)
		return
	end
	-- if player comes here, he used the waypoint command without a waypoint target
	-- this can't be happen here, because we a logic above launching SENTInce default topic on empty waypont commands
	pl:Write("You wisper the teleport command without a target and nothing happens.")
end

local message = event.message
if message ~= nil then
	if string.sub(event.message, 1, 9) == "waypoint " then
	  teleport_to_waypoint(string.sub(message,10))
	  return
	end
end

local function topic_default(waypoint_target) --(waypoint_target)
    ib:SetTitle("Waypoint")
    ib:SetMsg("\nThis is waypoint ".. waypoint_name ..".\n")
		if waypoint_target~=nil then
		  ib:SetMsg("\nYou used parameter  ".. waypoint_target ..".\n")
		end
		for what, data in waypoints_player do
			ib:AddLink(data.waypoint, "/say waypoint ".. data.waypoint);
    end
end

require("topic_list")

local tl = TopicList()
tl:SetDefault(topic_default)
ib:ShowSENTInce(game.GUI_NPC_MODE_NPC, tl:CheckMessage(event, true))