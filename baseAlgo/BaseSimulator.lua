local module = {}


function module:start(pop)
    
    
    local i, v
    local Obj
    local simOut = {}
    for i, v in ipairs(pop) do
        if (param.elaboration) then
            Obj = param.elaboration:elaborationFunc(v)
        else
            Obj = v
        end
        simOut[i] = param.fitness:fitnessFunc(Obj)
    end
    return simOut
end


return module