
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>

class CGame {
	bool theGameIsRunning = false;
public:
	CGame& RunThisAwesomeGame() 
	{
		std::cout << "Wolphins are awsome" << std::endl;
		theGameIsRunning = true;
		return *this;
	}

	void PrintThisShit() {
		std::cout << "Printing shit" << std::endl;
		std::cout << "Done prinitng shit!" << std::endl;
	}

};

void main() {
	
	CGame game;

	game.RunThisAwesomeGame().PrintThisShit();
	system("PAUSE");

}

/////////////////////////////////////////////////////////////////////////////
