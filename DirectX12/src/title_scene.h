#pragma once
#include "frame_work/scene_base.h"
#include "player.h"

class TitleScene :public SceneBase
{
public:
	TitleScene() :SceneBase("TitleScene") {}
	void SceneBase::InitObjectGenerate()override { InitInstantiate<Player>(); };
};