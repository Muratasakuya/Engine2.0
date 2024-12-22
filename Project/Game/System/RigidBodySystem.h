#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Collision/Managers/CollisionManager.h>

// c++
#include <memory>

//============================================================================*/
//	RigidBodySystem class
//============================================================================*/
class RigidBodySystem {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	RigidBodySystem() = default;
	~RigidBodySystem() = default;

	static void Init();

	static void Update();

	static void Inforamtion();

	static void AddCollider(Collider* collider);
	static void RemoveCollider(Collider* collider);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<CollisionManager> collisionManager_;

};