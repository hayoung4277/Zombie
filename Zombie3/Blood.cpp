#include "stdafx.h"
#include "Blood.h"
#include "SceneGame.h"

Blood::Blood(const std::string& name)
	: GameObject(name)
{
}

void Blood::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Blood::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Blood::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Blood::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Blood::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Blood::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void Blood::Release()
{
}

void Blood::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId));
	scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetOrigin(Origins::MC);
}

void Blood::Update(float dt)
{
	if (showTime > 3.f)
	{
		showTime = 0;
		scene->ReturnBlood(this);
	}
	showTime += dt;
}

void Blood::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}