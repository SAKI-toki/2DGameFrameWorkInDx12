/**
* @file keyboard_input.cpp
* @brief �L�[�{�[�h�̃C���v�b�g�N���X�̃����o�֐����`
* @author �ΎR�@�I
* @date 2018/12/20
*/
#include "keyboard_input.h"
#include "../../common/message_box.h"
#include "..//..//common/alias.h"

/**
* @brief �L�[�{�[�h��pimpl�C�f�B�I��
*/
class KeyboardInput::Impl
{
public:
	//�e�f�o�C�X
	ComPtr<IDirectInputDevice8> key_device;

	//�L�[�̏��
	BYTE old_diks[256]{};
	BYTE current_diks[256]{};

	void Destroy();
};

#pragma region public

/**
* @brief �L�[�{�[�h�̃R���X�g���N�^
*/
KeyboardInput::KeyboardInput() :
	pimpl(new Impl{})
{}

/**
* @brief �L�[�{�[�h�̃f�X�g���N�^
*/
KeyboardInput::~KeyboardInput()noexcept
{
	pimpl->Destroy();
}

/**
* @brief �L�[�{�[�h�̏�����
* @param hWnd �E�B���h�E�n���h��
* @return ����ɏ������������ǂ���
*/
HRESULT KeyboardInput::KeyboardInit(HWND hWnd)
{
	HRESULT hr = S_OK;
	ComPtr<IDirectInput8> dinput;
	hr = DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		reinterpret_cast<void**>(dinput.GetAddressOf()), nullptr);
	if (FAILED(hr))
	{
		Comment(L"DirectInput�̍쐬�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = dinput->CreateDevice(GUID_SysKeyboard, pimpl->key_device.GetAddressOf(), nullptr);
	if (FAILED(hr))
	{
		Comment(L"�f�o�C�X�̍쐬�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = pimpl->key_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		Comment(L"�f�[�^�t�H�[�}�b�g�̃Z�b�g�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = pimpl->key_device->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr))
	{
		Comment(L"�������x���̃Z�b�g�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}

	ResetKeyInfo();

	return hr;
}

/**
* @brief �L�[�{�[�h�̍X�V
* @return ����ɍX�V�������ǂ���
*/
HRESULT KeyboardInput::Update()
{
	HRESULT hr = S_OK;
	hr = pimpl->key_device->Acquire();
	if (FAILED(hr))
	{
		Comment(L"�f�o�C�X�̎擾�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::Update");
		return hr;
	}

	//�O�t���[���̏���old�Ɉڂ�
	memcpy(reinterpret_cast<void*>(pimpl->old_diks), 
		reinterpret_cast<void*>(pimpl->current_diks), 
		sizeof(pimpl->old_diks));

	hr = pimpl->key_device->GetDeviceState(sizeof(pimpl->current_diks), static_cast<void*>(pimpl->current_diks));
	if (FAILED(hr))
	{
		Comment(L"�f�o�C�X���̎擾�Ɏ��s",
			L"keyboard_input.cpp/KeyboardInput::Update");
		return hr;
	}
	return hr;
}

/**
* @brief �w�肳�ꂽ�L�[�������Ă��邩�ǂ���
* @param key �L�[�ԍ�
* return �L�[�������Ă��邩�ǂ���
*/
bool KeyboardInput::GetKey(int key)const noexcept
{
	return static_cast<bool>(pimpl->current_diks[key] & 0x80);
}

/**
* @brief �w�肳�ꂽ�L�[�����������ǂ���
* @param key �L�[�ԍ�
* return �L�[�����������ǂ���
*/
bool KeyboardInput::GetKeyDown(int key)const noexcept
{
	return static_cast<bool>(pimpl->current_diks[key] & 0x80) &&
		!static_cast<bool>(pimpl->old_diks[key] & 0x80);
}

/**
* @brief �w�肳�ꂽ�L�[�𗣂������ǂ���
* @param key �L�[�ԍ�
* return �L�[�𗣂������ǂ���
*/
bool KeyboardInput::GetKeyUp(int key)const noexcept
{
	return !static_cast<bool>(pimpl->current_diks[key] & 0x80) &&
		static_cast<bool>(pimpl->old_diks[key] & 0x80);
}

/**
* @brief �L�[�������Z�b�g
*/
void KeyboardInput::ResetKeyInfo()
{
	memset(reinterpret_cast<void*>(pimpl->current_diks), 0, sizeof(pimpl->current_diks));
	memset(reinterpret_cast<void*>(pimpl->old_diks), 0, sizeof(pimpl->old_diks));
}

#pragma endregion

#pragma region pimpl

/**
* @brief �L�[�{�[�h�̔j��
*/
void KeyboardInput::Impl::Destroy()
{
	// �L�[�{�[�h�f�o�C�X�ւ̃A�N�Z�X���J��
	if (key_device)
		key_device->Unacquire();
}

#pragma endregion
