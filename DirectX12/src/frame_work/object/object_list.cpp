#include "object_list.h"
#include <algorithm>

/**
* @brief �I�u�W�F�N�g�̃X�^�[�g
*/
HRESULT ObjectList::ObjectStart()
{
	return ExecuteObjects([](auto&& obj)
		{return obj->Start(); });
}

/**
* @brief �I�u�W�F�N�g�̍X�V
*/
HRESULT ObjectList::ObjectUpdate()
{
	HRESULT hr = S_OK;

	hr = ExecuteObjects([](auto&& obj)
		{return obj->Update(); });

	return hr;
}

/**
* @brief �I�u�W�F�N�g�̕`��
*/
HRESULT ObjectList::ObjectDraw()
{
	HRESULT hr = S_OK;

	std::vector<Object> sort_draw_objects(objects);
	std::sort(sort_draw_objects.begin(), sort_draw_objects.end(), [](Object& lhs, Object& rhs)
		{return lhs->layer < rhs->layer; });
	for (auto&& obj : sort_draw_objects)
	{
		if (!obj)continue;
		hr = obj->Draw();
		if (FAILED(hr))return hr;
	}
	return hr;
}

/**
* @brief �I�u�W�F�N�g�̔j��
*/
HRESULT ObjectList::ObjectDestroy()
{
	HRESULT hr = S_OK;

	hr = ExecuteObjects([](auto&& obj)
		{return obj->OnDestroy(); });

	objects.clear();
	generate_objects.clear();

	return hr;
}

/**
* @brief �I�u�W�F�N�g�̔j���X�V
*/
HRESULT ObjectList::ObjectDestroyUpdate()
{
	HRESULT hr = S_OK;

	//�t���O�������Ă���C�e���[�^�[��remove����
	auto remove_itr = std::remove_if(objects.begin(), objects.end(),
		[](auto&& obj) {return obj->destroy_flg; });

	for (auto itr = remove_itr; itr != objects.end(); ++itr)
	{
		//�j�����̏��������s
		hr = (*itr)->OnDestroy();
		if (FAILED(hr))return hr;
	}

	//�폜
	objects.erase(remove_itr, objects.end());

	return hr;
}

/**
* @brief �I�u�W�F�N�g�̐����X�V
*/
HRESULT ObjectList::ObjectGenerateUpdate()
{
	HRESULT hr = S_OK;

	if (!generate_objects.empty())
	{
		std::copy(generate_objects.begin(),
			generate_objects.end(),
			std::back_inserter(objects));

		generate_objects.clear();
	}

	return hr;
}

/**
* @brief �I�u�W�F�N�g�𖼑O����T��
*/
Object ObjectList::Find(const std::string& name)
{
	for (auto&& obj : objects)
	{
		if (obj->name == name)return obj;
	}
	for (auto&& obj : generate_objects)
	{
		if (obj->name == name)return obj;
	}
	return null_object;
}
