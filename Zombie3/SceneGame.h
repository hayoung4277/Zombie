#pragma once
#include "Scene.h"

class Player;
class Zombie;
class Bullet;
class Map;
class TileMap;
class UiHud;
class UiUpgrade;
class UiGameStart;
class UiGameOver;
class SpriteGo;
class ItemMgr;
class Blood;

class SceneGame : public Scene
{
public:
	enum class Status
	{
		Awake,
		Game,
		Upgrade,
		GameOver,
		Pause,
		None,
	};

protected:
	Status currentStatus = Status::Awake;
	Status beforeStatus = Status::None;

	TileMap* map;
	Player* player;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameOver;
	UiGameStart* uiGameStart;
	ItemMgr* itemMgr;

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	std::list<Blood*> bloods;
	ObjectPool<Blood> bloodPool;

	sf::Sprite cursor;

	int score = 0;
	int maxScore = 0;

	int wave;
	bool waveStart;
	bool bufSelected = false;

	int second = 0;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Reset();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateUpgrade(float dt);
	void UpdateGameOver(float dt);
	void UpdatePause(float dt);

	void SpawnZombies(int count);
	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	const std::list<Zombie*>& GetZombieList() const { return zombies; }

	sf::FloatRect GetTileBounds();
	sf::FloatRect GetMovableBounds();

	void OnZombieDie(Zombie* zombie);
	void OnPlayerDie();

	void SetScore(int score);
	void SetHighScore(int score);

	int GetScore() { return score; }
	void BufSelected();

	void SaveHighScore();

	void ReturnBlood(Blood* blood);
	void OnZombieBlood(Zombie* zombie);

	void UpgradeInfo(int i);
};
