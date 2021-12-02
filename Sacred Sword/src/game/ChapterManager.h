#ifndef CHAPTERMANAGER_H
#define CHAPTERMANAGER_H

#include "engine/States.h"
#include "engine/Chapter.h"
#include "game/Chapters/Chapter1.h"

class ss::Chapter;
class ss::Chapter1;

namespace ss
{
	class ChapterManager
		: public ss::States
	{
	private:
		std::queue<ss::Chapter*> chapters;

		void deleteLastChapter();
		void createChapter1();

		void fadeInOut();

	public:
		ChapterManager(sf::RenderWindow* const& window);
		~ChapterManager();

		void Update(const float& deltaTime);
		void Render(sf::RenderTarget* const& target);


		/////////////// LUA FUNCTIONS ///////////////

		void fadeStateIn(int speed);
		void fadeStateOut(int speed);

		static int lua_FadeStateIn(lua_State* L);
		static int lua_FadeStateOut(lua_State* L);
	};
}

#endif