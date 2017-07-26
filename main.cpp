#include "Game.h"
#include <iostream>

using namespace std;

int main(int argc, char*argv[]){
	if (argc < 2) {
		cout << "Usage: a.out file.xml" << endl;
		return 1;
	}

	Game * game = new Game(argv[1]);
	game->play();
	delete game;
	return 0;
}
