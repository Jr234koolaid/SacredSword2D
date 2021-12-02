#ifndef ANIMATOR_H
#define ANIMATOR_H

namespace ss
{
	class Animator
	{
	private:
		int fadeDirection;
		bool finishedMoving;
		bool finishedFading;

	public:
		Animator();
		~Animator();

		const int FadeInOut(int opacity, const int lowerOpacity, const int higherOpacity);
		const sf::Uint8 FadeIn(int from, const int to, const int speed);
		const sf::Uint8 FadeOut(int from, const int to, const int speed);

		void MoveInOut();
		const float MoveIn(float from, const float to, const float speed);
		const float MoveOut(float from, const float to, const float speed);

		inline const bool getFinishedMoving() const { return finishedMoving; }
		inline const bool getFinishedFading() const { return finishedFading; }
		inline void setFinishedMoving(const bool set) { finishedMoving = set; }
		inline void setFinishedFading(const bool set) { finishedFading = set; }
	};
}

#endif