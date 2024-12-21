#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/Editor/Base/BaseEditor.h>

// parts
#include <Game/Objects/Player/Parts/PlayerHead.h>
#include <Game/Objects/Player/Parts/PlayerBody.h>
#include <Game/Objects/Player/Parts/PlayerLeftArm.h>
#include <Game/Objects/Player/Parts/PlayerRightArm.h>

// c++
#include <memory>
#include <optional>
#include <initializer_list>
#include <unordered_set>

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

	// 補間
	struct LerpValue {

		// value
		float start;
		float end;
		float current;

		// timer
		float lerpTime;
		float lerpTimer;
	};

	// 移動
	enum class MoveBehaviour {

		Dash, // ダッシュ
		Jump, // ジャンプ
	};

	//========================================================================*/
	//* variables

	Input* input_ = nullptr;

	std::optional<MoveBehaviour> moveBehaviour_;              //* 依頼移動ビヘイビア
	std::unordered_set<MoveBehaviour> currentMoveBehaviours_; //* 現在の移動ビヘイビア

	//* base *//

	Vector3 baseTranslation_; //* 親の移動座標

	Vector3 velocity_; //* 移動速度
	Vector3 move_;     //* 移動量

	float rotationLerpRate_; //* 回転補間割合
	float moveDecay_;        //* 速度減衰率

	//* dash *//

	LerpValue dashSpeed_; //* ダッシュ速度

	//* jump *//

	float jumpStrength; //* ジャンプ力

	//* bool *//

	bool isDashing_;  //* ダッシュしたかどうか
	bool isOnGround_; //* 地面に着いているかどうか

	//* parts *//

	std::unique_ptr<PlayerHead> head_;

	std::unique_ptr<PlayerBody> body_;

	std::unique_ptr<PlayerLeftArm> leftArm_;
	std::unique_ptr<PlayerRightArm> rightArm_;

	//* headParameter *//

	float headYOffset_; //* bodyからの距離

	//* bodyParameter *//

	//* leftArmParameter *//

	Vector3 leftArmOffset_; //* bodyからの距離

	//* rightArmParameter *//

	Vector3 rightArmOffset_; //* bodyからの距離

	//========================================================================*/
	//* functions

	void InitParts();   //* 各部位の初期化
	void UpdateParts(); //* 各部位の更新処理

	//====================================*/
	//* move

	void Move();        //* 全体の移動を管理
	void MoveRequest(); //* 移動依頼

	void MoveWalk(); //* 通常移動、歩きに該当する
	void MoveDash(); //* ダッシュ、Rを押している間はダッシュ
	void MoveJump(); //* ジャンプ

	//====================================*/
	//* attack

	//====================================*/
	//* other

	// json
	void ApplyJson();
	void SaveJson();

	// helper
	void RotateToDirection();
	bool CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states);

};