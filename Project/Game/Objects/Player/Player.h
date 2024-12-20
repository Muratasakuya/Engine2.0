#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/Editor/Base/BaseEditor.h>

// parts
#include <Game/Objects/Player/Parts/PlayerHead.h>
#include <Game/Objects/Player/Parts/PlayerBody.h>

// c++
#include <memory>
#include <optional>
#include <initializer_list>
#include <bitset>

//============================================================================*/
//	Player class
//============================================================================*/
class Player :
	public BaseEditor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Player() = default;
	~Player() = default;

	void Init();

	void Update();

	//* imgui *//

	void ImGui() override;

	//* getter *//

	const Vector3& GetCenterTranslate() const { return baseTranslation_; }

	const WorldTransform& GetTargetTransform() const { return body_->GetWorldTransform(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structures

	// 移動
	enum class MoveBehaviour :
		std::size_t {

		Dash = 0, // ダッシュ
		Jump = 1, // ジャンプ

		Count
	};
	using MoveBehaviourBitset = std::bitset<static_cast<std::size_t>(MoveBehaviour::Count)>;

	//========================================================================*/
	//* variables

	Input* input_ = nullptr;

	std::optional<MoveBehaviour> moveBehaviour_; //* 依頼移動ビヘイビア
	MoveBehaviourBitset currentMoveBehaviour_;   //* 現在の移動ビヘイビア

	//* base *//

	Vector3 baseTranslation_; //* 親の移動座標

	Vector3 velocity_; //* 移動速度
	Vector3 move_;     //* 移動量

	float rotationLerpRate_; //* 回転補間割合

	//* parts *//

	std::unique_ptr<PlayerHead> head_;

	std::unique_ptr<PlayerBody> body_;

	//* headParameter *//

	float headYOffset_; //* bodyからの距離

	//* bodyParameter *//

	//========================================================================*/
	//* functions

	void InitParts();   //* 各部位の初期化
	void UpdateParts(); //* 各部位の更新処理

	//====================================*/
	//* move

	void Move(); //* 全体の移動を管理

	void RotateToDirection(); //* 回転の向きを移動に合わせる

	void MoveWalk(); //* 通常移動、歩きに該当する

	//====================================*/
	//* attack

	void Attack(); // 全体の攻撃を管理

	//====================================*/
	//* other

	// json
	void ApplyJson();
	void SaveJson();

	// helper
	bool CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states);

};