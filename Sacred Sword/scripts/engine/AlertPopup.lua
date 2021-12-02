local hostClassVar = nil
local bgPosX = nil
local bgPosY = nil
local half_bgSizeX = nil
local half_bgSizeY = nil

local buttons =
{
	{ "EXIT", true } ,
	{ "BACK", true }
}

local buttonIndex = nil
local buttonID = nil
local button = nil

function OnMenuEnter(hostClass, bgPX, bgPY, h_bgSX, h_bgSY)
	
	hostClassVar = hostClass
	bgPosX = bgPX
	bgPosY = bgPY
	half_bgSizeX = h_bgSX
	half_bgSizeY = h_bgSY

	LoadButtons()
	StartAnimationButtons()
end

function LoadButtons()

	local buffer = 115.0
	local height = 65.0
	local padding = 14.5
	local x = (bgPosX + half_bgSizeX) - buffer
	local y = (bgPosY + half_bgSizeY) - padding

	for i, buttonInfo in ipairs(buttons) do

		lua_CreateButton(hostClassVar, x, y, buttonInfo[1], buttonInfo[2])

		y = y + height
	end

	print("ALERTPOPUP::LUA::SUCCESS::Successfully created the buttons")
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
	local fadeSpeedRect = 19
	local fadeToText = 255
	local fadeSpeedText = 23

	for i = startOpacity, fadeToText, fadeSpeedText do

		lua_FadeFromToRect(hostClassVar, button, fadeToRect, fadeSpeedRect)
		lua_FadeFromToText(hostClassVar, button, fadeToText, fadeSpeedText)
		coroutine.yield();
	end
end