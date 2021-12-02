#ifndef ENTITY_H
#define ENTITY_H

namespace ss
{
	class Entity
	{
	protected:
		const float maxHealth, gravity, jumpF, fallM;
		float accel, decel, maxVel, hp;
		const int jumpT, hitRectT;
		int level, jumpTimer, hitRectTimer;
		bool isGrounded, isColliding, isDead;
		const std::string tag;

		bool set_texture = false; //remove

		sf::Vector2f velocity;
		sf::RectangleShape entityRect;
		sf::RectangleShape* hitRect = nullptr;
		sf::RectangleShape healthBarBorder;
		sf::RectangleShape healthBarFill;
		sf::IntRect textureRect;
		sf::Texture entityTexture;
		sf::Sprite entitySprite;

		inline void setPosition(const float x, const float y) { entityRect.setPosition(sf::Vector2f(x, y)); }
		inline void setIsColliding(const bool set) { isColliding = set; }
		inline void setAcceleration(const float set) { accel = set; }
		inline void setDeceleration(const float set) { decel = set; }
		inline void setMaxVelocity(const float set) { maxVel = set; }

		void calculateVelocity(const float& deltaTime);
		void updateHealthBarPosition();

	public:
		Entity(const float x, const float y, const std::string tag, const float hp = 100.f);
		virtual ~Entity();

		inline sf::FloatRect getEntityBounds() const { return sf::FloatRect(entityRect.getPosition().x, entityRect.getPosition().y, entityRect.getSize().x, entityRect.getSize().y); }
		sf::FloatRect getHitRect() const;
		inline const sf::Color& getTextureColor() { return entitySprite.getColor(); }
		inline const float getX() { return entityRect.getPosition().x; }
		inline const float getY() { return entityRect.getPosition().y; }
		inline const float getWidth() { return entityRect.getSize().x; }
		inline const float getHeight() { return entityRect.getSize().y; }
		inline const int getHitTimer() { return hitRectTimer; }
		inline const bool getIsGrounded() { return isGrounded; }
		inline const bool getIsDead() { return isDead; }

		void setTexture(const std::string path, sf::RenderWindow* const& rWindow);
		inline void setTextureColor(const sf::Color color) { entitySprite.setColor(color); }
		inline void setSize(const float width, const float height) { entityRect.setSize(sf::Vector2f(width, height)); }
		inline void setRectColor(const sf::Color set) { entityRect.setOutlineColor(set); }
		inline void setX(const float set) { entityRect.setPosition(sf::Vector2f(set, entityRect.getPosition().y)); }
		inline void setXVelocity(const float set) { velocity.x = set; }

		inline void deductHealth(const float remove) { hp -= remove; }
		inline void addHealth(const float add) { hp += add; }

		inline void MoveEntity(const float dirX) { velocity.x += accel * dirX; }
		void Jump();
		void createHitRect();

		const bool checkCollision(ss::Entity* const& obj1, ss::Entity* const& obj2);
		const bool checkCollision(sf::FloatRect obj1, sf::FloatRect obj2);

		virtual void Update(const float& deltaTime);
		virtual void Render(sf::RenderTarget* const& target);
	};
}

#endif