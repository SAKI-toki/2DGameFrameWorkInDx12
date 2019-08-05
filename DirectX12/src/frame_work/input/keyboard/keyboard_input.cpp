/**
* @file keyboard_input.cpp
* @brief キーボードのインプットクラスのメンバ関数を定義
* @author 石山　悠
* @date 2018/12/20
*/
#include "keyboard_input.h"
#include "../../common/message_box.h"
#include "..//..//common/alias.h"

/**
* @brief キーボードのpimplイディオム
*/
class KeyboardInput::Impl
{
public:
	//各デバイス
	ComPtr<IDirectInputDevice8> key_device;

	//キーの状態
	BYTE old_diks[256]{};
	BYTE current_diks[256]{};

	void Destroy();
};

#pragma region public

/**
* @brief キーボードのコンストラクタ
*/
KeyboardInput::KeyboardInput() :
	pimpl(new Impl{})
{}

/**
* @brief キーボードのデストラクタ
*/
KeyboardInput::~KeyboardInput()noexcept
{
	pimpl->Destroy();
}

/**
* @brief キーボードの初期化
* @param hWnd ウィンドウハンドラ
* @return 正常に初期化したかどうか
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
		Comment(L"DirectInputの作成に失敗",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = dinput->CreateDevice(GUID_SysKeyboard, pimpl->key_device.GetAddressOf(), nullptr);
	if (FAILED(hr))
	{
		Comment(L"デバイスの作成に失敗",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = pimpl->key_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		Comment(L"データフォーマットのセットに失敗",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}
	hr = pimpl->key_device->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr))
	{
		Comment(L"協調レベルのセットに失敗",
			L"keyboard_input.cpp/KeyboardInput::KeyboardInit");
		return hr;
	}

	ResetKeyInfo();

	return hr;
}

/**
* @brief キーボードの更新
* @return 正常に更新したかどうか
*/
HRESULT KeyboardInput::Update()
{
	HRESULT hr = S_OK;
	hr = pimpl->key_device->Acquire();
	if (FAILED(hr))
	{
		Comment(L"デバイスの取得に失敗",
			L"keyboard_input.cpp/KeyboardInput::Update");
		return hr;
	}

	//前フレームの情報をoldに移す
	memcpy(reinterpret_cast<void*>(pimpl->old_diks), 
		reinterpret_cast<void*>(pimpl->current_diks), 
		sizeof(pimpl->old_diks));

	hr = pimpl->key_device->GetDeviceState(sizeof(pimpl->current_diks), static_cast<void*>(pimpl->current_diks));
	if (FAILED(hr))
	{
		Comment(L"デバイス情報の取得に失敗",
			L"keyboard_input.cpp/KeyboardInput::Update");
		return hr;
	}
	return hr;
}

/**
* @brief 指定されたキーを押しているかどうか
* @param key キー番号
* return キーを押しているかどうか
*/
bool KeyboardInput::GetKey(int key)const noexcept
{
	return static_cast<bool>(pimpl->current_diks[key] & 0x80);
}

/**
* @brief 指定されたキーを押したかどうか
* @param key キー番号
* return キーを押したかどうか
*/
bool KeyboardInput::GetKeyDown(int key)const noexcept
{
	return static_cast<bool>(pimpl->current_diks[key] & 0x80) &&
		!static_cast<bool>(pimpl->old_diks[key] & 0x80);
}

/**
* @brief 指定されたキーを離したかどうか
* @param key キー番号
* return キーを離したかどうか
*/
bool KeyboardInput::GetKeyUp(int key)const noexcept
{
	return !static_cast<bool>(pimpl->current_diks[key] & 0x80) &&
		static_cast<bool>(pimpl->old_diks[key] & 0x80);
}

/**
* @brief キー情報をリセット
*/
void KeyboardInput::ResetKeyInfo()
{
	memset(reinterpret_cast<void*>(pimpl->current_diks), 0, sizeof(pimpl->current_diks));
	memset(reinterpret_cast<void*>(pimpl->old_diks), 0, sizeof(pimpl->old_diks));
}

#pragma endregion

#pragma region pimpl

/**
* @brief キーボードの破棄
*/
void KeyboardInput::Impl::Destroy()
{
	// キーボードデバイスへのアクセス権開放
	if (key_device)
		key_device->Unacquire();
}

#pragma endregion
