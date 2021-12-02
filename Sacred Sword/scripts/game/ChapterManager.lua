local hostClassVar = nil

function OnStateEnter(hostClass)

	hostClassVar = hostClass
end

function FadeIn(opacity, fadedIn, halfFadedIn)

	local speed = 2

	if opacity <= 195 then

		halfFadedIn = true
	end

	if opacity ~= 0 then

		fadedIn = false
		lua_SetFadeIn(hostClassVar, speed)
	elseif opacity == 0 then

		fadedIn = true
	end

	return fadedIn, halfFadedIn
end

function FadeOut(opacity, fadedOut)

	local speed = 5

	if opacity ~= 255 then

		fadedOut = false
		lua_SetFadeOut(hostClassVar, speed)
	elseif opacity == 255 then

		fadedOut = true
	end

	return fadedOut
end