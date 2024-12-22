#include "Bullet.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Utility/Direction.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	Bullet classMethods
//============================================================================*/

void Bullet::Init(const Vector3& translate, uint32_t index) {

	BaseGameObject::Init("sphere.obj");

	BaseGameObject::SetMeshRenderer("bullet" + std::to_string(index));

	transform_.translation = translate;
	transform_.scale = Vector3(0.5f, 0.5f, 0.5f);

	Collider::SetCollisionShapeSphere();
	Collider::radius_ = 0.6f;

	Collider::type_ = ColliderType::Type_PlayerBullet;
	Collider::targetType_ = ColliderType::Type_Enemy;
	Collider::name_ = "bullet" + std::to_string(index);

	speed_ = Direction::Forward() / 2.0f;

	isAlive_ = true;

	aliveTimer_ = 0.0f;
	aliveTime_ = 3.0f;

}

void Bullet::Update() {

	transform_.translation += speed_;

	Collider::centerPos_ = transform_.translation;
	Collider::SphereUpdate();

	BaseGameObject::Update();

	aliveTimer_ += GameSystem::GetDeltaTime();
	if (aliveTimer_ > aliveTime_) {

		isAlive_ = false;
	}

}

void Bullet::OnCollisionEnter(Collider* other) {

	if (other->GetType() == this->GetTargetType()) {

		isAlive_ = false;
	}
}