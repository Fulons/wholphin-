
#define THIS_IS_THE_START_OF_OUR_GAME_GUYS 0x13333337
#include <iostream>

class Game {
	bool theGameIsRunning = false;
public:
	void RunThisAwesomeGame() { theGameIsRunning = true; }
	void StopThisAweomeNess() { theGameIsRunning = false; }
};

void main() {
	Game game;
	std::cout << "Wolphins are awsome" << std::endl;
	game.RunThisAwesomeGame();
	game.StopThisAweomeNess();
	system("PAUSE");
}

