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
enum class Upgrade;

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

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	sf::Sprite cursor;

	int score = 0;
	int maxScore = 0;

	int wave;
	bool waveStart;

	int second = 0;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	void UpdateAwake(float dt);
	void UpdateGame(float dt);
	void UpdateUpgrade(float dt);
	void UpdateGameOver(float dt);
	void UpdatePause(float dt);

	void SetStatus(Status newStatus);

	void SpawnZombies(int count);
	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	const std::list<Zombie*>& GetZombieList() const { return zombies; }

	sf::FloatRect GetTileBounds();
	sf::FloatRect GetMovableBounds();

	void OnZombieDie(Zombie* zombie);
	void OnPlayerDie();

	void OnUpgrade(Upgrade up);

	void SetScore(int score);
	void SetHighScore(int score);
	void SetTime(int s);

	int GetScore() { return score; }

	void SaveHighScore();
};
