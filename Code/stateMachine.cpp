#include <iostream>

using namespace std;

enum EState
{
	OFF_STATE,
	ON_STATE,
	DETECTION_GRAFFITI_STATE,
	MANUAL_CONTROL,
	ERROR_STATE,
	MOVE_ROBOT_STATE,
	TRAJECTORY_DRAWING_STATE,
	CLEANING_STATE,
	MOVE_TO_START_STATE,
	WAIT_COMMAND_STATE
};

class Camera
{
public:

	EState getState()
	{
		return state;
	}
	
	int getEvent()
	{
		return item;
	}

	void setState(EState state)
	{
		this->state = state;
	}

	void processEvent(item)
	{
		switch (state)
		{
		case WAIT_COMMAND_STATE:
			switch (item)
			{
			case 0:
				state = OFF_STATE;
				break;
			case 1:
				state = MOVE_ROBOT_STATE;
				break;
			case 2:
				state = MANUAL_CONTROL;
				break;
			case 3:
				state = CLEANING_STATE;
				break;
			}
			break;

		case MOVE_ROBOT_STATE:
			int x, y;
			this->move_robot(x, y);
			state = WAIT_COMMAND_STATE;
			break;

		case MANUAL_CONTROL:
			this->manual_control();
			state = MOVE_TO_START_STATE;
			break;

		case ERROR_STATE:
			this->error(-1);
			state = MOVE_TO_START_STATE;
			break;

		case CLEANING_STATE:
			this->cleaning();
			state = MOVE_TO_START_STATE;
			break;

		case MOVE_TO_START_STATE:
			this->move_robot(0, 0);
			state = WAIT_COMMAND_STATE;
			break;

		case DETECTION_GRAFFITI_STATE:
			this->detection_graffiti();
			state = WAIT_COMMAND_STATE;
			break;

		case TRAJECTORY_DRAWING_STATE:
			this->trajectory_drawing();
			state = WAIT_COMMAND_STATE;
			break;
		};
	};

private:

	EState state;
	int item;

	int detection_graffiti() 
	{

	};

	int trajectory_drawing() 
	{

	};

	int error(int error_code)
	{
		cout << "Error: " << error_code << endl;
	};

	int cleaning() 
	{
		cout << "Robot cleaned the area" << endl;
	}

	int move_robot(int x, int y)
	{

	}

	int manual_control()
	{
		cout << "Manual control: ON" << endl;
	}
};

int main()
{
	Camera camSystem;
	camSystem.setState(EState::ON_STATE);
	int command;
	while (camSystem.getState() != OFF_STATE)
	{
		if (camSystem.getState == WAIT_COMMAND_STATE)
			command = camSystem.getEvent();
		camSystem.processEvent(command);
	}

	return 0;
}