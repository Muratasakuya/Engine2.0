#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

// object
#include <Game/Objects/Environment/Field.h>
#include <PG3/Cube.h>
#include <PG3/Command.h>
#include <PG3/InputHandler.h>

// c++
#include <string>
#include <memory>

//============================================================================*/
//	GameScene class
//============================================================================*/
class GameScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GameScene() = default;
	~GameScene() = default;

	void Run() override;

	void Init() override;

	void Update() override;

	void Finalize() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<Field> field_;

	InputHandler* inputHandler_;

	ICommand* iCommand_;

	std::unique_ptr<Cube> cube_;

	//========================================================================*/
	//* function

	void LoadAssets();

	void Draw2D();

};