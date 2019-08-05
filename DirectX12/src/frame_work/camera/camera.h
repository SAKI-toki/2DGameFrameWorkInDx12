/**
* @file camera.h
* @brief カメラクラス
* @author 石山　悠
* @date 2019/02/08
*/
#pragma once
#include "../common/alias.h"
#include <saki/singleton.h>
#include <memory>

/**
* @brief カメラクラス
*/
class Camera :public saki::singleton<Camera>
{
	class Impl;
	std::unique_ptr<Impl> pimpl;
public:
	Camera();
	~Camera()noexcept;
	void Update();
	Matrix GetView()const noexcept;
	Matrix GetProjection()const noexcept;
	Matrix GetViewMulProjection()const noexcept;
	Vec3 GetPos()const noexcept;
	void SetPos(const Vec3&) noexcept;
	float GetRotZ()const noexcept;
	void SetRotZ(const float) noexcept;
	float GetSize()const noexcept;
	void SetSize(const float) noexcept;
	bool IsInCamera(const Vec3&, const Vec3&);
};