#include "stdafx.h"
#include "UiUpgrade.h"
#include "SceneGame.h"

UiUpgrade::UiUpgrade(const std::string& name)
	: GameObject(name)
{
}

void UiUpgrade::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiUpgrade::SetRotation(float angle)
{
	rotation = angle;
}

void UiUpgrade::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiUpgrade::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiUpgrade::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiUpgrade::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;
	
	upgrades.resize((int)Upgrade::Count);
	upgrades[0].setString("1-INCREASED RATE OF FIRE");
	upgrades[1].setString("2-INCREASED CLIP SIZE(NEXT RELOAD)");
	upgrades[2].setString("3-INCREASED MAX HEALTH");
	upgrades[3].setString("4-INCREASED RUN SPEED");
	upgrades[4].setString("5-MORE AND BETTER HEALTH PICKUPS");
	upgrades[5].setString("6-MORE AND BETTER AMMO PICKUPS");
}

void UiUpgrade::Release()
{
}

void UiUpgrade::Reset()
{
	background.setTexture(TEXTURE_MGR.Get("graphics/background.png"));
	background.setPosition({ 0.f, 0.f });

	float textSize = 100.f;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");

	int count = (int)Upgrade::Count;
	for (int i = 0; i < count; ++i)
	{
		upgrades[i].setFont(font);
		upgrades[i].setCharacterSize(textSize);
		Utils::SetOrigin(upgrades[i], Origins::ML);
		upgrades[i].setPosition(200.f, (120.f * i + 1) + 200.f);
	}
}

void UiUpgrade::Update(float dt)
{
	
}

void UiUpgrade::FixedUpdate(float dt)
{
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (sceneGame != nullptr)
	{
		sf::Vector2f mousePos = sceneGame->ScreenToUi(InputMgr::GetMousePosition());
		for (int i = 0; i < upgrades.size(); ++i)
		{
			if (upgrades[i].getGlobalBounds().contains(mousePos))
			{
				upgrades[i].setFillColor(sf::Color::Red);
				if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
				{
					sceneGame->OnUpgrade((Upgrade)i);
					return;
				}
			}
			else
			{
				upgrades[i].setFillColor(sf::Color::White);
			}
		}
	}

}

void UiUpgrade::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	for (auto& text : upgrades)
	{
		window.draw(text);
	}
}
