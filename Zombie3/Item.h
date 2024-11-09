#pragma once
#include "GameObject.h"

class Player;

class Item : public GameObject
{
protected:
	sf::Sprite body;
	std::string texturId;
	Player* player;

	int ammo, health;

	ItemTypes type;

public:
	Item(const std::string& name = "", const std::string& textureId = "");
	~Item() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt)override;
	void Draw(sf::RenderWindow& window) override;

	//void SetItemManager(ItemManager* address);
	void Set(ItemTypes type);

};

