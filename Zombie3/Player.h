#pragma once

class SceneGame;

class Player : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 500.f;
	/*int gunAmmo = 10;
	int gunMaxAmmo = 200;
	int gunUseCount = 0;*/

	bool isShoot = false;

	SceneGame* sceneGame;

	/*float shootDelay = 0.5f;
	float shootTimer = 0.f;*/

	int maxHp = 100;
	int hp = 0;

	bool invincible;
	float invincibleTimer;

	sf::FloatRect moveableBounds;

public:
	Player(const std::string& name = "");
	~Player() = default;
	
	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::Sprite GetSprite() { return body; }

	bool IsShoot();
	void Shoot();
	/*void Reload();*/
	/*int GetGunAmmo() { return gunAmmo; }
	int GetGunMaxAmmo() { return gunMaxAmmo; }*/

	void OnDamage(int d);
};
