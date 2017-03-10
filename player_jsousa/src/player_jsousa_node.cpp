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
#include <tf/transform_broadcaster.h>

using namespace std;
using namespace boost;
using namespace tf;
using namespace ros;
namespace rwsua2017
{
	class MyPlayer: public Player
	{


	    public:
		ros::Subscriber sub;
        	TransformBroadcaster br;
		Transform t1;

	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {

		t1.setOrigin(tf::Vector3(1,1,0));
		Quaternion q;
		q.setRPY(0,0,0);
		t1.setRotation(q);
		br.sendTransform(StampedTransform(t1, Time::now(),"map",name));


		sub = n.subscribe("/make_a_play", 1000, &MyPlayer::makeAPlayCallback, this);

	    	cout<<"Initialised MyPlayer"<<endl;
	    }

	   void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	   {
		cout << "I received a MakeAPlay message" << endl;
		cout << "max_displacement = " << msg->max_displacement << endl;

		// Definição dos angulos de rotaçõ e valores de translação
		static int i=0;
		float turn_angle=M_PI/10;
		float displacement=0.5+(float)i/100;


		Transform t_mov;

		Quaternion q;
		q.setRPY(0, 0, turn_angle);
		t_mov.setRotation(q);
		t_mov.setOrigin( Vector3(displacement,0.0, 0.0) );
		Transform t = t1 * t_mov;
		br.sendTransform(StampedTransform(t, Time::now(), "map", name));
		t1=t;
i++;
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
