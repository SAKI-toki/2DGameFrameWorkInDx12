#pragma once
#include "object_base.h"
#include <vector>
#include <memory>

/**
* @brief オブジェクトリスト
*/
class ObjectList
{
	inline static std::vector<Object> objects;
	inline static std::vector<Object> generate_objects;

	template<typename T>
	static Object Instantiate(const Transform& transform, const std::string& name)
	{
		generate_objects.push_back(Object{ std::shared_ptr<ObjectBase>(new T) });
		(*generate_objects.rbegin())->Start();
		(*generate_objects.rbegin())->transform = transform;
		(*generate_objects.rbegin())->name = name;
		return (*generate_objects.rbegin());
	}
	template<typename T>
	static void InitInstantiate(const Transform& transform, const std::string& name)
	{
		objects.push_back(Object{ std::shared_ptr<ObjectBase>(new T) });
		(*objects.rbegin())->transform = transform;
		(*objects.rbegin())->name = name;
	}

	/**
	* @brief オブジェクトの実行
	*/
	template<typename FuncType>
	static HRESULT ExecuteObjects(FuncType&& f)
	{
		HRESULT hr = S_OK;
		for (auto&& obj : objects)
		{
			if (!obj)continue;
			hr = f(obj);
			if (FAILED(hr))return hr;
		}
		return hr;
	}
public:
	ObjectList() = delete;
	static HRESULT ObjectStart();
	static HRESULT ObjectUpdate();
	static HRESULT ObjectDraw();
	static HRESULT ObjectDestroy();
	static HRESULT ObjectDestroyUpdate();
	static HRESULT ObjectGenerateUpdate();
	static Object Find(const std::string&);
	template<typename T,
		std::enable_if_t<
		std::is_class_v<T>&&
		std::is_base_of_v<ObjectBase, T>, std::nullptr_t>>
		friend Object Instantiate();
	template<typename T,
		std::enable_if_t<
		std::is_class_v<T>&&
		std::is_base_of_v<ObjectBase, T>, std::nullptr_t>>
		friend Object Instantiate(const Transform&, const std::string&);
	template<typename T,
		std::enable_if_t<
		std::is_class_v<T>&&
		std::is_base_of_v<ObjectBase, T>, std::nullptr_t>>
		friend Object Instantiate(const Vec3&, const std::string&);
	template<typename T,
		std::enable_if_t<
		std::is_class_v<T>&&
		std::is_base_of_v<ObjectBase, T>, std::nullptr_t>>
		friend void InitInstantiate(const Transform&, const std::string&);
};

/**
* @brief オブジェクトの生成
*/
template<typename T,
	std::enable_if_t<
	std::is_class_v<T>&&
	std::is_base_of_v<ObjectBase, T>, std::nullptr_t>
	= nullptr>
	Object Instantiate()
{
	return ObjectList::Instantiate<T>(Transform{}, "Object");
}
template<typename T,
	std::enable_if_t<
	std::is_class_v<T>&&
	std::is_base_of_v<ObjectBase, T>, std::nullptr_t>
	= nullptr>
	Object Instantiate(
		const Transform & transform,
		const std::string & name = "Object")
{
	return ObjectList::Instantiate<T>(transform, name);
}
template<typename T,
	std::enable_if_t<
	std::is_class_v<T>&&
	std::is_base_of_v<ObjectBase, T>, std::nullptr_t>
	= nullptr>
	Object Instantiate(
		const Vec3& position,
		const std::string& name = "Object")
{
	return ObjectList::Instantiate<T>(Transform{ position,saki::vector3_zero<float>,saki::vector3_one<float> }, name);
}
/**
* @brief シーン初期化時の生成
*/
template<typename T,
	std::enable_if_t<
	std::is_class_v<T>&&
	std::is_base_of_v<ObjectBase, T>, std::nullptr_t>
	= nullptr>
	void InitInstantiate(
		const Transform & transform = { {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f} },
		const std::string & name = "Object")
{
	ObjectList::InitInstantiate<T>(transform, name);
}
