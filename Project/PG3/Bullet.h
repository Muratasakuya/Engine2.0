#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>
#include <Game/3D/Collision/Collider.h>

//============================================================================*/
//	Bullet class
//============================================================================*/
class Bullet :
	public BaseGameObject, public Collider {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Bullet() = default;
	~Bullet() = default;

	void Init(const Vector3& translate, uint32_t index);

	void Update();

	void OnCollisionEnter(Collider* other) override;

	//* getter *//

	bool IsAlive() const { return isAlive_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Vector3 speed_;  //* 移動速度

	//* 生存時間
	float aliveTime_;
	float aliveTimer_;
	bool isAlive_;    //* 生存フラグ

};