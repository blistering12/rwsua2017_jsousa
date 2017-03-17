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

	   float getAngleTo(string player_name,float time_to_wait = 0.1)
	   {
		tf::StampedTransform trans;
		ros::Time now = Time(0); //get the latest transform received
		try{
			listener.waitForTransform(name,player_name,now,Duration(time_to_wait));
			listener.lookupTransform(name, player_name,now, trans);
		}
		catch (tf::TransformException &ex) {
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
		}
		float x = trans.getOrigin().x();
		float y = trans.getOrigin().y();
		double angle = atan2(y,x);
	   }

	tf::StampedTransform getPose(float time_to_wait=0.1)
	{
		tf::StampedTransform trans;
	        ros::Time now = Time(0); //get the latest transform received
		try{
			listener.waitForTransform("map",name,now,Duration(time_to_wait));
			listener.lookupTransform("map",name,now, trans);
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

		float turn_angle=getAngleTo("brocha");
		float displacement=msg->max_displacement;
		move(displacement,turn_angle,MAX_ANGLE);

			double dist[3];

			dist[0] = getDistFromTo(name, "rmartins");
			dist[1] = getDistFromTo(name, "jferreira");
			dist[2] = getDistFromTo(name, "fsilva");

			int safedist = 1;
			double angleC;
			if(dist[0] < safedist || dist[1] < safedist || dist[2] < safedist){
				if(dist[0] < safedist){
					angleC = -getAngleTo("rmartins");
				}else if(dist[1] < safedist){
					angleC = -getAngleTo("jferreira");
				}else{
						angleC = -getAngleTo("fsilva");
				}
			}else{
				angleC = getAngleTo("brocha");
			}

			move(displacement, angleC, MAX_ANGLE);
	   }

//void makeAPlay(const rwsua2017_msgs::MakeAPlay::ConstPtr& msg)
//		{
//		  cout << "received a makeAPlay msg" << endl;
//			cout << "max_dispalcemente: " << msg->max_displacement << endl; 

//			//float turn_angle = M_PI/10;
//			float displacement = msg->max_displacement;

//			

//			double dist[3];

//			dist[0] = getDistFromTo(name, "rmartins");
//			dist[1] = getDistFromTo(name, "jferreira");
//			dist[2] = getDistFromTo(name, "fsilva");

//			int safedist = 1;
//			double angleC;
//			if(dist[0] < safedist || dist[1] < safedist || dist[2] < safedist){
//				if(dist[0] < safedist){
//					angleC = -getAngleFromTo(name,"rmartins");
//				}else if(dist[1] < safedist){
//					angleC = -getAngleFromTo(name,"jferreira");
//				}else{
//						angleC = -getAngleFromTo(name,"fsilva");
//				}
//			}else{
//				angleC = getAngleFromTo(name,"brocha");
//			}

//			move(displacement, angleC, displacement, MAX_ANGLE);

//		}


double getDistFromTo(string from, string to){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	double dist = 0;
	 try{
			listener.waitForTransform(from,to,now, ros::Duration(time_to_wait));
     listener.lookupTransform(from, to,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }


			double x = transform.getOrigin().x();
        double y = transform.getOrigin().y();
        dist = sqrt(x*x + y*y);

	return dist;
}
	void move(float displacement, float turn_angle, float max_turn_angle)
	{

		if (turn_angle> max_turn_angle){turn_angle=max_turn_angle;}
		else if (turn_angle< -max_turn_angle){turn_angle=-max_turn_angle;}



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
