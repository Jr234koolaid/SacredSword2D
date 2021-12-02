#include "pch.h"
#include "PauseMenu.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::PauseMenu::PauseMenu(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font)
	: ss::Menu(window, L, font)
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/game/PauseMenu.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "MAINMENU::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_CreateButton", lua_CreateButton);
	lua_register(L, "lua_FadeFromToRect", lua_FadeButtonRect);
	lua_register(L, "lua_FadeFromToText", lua_FadeButtonText);

	if (!menuFont.loadFromFile("./res/fonts/SpecialElite-Regular.ttf"))
		std::cout << "ERROR::PAUSEMENU::INITFONT::Uh oh" << std::endl;

	setBackgroundPosition();

	lua_getglobal(L, "OnMenuEnter");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, background.getPosition().x);
		lua_pushnumber(L, background.getPosition().y);

		checkLua(L, lua_pcall(L, 3, 0, 0));
	}

	{
		bg.setFillColor(sf::Color(20, 20, 20, 235));
		bg.setSize(sf::Vector2f(700.f, rWindow->getView().getSize().y));

		const float posX = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f);
		const float posY = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f);

		bg.setPosition(sf::Vector2f(posX, posY));
	}

	{
		pauseLogo.setSize(sf::Vector2f(500.f, 125.f));

		const float posX = background.getPosition().x + 100.f;
		const float posY = background.getPosition().y + 100.f;

		pauseLogo.setPosition(sf::Vector2f(posX, posY));
		pauseLogo.setFillColor(sf::Color::Transparent);
		pauseLogo.setOutlineColor(sf::Color::Cyan);
		pauseLogo.setOutlineThickness(1.5f);
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::PauseMenu::~PauseMenu()
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			deletePointers(i.second);
	}
	buttons.clear();
	std::cout << "~PAUSEMENU::Deleted the buttons and cleared the button map" << std::endl;

	if (!optionsMenu.empty())
	{
		for (auto& i : optionsMenu)
			if (i != nullptr)
				deletePointers(i);
	}
	optionsMenu.clear();
	std::cout << "~PAUSEMENU::Cleared the OptionsScreen vector" << std::endl;

	if (!alerts.empty())
	{
		for (auto& i : alerts)
			if (i != nullptr)
				deletePointers(i);
	}
	alerts.clear();
	std::cout << "~PAUSEMENU::Cleared the Alerts vector" << std::endl;
}



////////////////////////////// POINTER FUNCTIONS //////////////////////////////

void ss::PauseMenu::createAlertPopup(const std::string name, const std::string alertText)
{
	alerts.push_back(new ss::AlertPopup(rWindow, L, menuFont, alertText));
	alert = true;
	inOptions = false;

	std::cout << "PAUSEMENU::Created an alert screen" << std::endl;
}

void ss::PauseMenu::createOptionScreen(const std::string name)
{
	optionsMenu.push_back(new ss::OptionsMenu(rWindow, L, menuFont));
	alert = false;
	inOptions = true;

	std::cout << "PAUSEMENU::Created an options screen" << std::endl;
}

void ss::PauseMenu::deleteAlertPopup(ss::AlertPopup*& alertP)
{
	deletePointers(alertP);
	alert = false;
	alerts.clear();

	std::cout << "PAUSEMENU::Deleted the open alert screen" << std::endl;
}

void ss::PauseMenu::deleteOptionScreen(ss::OptionsMenu*& optionsP)
{
	deletePointers(optionsP);
	inOptions = false;
	optionsMenu.clear();

	std::cout << "PAUSEMENU::Deleted the open options screen" << std::endl;
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::PauseMenu::setBackgroundPosition()
{
	const float xPos = rWindow->getView().getCenter().x - (rWindow->getView().getSize().x / 2.f);
	const float yPos = rWindow->getView().getCenter().y - (rWindow->getView().getSize().y / 2.f);

	if (background.getPosition().x != xPos || background.getPosition().y != yPos)
		background.setPosition(sf::Vector2f(xPos, yPos));
}

void ss::PauseMenu::updateAlertPopup(const float& deltaTime, const sf::Vector2f& mousePosView)
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
					exit = true;
			}
		}
	}
}

