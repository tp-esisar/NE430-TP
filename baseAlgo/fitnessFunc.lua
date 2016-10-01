module = {}

function module:fitnessFunc(obj)
    local sum=0, v
    
    for v=0, param.genomeLength-1 do
        sum = sum + obj[v]
    end
    return sum/param.genomeLength
    
end



return module