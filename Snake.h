#pragma once
#include <windows.h> //to see stuff
#include <vector>
#include "Network.h"
#include "Matrix.h"
#include <iostream>
#include <string>

class Zone {

	friend class Snake;
	friend class Genome;

public:

	Zone();
	Zone(const Zone& other); 
	Zone(size_t _height, size_t width); //creates a zone with borders on each sides

	Zone& operator =(const Zone& other);
	void SpawnFruit(); //spawn a fruit at a random position within the borders
	void Clear(); //clear the zone
	void Display();

private:
	std::string zone;
	size_t height;
	size_t width;
	std::vector<size_t> Fruit;
	
};


class Snake{
	friend class Zone;
	friend class Genome;

public:

	Snake(Zone& _zone); //initial position and direction are randomized to (hopefully) prevent overfitting, snake are born with 3 parts
	Snake(); //initialize a snake without attaching it to a zone 
	void Move(Zone& zone); //move a snake in a zone if he's alive based on its direction
	void Decide(Zone& zone); //changes its direction based on its brain's output and check if it dies due to its decision
	void See(Zone& zone); //check 8 directions to "see" if there's either an obstacle or food or a snake part and update the surroundings Matrix subsequantialy with the distance to these objects
	void Check(Zone& zone); //Check if the snake died i.e. if it's directed directly toward either an obstacle or a snake part
	void Grow(Zone& zone); //add a snake part ahead of the snake
	void Play(Zone& zone); //let the snake play and print its fitness value
	void PlayToShow(Zone& zone);
	Network GetBrain() { return brain; }
	Matrix GetSurroundings() { return Surroundings; };
	void DisplaySurroundings() { Surroundings.display(); }; //display the surroundings matrix
	void DisplayFitness() { std::cout << fitness << std::endl; };
	void PutBrain(Network net) { brain = net; };
	size_t sub(size_t a, size_t b) { if (b > a) { return b - a; } else { return a - b; } };

private:
	std::vector<std::vector<size_t>> body;
	char direction;
	size_t fitness;
	Network brain;
	Matrix Surroundings;
	bool dead = 0;
	size_t movesleft = 200;
	size_t size = 0;

}; 