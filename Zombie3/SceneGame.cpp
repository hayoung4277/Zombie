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
#include "ItemMgr.h"
#include "Blood.h"
#include <fstream>

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
	itemMgr = AddGo(new ItemMgr("ItemMgr"));
	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	waveStart = true;

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	itemMgr->SetActive(true);

	score = 0;
	wave = 1;

	uiHud->SetWave(wave);
	uiGameOver->SetActive(false);
	uiUpgrade->SetActive(false);
	player->SetUiHud(uiHud);

	zombieSpawnArea = map->GetMapBounds();
	currentStatus = Status::Awake;
	beforeStatus = Status::None;

	Scene::Enter();
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

void SceneGame::Reset()
{
	score = 0;
	wave = 1;
	waveStart = true;
	beforeStatus = Status::None;
	currentStatus = Status::Game;


	player->Reset();
	for (auto zombie : zombies) {
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets) {
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	uiGameOver->SetActive(false);
	uiUpgrade->SetActive(false);
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	itemMgr->ResetSpawn();
	itemMgr->SetSpawn(true);

	uiHud->SetWave(wave);
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
		if (beforeStatus == Status::Upgrade)
		{
			uiHud->SetWave(wave);
			waveStart = true;
			bufSelected = false;
			uiUpgrade->SetActive(false);
			beforeStatus = Status::None;
		}
		UpdateGame(dt);
		break;
	case SceneGame::Status::Upgrade:
		FRAMEWORK.SetTimeScale(0.f);
		
		if (beforeStatus == Status::Game)
		{
			bufSelected = false;
			uiUpgrade->SetActive(true);
			beforeStatus = Status::None;
		}
		if (bufSelected == true)
		{
			bufSelected = false;
			uiUpgrade->SetActive(false);
			itemMgr->ResetSpawn();
			beforeStatus = Status::Upgrade;
			currentStatus = Status::Game;
		}
		UpdateUpgrade(dt);
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
	FRAMEWORK.SetTimeScale(0.f);
	score = 0;
	SetScore(score);
	uiGameStart->SetActive(true);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		beforeStatus = Status::Awake;
		currentStatus = Status::Game;
	}
}

void SceneGame::UpdateGame(float dt)
{
	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		currentStatus = Status::Pause;
		beforeStatus = Status::Game;
		return;
	}

	uiGameStart->SetActive(false);
	uiUpgrade->SetActive(false);
	uiGameOver->SetActive(false);
	if (beforeStatus == Status::GameOver)
	{
		score = 0;
		second = 0;

		SetScore(score);
		player->Reset();
	}
	FRAMEWORK.SetTimeScale(1.f);

	if (waveStart == true) {
		SpawnZombies(10 + 10 * wave);
		waveStart = false;
	}
	uiHud->SetScore(score);
	uiHud->SetZombieCount(zombies.size());
	uiHud->SetWave(wave);
	if (zombies.size() == 0) {
		waveStart = true;
		wave++;
	}

	if (GetScore() != 0 && GetScore() % 1000 == 0)
	{
		currentStatus = Status::Upgrade;
		beforeStatus = Status::Game;
	}
}

void SceneGame::UpdateUpgrade(float dt)
{
	if (beforeStatus == Status::Game && InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		currentStatus = Status::Game;
		beforeStatus = Status::Upgrade;
	}
}

void SceneGame::UpdateGameOver(float dt)
{
	FRAMEWORK.SetTimeScale(0.f);
	uiGameOver->SetActive(true);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		currentStatus = Status::Game;
		beforeStatus = Status::GameOver;
	}
	SaveHighScore();
}

void SceneGame::UpdatePause(float dt)
{
	FRAMEWORK.SetTimeScale(0.f);

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		currentStatus = Status::Game;
		beforeStatus = Status::Pause;
	}
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		AddGo(zombie);

		zombie->SetType((Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1));
		auto tilebounds = map->GetGlobalBounds();
		zombie->SetPosition({ Utils::RandomRange(tilebounds.left, tilebounds.left + tilebounds.width), Utils::RandomRange(tilebounds.top, tilebounds.top + tilebounds.height) });
		
		/*Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1);
		zombie->SetType(zombieType);

		sf::Vector2f pos = Utils::RandomPointInRect(zombieSpawnArea);
		zombie->SetPosition(pos);*/

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
	OnZombieBlood(zombie);
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
	SetScore(score + 100);
	if (maxScore < score)
	{
		maxScore = score;
	}
	SetHighScore(maxScore);
}

void SceneGame::OnPlayerDie()
{
	currentStatus = Status::GameOver;
	beforeStatus = Status::Game;
}

void SceneGame::SetScore(int score)
{
	this->score = score;
	uiHud->SetScore(this->score);
}

void SceneGame::SetHighScore(int score)
{
	maxScore = score;
	uiHud->SetHiScore(maxScore);
}

void SceneGame::BufSelected()
{
	bufSelected = true;
}

void SceneGame::SaveHighScore()
{
	/*std::ofstream os("HighScore.dat", std::ofstream::binary);
	if (!os)
	{
		std::cerr << "Err!" << std::endl;
		exit(1);
	}

	is.write((char*)maxScore, sizeof(maxScore));
	is.close();*/
}

void SceneGame::ReturnBlood(Blood* blood)
{
	RemoveGo(blood);
	bloodPool.Return(blood);
	bloods.remove(blood);
}

void SceneGame::OnZombieBlood(Zombie* zombie)
{
	Blood* blood = bloodPool.Take();
	bloods.push_back(blood);

	sf::Vector2f pos = zombie->GetPosition();
	blood->SetPosition(pos);

	AddGo(blood);
}

void SceneGame::UpgradeInfo(int i)
{
	switch (i)
	{
	case 0:
	{
		player->BufRateOfFire(0.02f);
		break;
	}
	case 1:
	{
		player->BufClipSize(100);
		break;
	}
	case 2:
	{
		player->BufMaxHp(50);
		break;
	}
	case 3:
		player->BufSpeed(20.f);
		break;
	case 4:
		itemMgr->BufMedRegen(1);
		break;
	case 5:
		itemMgr->BufAmmoRegen(1);
		break;
	}

	if(InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		beforeStatus = Status::Upgrade;
		currentStatus = Status::Game;
	}
}