void ss::PauseMenu::updateOptionScreen(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (!optionsMenu.empty())
	{
		for (auto& i : optionsMenu)
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

void ss::PauseMenu::updateButtons(const sf::Vector2f& mousePosView)
{
	if (!buttons.empty())
	{
		for (auto& i : buttons)
			i.second->Update(mousePosView);

		if (buttons["RESUME"]->isPressed())
			quit = true;
		else if (buttons["OPTIONS"]->isPressed())
			createOptionScreen("OPTIONS");
		else if (buttons["MAINMENU"]->isPressed())
			createAlertPopup("EXIT", "ARE YOU SURE YOU WANT TO\nGO BACK TO THE MAIN MENU?");
	}
}

void ss::PauseMenu::Update(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (!alert && !inOptions)
	{
		setBackgroundPosition();

		lua_getglobal(L, "AnimateButtons");
		if (lua_isfunction(L, -1))
			checkLua(L, lua_pcall(L, 0, 0, 0));

		updateButtons(mousePosView);
	}
	else
	{
		if (alert)
			updateAlertPopup(deltaTime, mousePosView);
		else if (inOptions)
			updateOptionScreen(deltaTime, mousePosView);
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::PauseMenu::Render(sf::RenderTarget* const& target)
{
	target->draw(background);
	target->draw(bg);

	if (!buttons.empty())
		for (auto& i : buttons)
			i.second->Render(target);

	target->draw(pauseLogo);

	if (alert)
	{
		if (!alerts.empty())
			for (auto& i : alerts)
				if (i != nullptr)
					i->Render(target);
	}
	else if (inOptions)
	{
		if (!optionsMenu.empty())
			for (auto& i : optionsMenu)
				if (i != nullptr)
					i->Render(target);
	}
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::PauseMenu::createButton(const float x, const float y, const std::string text, const bool isActive)
{
	buttons[text] = new ss::Button(x, y, text, menuFont, isActive);
}

void ss::PauseMenu::fadeButtonRect(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonRectOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonRectOpacity(), to, speed));
}

void ss::PauseMenu::fadeButtonText(const std::string button, const int to, const int speed)
{
	if (!buttons[button]->getAnimator().getFinishedFading())
		buttons[button]->setButtonTextOpacity(buttons[button]->getAnimator().FadeIn(buttons[button]->getButtonTextOpacity(), to, speed));
}

int ss::PauseMenu::lua_CreateButton(lua_State* L)
{
	if (lua_gettop(L) != 5)
		return -1;

	ss::PauseMenu* pausemenu = static_cast<ss::PauseMenu*>(lua_touserdata(L, 1));
	const float posX = static_cast<float>(lua_tonumber(L, 2));
	const float posY = static_cast<float>(lua_tonumber(L, 3));
	const std::string text = static_cast<std::string>(lua_tostring(L, 4));
	const bool active = static_cast<bool>(lua_toboolean(L, 5));

	pausemenu->createButton(posX, posY, text, active);

	return 0;
}

int ss::PauseMenu::lua_FadeButtonRect(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::PauseMenu* pause = static_cast<ss::PauseMenu*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	pause->fadeButtonRect(button, toOpacity, speed);

	return 0;
}

int ss::PauseMenu::lua_FadeButtonText(lua_State* L)
{
	if (lua_gettop(L) != 4)
		return -1;

	ss::PauseMenu* pause = static_cast<ss::PauseMenu*>(lua_touserdata(L, 1));
	const std::string button = static_cast<std::string>(lua_tostring(L, 2));
	const int toOpacity = static_cast<int>(lua_tonumber(L, 3));
	const int speed = static_cast<int>(lua_tonumber(L, 4));

	pause->fadeButtonText(button, toOpacity, speed);

	return 0;
}