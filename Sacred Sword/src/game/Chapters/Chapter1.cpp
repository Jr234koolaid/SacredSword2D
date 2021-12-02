#include "pch.h"
#include "Chapter1.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::Chapter1::Chapter1(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font)
	: ss::Chapter(window, L, font)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/game/Chapter1.lua"));

	if (!checkScript)
	{
		exit = true;
		std::cout << "CHAPTER1::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_CreateTrees", lua_CreateTrees);
	lua_register(L, "lua_CreateGround", lua_CreateGround);
	lua_register(L, "lua_CreateFoliage", lua_CreateFoliage);
	lua_register(L, "lua_CreatePlayer", lua_CreatePlayer);
	lua_register(L, "lua_CreateEnemies", lua_CreateEnemies);
	lua_register(L, "lua_CreateDialogueBounds", lua_CreateDialogueBounds);

	inGameplay = true;

	lua_getglobal(L, "OnChapterEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, rWindow->getView().getSize().x);

		checkLua(L, lua_pcall(L, 2, 0, 0));
	}

	{
		const float xPos = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f);
		const float yPos = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f);

		initBGPos = xPos;

		if (!bgTexture.loadFromFile("./res/sprites/chapters/chapter1/bg2.png"))
			std::cout << "CHAPTER1::ERROR::Error loading background file" << std::endl;

		bgSprite.setTexture(bgTexture);
		bgSprite.setPosition(sf::Vector2f(xPos, yPos));
		bgSprite.setColor(sf::Color(225, 225, 225, 125));
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::Chapter1::~Chapter1()
{
	if (!l1Treez.empty())
	{
		for (auto& i : l1Treez)
			deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the trees" << std::endl;
	}
	l1Treez.clear();

	if (!l2Treez.empty())
	{
		for (auto& i : l2Treez)
			deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the trees" << std::endl;
	}
	l2Treez.clear();

	if (!l3Treez.empty())
	{
		for (auto& i : l3Treez)
			deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the trees" << std::endl;
	}
	l3Treez.clear();

	if (!grounds.empty())
	{
		for (auto& i : grounds)
			if (i != nullptr)
				deletePointers(i);

		std::cout << "~CHAPTER1::Cleared the grounds" << std::endl;
	}
	grounds.clear();

	if (!foliage.empty())
	{
		for (auto& i : foliage)
			deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the foliage" << std::endl;
	}
	foliage.clear();

	if (!dComps.empty())
	{
		for (auto& i : dComps)
			if (i != nullptr)
				deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the open Dialogue Scene" << std::endl;
	}
	dComps.clear();

	if (!dBounds.empty())
	{
		for (auto& i : dBounds)
			deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the Dialogue Bounds" << std::endl;
	}
	dBounds.clear();

	deletePointers(player);
	std::cout << "~CHAPTER1::Deleted the player" << std::endl;

	if (!enemiez.empty())
	{
		for (auto& i : enemiez)
			if (i != nullptr)
				deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the enemiez" << std::endl;
	}
	enemiez.clear();

	if (!pauseMenu.empty())
	{
		for (auto& i : pauseMenu)
			if (i != nullptr)
				deletePointers(i);

		std::cout << "~CHAPTER1::Deleted the open PauseMenu" << std::endl;
	}
	pauseMenu.clear();

	rWindow->setView(rWindow->getDefaultView());
	std::cout << "~CHAPTER1::Reset the view" << std::endl;
}



////////////////////////////// POINTER FUNCTIONS //////////////////////////////

void ss::Chapter1::createPauseMenu()
{
	pauseMenu.push_back(new ss::PauseMenu(rWindow, L, textFont));
	inGameplay = false;
	paused = true;

	std::cout << "CHAPTER1::Created a pause menu" << std::endl;
}

void ss::Chapter1::createDialogueScene(const uint32_t scene)
{
	dComps.push_back(new ss::DialogueComponent(rWindow, L, textFont, scene));
	inGameplay = false;
	inCutscene = true;

	std::cout << "CHAPTER1::Created a Dialogue Scene" << std::endl;
}

void ss::Chapter1::deletePauseMenu(ss::PauseMenu*& pMenu)
{
	deletePointers(pMenu);

	if (!inCutscene)
		inGameplay = true;

	paused = false;
	pauseMenu.clear();

	std::cout << "CHAPTER1::Deleted the open pause menu" << std::endl;
}

void ss::Chapter1::deleteDialogueScene(ss::DialogueComponent*& dial)
{
	deletePointers(dial);
	inGameplay = true;
	inCutscene = false;
	dComps.clear();

	std::cout << "CHAPTER1::Deleted the Dialogue Scene" << std::endl;
}



////////////////////////////// UPDATE METHODS //////////////////////////////

const bool ss::Chapter1::updateFuntionKeys()
{
	if (updateWait())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		{
			wait = waitMax;

			createPauseMenu();

			return true;
		}
	}

	return false;
}

