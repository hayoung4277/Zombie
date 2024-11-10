#include "stdafx.h"
#include "Item.h"
#include "ItemMgr.h"
#include "SceneGame.h"

ItemMgr::ItemMgr(const std::string& name)
	:GameObject(name)
{
}

void ItemMgr::Init()
{
	scenegame = (SceneGame*)(SCENE_MGR.GetCurrentScene());
}

void ItemMgr::Release()
{
	for (auto item : items)
	{
		itemPool.Return(item);
	}
	items.clear();
}

void ItemMgr::Reset()
{
	for (auto item : items)
	{
		item->Reset();
	}
}

void ItemMgr::Update(float dt)
{
	regenTimer += dt;
	if (regenTimer > regenInterval && spawn == true)
	{
		Spawn(ammoRegenAmount, ItemTypes::Ammo);
		Spawn(medRegenAmount, ItemTypes::MediKit);
		regenTimer = 0.f;
	}
	for (auto item : items)
	{
		item->Update(dt);
	}
}

void ItemMgr::FixedUpdate(float dt)
{
	for (auto item : items)
	{
		item->FixedUpdate(dt);
	}
	for (int i = 0; i < removeList.size(); i++)
	{
		items.remove(removeList.front());
		removeList.pop_front();
	}
}

void ItemMgr::Draw(sf::RenderWindow& window)
{
	for (auto item : items)
	{
		item->Draw(window);
	}
}

void ItemMgr::OnItemUsed(Item* item)
{
	itemPool.Return(item);
	removeList.push_back(item);
}

void ItemMgr::Spawn(int count, ItemTypes type)
{
	sf::FloatRect playArea = scenegame->GetTileBounds();
	for (int i = 0; i < count; i++) {
		auto obj = itemPool.Take();
		obj->Set(type);
		sf::Vector2f pos;
		pos.x = playArea.left + Utils::RandomRange(0.f, playArea.width);
		pos.y = playArea.top + Utils::RandomRange(0.f, playArea.height);
		obj->SetPosition(pos);
		obj->SetItemManager(this);
		items.push_back(obj);
	}
}

void ItemMgr::ResetSpawn()
{
	auto it = items.begin();
	while (it != items.end())
	{
		itemPool.Return(*it);
		it = items.erase(it);
	}
	regenTimer = 0;
}

void ItemMgr::SetSpawn(bool b)
{
	spawn = b;
}

void ItemMgr::BufAmmoRegen(int rate)
{
	ammoRegenAmount += rate;
}

void ItemMgr::BufMedRegen(int rate)
{
	medRegenAmount += rate;
}
