
local module = {}


function module:display(pop, simOut)
    local i, v, g
    for i, v in ipairs(pop) do 
        
        io.write("ID: ", i, "\tScore: ")
        if (not simOut) then
            io.write("no score")
        else
            io.write(simOut[i])
        end
        io.write("\n")
        
        for g=0, param.genomeLength-1 do
            io.write("|\t\t", v[g], "\n")
        end
        io.write("\n")
        
    end
end

return module