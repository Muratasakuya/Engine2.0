#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

//============================================================================*/
//	TitleScene class
//============================================================================*/
class TitleScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TitleScene() = default;
	~TitleScene() = default;

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

};