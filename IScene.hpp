#pragma once
#include <Windows.h>
#include "sprite.h"
#define DRAW_CALL_STR Engine::GetInstance()->GetConsoleRenderer()->AddStringDrawCall
#define DRAW_CALL_SPR Engine::GetInstance()->GetConsoleRenderer()->AddSpriteDrawCall

enum SceneState
{
	None = 0,
	Menu,
	Play,
	End
};

class IScene
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void CreateDrawCall() = 0;
	virtual SceneState& GetSceneState() = 0;

	virtual ~IScene() = default;
};