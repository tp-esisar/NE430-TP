local module = {}


function module:check(state)
	if param.nbSimul == -1 then
		return true
	end
    if (state.iterations > param.nbSimul) then
        return false
    else
        state.iterations = state.iterations+1
        return true
    end
    
end



return module