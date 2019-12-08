#pragma once
#include <windows.h> //to see stuff
#include <vector>
#include "Network.h"
#include "Matrix.h"
#include <iostream>
#include <string>
#include <random>

class Zone {

	friend class Snake;
	friend class Genome;

public:

	Zone();
	Zone(const Zone& other); 
	Zone(size_t _height, size_t width); //creates a zone with borders on each sides

	Zone& operator =(const Zone& other);
	void SpawnFruit(); //spawn a fruit at a random position within the borders
	void SpawnFruit(size_t x, size_t y);
	void Clear(); //clear the zone
	void Display();

private:
	std::string zone;
	size_t height;
	size_t width;
	std::vector<size_t> Fruit;
	std::default_random_engine FruitGen;

};


class Snake{
	friend class Zone;
	friend class Genome;

public:

	Snake(); //initialize a snake without drawing it in a zone
	void Move(Zone& zone); //move a snake in a zone if he's alive based on its direction
	void Decide(Zone& zone); //changes its direction based on its brain's output and check if it dies due to its decision
	void See(Zone& zone); //check 8 directions to "see" if there's either an obstacle or food or a snake part and update the surroundings Matrix subsequantialy with the distance to these objects
	void Grow(Zone& zone); //add a snake part ahead of the snake
	void Play(Zone& zone); //draw the snake in a zone and let it play
	void PlayToShow(Zone& zone);
	Network GetBrain() { return brain; }
	Matrix GetSurroundings() { return Surroundings; };
	double GetFitness() { return fitness; }
	void DisplaySurroundings() { Surroundings.display(); }; //display the surroundings matrix
	void DisplayFitness() { std::cout << fitness << std::endl; };
	void PutBrain(Network net) { brain = net; };
	size_t sub(size_t a, size_t b) { if (b > a) { return b - a; } else { return a - b; } }; //needed in dist

private:
	std::vector<std::vector<size_t>> body;
	char direction;
	double fitness = 0;
	Network brain;
	Matrix Surroundings;
	bool dead = 0;
	bool hasgrown = 0;
	size_t movesleft = 200;
	int movesdone = 0;
	size_t growcount = 0;
	size_t size = 4;

}; 