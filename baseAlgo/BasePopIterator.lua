module = {}



function module:newPop(pop, simOut)
    local hashTab = sortedHash(simOut)
    local survivors = {}
    local i
    local Genome = param.genomeConstruct
    for i=1, param.survivorsByRound do
        survivors[i]=pop[hashTab[i]]
    end
    
    for i=param.survivorsByRound+1, param.nbCrossover do
        local r1=math.random(param.survivorsByRound)
        local r2=math.random(param.survivorsByRound)
        survivors[i]=crossover(survivors[r1], survivors[r2], param.genomeLength)
        if param.mutation then
            mutator(survivors[i])
        end
    end
    for i=param.nbCrossover+1, param.genomeCount do
        local Genome = param.genomeConstruct
        survivors[i]=Genome(param.genomeLength)
        survivors[i][0] = math.random(16)
        for j=1, param.genomeLength-1 do
            survivors[i][j] = math.random(11)
        end
    end
    
    return survivors
    
    
end
        
    
function sortedHash(simOut)
    local sortedSim = {}
    local hashTab = {}
    local i, v, g
    for i, v in ipairs(simOut) do
        sortedSim[i]=v
        hashTab[i]=i
    end
    for g=1, #sortedSim do
        for i=1, #sortedSim-1 do
            if (sortedSim[i] > sortedSim[i+1]) then
                local k=sortedSim[i]
                local j=hashTab[i]
                sortedSim[i]=sortedSim[i+1]
                hashTab[i]=hashTab[i+1]
                sortedSim[i+1]=k
                hashTab[i+1]=j
            end
        end
    end
    return hashTab
end

-- local sim = {10,12,9,13,2,1,45,45,9,8}
-- local test = sortedHash(sim)
-- local i, v
-- for i, v in ipairs(test) do
--     io.write(v,"\t",sim[v], "\n")
-- end

    
    
function crossover(genome1, genome2, genomeLength)
    local Genome = param.genomeConstruct
    local g2 = genome2
    local g1 = genome1
    local newGenome = Genome(genomeLength)
    local i 
    local cut = math.random(genomeLength-20)-1
    local swap = math.random()
    if (swap <0.5) then 
        local temp = g1
        g1 = g2
        g2 = temp
    end
    
    for i=0 , genomeLength-1 do
        if (i <= cut) then
            newGenome[i] = g1[i]
        else
            newGenome[i] = g2[i]
        end
    end
    return newGenome

end

function mutator(genome)
    local value
    for position=0, param.genomeLength-1 do
        if 1==math.random(param.mutationChance) then
            if position >= 1 then
                value = math.random(11)
            else
                value = math.random(16)
            end
            genome[position] = value
        end
    end
    return genome
end

return module