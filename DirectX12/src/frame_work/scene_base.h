#pragma once
#include "common/d3d12.h"
#include <memory>
#include <utility>
#include <string>
class SceneBase;
using ScenePtrType = std::shared_ptr<SceneBase>;

class SceneBase
{
public:
	SceneBase(const std::string& scene_name) :name(scene_name) {}

	virtual void InitObjectGenerate() = 0;

	virtual ~SceneBase() {}

	std::string name;
};

#include "scene/scene_manager.h"