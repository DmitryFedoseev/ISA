#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

int main()
{
    char msg[10];
	int sock;
	struct sockaddr_in addr;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
    int code;
    string response, x, y;

    while (true) {
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
	}
        response = to_string(code) + " " + x + " " + y;
        strcpy(msg, response.c_str());
        sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&addr, sizeof(addr));
    }

    close(sock);
    return 0;
}
