#pragma once
#include "../common/alias.h"
#include "../common/d3d12.h"
#include "../sprite/sprite.h"
#include <string>
#include <memory>

class ObjectBase
{
	bool destroy_flg = true;
protected:
	int layer = 0;
	virtual HRESULT Start() { return S_OK; }
	virtual HRESULT Update() { return S_OK; }
	virtual HRESULT Draw() { return S_OK; }
	virtual HRESULT OnDestroy() { return S_OK; }
public:
	virtual ~ObjectBase() {}
	Transform transform{};
	std::string name{};

	void DestroyThis() { destroy_flg = true; }

	friend class ObjectList;
	friend class Object;
};

static std::shared_ptr<ObjectBase> null_object_ptr = nullptr;

class Object
{
	std::shared_ptr<ObjectBase> object;
public:
	Object() = default;
	Object(const Object&) = default;
	Object& operator=(const Object&) = default;
	explicit Object(std::shared_ptr<ObjectBase> obj)
	{
		object = obj;
		if (object != null_object_ptr)object->destroy_flg = false;
	}
	std::shared_ptr<ObjectBase>& operator->()
	{
		return object;
	}

	operator bool()
	{
		return object != null_object_ptr && !(object->destroy_flg);
	}

	void DestroyThis()
	{
		if (object != null_object_ptr)
			object->destroy_flg = true;
	}

	friend class ObjectList;
	friend void ObjectDestroy(Object&);
};

inline void ObjectDestroy(Object& obj)
{
	obj.DestroyThis();
}

static Object null_object(null_object_ptr);