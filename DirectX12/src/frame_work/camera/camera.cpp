/**
* @file camera.cpp
* @brief �J�����N���X�̊֐���`
* @author �ΎR�@�I
* @date 2019/02/08
*/
#include "camera.h"
#include <limits>
#include <saki/compare/is_fit.h>

/**
* @brief �J�����N���X��pimpl�C�f�B�I��
*/
class Camera::Impl
{
public:
	//�߂�����
	static constexpr float NEAR_Z = 0.0f;
	//��������
	static constexpr float FAR_Z = std::numeric_limits<float>::max();
	//�s��
	Matrix view, projection, view_mul_projection;
	//�ʒu
	Vec3 pos{ 0.0f,0.0f,-10.0f };
	//Z����]
	float rot_z{ 0.0f };
	//�J�����̃T�C�Y
	float camera_size{ 1.0f };
	//�s��̍X�V
	void MatrixUpdate();
};

#pragma region public

/**
* @brief �J�����̃R���X�g���N�^
*/
Camera::Camera() :
	pimpl(new Impl{})
{
	pimpl->MatrixUpdate();
}

//�f�X�g���N�^�̃f�t�H���g�w��
Camera::~Camera()noexcept = default;
/**
* @brief �J�����̍X�V
*/
void Camera::Update()
{
	pimpl->MatrixUpdate();
}

/**
* @brief �r���[�s��̎擾
* @return �r���[�s��
*/
Matrix Camera::GetView()const noexcept
{
	return pimpl->view;
}

/**
* @brief �v���W�F�N�V�����s��̎擾
* @return �v���W�F�N�V�����s��
*/
Matrix Camera::GetProjection()const noexcept
{
	return pimpl->projection;
}

/**
* @brief �r���[�s��ƃv���W�F�N�V�����s����������s��̎擾
* @return �r���[�s��ƃv���W�F�N�V�����s����������s��
*/
Matrix Camera::GetViewMulProjection()const noexcept
{
	return pimpl->view_mul_projection;
}

/**
* @brief �ʒu�̃Q�b�^
* @return �ʒu
*/
Vec3 Camera::GetPos()const noexcept
{
	return pimpl->pos;
}

/**
* @brief �ʒu�̃Z�b�^
* @param _pos �V�����J�����̈ʒu
*/
void Camera::SetPos(const Vec3& camera_pos) noexcept
{
	pimpl->pos = camera_pos;
}

/**
* @brief �J������z����]�̃Q�b�^
* @return �J������z����]
*/
float Camera::GetRotZ()const noexcept
{
	return pimpl->rot_z;
}

/**
* @brief �J������z����]�̃Z�b�^
* @param camera_rot_z �V�����J������z����]
*/
void Camera::SetRotZ(const float camera_rot_z) noexcept
{
	pimpl->rot_z = camera_rot_z;
}

/**
* @brief �J�����̃T�C�Y�̃Q�b�^
*/
float Camera::GetSize()const noexcept
{
	return pimpl->camera_size;
}

/**
* @brief �J�����̃T�C�Y�̃Z�b�^
*/
void Camera::SetSize(const float size) noexcept
{
	pimpl->camera_size = size;
}

/**
* @brief �J�����ɓ����Ă��邩�ǂ���
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
* @brief �s��̍X�V
*/
void Camera::Impl::MatrixUpdate()
{
	Vector vec_pos{ pos.x,pos.y,pos.z };
	Vector vec_look_at = DirectX::XMVectorAdd(vec_pos, { 0,0,1 });
	//�r���[�s��
	view = DirectX::XMMatrixLookAtLH(vec_pos, vec_look_at, { 0.0f,1.0f,0.0f });
	//�v���W�F�N�V�����s��
	//2D�p�̃v���W�F�N�V����
	projection = DirectX::XMMatrixOrthographicLH(
		16 * camera_size, 9 * camera_size, NEAR_Z, FAR_Z);
	//�r���[�ƃv���W�F�N�V�����s���������(Z����]���l��)
	view_mul_projection = view * DirectX::XMMatrixRotationRollPitchYaw(0, 0, rot_z) * projection;
}

#pragma endregion