#pragma once

#include <Game/3D/Object/BaseGameObject.h>
#include <Game/3D/Collision/Collider.h>

class Test :
	public BaseGameObject, public Collider {
public:

	Test() = default;
	~Test() = default;

	void Init(const std::string& modelName);

	void Update();

};