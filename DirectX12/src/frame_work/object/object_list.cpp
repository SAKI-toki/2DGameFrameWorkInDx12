#include "object_list.h"
#include <algorithm>

/**
* @brief オブジェクトのスタート
*/
HRESULT ObjectList::ObjectStart()
{
	return ExecuteObjects([](auto&& obj)
		{return obj->Start(); });
}

/**
* @brief オブジェクトの更新
*/
HRESULT ObjectList::ObjectUpdate()
{
	HRESULT hr = S_OK;

	hr = ExecuteObjects([](auto&& obj)
		{return obj->Update(); });

	return hr;
}

/**
* @brief オブジェクトの描画
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
* @brief オブジェクトの破棄
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
* @brief オブジェクトの破棄更新
*/
HRESULT ObjectList::ObjectDestroyUpdate()
{
	HRESULT hr = S_OK;

	//フラグが立っているイテレーターをremoveする
	auto remove_itr = std::remove_if(objects.begin(), objects.end(),
		[](auto&& obj) {return obj->destroy_flg; });

	for (auto itr = remove_itr; itr != objects.end(); ++itr)
	{
		//破棄時の処理を実行
		hr = (*itr)->OnDestroy();
		if (FAILED(hr))return hr;
	}

	//削除
	objects.erase(remove_itr, objects.end());

	return hr;
}

/**
* @brief オブジェクトの生成更新
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
* @brief オブジェクトを名前から探す
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
