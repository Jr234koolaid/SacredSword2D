#include "pch.h"
#include "MainMenuState.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::MainMenuState::MainMenuState(sf::RenderWindow* const& window)
	: States(window)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/mainmenu/MainMenu.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "MAINMENU::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_CreateButton", lua_CreateButton);
	lua_register(L, "lua_FadeFromToRect", lua_FadeButtonRect);
	lua_register(L, "lua_FadeFromToText", lua_FadeButtonText);
	lua_register(L, "lua_SetFadeIn", lua_FadeStateIn);
	lua_register(L, "lua_SetFadeOut", lua_FadeStateOut);

	if (!textFont.loadFromFile("./res/fonts/SpecialElite-Regular.ttf"))
		std::cout << "ERROR::MAINMENU::INITFONT::Uh oh" << std::endl;

	lua_getglobal(L, "OnStateEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, rWindow->getView().getCenter().x);
		lua_pushnumber(L, rWindow->getView().getCenter().y);
		lua_pushnumber(L, rWindow->getView().getSize().x);
		lua_pushnumber(L, rWindow->getView().getSize().y);

		checkLua(L, lua_pcall(L, 5, 0, 0));
	}

	{
		float posX = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f) + 50.f;
		float posY = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f) + 50.f;

		buttonContainerLogo.setSize(sf::Vector2f(330.f, 405.f));

		title.setFont(textFont);
		title.setCharacterSize(85);
		title.setString("SACRED SWORD");
		title.setPosition(sf::Vector2f(posX, posY));

		posY = title.getPosition().y + title.getGlobalBounds().height + 25.f;

		demo.setFont(textFont);
		demo.setCharacterSize(25);
		demo.setString("Demo");
		demo.setPosition(sf::Vector2f(posX, posY));

		posX = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f) + 50.f;
		posY = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f) + (rWindow->getView().getSize().y / 2.f) - 150.f;

		buttonContainerLogo.setPosition(sf::Vector2f(posX, posY));
		buttonContainerLogo.setFillColor(sf::Color::Transparent);
		buttonContainerLogo.setOutlineColor(sf::Color::Cyan);
		buttonContainerLogo.setOutlineThickness(2.f);
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::MainMenuState::~MainMenuState()
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			deletePointers(i.second);
	}
	buttons.clear();
	std::cout << "~MAINMENU::deleted the buttons and cleared the button map" << std::endl;

	if (!optionScreens.empty())
	{
		for (auto& i : optionScreens)
			if (i != nullptr)
				deletePointers(i);
	}
	optionScreens.clear();
	std::cout << "~MAINMENU::Cleared the OptionsScreen vector" << std::endl;

	if (!alerts.empty())
	{
		for (auto& i : alerts)
			if (i != nullptr)
				deletePointers(i);
	}
	alerts.clear();
	std::cout << "~MAINMENU::Cleared the Alerts vector" << std::endl;
}



////////////////////////////// POINTER FUNCTIONS //////////////////////////////

void ss::MainMenuState::createAlertPopup(const std::string name, const std::string alertText)
{
	alerts.push_back(new ss::AlertPopup(rWindow, L, textFont, alertText));
	paused = true;
	alert = true;
	std::cout << "MAINMENU::Created an alert screen" << std::endl;
}

void ss::MainMenuState::createOptionScreen(const std::string name)
{
	optionScreens.push_back(new ss::OptionsMenu(rWindow, L, textFont));
	paused = true;
	std::cout << "MAINMENU::Created an options screen" << std::endl;
}

void ss::MainMenuState::deleteAlertPopup(ss::AlertPopup*& alertP)
{
	deletePointers(alertP);
	paused = false;
	alert = false;
	alerts.clear();

	std::cout << "MAINMENU::Deleted the open alert screen" << std::endl;
}

void ss::MainMenuState::deleteOptionScreen(ss::OptionsMenu*& optionsP)
{
	deletePointers(optionsP);
	paused = false;
	optionScreens.clear();

	std::cout << "MAINMENU::Deleted the open options screen" << std::endl;
}



////////////////////////////// UPDATE METHODS //////////////////////////////

void ss::MainMenuState::fadeInOut()
{
	fadeRect.setPosition(rWindow->getView().getCenter() - (rWindow->getView().getSize() / 2.f));

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
		transferGame = true;
}

void ss::MainMenuState::updateAlertPopup(const float& deltaTime)
{
	if (!alerts.empty())
	{
		for (auto& i : alerts)
		{
			if (i != nullptr)
			{
				i->Update(deltaTime, mousePosView);

				if (i->getQuit())
					deleteAlertPopup(i);
				else if (i->getExit())
				{
					sFadingOut = true;
					fadeState = 1;
				}
			}
		}
	}
}

