#pragma once
#include <Windows.h>

class IScene
{
public:
	virtual void Initialize();
	virtual void Release();
};