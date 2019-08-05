/**
* @file scene_manager.h
* @brief �V�[�����Ǘ�����N���X
* @author �ΎR�@�I
* @date 2019/02/08
*/
#pragma once
#include "../common/d3d12.h"
#include "../scene_base.h"
#include <type_traits>
#include <saki/singleton.h>
#include <memory>
#include <saki/type_traits/enable_if_nullptr.h>

/**
* @brief �V�[�����Ǘ�����N���X
*/
class SceneManager :public saki::singleton<SceneManager>
{
	ScenePtrType current_scene_ptr;
	ScenePtrType next_scene_ptr = nullptr;
public:
	HRESULT Init();
	HRESULT Update();
	HRESULT Render();
	HRESULT Destroy();
	HRESULT SwitchScene();
	std::string GetCurrentSceneName()const;
	/**
	* @brief �V�[���̐؂�ւ��\��
	*/
	template<typename T,
		saki::enable_if_nullptr_t<std::is_base_of_v<SceneBase, T>> = nullptr>
		void LoadScene()
	{
		next_scene_ptr = std::make_shared<T>();
	}
};