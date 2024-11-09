#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "Zombie.h"
#include "UiHud.h"

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

	hp = maxHp = 100;
	gunAmmo = 7;
	gunMaxAmmo = 50;
	reloadTimer = 0;

	invincible = false;

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };


	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	hp = maxHp = 100;
	gunAmmo = 10;
	gunMaxAmmo = 200;

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
	if (hp == 0)
	{
		return;
	}

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

	shootTimer += dt;

	if (gunAmmo != 0)
	{
		if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
		{
			shootTimer = 0.f;
			Shoot();
		}
	}

	reloadTimer += dt;

	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		if(reloadTimer > reloadDelay)
		{
			if (gunAmmo == 0)
			{
				gunMaxAmmo - gunAmmo;
				Reload();
			}
			else if (gunAmmo != 0)
			{
				gunMaxAmmo - gunUseCount;
				Reload();
			}
			reloadTimer = 0;
		}
	}

	if (invincible == true)
	{
		invincibleTimer += dt;
		if (invincibleDelay < invincibleTimer)
		{
			invincible = false;
		}
	}
}

void Player::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	if (hp == 0)
	{
		sceneGame->OnPlayerDie();
	}
	
	uiHud->SetHp(hp, maxHp);
	uiHud->SetAmmo(gunAmmo, gunMaxAmmo);
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Player::SetUiHud(UiHud* hud)
{
	uiHud = hud;
}

bool Player::IsShoot()
{
	return isShoot;
}

void Player::Shoot()
{
	if(gunAmmo > 0)
	{
		Bullet* bullet = sceneGame->TakeBullet();
		bullet->Fire(position, look, 1000.f, 10);
		gunAmmo--;
		gunUseCount++;
	}
}

void Player::Reload()
{
	gunAmmo = 10;
}

void Player::OnDamage(int d)
{
	if (invincible == false)
	{
		hp = Utils::Clamp(hp - d, 0, maxHp);
		invincible = true;
		invincibleTimer = 0;
	}
}
