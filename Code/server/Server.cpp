#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

struct Position 
{
	int x;
	int y;
};

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

class  CommandStrategy
{
public:

	virtual string getCommand() = 0;
	virtual int init() = 0;
	virtual ~CommandStrategy() {}

};

class  ConsoleStrategy : public CommandStrategy
{
public:

	int init() override {
		return 0;
	};


	string getCommand() override
	{
		string response, x, y;
		int code = -1;
		cout << "Command list:\n 0 - off\n 1 - move robot on position\n 2 - manual control\n 3 - start cleaning\n Enter command:\t" << endl;
		cin >> code;

		while (code < 0 && code > 3)
		{
			cout << "Command error" << endl;
			cin >> code;
		}

		switch (code)
		{
		case 1:
			cout << "Enter x, y:" << endl;
			cin >> x >> y;
			break;
		}

		response = to_string(code) + " " + x + " " + y;
		return response;
	}

};

class NetworkStrategy : public CommandStrategy
{
private:

	int sock;
	struct sockaddr_in addr;
	char buf[10];
	int bytes_read;

public:

	int init() override
	{
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		addr.sin_family = AF_INET;
		addr.sin_port = htons(3425);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			perror("bind");
			return 1;
		}
		return 0;
	}

	string getCommand() override
	{
		cout << "Waiting for a command\n";
		string response;
		bytes_read = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
		buf[bytes_read + 1] = '\0';
		response = buf;
		return response;
	}

};

class Camera
{
public:

	Camera(CommandStrategy* cmdshow, Position* r_pos) : state(WAIT_COMMAND_STATE), cmd(cmdshow), pos(r_pos)
	{
		cmd->init();
	};

	~Camera()
	{
		delete cmd;
		delete pos;
	};

	EState getState()
	{
		return state;
	};

	void setState(EState state)
	{
		this->state = state;
	};

	void processEvent(string command)
	{
		istringstream cmd_command(command);	
		int item;
		cmd_command >> item;

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
			int xPos, yPos;
			cmd_command >> xPos;
			cmd_command >> yPos;
			this->move_robot(xPos, yPos);
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
	CommandStrategy* cmd;
	Position* pos;

	int detection_graffiti()
	{
		cout << "Graffity detected" << endl;
	};

	int trajectory_drawing()
	{
		cout << "The trajectory of the movement is drawn" << endl;
	};

	int error(int error_code)
	{
		cout << "Error: " << error_code << endl;
	};

	int cleaning()
	{
		cout << "Robot cleaned the area" << endl;
	}

	int move_robot(int xPos, int yPos)
	{
		this->pos->x = xPos;
		this->pos->y = yPos;
		cout << "Robot position: x = " << this->pos->x  << " y = " << this->pos->y << endl;
	}

	int manual_control()
	{
		cout << "Manual control: ON" << endl;
	}

	string getEvent() 
	{
		return cmd->getCommand();
	}
};

int main()
{
	Camera* camSystem;
	camSystem->setState(EState::ON_STATE);

	std::cout << "Choose method of control:\n c = соnsole\n n = network\n): ";
	char choice;
	std::cin >> choice;
	if (choice == 'c') {
		camSystem = new Camera(new ConsoleStrategy, new Position);
	}
	else if (choice == 'n') {
		camSystem = new Camera(new NetworkStrategy, new Position);
	}


	string command;
	while (camSystem->getState() != OFF_STATE)
	{
		if (camSystem->getState == WAIT_COMMAND_STATE)
			command = camSystem->getEvent();
		camSystem->processEvent(command);
	}

	return 0;
}