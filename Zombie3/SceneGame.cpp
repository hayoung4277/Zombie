#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
#include "UiGameStart.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	map = AddGo(new TileMap("TileMap"));
	player = AddGo(new Player("Player"));
	uiHud = AddGo(new UiHud("UiHud"));
	uiUpgrade = AddGo(new UiUpgrade("UiUpgrade"));
	uiGameOver = AddGo(new UiGameOver("UiGameOver"));
	uiGameStart = AddGo(new UiGameStart("UiGameStart"));
	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	Scene::Enter();

	zombieSpawnArea = map->GetMapBounds();

	uiUpgrade->SetActive(false);
	uiGameOver->SetActive(false);

	SetStatus(Status::Awake);
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);

	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	for (auto blood : bloods)
	{
		RemoveGo(blood);
		bloodPool.Return(blood);
	}
	bloods.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	cursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::L))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SpawnZombies(20);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		uiGameOver->SetActive(!uiGameOver->IsActive());
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

	switch (currentStatus)
	{
	case SceneGame::Status::Awake:
		UpdateAwake(dt);
		break;
	case SceneGame::Status::Game:
		UpdateGame(dt);
		break;
	case SceneGame::Status::GameOver:
		UpdateGameOver(dt);
		break;
	case SceneGame::Status::Pause:
		UpdatePause(dt);
		break;
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveView = window.getView();
	window.setView(uiView);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::UpdateAwake(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SetStatus(Status::Game);
	}
}

void SceneGame::UpdateGame(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SetStatus(Status::Pause);
		return;
	}

	SetTime(second + 163.f * dt);
}

void SceneGame::UpdateGameOver(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SetStatus(Status::Game);
	}
}

void SceneGame::UpdatePause(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SetStatus(Status::Game);
	}
}

void SceneGame::SetStatus(Status newStatus)
{
	Status prevStatus = currentStatus;
	currentStatus = newStatus;

	switch (currentStatus)
	{
	case SceneGame::Status::Awake:
		FRAMEWORK.SetTimeScale(0.f);
		score = 0;
		SetScore(score);
		uiGameStart->SetActive(true);
		break;

	case SceneGame::Status::Game:
		uiGameStart->SetActive(false);
		if (prevStatus == Status::GameOver)
		{
			score = 0;
			second = 0;

			SetScore(score);
			player->Reset();
		}
		SpawnZombies(20);
		FRAMEWORK.SetTimeScale(1.f);
		break;

	case SceneGame::Status::GameOver:
		FRAMEWORK.SetTimeScale(0.f);
		uiGameOver->SetActive(true);
		break;

	case SceneGame::Status::Pause:
		FRAMEWORK.SetTimeScale(0.f);
		break;
	}
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1);
		zombie->SetType(zombieType);

		sf::Vector2f pos = Utils::RandomPointInRect(zombieSpawnArea);
		zombie->SetPosition(pos);

		AddGo(zombie);
	}
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

sf::FloatRect SceneGame::GetTileBounds()
{
	return map->GetGlobalBounds();;
}

sf::FloatRect SceneGame::GetMovableBounds()
{
	return map->GetMapBounds();
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
	SetScore(score + 100);
}

void SceneGame::OnPlayerDie(Player* player)
{
	SetStatus(Status::GameOver);
}

void SceneGame::OnUpgrade(Upgrade up)
{
	uiUpgrade->SetActive(false);
	std::cout << (int)up << std::endl;
}

void SceneGame::SetScore(int score)
{
	this->score = score;
	uiHud->SetScore(this->score);
}

void SceneGame::SetHp(int hp)
{
	this->hp = hp;
	uiHud->SetHp(this->hp, 1.f);
}

void SceneGame::SetTime(int s)
{
	second = s;

	uiHud->SetTime(second);
}
