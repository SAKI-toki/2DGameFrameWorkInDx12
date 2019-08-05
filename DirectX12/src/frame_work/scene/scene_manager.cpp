/**
* @file scene_manager.cpp
* @brief �V�[�����Ǘ�����N���X�̃����o�֐����`
* @author �ΎR�@�I
* @date 2019/02/08
*/
#include "scene_manager.h"
#include "../../title_scene.h"
#include "../object/object_list.h"
#include "../sprite/command_list/sprite_command_list.h"

#pragma region public

/**
* @brief �V�[�����Ǘ�����N���X�̏�����
* @return �����������ǂ���
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
* @brief �V�[�����Ǘ�����N���X�̍X�V
* @return �����������ǂ���
*/
HRESULT SceneManager::Update()
{
	HRESULT hr = S_OK;

	hr = SwitchScene();
	if (FAILED(hr))return hr;

	hr = ObjectList::ObjectUpdate();
	if (FAILED(hr))return hr;


	//�����\��̃I�u�W�F�N�g�𐶐�
	hr = ObjectList::ObjectGenerateUpdate();
	if (FAILED(hr))return hr;
	//�j���\��̃I�u�W�F�N�g��j��
	hr = ObjectList::ObjectDestroyUpdate();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief �V�[�����Ǘ�����N���X�̕`��
* @return �����������ǂ���
*/
HRESULT SceneManager::Render()
{
	HRESULT hr = S_OK;

	hr = ObjectList::ObjectDraw();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief �V�[�����Ǘ�����N���X�̔j��
* @return �����������ǂ���
*/
HRESULT SceneManager::Destroy()
{
	HRESULT hr = S_OK;

	hr = ObjectList::ObjectDestroy();
	if (FAILED(hr))return hr;

	return hr;
}

/**
* @brief �V�[���̐؂�ւ�
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