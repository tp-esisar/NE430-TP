
module = {}

function benchCmd(cmd)
	local res = os.execute("/usr/bin/time -f \"%M\" " .. cmd .. " 2> temp")
	local tmp = io.open("temp")
	local initTime = tonumber(tmp:read("*line"))
	local lookupTime = tonumber(tmp:read("*line"))
	local mem = tonumber(tmp:read("*line"))
	return initTime, lookupTime, mem
end

function module:fitnessFunc(obj)
    local initTime, lookupTime, mem = benchCmd("./bench " .. obj)
    mem = mem/1000000
    io.write(" "..initTime.." "..lookupTime.." "..mem.."\n")
    
end

--module:fitnessFunc(" 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1")
--module:fitnessFunc(" 16 4 8 4")


return module
