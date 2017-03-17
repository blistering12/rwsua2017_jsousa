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
#include <tf/transform_listener.h>

#define MAX_ANGLE M_PI/30

double randNumber(){
		struct timeval t1;
		gettimeofday(&t1,NULL);
		srand(t1.tv_usec);
		double x =((((double)rand()/(double)RAND_MAX)*2 -1)*5);

		return x;
}

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
		TransformListener listener;


	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {
		Transform t1;
		t1.setOrigin(tf::Vector3(randNumber(),randNumber(),0));
		Quaternion q;
		q.setRPY(0,0,randNumber());
		t1.setRotation(q);
		br.sendTransform(StampedTransform(t1, Time::now(),"map",name));


		sub = n.subscribe("/make_a_play/dog", 1000, &MyPlayer::makeAPlayCallback, this);

	    	cout<<"Initialised MyPlayer"<<endl;
	    }

	   float getAngleTo(string player_name)
	   {
		       tf::StampedTransform trans;
		try{
			listener.lookupTransform(name, player_name,ros::Time(0), trans);
		}
		catch (tf::TransformException &ex) {
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
		}
		float x = trans.getOrigin().x();
		float y = trans.getOrigin().y();
		double angle = atan2(y,x);
	   }

	tf::StampedTransform getPose(void)
	{
		tf::StampedTransform trans;
		try{
			listener.lookupTransform("map",name,ros::Time(0), trans);
		}
		catch (tf::TransformException &ex) {
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
		}
		return trans;
	}

	   void makeAPlayCallback(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
	   {
		cout << "I received a MakeAPlay message" << endl;
		cout << "max_displacement = " << msg->max_displacement << endl;

		// Definição dos angulos de rotação e valores de translação

		float turn_angle=getAngleTo("dcorreia");
		if (turn_angle> MAX_ANGLE){turn_angle=MAX_ANGLE;}
		if (turn_angle< -MAX_ANGLE){turn_angle=-MAX_ANGLE;}
		float displacement=msg->max_displacement;


		Transform t_mov;

		Quaternion q;
		q.setRPY(0, 0, turn_angle);
		t_mov.setRotation(q);
		t_mov.setOrigin( Vector3(displacement,0.0, 0.0) );
		Transform t = getPose() * t_mov;
		br.sendTransform(StampedTransform(t, Time::now(), "map", name));

		
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
