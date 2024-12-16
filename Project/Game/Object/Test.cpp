#include "Test.h"

void Test::Init(const std::string& modelName) {

	BaseGameObject::Init(modelName);

	Collider::SetCollisionShapeSphere();

}

void Test::Update() {

	Collider::centerPos_ = transform_.translation;

	BaseGameObject::Update();

}