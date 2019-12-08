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
	size_t GetSize() { return size; };
	Genome(const Network& net); //converts the weights of a network to a 1d genome
	static Genome WeightsToGenome(const Network& net); //same but not a constructor
	static Genome BiasToGenome(const Network& net); //converts the bias of a network to a 1d genome
	static Genome Mutate(const Genome& other,double mrate, double delta); //changes randomly parts of the genome to maintain diversity within the population (~5% of the genome)
	static std::vector<Snake> Select(std::vector<Snake> pop, double percent); //selects a given percentage of the snakes based on their fitness value
	static Genome Mate(const Genome& A, const Genome& B); //merges 2 genomes in one using random point crossover
	Genome& operator =(const Genome& other); //to make the header more easy to use
	void GenomeToWeights(Network& net); //implementes new weights into the network
	void GenomeToBias(Network& net); //implementes new bias into the network
	/*static Snake Train(Zone& zone, size_t pop); //trains one generation*/
	static Network Train(Zone& zone, size_t pop, size_t generations, bool affichage = 0); //trains "generations" generation
	void Display(); //display the genome

private:
	double* genes;
	size_t size;
};