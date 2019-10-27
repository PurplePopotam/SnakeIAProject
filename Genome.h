#pragma once
#include "Matrix.h"
#include "Network.h"

class Genome {

public:
	Genome();
	Genome(const Genome& other);
	Genome(size_t _size);
	~Genome();
	Genome(const Network& net); ////converts the weights of a network to a 1d genome
	void Mutate(); //changes randomly parts of the genome to maintain diversity within the population (~5% of the genome)
	void Display(); //display the genome
	static Genome Mate(const Genome& A, const Genome& B); //merges 2 genomes in one using uniform cross-over
	Genome& operator =(const Genome& other); //to make the header more easy to use
	void GenomeToWeights(Network& net); //implementes new weights into the network  

	
private:
	double* genes;
	size_t size;
};