#include "Collider.h"

//============================================================================*/
//	Collider classMethods
//============================================================================*/

void Collider::SetCollisionShapeSphere() {

	shape_ = CollisionShapes::Sphere::Default();
	shapeType_ = ShapeType::Type_Sphere;
}

void Collider::SetCollisionShapeOBB() {

	shape_ = CollisionShapes::OBB::Default();;
	shapeType_ = ShapeType::Type_OBB;
}

void Collider::OBBUpdate() {

	if (shape_ && std::holds_alternative <CollisionShapes::OBB>(*shape_)) {
		CollisionShapes::OBB& obb = std::get<CollisionShapes::OBB>(*shape_);

		obb.center = centerPos_;
		obb.rotate = rotate_;
		obb.size = size_;
	} else {
		assert(false && "collisionShape is not OBB");
	}
}