void ss::Chapter1::updatePlayer(const float& deltaTime)
{
	if (!player->getIsDead())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			player->MoveEntity(-1);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			player->MoveEntity(1);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			if (player->getIsGrounded())
				player->Jump();

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && player->getHitTimer() == 0)
			player->createHitRect();

		player->Update(deltaTime);
		player->MoveCamera(playerCamera);
	}
	else
		exit = true;
}

void ss::Chapter1::updateEnemies(const float& deltaTime)
{
	if (!enemiez.empty())
	{
		for (auto& i : enemiez)
		{
			if (i != nullptr)
			{
				i->Update(deltaTime);

				if (!player->getIsDead())
				{
					if (i->checkCollision(i->getEnemyView(), player->getEntityBounds()) && !i->checkCollision(i, player))
						i->FollowPlayer(player, i);
					else if (i->checkCollision(i, player))
					{
						i->deductHealth(0.3f);
						player->deductHealth(0.7f);
					}

					if (i->checkCollision(i->getEntityBounds(), player->getHitRect()))
						i->deductHealth(1.f);

					if (i->getIsDead())
					{
						deletePointers(i);
						break;
					}
				}

				if (!grounds.empty())
				{
					for (auto& j : grounds)
						if (i->checkCollision(i, j))
							break;
				}
			}
		}
	}
}

void ss::Chapter1::updateEnvironment(const float& deltaTime)
{
	const float dist = (playerCamera.getCenter().x - (playerCamera.getSize().x / 2.f));

	if (!dBounds.empty())
	{
		for (auto& i : dBounds)
		{
			if (player->checkCollision(player, i))
			{
				if (player->getX() < 1000.f)
					createDialogueScene(1);
				else
					createDialogueScene(2);
				break;
			}
		}
	}

	if (!grounds.empty())
	{
		for (auto& i : grounds)
			if (player->checkCollision(player, i))
				break;
	}

	for (auto& i : l1Treez)
	{
		i->Update(deltaTime);
		i->setX(i->getStartPos() + dist * i->getParallaxEffect());
	}

	for (auto& i : l2Treez)
	{
		i->Update(deltaTime);
		i->setX(i->getStartPos() + dist * i->getParallaxEffect());
	}

	for (auto& i : l3Treez)
	{
		i->Update(deltaTime);
		i->setX(i->getStartPos() + dist * i->getParallaxEffect());
	}
	{
		const float xPos = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f);
		const float yPos = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f);

		bgSprite.setPosition(sf::Vector2f(initBGPos + dist * 0.95f, yPos));
	}
}

void ss::Chapter1::updateDialogue(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (!dComps.empty())
	{
		for (auto& i : dComps)
			if (i != nullptr)
				i->Update(deltaTime, mousePosView);
	}

	if (updateWait())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O))
		{
			wait = waitMax;

			if (!dComps.empty())
			{
				for (auto& i : dComps)
					if (i != nullptr)
						deleteDialogueScene(i);
			}
		}
	}
}

void ss::Chapter1::updatePauseMenu(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (!pauseMenu.empty())
	{
		for (auto& i : pauseMenu)
		{
			if (i != nullptr)
			{
				i->Update(deltaTime, mousePosView);

				if (i->getQuit())
					deletePauseMenu(i);
				else if (i->getExit())
					exit = true;
			}
		}
	}

	if (updateWait())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		{
			wait = waitMax;

			if (!pauseMenu.empty())
			{
				for (auto& i : pauseMenu)
					if (i != nullptr)
						deletePauseMenu(i);
			}
		}
	}
}

