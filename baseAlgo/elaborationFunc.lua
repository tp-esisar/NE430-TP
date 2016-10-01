local module = {}

function module:elaborationFunc(genome)
    local i
    local arguments = ""
    for i=0, param.genomeLength-1 do
    	arguments = arguments .. " " .. tostring(genome[i])
    end
    return arguments
    
end


return module