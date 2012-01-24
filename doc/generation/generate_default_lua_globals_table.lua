#!/usr/bin/env lua

local writer = print

-- This line changes default output from printing to the file we typically generate.
local fn = "default_lua_globals.lua"

if arg[1] then
	fn = arg[1]
end

if fn then
	local f = assert(io.open(fn, 'w'))
	writer = function(s)
		f:write(s, '\n')
	end
end

writer "return {"
for k, v in pairs(_G) do
	writer( ([[ [%q] = true, ]]):format(k))
end
writer "}"
