/**
* @file scene_manager.cpp
* @brief シーンを管理するクラスのメンバ関数を定義
* @author 石山　悠
* @date 2019/02/08
*/
#include "scene_manager.h"
#include "../../title_scene.h"
#include "../object/object_list.h"
#include "../sprite/command_list/sprite_command_list.h"

#pragma region public

/**
* @brief シーンを管理するクラスの初期化
* @return 成功したかどうか
*/
HRESULT SceneManager::Init()
{
	HRESULT hr = S_OK;

	current_scene_ptr = std::make_shared<TitleScene>();
	hr = SpriteCommandList::getinstance()->Init();
	if (FAILED(hr))return hr;
	current_scene_ptr->InitObjectGenerate();
	hr = ObjectList::ObjectStart();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief シーンを管理するクラスの更新
* @return 成功したかどうか
*/
HRESULT SceneManager::Update()
{
	HRESULT hr = S_OK;

	hr = SwitchScene();
	if (FAILED(hr))return hr;

	hr = ObjectList::ObjectUpdate();
	if (FAILED(hr))return hr;


	//生成予定のオブジェクトを生成
	hr = ObjectList::ObjectGenerateUpdate();
	if (FAILED(hr))return hr;
	//破棄予定のオブジェクトを破棄
	hr = ObjectList::ObjectDestroyUpdate();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief シーンを管理するクラスの描画
* @return 成功したかどうか
*/
HRESULT SceneManager::Render()
{
	HRESULT hr = S_OK;

	hr = ObjectList::ObjectDraw();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief シーンを管理するクラスの破棄
* @return 成功したかどうか
*/
HRESULT SceneManager::Destroy()
{
	HRESULT hr = S_OK;

	hr = ObjectList::ObjectDestroy();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief シーンの切り替え
*/
HRESULT SceneManager::SwitchScene()
{
	HRESULT hr = S_OK;

	if (!next_scene_ptr)return hr;

	hr = ObjectList::ObjectDestroy();
	if (FAILED(hr))return hr;
	current_scene_ptr = next_scene_ptr;
	next_scene_ptr = nullptr;
	current_scene_ptr->InitObjectGenerate();
	hr = ObjectList::ObjectStart();
	if (FAILED(hr))return hr;

	return hr;
}

std::string SceneManager::GetCurrentSceneName()const
{
	return current_scene_ptr->name;
}

#pragma endregion