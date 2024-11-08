#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "Zombie.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
}

void Player::Release()
{
}

void Player::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };

	//shootTimer = shootDelay;

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	hp = maxHp = 100;
	//gunAmmo = 50;

	invincible = false;

	moveableBounds = sceneGame->GetMovableBounds();
	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };
}

void Player::Update(float dt)
{
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		Utils::Normailize(direction);
	}

	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseWorldPos - position);

	SetRotation(Utils::Angle(look));
	SetPosition(position + direction * speed * dt);

	/*shootTimer += dt;

	if (gunAmmo != 0)
	{
		if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			shootTimer = 0.f;
			Shoot();
			gunAmmo--;
			gunUseCount++;
		}
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		if (gunAmmo == 0)
		{
			gunMaxAmmo - gunUseCount;
			Reload();
		}
		else if (gunAmmo != 0)
		{
			gunMaxAmmo - gunUseCount;
			Reload();
		}
	}*/
}

void Player::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	const auto& list = sceneGame->GetZombieList();
	for (auto zombie : list)
	{
		if (!zombie->IsActive())
			continue;

		sf::FloatRect bounds = GetGlobalBounds();
		sf::FloatRect zombieBounds = zombie->GetGlobalBounds();

		if (bounds.intersects(zombieBounds))
		{
			HitBox& boxZombie = zombie->GetHitBox();
			if (Utils::CheckCollision(hitbox, boxZombie))
			{

			}
			break;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

bool Player::IsShoot()
{
	return isShoot;
}

void Player::Shoot()
{
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
}

//void Player::Reload()
//{
//	gunAmmo = 10;
//}

void Player::OnDamage(int d)
{
	if (invincible == false)
	{
		hp = Utils::Clamp(hp - d, 0, maxHp);
		invincible = true;
		invincibleTimer = 0;
	}
}
