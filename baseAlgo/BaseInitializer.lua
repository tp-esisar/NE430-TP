local ffi = require "ffi"

param = {
    genomeLength = 32,
    genomeCount = 30,
    genomeConstruct = ffi.typeof("char[?]"),
    genomeSeed = os.time(),
    survivorsByRound = 10,  
    crossover = true,
    nbCrossover = 15,
    mutation = true,
    mutationChance = 30,
    nbSimul = -1,
    popInit = require "./popInit",
    displayer = require "./BaseDisplayer",
    simulator = require "./BaseSimulator",
    popIterator = require "./BasePopIterator",
    stoper = require "./BaseStoper",
    fitness = require "./fitnessFunction",
    elaboration = require "./elaborationFunc"
}
--return param
