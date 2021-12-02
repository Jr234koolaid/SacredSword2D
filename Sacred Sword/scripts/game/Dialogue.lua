local hostClassVar = nil
local going = true
local scene = nil
local scriptIndex = 1
local characterIndex = 1

local script =
{
	{
		"I  can't  turn  back  now . . .\n",
		"I  have  to  keep  going  foward . . ."
	},
	{
		"Things  look  tough  over  there .   ",
		"I  don't  know  why  but  I'm  afriad . . .\n",
		"My  whole  body  is  paralyzed . . .\n",
		"Entering  this  town . . .   ",
		"For  the  first  time  in  5  years ."
	}
}

local characters =
{
	"Angela"
}

function CallDialogue(hostClass, sceneIndex)

	hostClassVar = hostClass
	scene = sceneIndex
	
	local openingSentence = script[scene][scriptIndex]
	local openingCharacter = characters[characterIndex]
	lua_SetSentence(hostClassVar, openingSentence)
	lua_SetCharacter(hostClassVar, openingCharacter)

	script[scene][scriptIndex] = {parsing = coroutine.create(DoDialogue)}
end

function IssueNext(text, mousePressed)

	if going ~= true then
		if mousePressed then
			if scriptIndex < #script[scene] then
				if coroutine.status(script[scene][scriptIndex].parsing) == 'dead' then

					scriptIndex = scriptIndex + 1

					local nextSentence = script[scene][scriptIndex]
					lua_SetSentence(hostClassVar, nextSentence)
					script[scene][scriptIndex] = {parsing = coroutine.create(DoDialogue)}

					mousePressed = false
					return mousePressed
				end
			end
		end
	end

	if coroutine.status(script[scene][scriptIndex].parsing) ~= 'dead' then

		going = true
		coroutine.resume(script[scene][scriptIndex].parsing, text)
	end
end

function DoDialogue(text)

	for i = 1, string.len(text) do

		lua_DisplayChar(hostClassVar, string.sub(text, i, i))

		if i ~= (string.len(text) -1) then
			coroutine.yield()
		end
	end

	going = false
end