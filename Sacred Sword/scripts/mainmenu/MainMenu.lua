local hostClassVar = nil
local windowSizeX = nil
local windowSizeY = nil
local windowCenterX = nil
local windowCenterY = nil

local buttons =
{
	{ "NEW GAME", true } ,
	{ "CONTINUE", false } ,
	{ "MY ROOM", false } ,
	{ "OPTIONS", true } ,
	{ "QUIT", true }
}

local buttonIndex = nil
local buttonID = nil
local button = nil

function OnStateEnter(hostClass, winCenterX, winCenterY, winSizeX, winSizeY)

	hostClassVar = hostClass
	windowSizeX = winSizeX
	windowSizeY = winSizeY
	windowCenterX = winCenterX
	windowCenterY = winCenterY

	LoadButtons()
	StartAnimationButtons()
end

function LoadButtons()

	local buffer = 100.0
	local height = 65.0
	local x = (windowCenterX - (windowSizeX / 2.0)) + 100.0
	local y = (windowCenterY - (windowSizeY / 2.0)) + (windowSizeY / 2.0) - buffer

	for i, buttonInfo in ipairs(buttons) do

		lua_CreateButton(hostClassVar, x, y, buttonInfo[1], buttonInfo[2])

		y = y + height
	end

	print("MAINMENU::LUA::SUCCESS::Successfully created the buttons")
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

function StartAnimationButtons()

	buttonIndex = 1
	buttonID = 1
	button = buttons[buttonIndex][buttonID]	

	buttons[buttonIndex] = {moving = coroutine.create(AnimateButton)}
end

function AnimateButtons()

	if buttonIndex < table.getn(buttons) then
		if coroutine.status(buttons[buttonIndex].moving) == 'dead' then

			buttonIndex = buttonIndex + 1
			button = buttons[buttonIndex][buttonID]

			buttons[buttonIndex] = {moving = coroutine.create(AnimateButton)}
		end
	end

	if coroutine.status(buttons[buttonIndex].moving) ~= 'dead' then

		coroutine.resume(buttons[buttonIndex].moving)
	end
end

function AnimateButton()

	local startOpacity = 0
	local fadeToRect = 190
	local fadeSpeedRect = 21
	local fadeToText = 255
	local fadeSpeedText = 25

	for i = startOpacity, fadeToText, fadeSpeedText do

		lua_FadeFromToRect(hostClassVar, button, fadeToRect, fadeSpeedRect)
		lua_FadeFromToText(hostClassVar, button, fadeToText, fadeSpeedText)
		coroutine.yield();
	end
end