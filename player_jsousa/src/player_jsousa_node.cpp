//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>
#include <vector>

//ROS INCLUDES
#include "ros/ros.h"

//Local INCLUDES
#include <rwsua2017_libs/player.h>
#include <rwsua2017_msgs/MakeAPlay.h>

using namespace std;
using namespace boost;
namespace rwsua2017
{
	class MyPlayer: public Player
	{
	    ros::Subscriber sub;

	    public:
	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {

		sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);

	    	cout<<"Initialised MyPlayer"<<endl;
	    }

	   void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	   {
		cout << "I received a MakeAPlay message" << endl;
		cout << "max_displacement = " << msg->max_displacement << endl;
	   }

	    vector<string> teammates; // std::vector<boost::shared_ptr<Player>> teammates;
	};
}


using namespace rwsua2017;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "player_jsousa");
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


    ros::spin();
    return 1;
}
