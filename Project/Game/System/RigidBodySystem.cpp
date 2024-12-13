#include "RigidBodySystem.h"

//============================================================================*/
//	RigidBodySystem classMethods
//============================================================================*/

std::unique_ptr<CollisionManager> RigidBodySystem::collisionManager_ = nullptr;

void RigidBodySystem::Init() {

	collisionManager_ = std::make_unique<CollisionManager>();

}

void RigidBodySystem::Update() {

	collisionManager_->UpdateAllCollisions();

}

void RigidBodySystem::Inforamtion() {

	collisionManager_->DisplayCollisionLogs();
}