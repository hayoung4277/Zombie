#pragma once

class Player;
class ItemMgr;

class UiUpgrade : public GameObject
{

protected:
	std::vector<sf::Text> text;
	sf::Sprite background;

	Player* player;
	ItemMgr* itemMgr;

public:
	UiUpgrade(const std::string& name = "");
	~UiUpgrade() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void CheckTextClick();

	void Draw(sf::RenderWindow& window) override;
};
