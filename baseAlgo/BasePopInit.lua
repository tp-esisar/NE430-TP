local module = {}





function module:generatePopulation() 
    
    local Genome = param.genomeConstruct
    math.randomseed(param.genomeSeed)
    local population = {}
    local i
    local j
    for i=1, param.genomeCount do
        population[i]=Genome(param.genomeLength)
        
        for j=0, param.genomeLength-1 do
            
            population[i][j] = math.random()
            
        end
    end
    return population
    
end

return module