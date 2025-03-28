#pragma once
#include <Windows.h>
#include "sprite.h"

namespace MyGame
{
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
		virtual void Render() = 0;
		virtual SceneState& GetSceneState() = 0;
		virtual ~IScene() = default;
	};
}

