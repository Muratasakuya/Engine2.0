#include "Player.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/JsonAdapter.h>
#include <Game/System/EnvironmentSystem.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	Player classMethods
//============================================================================*/

void Player::InitParts() {

	head_ = std::make_unique<PlayerHead>();
	head_->Init();

	body_ = std::make_unique<PlayerBody>();
	body_->Init();

	//========================================================================*/
	//* parent *//

	head_->SetParent(body_->GetWorldTransform()); // parent -> body

	//========================================================================*/
	//* json *//

	ApplyJson();

	head_->SetYOffset(headYOffset_);

}

void Player::Init() {

	input_ = Input::GetInstance();

	BaseEditor::SetEditor("player");

	parentFolderName_ = "Player/";

	InitParts();

}

void Player::Update() {

	UpdateParts();

}

void Player::UpdateParts() {

	Move(); //* 移動処理

	//========================================================================*/
	//* parentを先に更新 *//

	body_->Update();

	head_->Update();

}

void Player::Move() {

	MoveWalk();          // 通常歩き移動
	RotateToDirection(); // 移動に回転を合わせる

}

void Player::RotateToDirection() {

	Vector3 direction = Vector3(move_.x, 0.0f, move_.z).Normalize();

	if (direction.Length() <= 0.0f) {
		return;
	}

	Quaternion targetRotation = Quaternion::LookRotation(direction, Direction::Up());
	Quaternion rotation = Quaternion::Slerp(body_->GetRotation(), targetRotation, rotationLerpRate_);

	body_->SetRotate(rotation);

}

void Player::MoveWalk() {

	move_.Init();

	move_ = Vector3(input_->GetLeftStickVal().x, 0.0f, input_->GetLeftStickVal().y);
	move_ = move_.Normalize() * velocity_;

	Matrix4x4 rotateMatrix =
		Matrix4x4::MakeRotateMatrix(EnvironmentSystem::GetFollowCameraRotate());
	move_ = Vector3::TransferNormal(move_, rotateMatrix);
	move_.y = 0.0f;

	baseTranslation_ += move_;
	body_->SetTranslate(baseTranslation_);

}

void Player::Attack() {


}

void Player::ImGui() {

	if (ImGui::Button("Save")) {
		SaveJson();
	}

	ImGui::DragFloat("headYOffset", &headYOffset_, 0.01f);
	ImGui::DragFloat("rotationLerpRate", &rotationLerpRate_, 0.01f);
	ImGui::DragFloat3("moveVelocity", &velocity_.x, 0.01f);

}

void Player::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_ + GetName() + "EditParameter.json");

	headYOffset_ = data["headYOffset"];
	rotationLerpRate_ = data["rotationLerpRate"];
	velocity_ = JsonAdapter::ToVector3(data["moveVelocity"]);

}

void Player::SaveJson() {

	Json data;

	data["headYOffset"] = headYOffset_;
	data["rotationLerpRate"] = rotationLerpRate_;
	data["moveVelocity"] = JsonAdapter::FromVector3(velocity_);

	JsonAdapter::Save(parentFolderName_ + GetName() + "EditParameter.json", data);

}

bool Player::CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states) {

	for (auto state : states) {
		if (!currentMoveBehaviour_.test(static_cast<std::size_t>(state))) {

			return false;
		}
	}
	return true;
}