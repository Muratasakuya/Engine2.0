#include "Cube.h"

//============================================================================*/
//	Cube classMethods
//============================================================================*/

void Cube::Init() {

	BaseGameObject::Init("cube.obj");

	BaseGameObject::SetMeshRenderer("cube");

	transform_.translation.y += 2.0f;

	speed_ = 0.25f;

}

void Cube::Update() {

	BaseGameObject::Update();

}

void Cube::MoveRight() {

	this->transform_.translation.x += this->speed_;
}

void Cube::MoveLeft() {

	this->transform_.translation.x -= this->speed_;
}