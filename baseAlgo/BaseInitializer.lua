local ffi = require "ffi"

param = {
    genomeLength = 7,
    genomeCount = 100,
    genomeSeed = os.time(),
    survivorsByRound = 10,  
    crossover = true,
    crossoverNbr = 7,
    mutation = true,
    mutationChance = 100,
    nbSimul = 100000,
    ffi.typeof("float[?]")
    popInit = require "./BasePopInit",
    displayer = require "./BaseDisplayer",
    simulator = require "./BaseSimulator",
    popIterator = require "./BasePopIterator",
    stoper = require "./BaseStoper",
    fitness = require "./fitnessFunc",
    elaboration = require "./elabFunc"
}
--return param
