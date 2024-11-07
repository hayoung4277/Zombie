#include "stdafx.h"
#include "UiGameOver.h"

UiGameOver::UiGameOver(const std::string& name)
	: GameObject(name)
{
}

void UiGameOver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameOver::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameOver::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiGameOver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiGameOver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiGameOver::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiGameOver::Release()
{
}

void UiGameOver::Reset()
{
	message.setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
	message.setCharacterSize(200.f);
	message.setPosition(FRAMEWORK.GetWindowSizeF() * 0.5f);
	message.setString("GAME OVER!!\n\nPRESS ENTER\n\nTO CONTINUE");
	Utils::SetOrigin(message, Origins::MC);
}

void UiGameOver::Update(float dt)
{
}

void UiGameOver::Draw(sf::RenderWindow& window)
{
	window.draw(message);
}
