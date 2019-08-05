#pragma once
#include "frame_work/scene_base.h"

class GameScene :public SceneBase
{
public:
	GameScene() :SceneBase("GameScene") {}
	void SceneBase::InitObjectGenerate()override {};
};