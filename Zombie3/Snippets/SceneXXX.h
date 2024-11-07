#pragma once

class SceneXXX : public Scene
{
protected:

public:
	SceneXXX();
	virtual ~SceneXXX() = default;

	void Init() override;
	void Release() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
};

