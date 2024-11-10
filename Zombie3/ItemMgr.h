#pragma once
#include "GameObject.h"

class Item;
class SceneGame;

class ItemMgr : public GameObject
{
protected:
	sf::FloatRect Rect;

	std::list<Item*> items;
	std::list<Item*> removeList;
	ObjectPool<Item> itemPool;

	SceneGame* scenegame;

	float regenTimer;
	float regenInterval = 10.f;
	int ammoRegenAmount = 5;
	int medRegenAmount = 5;
	bool spawn = true;

public:
	ItemMgr(const std::string& name = "");
	~ItemMgr() = default;

	void Init() override;
	void Release() override;
	void Reset() override;

	void Update(float dt) override;
	void FixedUpdate(float dt);

	void Draw(sf::RenderWindow& window) override;

	void OnItemUsed(Item* item);

	void Spawn(int count, ItemTypes type);
	void ResetSpawn();

	void SetSpawn(bool b);
	void BufAmmoRegen(int rate);
	void BufMedRegen(int rate);
};