void ss::Chapter1::Update(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (inGameplay && !paused && !updateFuntionKeys())
	{
		updatePlayer(deltaTime);

		rWindow->setView(playerCamera);

		updateEnemies(deltaTime);
		updateEnvironment(deltaTime);
	}
	else if (inCutscene && !paused && !updateFuntionKeys())
		updateDialogue(deltaTime, mousePosView);

	if (paused)
		updatePauseMenu(deltaTime, mousePosView);
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::Chapter1::Render(sf::RenderTarget* const& target)
{
	target->draw(bgSprite);

	if (!l3Treez.empty())
	{
		for (auto& i : l3Treez)
			i->Render(target);
	}

	if (!l2Treez.empty())
	{
		for (auto& i : l2Treez)
			i->Render(target);
	}

	if (!l1Treez.empty())
	{
		for (auto& i : l1Treez)
			i->Render(target);
	}

	if (!dBounds.empty())
	{
		for (auto& i : dBounds)
			i->Render(target);
	}

	player->Render(target);

	if (!enemiez.empty())
	{
		for (auto& i : enemiez)
			if (i != nullptr)
				i->Render(target);
	}

	if (!foliage.empty())
	{
		for (auto& i : foliage)
			i->Render(target);
	}

	if (!grounds.empty())
	{
		for (auto& i : grounds)
			i->Render(target);
	}

	if (inCutscene)
	{
		if (!dComps.empty())
		{
			for (auto& i : dComps)
				if (i != nullptr)
					i->Render(target);
		}
	}

	if (paused)
	{
		if (!pauseMenu.empty())
		{
			for (auto& i : pauseMenu)
				if (i != nullptr)
					i->Render(target);
		}
	}
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::Chapter1::createPlayer(const float x, const float y, const std::string name)
{
	player = new ss::Player(x, y, name);
	{
		playerCamera.setSize(sf::Vector2f(static_cast<float>(rWindow->getSize().x), static_cast<float>(rWindow->getSize().y)));

		const float posX = (player->getX() + player->getWidth()) - (player->getWidth() / 2.f);

		playerCamera.setCenter(sf::Vector2f(posX, player->getY()));
	}
}

void ss::Chapter1::createEnemy(const float x, const float y, const float width, const float height, const std::string group)
{
	enemiez.push_back(new ss::Enemy(x, y, width, height, group));
}

void ss::Chapter1::createGround(const float x, const float y, const std::string texturePath)
{
	grounds.push_back(new ss::WorldObject(x, y));
	grounds.back()->setRectColor(sf::Color::Magenta);
	grounds.back()->setTexture(texturePath, rWindow);
}

void ss::Chapter1::createTree(const float x, const float y, const float width, const float height, const float p, const std::string texturePath, const uint32_t layer)
{
	switch (layer)
	{
	case 1:
		l1Treez.push_back(new ss::WorldObject(x, y, width, height, "Trees"));
		l1Treez.back()->setParallaxEffect(p);
		l1Treez.back()->setTexture(texturePath, rWindow);
		l1Treez.back()->setTextureColor(sf::Color(175, 175, 175));
		break;
	case 2:
		l2Treez.push_back(new ss::WorldObject(x, y, width, height, "Trees"));
		l2Treez.back()->setParallaxEffect(p);
		l2Treez.back()->setTexture(texturePath, rWindow);
		l2Treez.back()->setTextureColor(sf::Color(125, 125, 125));
		break;
	case 3:
		l3Treez.push_back(new ss::WorldObject(x, y, width, height, "Trees"));
		l3Treez.back()->setParallaxEffect(p);
		l3Treez.back()->setTexture(texturePath, rWindow);
		l3Treez.back()->setTextureColor(sf::Color(75, 75, 75));
		break;
	}
}

void ss::Chapter1::createFoliage(const float x, const float y, const float width, const float height, const std::string texturePath)
{
	foliage.push_back(new ss::WorldObject(x, y, width, height, "Foliage"));
	foliage.back()->setRectColor(sf::Color::Cyan);
	foliage.back()->setTexture(texturePath, rWindow);
}

void ss::Chapter1::createDialogueBounds(const float x, const float y, const float width, const float height)
{
	dBounds.push_back(new ss::WorldObject(x, y, width, height, "Bounds"));
	dBounds.back()->setRectColor(sf::Color::Yellow);
}

int ss::Chapter1::lua_CreatePlayer(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string name = static_cast<std::string>(lua_tostring(L, 4));

	chapter1->createPlayer(posX, posY, name);

	return 0;
}

int ss::Chapter1::lua_CreateEnemies(lua_State* L)
{
	if (lua_gettop(L) != 6)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const float width = static_cast<float>(lua_tonumber(L, 4));
	const float height = static_cast<float>(lua_tonumber(L, 5));
	const std::string name = static_cast<std::string>(lua_tostring(L, 6));

	chapter1->createEnemy(posX, posY, width, height, name);

	return 0;
}

int ss::Chapter1::lua_CreateGround(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string path = static_cast<std::string>(lua_tostring(L, 4));

	chapter1->createGround(posX, posY, path);

	return 0;
}

int ss::Chapter1::lua_CreateTrees(lua_State* L)
{
	if (lua_gettop(L) != 8)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const float width = static_cast<float>(lua_tonumber(L, 4));
	const float height = static_cast<float>(lua_tonumber(L, 5));
	const float pE = static_cast<float>(lua_tonumber(L, 6));
	const std::string path = static_cast<std::string>(lua_tostring(L, 7));
	const uint32_t layer = static_cast<uint32_t>(lua_tonumber(L, 8));

	chapter1->createTree(posX, posY, width, height, pE, path, layer);

	return 0;
}

int ss::Chapter1::lua_CreateFoliage(lua_State* L)
{
	if (lua_gettop(L) != 6)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const float width = static_cast<float>(lua_tonumber(L, 4));
	const float height = static_cast<float>(lua_tonumber(L, 5));
	const std::string path = static_cast<std::string>(lua_tostring(L, 6));

	chapter1->createFoliage(posX, posY, width, height, path);

	return 0;
}

int ss::Chapter1::lua_CreateDialogueBounds(lua_State* L)
{
	if (lua_gettop(L) != 5)
		return -1;

	ss::Chapter1* chapter1 = static_cast<ss::Chapter1*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const float width = static_cast<float>(lua_tonumber(L, 4));
	const float height = static_cast<float>(lua_tonumber(L, 5));

	chapter1->createDialogueBounds(posX, posY, width, height);

	return 0;
}