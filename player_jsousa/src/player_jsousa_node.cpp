//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>
#include <vector>
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>

using namespace std;
namespace rwsua2017
{
	class MyPlayer: public Player
	{
	    public:
	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {
	    	cout<<"Initialised MyPlayer"<<endl;
	    }

	    vector<string> teammates; // std::vector<boost::shared_ptr<Player>> teammates;
	};
}


using namespace rwsua2017;

int main()
{
    //Creating an instance of class Player
//    Player player("jsousa");
//    Player player("jsousa","blue");
//    player.setTeamName();

    //Creating an instance of class MyPlayer
    MyPlayer player("jsousa","asdasddas");
    player.teammates.push_back("fsilva");
    player.teammates.push_back("vsilva");

    cout << "player.name is " << player.name << endl;
    cout << "team is " << player.get_team_name() << endl;
    cout << "teammates" << endl;


    return 1;
}
