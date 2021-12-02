--[[		INTIALIZATION OF CHAPTER 1 WORLD		]]--

local hostClassVar = nil
local windowSizeX = nil
local groundY = 875.0
local spawnY = 460.0
local randW = nil
local randH = nil

function OnChapterEnter(hostClass, winSX)
	
	hostClassVar = hostClass
	windowSizeX = winSX

	math.randomseed(1234)

	CreateTrees()
	CreateDialogueBounds()
	CreateGround()
	CreateFoliage()
	CreatePlayer()
	CreateEnemies()
end


function CreatePlayer()

	local x = (windowSizeX / 2.0) + 100.0
	local name = "Angela"

	lua_CreatePlayer(hostClassVar, x, spawnY, name)

	print("CHAPTER1::LUA::SUCCESS::Successfully created the player")
end


function CreateEnemies()

	local width = 175.0
	local height = 385.0

	local enemies =
	{
		{"E1", 1400.0, spawnY},
		{"E2", 4653.0, spawnY}
	}

	for i, e in ipairs(enemies) do

		lua_CreateEnemies(hostClassVar, e[2], e[3], width, height, e[1])

		print("CHAPTER1::LUA::SUCCESS::Successfully created an enemy")
	end
end


function CreateGround()

	local path = "./res/sprites/chapters/chapter1/ground.png";

	local ground =
	{
		{-100.0, groundY},
		{1100.0, groundY},
		{2300.0, groundY},
		{3500.0, groundY},
		{4700.0, groundY},
		{5900.0, groundY}
	}

	for i, g in ipairs(ground) do

		lua_CreateGround(hostClassVar, g[1], g[2], path)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a ground object")
	end
end


function CreateTrees()

	local width = 295.0 * 0.8
	local height = 495.0 * 0.8
	local pEffect = 0.8
	local path = "./res/sprites/chapters/chapter1/treeR2.png"

	local trees =
	{
		-145.0,
		279.0,
		923.0,
		1687.0
	}

	for i, t in ipairs(trees) do

		randW = math.random(1, 165)
		randH = math.random(1, 115)

		local tHeight = height + randH
		local y = groundY - tHeight

		lua_CreateTrees(hostClassVar, t, y, width + randW, tHeight, pEffect, path, 3)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a tree object (Layer 3)")
	end

	width = 270.0 * 1.05
	height = 675.0 * 1.05
	pEffect = 0.6
	path = "./res/sprites/chapters/chapter1/treeT2.png"

	trees =
	{
		-195.0,
		216.0,
		571.0,
		1110.0,
		1508.0,
		1854.0
	}

	for i, t in ipairs(trees) do

		randW = math.random(1, 135)
		randH = math.random(1, 245)

		local tHeight = height + randH
		local y = groundY - tHeight

		lua_CreateTrees(hostClassVar, t, y, width + randW, tHeight, pEffect, path, 2)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a tree object (Layer 2)")
	end

	width = 295.0 * 1.25
	height = 495.0 * 1.25
	pEffect = 0.3
	path = "./res/sprites/chapters/chapter1/treeR2.png"

	trees =
	{
		-105.0,
		564.0,
		1141.0,
		2037.0,
		2738.0,
		3212.0,
		3944.0
	}

	for i, t in ipairs(trees) do

		randW = math.random(1, 165)
		randH = math.random(1, 115)

		local tHeight = height + randH
		local y = groundY - tHeight

		lua_CreateTrees(hostClassVar, t, y, width + randW, tHeight, pEffect, path, 1)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a tree object (Layer 1)")
	end
end


function CreateFoliage()

	local width = 320.0
	local height = 185.0
	local path = "./res/sprites/chapters/chapter1/bush2.png"

	local foliage =
	{
		-29.0,
		691.0,
		1556.0,
		2512.0,
		2978.0,
		3962.0,
		4786.0,
		5271.0
	}

	for i, f in ipairs(foliage) do

		randW = math.random(1, 235)
		randH = math.random(1, 75)

		local tHeight = height + randH
		local y = groundY - tHeight

		lua_CreateFoliage(hostClassVar, f, y, width + randW, tHeight, path)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a foliage object")
	end
end


function CreateDialogueBounds()

	local width = 100.0
	local height = 500.0
	local y = groundY - height

	local bounds =
	{
		{-75.0, y},
		{6000.0, y}
	}

	for i, b in ipairs(bounds) do

		lua_CreateDialogueBounds(hostClassVar, b[1], b[2], width, height)

		print("CHAPTER1::LUA::SUCCESS::Successfully created a dialogue bound")
	end
end