#pragma once
#include "IScene.hpp"

class MenuScene : public IScene
{
	virtual void Initialize();

	virtual void Update();

	virtual void Render();

	virtual void Release();
};