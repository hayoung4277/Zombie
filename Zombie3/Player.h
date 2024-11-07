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
	int gunAmmo = 10;

	SceneGame* sceneGame;

	float shootDelay = 0.5f;
	float shootTimer = 0.f;

	int hp = 0;

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

	void Shoot();
	void Reload();

	void OnDamage(int d);
};
