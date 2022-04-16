#include <iostream>;
using namespace std;

class Controler
{
public:
    virtual void get_command()
    {
        cout << "Controler";
    }
};

class ConsoleCommand : public Controler
{
public:
    void get_command() {
        cout << "ConsoleCommand";
    }
};

class NetControler : public Controler
{
public:
    void get_command()
    {
        cout << "NetControler";
    }
};

class Robot
{
private:
    int coordinat;
    Controler *cont;
public:
    Robot(Controler* control)
    {
        cont = control;
    }
    void get()
    {
        cont->get_command();
    }

    void move_robot() {}
    
};

class Camera
{
    void detection_robot() {}
    void detection_graffiti() {}
};

class System
{
private:
    Robot rob;
    Camera cam;
public:
    void on() {}
    void off() {}
    void read_coord() {}
    void trajectory_drawing() {}
    void transfer_inf_robot() {}

};

int main()
{   
    Robot rob(new ConsoleCommand);
    rob.get();
    ConsoleCommand cmd;
    return 0;
}