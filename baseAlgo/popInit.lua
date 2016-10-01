local module = {}


function module:generatePopulation() 
    
    local Genome = param.genomeConstruct
    math.randomseed(param.genomeSeed)
    local population = {}
    local i,j
    for i=1, param.genomeCount do
        population[i]=Genome(param.genomeLength)
        population[i][0] = math.random(16)
        for j=1, param.genomeLength-1 do
            population[i][j] = math.random(11)
        end
    end
    return population
    
end
math.randomseed(os.time())
io.write(math.random(16))

return module