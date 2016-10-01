local module = {}


function module:check(state)
    if (state.iterations > param.nbSimul) then
        return false
    else
        state.iterations = state.iterations+1
        return true
    end
    
end



return module