#ifndef CHAPTER1_H
#define CHAPTER1_H

#include "engine/Chapter.h"

class ss::Player;
class ss::Enemy;
class ss::WorldObject;
class ss::PauseMenu;
class ss::DialogueComponent;

namespace ss
{
	class Chapter1 :
		public ss::Chapter
	{
	private:
		float initBGPos;

		sf::Texture bgTexture;
		sf::Sprite bgSprite;

		ss::Player* player = nullptr;

		std::vector<ss::Enemy*> enemiez;
		std::vector<ss::WorldObject*> grounds;
		std::vector<ss::PauseMenu*> pauseMenu;
		std::vector<ss::WorldObject*> l1Treez;
		std::vector<ss::WorldObject*> l2Treez;
		std::vector<ss::WorldObject*> l3Treez;
		std::vector<ss::WorldObject*> foliage;
		std::vector<ss::DialogueComponent*> dComps;
		std::vector<ss::WorldObject*> dBounds;

		void createPauseMenu();
		void createDialogueScene(const uint32_t scene);
		void deletePauseMenu(ss::PauseMenu*& pMenu);
		void deleteDialogueScene(ss::DialogueComponent*& dial);

		const bool updateFuntionKeys();

		void updatePlayer(const float& deltaTime);
		void updateEnemies(const float& deltaTime);
		void updateEnvironment(const float& deltaTime);
		void updateDialogue(const float& deltaTime, const sf::Vector2f& mousePosView);
		void updatePauseMenu(const float& deltaTime, const sf::Vector2f& mousePosView);

	public:
		Chapter1(sf::RenderWindow* const& window, lua_State*& L, const sf::Font& font);
		~Chapter1();

		void Update(const float& deltaTime, const sf::Vector2f& mousePosView);
		void Render(sf::RenderTarget* const& target);


		/////////////// LUA FUNCTIONS ///////////////

		void createPlayer(const float x, const float y, const std::string name);
		void createEnemy(const float x, const float y, const float width, const float height, const std::string group);
		void createGround(const float x, const float y, const std::string texturePath);
		void createTree(const float x, const float y, const float width, const float height, const float p, const std::string texturePath, const uint32_t layer);
		void createFoliage(const float x, const float y, const float width, const float height, const std::string texturePath);
		void createDialogueBounds(const float x, const float y, const float width, const float height);

		static int lua_CreatePlayer(lua_State* L);
		static int lua_CreateEnemies(lua_State* L);
		static int lua_CreateGround(lua_State* L);
		static int lua_CreateTrees(lua_State* L);
		static int lua_CreateFoliage(lua_State* L);
		static int lua_CreateDialogueBounds(lua_State* L);
	};
}

#endif