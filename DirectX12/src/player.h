#pragma once
#include "frame_work/object.h"
#include "ball.h"
#include "frame_work/time.h"
#include "frame_work/input.h"
#include <algorithm>
#include "frame_work/text_ui.h"
class Player : public ObjectBase
{
	static constexpr float interval = 1.0f;
	float time = Player::interval;
	std::vector<Object> balls;
	Sprite sprite;

protected:
	HRESULT Start()override
	{
		HRESULT hr = S_OK;

		hr = sprite.Init(L"resources/texture/player.sakitex");
		sprite.SetOrderInLayer(1);
		layer = sprite.GetOrderInLayer();
		transform.set_scale({ 0.1f,0.1f,0.1f });

		return hr;
	}
	HRESULT Update()override
	{
		time += Time::getinstance()->GetElapsedTime();
		if (KeyboardInput::getinstance()->GetKey(DIK_SPACE) && time > interval)
		{
			time = 0.0f;
			balls.push_back(Instantiate<Ball>(transform.get_pos()));
		}
		balls.erase(std::remove_if(balls.begin(), balls.end(), [](Object& obj) {return !(obj); }), balls.end());

		return S_OK;
	}
	HRESULT Draw()override
	{
		HRESULT hr = S_OK;

		hr = sprite.Draw(transform);
		TextUi::getinstance()->DrawString(std::to_wstring(balls.size()), {});

		return hr;
	}
};