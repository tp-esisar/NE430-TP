

require "./BaseInitializer"

local state = {
    
    iterations = 0
}
local modules

function run()
    
    local population = param.popInit:generatePopulation()
    local continue = true
    if (param.displayer) then
        io.write("population de base\n")
        param.displayer:display(population)
    end
    local simOut
    while continue do
    
        simOut = param.simulator:start(population)
        population = param.popIterator:newPop(population, simOut)
        continue = param.stoper:check(state)
        
    end
    
    if (param.displayer) then
        param.displayer:display(population, simOut)
    end
    
    if (param.saver) then
        param.saver:save(population, simOut)
    end
    
    
end

run()