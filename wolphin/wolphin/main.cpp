
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>

class CGame {
	bool theGameIsRunning = false;
public:
	void RunThisAwesomeGame() 
	{
		std::cout << "Wolphins are awsome" << std::endl;
		theGameIsRunning = true; 
	}


};

void main() {
	
	CGame game;

	game.RunThisAwesomeGame();
	system("PAUSE");

}

/////////////////////////////////////////////////////////////////////////////
