#include <iostream>
#include "Matrix.h"
#include "Network.h"
#include "Genome.h"
#include "Snake.h"
#include<cstdlib>
#include<ctime>
#include <chrono>

int main()
{
	srand(time(NULL));
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	Zone zone(40, 17);
	Network net = Genome::Train(zone, 500, 50, 1);
	

}