#pragma once

//============================================================================*/
//	include
//============================================================================*/

// front
class Cube;

//============================================================================*/
//	Command class
//============================================================================*/

class ICommand {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ICommand() = default;
	virtual ~ICommand() = default;

	virtual void Exec(Cube& cube) = 0;

};

class MoveRightCommand :
	public ICommand {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	MoveRightCommand() = default;
	~MoveRightCommand() = default;

	void Exec(Cube& cube) override;

};

class MoveLeftCommand :
	public ICommand {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	MoveLeftCommand() = default;
	~MoveLeftCommand() = default;

	void Exec(Cube& cube) override;

};