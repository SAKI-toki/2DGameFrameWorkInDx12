#pragma once
#include "frame_work/object.h"
#include "frame_work/time.h"
#include <saki/random.h>
#include <saki/math/pi.h>
#include "frame_work/camera.h"

class Ball :public ObjectBase
{
	static constexpr float speed = 3.0f;
	Vec3 dir;
	Sprite sprite;
protected:
	HRESULT Start()override
	{
		HRESULT hr = S_OK;

		this->name = "Ball";
		hr = sprite.Init(L"resources/texture/bullet.sakitex");
		float angle = saki::random(0.0f, saki::PI<float> * 2);
		dir.x = std::cos(angle);
		dir.y = std::sin(angle);
		sprite.SetOrderInLayer(0);
		layer = sprite.GetOrderInLayer();

		return hr;
	}
	HRESULT Update()override
	{
		auto pos = transform.get_pos();
		auto scale = transform.get_scale();
		auto tex_size = sprite.GetSize();
		scale.x *= tex_size.x;
		scale.y *= tex_size.y;
		if (!Camera::getinstance()->IsInCamera(pos, scale))
		{
			DestroyThis();
		}
		else
		{
			pos += dir * speed * Time::getinstance()->GetElapsedTime();
			transform.set_pos(pos);
		}
		return S_OK;
	}

	HRESULT Draw()override
	{
		HRESULT hr = S_OK;

		hr = sprite.Draw(transform);

		return hr;
	}
public:
};