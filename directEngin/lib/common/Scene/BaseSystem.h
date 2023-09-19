#pragma once
#include "SceneManager.h"

/// @brief base class for Systems
class BaseSystem
{

public:

	virtual void onUpdate(SceneManager& scene, float dt) {};

	virtual void onFrame(SceneManager& scene) {};
};

