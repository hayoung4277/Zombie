#pragma once

class SceneGame;
class UiHud;
class Item;

class Player : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 300.f;
	int clip;
	int clipSize;
	int maxAmmo = 100;
	int gunUseCount = 0;

	float reloadTimer = 0;
	float reloadDelay = 1.f;

	bool isShoot = false;

	SceneGame* sceneGame;
	UiHud* uiHud;

	float shootDelay = 0.5f;
	float shootTimer = 0.f;

	int maxHp = 100;
	int hp;

	bool invincible;
	float invincibleTimer;
	float invincibleDelay = 0.5f;

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

	void SetUiHud(UiHud* hud);

	void Shoot();
	void Reload();
	
	void OnDamage(int d);

	void OnItemGet(ItemTypes type, int amount);

	void BufRateOfFire(float rate);
	void BufClipSize(int size);
	void BufMaxHp(int hp);
	void BufSpeed(float speed);
};
