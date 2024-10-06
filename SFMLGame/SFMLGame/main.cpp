#include "SFML/Graphics.hpp"	
#include "GameEngine.h"

int main(int argc, char * argv[])
{
	GameEngine game("bin/AssetsConfig.txt");
	game.run();
}
