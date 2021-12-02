#include "pch.h"
#include "DialogueComponent.h"

////////////////////////////// CONSTRUCTOR //////////////////////////////

ss::DialogueComponent::DialogueComponent(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font, const uint32_t scene)
	: ss::Menu(window, L, font),
	displayImage(false), displayDialogue(true), displayBackground(true), mousePressed(false),
	setStr("null"), dialogueString("")
{
	checkScript = checkLua(L, luaL_dofile(L, "./scripts/game/Dialogue.lua"));

	if (!checkScript)
	{
		quit = true;
		std::cout << "MAINMENU::ERROR::Could not find LUA script!" << std::endl;
	}

	lua_register(L, "lua_SetSentence", lua_SetString);
	lua_register(L, "lua_SetCharacter", lua_SetDialogueCharacter);
	lua_register(L, "lua_DisplayChar", lua_SetDialogueText);

	{
		const sf::Color bgColor(10, 10, 10, 200);

		const float buffer = 50.f;

		textBg.setSize(sf::Vector2f(900.f, 350.f));
		textBg.setFillColor(bgColor);

		float posX = background.getPosition().x + (background.getSize().x / 2.f) - (textBg.getSize().x / 2.f);
		float posY = background.getPosition().y + background.getSize().y - textBg.getSize().y - buffer;

		textBg.setPosition(sf::Vector2f(posX, posY));
	}

	{
		dialogueText.setFont(textFont);
		dialogueText.setCharacterSize(25);

		const float buffer = 100.f;
		const float posX = textBg.getPosition().x + (buffer / 2.f);
		const float posY = textBg.getPosition().y + buffer;

		dialogueText.setPosition(sf::Vector2f(posX, posY));

		dialogueCharacter.setFont(textFont);
		dialogueCharacter.setCharacterSize(25);

		dialogueCharacter.setPosition(sf::Vector2f(posX, posY - 70.f));
	}

	lua_getglobal(L, "CallDialogue");
	if (lua_isfunction(L, -1))
	{
		lua_pushlightuserdata(L, this);
		lua_pushnumber(L, scene);

		checkLua(L, lua_pcall(L, 2, 0, 0));
	}
}



////////////////////////////// DESTRUCTOR //////////////////////////////

ss::DialogueComponent::~DialogueComponent()
{
}



////////////////////////////// UPDATE METHOD //////////////////////////////

void ss::DialogueComponent::Update(const float& deltaTime, const sf::Vector2f& mousePosView)
{
	if (background.getGlobalBounds().contains(mousePosView))
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mousePressed = true;

	lua_getglobal(L, "IssueNext");
	if (lua_isfunction(L, -1))
	{
		lua_pushstring(L, setStr.c_str());
		lua_pushboolean(L, mousePressed);

		checkLua(L, lua_pcall(L, 2, 1, 0));

		mousePressed = static_cast<bool>(lua_toboolean(L, -1));
	}
}



////////////////////////////// RENDER METHOD //////////////////////////////

void ss::DialogueComponent::Render(sf::RenderTarget* const& target)
{
	target->draw(background);
	target->draw(textBg);

	target->draw(dialogueText);
	target->draw(dialogueCharacter);
}



////////////////////////////// LUA FUNCTIONS //////////////////////////////

void ss::DialogueComponent::setDialogueText(const std::string text, const uint32_t op)
{
	dialogueString += text;
	dialogueText.setString(dialogueString);
}

void ss::DialogueComponent::setDialogueCharacter(const std::string text)
{
	dialogueCharacter.setString(text);
}

void ss::DialogueComponent::setString(const std::string text)
{
	setStr = text;
}

int ss::DialogueComponent::lua_SetDialogueText(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::DialogueComponent* dialogue = static_cast<ss::DialogueComponent*>(lua_touserdata(L, 1));
	const std::string str = static_cast<std::string>(lua_tostring(L, 2));

	dialogue->setDialogueText(str, 0);

	return 0;
}

int ss::DialogueComponent::lua_SetDialogueCharacter(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::DialogueComponent* dialogue = static_cast<ss::DialogueComponent*>(lua_touserdata(L, 1));
	const std::string str = static_cast<std::string>(lua_tostring(L, 2));

	dialogue->setDialogueCharacter(str);

	return 0;
}

int ss::DialogueComponent::lua_SetString(lua_State* L)
{
	if (lua_gettop(L) != 2)
		return -1;

	ss::DialogueComponent* dialogue = static_cast<ss::DialogueComponent*>(lua_touserdata(L, 1));
	const std::string str = static_cast<std::string>(lua_tostring(L, 2));

	dialogue->setString(str);

	return 0;
}