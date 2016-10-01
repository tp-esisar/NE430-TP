
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
    
    local score = 1000*initTime + 10000*lookupTime + 1500*mem
    io.write("["..obj.."]\n")
    io.write("("..initTime.." "..lookupTime.." "..mem..") -> "..score.."\n")
    return score
    
end

--module:fitnessFunc(" 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1")
--module:fitnessFunc(" 16 8 4 2 2")


return module
