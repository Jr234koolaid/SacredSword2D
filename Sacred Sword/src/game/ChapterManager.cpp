#include "pch.h"
#include "ChapterManager.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::ChapterManager::ChapterManager(sf::RenderWindow* const& window)
	: States(window)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/game/ChapterManager.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "CHAPTERMANAGER::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_SetFadeIn", lua_FadeStateIn);
	lua_register(L, "lua_SetFadeOut", lua_FadeStateOut);

	if (!textFont.loadFromFile("./res/fonts/SourceSansPro-Regular.ttf"))
		std::cout << "ERROR::CHATPERMANAGER::INITFONT::Uh oh" << std::endl;

	lua_getglobal(L, "OnStateEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);

		checkLua(L, lua_pcall(L, 1, 0, 0));
	}

	chapters.emplace(new ss::Chapter1(rWindow, L, textFont));
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::ChapterManager::~ChapterManager()
{
	while (!chapters.empty())
	{
		if (chapters.front() != nullptr)
		{
			delete chapters.front();
			chapters.front() = nullptr;
			chapters.pop();
		}
	}

	std::cout << "~GAMEMANAGER::Deleted the ChapterManager" << std::endl;
}



////////////////////////////// POINTER FUNCTIONS //////////////////////////////

void ss::ChapterManager::deleteLastChapter()
{
	delete chapters.front();
	chapters.front() = nullptr;
	chapters.pop();

	std::cout << "CHAPTERMANAGER::Deleted a chapter" << std::endl;
}

void ss::ChapterManager::createChapter1()
{
	deleteLastChapter();

	chapters.emplace(new ss::Chapter1(rWindow, L, textFont));
	std::cout << "CHAPTERMANAGER::Created Chapter 1" << std::endl;
}



////////////////////////////// UPDATE METHODS //////////////////////////////

void ss::ChapterManager::fadeInOut()
{
	if (!fadedIn)
	{
		lua_getglobal(L, "FadeIn");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, static_cast<int>(fadeRect.getFillColor().a));
			lua_pushboolean(L, fadedIn);
			lua_pushboolean(L, hFadedIn);

			checkLua(L, lua_pcall(L, 3, 2, 0));

			fadedIn = static_cast<bool>(lua_toboolean(L, -2));
			hFadedIn = static_cast<bool>(lua_toboolean(L, -1));
		}
	}
	else if (sFadingOut && fadedIn)
	{
		lua_getglobal(L, "FadeOut");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, static_cast<int>(fadeRect.getFillColor().a));
			lua_pushboolean(L, fadedOut);

			checkLua(L, lua_pcall(L, 2, 1, 0));

			fadedOut = static_cast<bool>(lua_toboolean(L, -1));
		}
	}

	if (fadedOut && fadeState == 1)
		quit = true;
	else if (fadedOut && fadeState == 2)
	{
		deleteLastChapter();
		transferMainMenu = true;
	}
}

void ss::ChapterManager::Update(const float& deltaTime)
{
	UpdateMousePos(rWindow);

	if (hFadedIn && !fadedOut)
	{
		if (!chapters.empty())
		{
			chapters.back()->Update(deltaTime, mousePosView);

			if (chapters.back()->getState() == 3)
			{
				sFadingOut = true;
				fadeState = 2;
			}
		}
	}

	if (!fadedIn || sFadingOut)
	{
		fadeRect.setPosition(rWindow->getView().getCenter() - (rWindow->getView().getSize() / 2.f));

		fadeInOut();
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::ChapterManager::Render(sf::RenderTarget* const& target)
{
	if (!chapters.empty())
		chapters.back()->Render(target);

	if (!fadedIn || sFadingOut)
		target->draw(fadeRect);
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::ChapterManager::fadeStateIn(int speed)
{
	if (static_cast<int>(fadeRect.getFillColor().a) < speed)
		speed -= 1;

	fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b, fadeRect.getFillColor().a - speed));
}

void ss::ChapterManager::fadeStateOut(int speed)
{
	if (static_cast<int>(fadeRect.getFillColor().a) + speed > 255)
		speed = (static_cast<int>(fadeRect.getFillColor().a) + speed) % 2;

	fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b, fadeRect.getFillColor().a + speed));
}

int ss::ChapterManager::lua_FadeStateIn(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::ChapterManager* chapters = static_cast<ss::ChapterManager*>(lua_touserdata(L, 1));
	const int speed = static_cast<int>(lua_tonumber(L, 2));

	chapters->fadeStateIn(speed);

	return 0;
}

int ss::ChapterManager::lua_FadeStateOut(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::ChapterManager* chapters = static_cast<ss::ChapterManager*>(lua_touserdata(L, 1));
	const int speed = static_cast<int>(lua_tonumber(L, 2));

	chapters->fadeStateOut(speed);

	return 0;
}