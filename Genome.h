#pragma once
#include "Matrix.h"
#include "Network.h"
#include "Snake.h"

class Genome {

	friend class Snake;
	friend class Zone;
public:
	Genome();
	Genome(const Genome& other);
	Genome(size_t _size);
	~Genome();
	Genome(const Network& net); ////converts the weights of a network to a 1d genome
	Genome Mutate(); //changes randomly parts of the genome to maintain diversity within the population (~5% of the genome)
	static std::vector<Snake> Parents(std::vector<Snake>); //selects 2 snakes with highest fitness value within a list of Snakes
	static Genome Mate(const Genome& A, const Genome& B); //merges 2 genomes in one using uniform cross-over
	Genome& operator =(const Genome& other); //to make the header more easy to use
	void GenomeToWeights(Network& net); //implementes new weights into the network  
	static Snake Train(Zone& zone, size_t pop, size_t generations); 
	void Display(); //display the genome

private:
	double* genes;
	size_t size;
};