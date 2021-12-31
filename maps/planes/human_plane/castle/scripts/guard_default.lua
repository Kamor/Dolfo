local pl = event.activator
local me = event.me

if pl.level<11 then -- 11 means player is strong enough to fight on demon plane
	local rnd = math.random(4)
	if rnd == 1 then
		pl:Write("I watch you!")
	elseif rnd == 2 then
		pl:Write("Don't make trouble!")
	elseif rnd == 3 then
		pl:Write("Don't look at me!")
	elseif rnd == 4 then
		pl:Write("Keep walking!")
	end

elseif pl.level <100 then
	pl:Write("Hi, " .. pl.name .. ", nice to see you again.")
elseif pl.level <110 then
	pl:Write("I am proud to know you, " .. pl.name .. ".")
else
	pl:Write("My lord, I am proud to serve you.")
end
