#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>
#include <Game/3D/Collision/Collider.h>

//============================================================================*/
//	Enemy class
//============================================================================*/
class Enemy :
	public BaseGameObject, public Collider {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Enemy() = default;
	~Enemy() = default;

	void Init();

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

	bool isAlive_;    //* 生存フラグ

};