/**
* @file sprite.h
* @brief スプライトクラス
* @author 石山　悠
* @date 2019/02/08
*/
#pragma once
#include "../common/d3d12.h"
#include "../common/alias.h"
#include <memory>
#include <string>
#include "../command_list/Execute/execute_command_list.h"

/**
* @brief スプライトクラス
*/
class Sprite
{
	class Impl;
	std::unique_ptr<Impl> pimpl;
public:
	Sprite();
	~Sprite()noexcept;
	Sprite(Sprite&&)noexcept;
	Sprite& operator=(Sprite&&)noexcept;
	HRESULT Init(const std::wstring&);
	HRESULT UpdateColor(const Float4& color);
	HRESULT Draw(const Transform& transform);
	Vec2 GetSize()const noexcept;
	int GetOrderInLayer()const noexcept;
	void SetOrderInLayer(const int)noexcept;
};