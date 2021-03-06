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
#include <visualization_msgs/Marker.h>

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
		Publisher vis_pub;


	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {
		Transform t1;
		t1.setOrigin(tf::Vector3(randNumber(),randNumber(),0));
		Quaternion q;
		q.setRPY(0,0,randNumber());
		t1.setRotation(q);
		br.sendTransform(StampedTransform(t1, Time::now(),"map",name));


		sub = n.subscribe("/make_a_play/cheetah", 1000, &MyPlayer::makeAPlayCallback, this);
		vis_pub = n.advertise<visualization_msgs::Marker>( "/bocas", 0 );

	    	ROS_INFO_STREAM("Initialised MyPlayer");
	    }

float getAngleFromTo(string namee, string player_name)
	   {
		tf::StampedTransform trans;
		ros::Time now = Time(0); //get the latest transform received
		try{
			listener.waitForTransform(namee,player_name,now,Duration(0.1));
			listener.lookupTransform(namee, player_name,now, trans);
		}
		catch (tf::TransformException &ex) {
		ROS_ERROR("%s",ex.what());
		ros::Duration(1.0).sleep();
		}
		float x = trans.getOrigin().x();
		float y = trans.getOrigin().y();
		double angle = atan2(y,x);
		return angle;
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
		ROS_INFO_STREAM("I received a MakeAPlay message");
		ROS_INFO_STREAM("max_displacement = " << msg->max_displacement);

		// Definição dos angulos de rotação e valores de translação


		float displacement=msg->max_displacement;


			double dist[3];
			double mindistH = 100000;
			int idxH = 0;
			for(int i = 0; i< msg->green_alive.size();i++){
				dist[i] = getDistFromTo(name, msg->green_alive[i]);
				if(dist[i] < mindistH){
							mindistH = dist[i];
							idxH = i;
					}
			}

			double mindistP = 100000;
			int idxP = 0;
			for(int i = 0; i< msg->red_alive.size();i++){
				dist[i] = getDistFromTo(name, msg->red_alive[i]);
				if(dist[i] < mindistP){
							mindistP = dist[i];
							idxP = i;
					}
			}
			bool keepHunt = false;
			if(mindistP < mindistH) keepHunt = true;
			int safedist = 2;
			double angleC;
			if(mindistH < safedist && !keepHunt ){
					if(msg->green_alive.size() > 0){
						angleC = -getAngleFromTo(name,msg->green_alive[idxH]);
					}else{
						angleC = MAX_ANGLE;
					}
				if(checkLimits()){
						angleC = M_PI/80;
				}
			}else{
				double mindist = 1000000;
				int idx = 0;
				for(int i = 0; i< msg->red_alive.size();i++){
					double dis = getDistFromTo(name, msg->red_alive[i]);
					if(dis < mindist){
							mindist = dis;
							idx = i;
					}
				}
			
				if(msg->red_alive.size() > 0){
					angleC = getAngleFromTo(name,msg->red_alive[idx]);
					}else{
						angleC = MAX_ANGLE;
					}
			}

			move(displacement, angleC, MAX_ANGLE);

        //enviar boca
        visualization_msgs::Marker marker;
        marker.header.frame_id = name;
        marker.header.stamp = ros::Time();
        marker.ns = name;
        marker.id = 0;
        marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = 0; marker.pose.position.y = 0.6; marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0; marker.pose.orientation.y = 0.0; marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.z = 0.4;
        marker.color.a = 1.0; // Don't forget to set the alpha!
        marker.color.r = 0.3;
        marker.color.g = 0.3;
        marker.color.b = 0.3;
        marker.frame_locked = 1;
        marker.lifetime = ros::Duration(1);
        marker.text = "ez pz lemon squeezy";
        vis_pub.publish(marker);

	   }



double getDistFromTo(string from, string to){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	double dist = 0;
	 try{
	listener.waitForTransform(from,to,now, ros::Duration(time_to_wait));
	listener.lookupTransform(from, to,now,transform);
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

bool checkLimits(){

	tf::StampedTransform transform;
	ros::Time now = ros::Time(0);
	float time_to_wait = 0.1;
	double dist = 0;
	 try{
			listener.waitForTransform("/map",name,now, ros::Duration(time_to_wait));
     listener.lookupTransform("/map", name,
															now,transform);
    }
    catch (tf::TransformException ex){
      ROS_ERROR("%s",ex.what());
      //ros::Duration(1.0).sleep();
    }


		double x = transform.getOrigin().x();
    double y = transform.getOrigin().y();

		double safedist = 1;

		if(abs(x) > 5-safedist  ||  abs(y) > 5-safedist){
				return true;
		}
		return false;

}	

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

    ROS_INFO_STREAM("player.name is " << player.name);
    ROS_INFO_STREAM("team is " << player.get_team_name());
    ROS_INFO_STREAM("teammates");


    ros::spin();
    return 1;
}
