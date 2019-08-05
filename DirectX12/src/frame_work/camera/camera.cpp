/**
* @file camera.cpp
* @brief カメラクラスの関数定義
* @author 石山　悠
* @date 2019/02/08
*/
#include "camera.h"
#include <limits>
#include <saki/compare/is_fit.h>

/**
* @brief カメラクラスのpimplイディオム
*/
class Camera::Impl
{
public:
	//近い距離
	static constexpr float NEAR_Z = 0.0f;
	//遠い距離
	static constexpr float FAR_Z = std::numeric_limits<float>::max();
	//行列
	Matrix view, projection, view_mul_projection;
	//位置
	Vec3 pos{ 0.0f,0.0f,-10.0f };
	//Z軸回転
	float rot_z{ 0.0f };
	//カメラのサイズ
	float camera_size{ 1.0f };
	//行列の更新
	void MatrixUpdate();
};

#pragma region public

/**
* @brief カメラのコンストラクタ
*/
Camera::Camera() :
	pimpl(new Impl{})
{
	pimpl->MatrixUpdate();
}

//デストラクタのデフォルト指定
Camera::~Camera()noexcept = default;
/**
* @brief カメラの更新
*/
void Camera::Update()
{
	pimpl->MatrixUpdate();
}

/**
* @brief ビュー行列の取得
* @return ビュー行列
*/
Matrix Camera::GetView()const noexcept
{
	return pimpl->view;
}

/**
* @brief プロジェクション行列の取得
* @return プロジェクション行列
*/
Matrix Camera::GetProjection()const noexcept
{
	return pimpl->projection;
}

/**
* @brief ビュー行列とプロジェクション行列をかけた行列の取得
* @return ビュー行列とプロジェクション行列をかけた行列
*/
Matrix Camera::GetViewMulProjection()const noexcept
{
	return pimpl->view_mul_projection;
}

/**
* @brief 位置のゲッタ
* @return 位置
*/
Vec3 Camera::GetPos()const noexcept
{
	return pimpl->pos;
}

/**
* @brief 位置のセッタ
* @param _pos 新しいカメラの位置
*/
void Camera::SetPos(const Vec3& camera_pos) noexcept
{
	pimpl->pos = camera_pos;
}

/**
* @brief カメラのz軸回転のゲッタ
* @return カメラのz軸回転
*/
float Camera::GetRotZ()const noexcept
{
	return pimpl->rot_z;
}

/**
* @brief カメラのz軸回転のセッタ
* @param camera_rot_z 新しいカメラのz軸回転
*/
void Camera::SetRotZ(const float camera_rot_z) noexcept
{
	pimpl->rot_z = camera_rot_z;
}

/**
* @brief カメラのサイズのゲッタ
*/
float Camera::GetSize()const noexcept
{
	return pimpl->camera_size;
}

/**
* @brief カメラのサイズのセッタ
*/
void Camera::SetSize(const float size) noexcept
{
	pimpl->camera_size = size;
}

/**
* @brief カメラに入っているかどうか
*/
bool Camera::IsInCamera(const Vec3& check_pos, const Vec3& size)
{
	return saki::is_fit(check_pos.x,
		pimpl->pos.x - 16 * pimpl->camera_size / 2 - size.x / 2,
		pimpl->pos.x + 16 * pimpl->camera_size / 2 + size.x / 2) &&
		saki::is_fit(check_pos.y,
			pimpl->pos.y - 9 * pimpl->camera_size / 2 - size.y / 2,
			pimpl->pos.y + 9 * pimpl->camera_size / 2 + size.y / 2);
}

#pragma endregion

#pragma region pimpl

/**
* @brief 行列の更新
*/
void Camera::Impl::MatrixUpdate()
{
	Vector vec_pos{ pos.x,pos.y,pos.z };
	Vector vec_look_at = DirectX::XMVectorAdd(vec_pos, { 0,0,1 });
	//ビュー行列
	view = DirectX::XMMatrixLookAtLH(vec_pos, vec_look_at, { 0.0f,1.0f,0.0f });
	//プロジェクション行列
	//2D用のプロジェクション
	projection = DirectX::XMMatrixOrthographicLH(
		16 * camera_size, 9 * camera_size, NEAR_Z, FAR_Z);
	//ビューとプロジェクション行列をかける(Z軸回転を考慮)
	view_mul_projection = view * DirectX::XMMatrixRotationRollPitchYaw(0, 0, rot_z) * projection;
}

#pragma endregion