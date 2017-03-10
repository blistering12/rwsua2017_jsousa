//#include <iostream>

//int main()
//{
//    std::cout << "Hello world" << std::endl;           
//    return 1;                                                      
//}

#include <iostream>
#include <vector>

using namespace std;
namespace rwsua2017_jsousa
{
	class Player
	{
	    public:

	    //Constructor with the same name as the class
	    Player(string argin_name, string argin_teamname="green") { this->name = argin_name; setTeamName(argin_teamname);}

	    //Set team name, if given a correct team name (accessor)
	    int setTeamName(string team)
	    {
		if (team=="red" || team=="green" || team=="blue")
		{
		    this->team = team;
		    return 1;
		}
		else
		{
		    cout << "cannot set team name to " << team << endl;
		    return 0;
		}
	    }

	    //Gets team name (accessor)
	    string getTeam(void) {return team;}

	    string name; //A public atribute

	    void setTeamName(void) {setTeamName("red");}


	    private:
	    string team;
	};

	class MyPlayer: public Player
	{
	    public:
	    MyPlayer(string argin_name, string argin_teamname): Player(argin_name,argin_teamname)
	    {
	    	cout<<"Initialised MyPlayer"<<endl;
	    }

	    vector<string> teammates; // std::vector<boost::shared_ptr<Player>> teammates;
	    
	    void GetTeamMates()
	    {
		for (size_t i=0;i<this->teammates.size();i++) //size_t = unsigned long int
		{
			cout << this->teammates[i] << endl;
		}
	    }
	};
}


using namespace rwsua2017_jsousa;

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
    cout << "team is " << player.getTeam() << endl;
    cout << "teammates" << endl;

    player.GetTeamMates();
    return 1;
}