void ss::MainMenuState::updateOptionScreen(const float& deltaTime)
{
	if (!optionScreens.empty())
	{
		for (auto& i : optionScreens)
		{
			if (i != nullptr)
			{
				i->Update(deltaTime, mousePosView);

				if (i->getQuit())
					deleteOptionScreen(i);
			}
		}
	}
}

void ss::MainMenuState::updateButtons()
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			i.second->Update(mousePosView);

		if (buttons["NEW GAME"]->isPressed())
		{
			sFadingOut = true;
			fadeState = 2;
		}
		else if (buttons["CONTINUE"]->isPressed())
			;
		else if (buttons["MY ROOM"]->isPressed())
			;
		else if (buttons["OPTIONS"]->isPressed())
			createOptionScreen("OPTIONSCREEN");
		else if (buttons["QUIT"]->isPressed())
			createAlertPopup("ALERT", "Are you sure you want to exit?");
	}
}

void ss::MainMenuState::Update(const float& deltaTime)
{
	UpdateMousePos(rWindow);

	if (!fadedIn || sFadingOut)
		fadeInOut();

	if (hFadedIn && !fadedOut)
	{
		lua_getglobal(L, "AnimateButtons");
		if (lua_isfunction(L, -1))
			checkLua(L, lua_pcall(L, 0, 0, 0));

		if (paused)
		{
			if (alert)
				updateAlertPopup(deltaTime);
			else
				updateOptionScreen(deltaTime);
		}
		else
			updateButtons();
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::MainMenuState::Render(sf::RenderTarget* const& target)
{
	if (hFadedIn)
	{
		if (!buttons.empty())
			for (auto& i : buttons)
				i.second->Render(target);

		target->draw(buttonContainerLogo);
		target->draw(title);
		target->draw(demo);
	}

	if (paused)
	{
		if (alert)
		{
			if (!alerts.empty())
				for (auto& i : alerts)
					if (i != nullptr)
						i->Render(target);
		}
		else
		{
			if (!optionScreens.empty())
				for (auto& i : optionScreens)
					if (i != nullptr)
						i->Render(target);
		}
	}

	if (!fadedIn || sFadingOut)
		target->draw(fadeRect);
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::MainMenuState::createButton(const float x, const float y, const std::string text, const bool isActive)
{
	buttons[text] = new ss::Button(x, y, text, textFont, isActive);
}

void ss::MainMenuState::fadeButtonRect(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonRectOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonRectOpacity(), to, speed));
}

void ss::MainMenuState::fadeButtonText(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonTextOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonTextOpacity(), to, speed));
}

void ss::MainMenuState::fadeStateIn(int speed)
{
	if (static_cast<int>(fadeRect.getFillColor().a) < speed)
		speed -= 1;

	fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b, fadeRect.getFillColor().a - speed));
}

void ss::MainMenuState::fadeStateOut(int speed)
{
	if (static_cast<int>(fadeRect.getFillColor().a) + speed > 255)
		speed = (static_cast<int>(fadeRect.getFillColor().a) + speed) % 2;

	fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b, fadeRect.getFillColor().a + speed));
}

int ss::MainMenuState::lua_CreateButton(lua_State* L)
{
	if (lua_gettop(L) != 5)
		return -1;

	ss::MainMenuState* mainmenu = static_cast<ss::MainMenuState*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string text = static_cast<std::string>(lua_tostring(L, 4));
	const bool active = static_cast<bool>(lua_toboolean(L, 5));

	mainmenu->createButton(posX, posY, text, active);

	return 0;
}

int ss::MainMenuState::lua_FadeButtonRect(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::MainMenuState* mainmenu = static_cast<ss::MainMenuState*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	mainmenu->fadeButtonRect(button, toOpacity, speed);

	return 0;
}

int ss::MainMenuState::lua_FadeButtonText(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::MainMenuState* mainmenu = static_cast<ss::MainMenuState*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	mainmenu->fadeButtonText(button, toOpacity, speed);

	return 0;
}

int ss::MainMenuState::lua_FadeStateIn(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::MainMenuState* mainmenu = static_cast<ss::MainMenuState*>(lua_touserdata(L, 1));
	const int speed = static_cast<int>(lua_tonumber(L, 2));

	mainmenu->fadeStateIn(speed);

	return 0;
}

int ss::MainMenuState::lua_FadeStateOut(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::MainMenuState* mainmenu = static_cast<ss::MainMenuState*>(lua_touserdata(L, 1));
	const int speed = static_cast<int>(lua_tonumber(L, 2));

	mainmenu->fadeStateOut(speed);

	return 0;
}