#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"

Zombie::Zombie(const std::string& name) : GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Zombie::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Zombie::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Zombie::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Zombie::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Zombie::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(type);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureZombieId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });

	attackTimer = 0;
}

void Zombie::Update(float dt)
{
	if (player != nullptr && Utils::Distance(position, player->GetPosition()) > 10)
	{
		direction = Utils::GetNormal(player->GetPosition() - position);
		SetRotation(Utils::Angle(direction));
		SetPosition(position + direction * speed * dt);
	}

	hitbox.UpdateTr(body, GetLocalBounds());
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}

void Zombie::SetType(Types type)
{
	this->type = type;
	switch (this->type)
	{
	case Types::Bloater:
		textureZombieId = "graphics/bloater.png";
		maxHp = 50;
		speed = 100.f;
		damage = 20;
		break;
	case Types::Chaser:
		textureZombieId = "graphics/chaser.png";
		maxHp = 20;
		speed = 75.f;
		damage = 25;
		break;
	case Types::Crawler:
		textureZombieId = "graphics/crawler.png";
		maxHp = 10;
		speed = 50.f;
		damage = 30;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(textureZombieId), true);
	hp = maxHp;
}

void Zombie::OnDamage(int d, float dt)
{
	hp -= d;
	if (hp <= 0 && sceneGame != nullptr)
	{
		sceneGame->OnZombieDie(this);

		/*body.setTexture(TEXTURE_MGR.Get(textureBloodId));
		bloodTimer = 0;
		bloodTimer += dt;
		if(bloodTimer > bloodDuration)
		{
			sceneGame->OnZombieDie(this);
		}*/
	}
}

void Zombie::AttackUpdate(float dt)
{
	attackTimer += dt;
	sf::Sprite playerSprite = player->GetSprite();
	if (attackTimer > attackInterval && Utils::CheckCollision(body, playerSprite))
	{
		player->OnDamage(damage);
		attackTimer = 0;
	}
}
