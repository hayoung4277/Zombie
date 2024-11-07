#include "stdafx.h"
#include "UiGameStart.h"

UiGameStart::UiGameStart(const std::string& name)
	:GameObject(name)
{
}

void UiGameStart::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameStart::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameStart::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiGameStart::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiGameStart::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiGameStart::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiGameStart::Release()
{
}

void UiGameStart::Reset()
{
	message.setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
	message.setCharacterSize(200.f);
	message.setPosition(FRAMEWORK.GetWindowSizeF() * 0.5f);
	message.setString("PRESS ENTER TO START");
	Utils::SetOrigin(message, Origins::MC);
}

void UiGameStart::Update(float dt)
{
}

void UiGameStart::Draw(sf::RenderWindow& window)
{
	window.draw(message);
}
