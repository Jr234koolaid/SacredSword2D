local hostClassVar = nil
local bgPosX = nil
local bgPosY = nil

local buttons =
{
	{ "WINDOW", false } ,
	{ "TEXT", false } ,
	{ "AUDIO", false } ,
	{ "CREDITS", false } ,
	{ "BACK", true }
}

local buttonIndex = nil
local buttonID = nil
local button = nil

function OnMenuEnter(hostClass, bgPX, bgPY)
	
	hostClassVar = hostClass
	bgPosX = bgPX
	bgPosY = bgPY	

	LoadButtons()
	StartAnimationButtons()
end

function LoadButtons()

	local buffer = 175.0
	local height = 75.0
	local x = bgPosX + 25.0
	local y = bgPosY + buffer

	for i, buttonInfo in ipairs(buttons) do

		lua_CreateButton(hostClassVar, x, y, buttonInfo[1], buttonInfo[2])

		y = y + height
	end

	print("OPTIONSMENU::LUA::SUCCESS::Successfully created the buttons")
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
	local fadeSpeedRect = 20
	local fadeToText = 255
	local fadeSpeedText = 24

	for i = startOpacity, fadeToText, fadeSpeedText do

		lua_FadeFromToRect(hostClassVar, button, fadeToRect, fadeSpeedRect)
		lua_FadeFromToText(hostClassVar, button, fadeToText, fadeSpeedText)
		coroutine.yield();
	end
end