#include "Enemy.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Utility/Direction.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	Enemy classMethods
//============================================================================*/

void Enemy::Init() {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetMeshRenderer("enemy");

	transform_.translation.y += 2.0f;
	transform_.translation.z += 12.0f;

	materials_.front().properties.color = Color::Red();
	
	Collider::SetCollisionShapeOBB();

	Collider::type_ = ColliderType::Type_Enemy;
	Collider::targetType_ = ColliderType::Type_PlayerBullet;
	Collider::name_ = BaseGameObject::GetName();

	isAlive_ = true;

}

void Enemy::Update() {

	Collider::centerPos_ = transform_.translation;
	Collider::rotate_ = transform_.rotation;
	Collider::size_ = transform_.scale;

	Collider::OBBUpdate();

	BaseGameObject::Update();

}

void Enemy::OnCollisionEnter(Collider* other) {

	if (other->GetType() == this->GetTargetType()) {

		isAlive_ = false;
	}
}