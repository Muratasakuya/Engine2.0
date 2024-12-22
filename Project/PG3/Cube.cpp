#include "Cube.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>

//============================================================================*/
//	Cube classMethods
//============================================================================*/

void Cube::Init() {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetMeshRenderer("cube");

	transform_.translation.y += 2.0f;

	bulletIndex_ = 0;

}

void Cube::Update() {

	Shoot();

	bullets_.remove_if([](const std::unique_ptr<Bullet>& bullet) {
		return !bullet->IsAlive(); });

	for (const auto& bullet : bullets_) {

		bullet->Update();
	}

	BaseGameObject::Update();

}

void Cube::Shoot() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		auto newBullet = std::make_unique<Bullet>();
		newBullet->Init(transform_.GetWorldPos(), bulletIndex_);

		bullets_.emplace_back(std::move(newBullet));

		++bulletIndex_;
	}
}