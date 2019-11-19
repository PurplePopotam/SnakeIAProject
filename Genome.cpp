#include "Genome.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>

Genome::Genome()
{
	genes = nullptr;
	size = 0;
}

Genome::Genome(size_t _size)
{
	size = _size;
	genes = new double[size];
}

Genome::Genome(const Genome& other)
{
	size = other.size;
	genes = new double[size];

	for (size_t j = 0; j < size; j++)
	{
		*(genes + j) = *(other.genes + j);
	}
}

Genome& Genome::operator=(const Genome& other)
{
	size = other.size;
	genes = new double[size];

	for (size_t j = 0; j < size; j++)
	{
		*(genes + j) = *(other.genes + j);
	}

	return *this;
}

Genome::~Genome()
{
	delete genes;
	size = 0;
}

Genome::Genome(const Network& net)
{
	size = 0;
	for (size_t i = 0; i < net.layer - 1; i++)
	{
		size += net.weights[i].Height * net.weights[i].Width;
	}

	size_t begin = 0;
	genes = new double[size];

	for (size_t i = 0; i < net.layer - 1; i++)
	{
		for (size_t j = 0; j < net.weights[i].Width * net.weights[i].Height; j++)
		{
			*(genes + begin + j) = *(net.weights[i].values + j);
		}
		begin += net.weights[i].Width * net.weights[i].Height;
	}
}

void Genome::Display()
{
	for (size_t i = 0; i < size; i++)
	{
		std::cout << *(genes + i) << " ";
	}
	std::cout << std::endl << std::endl;
}

Genome Genome::Mutate()
{
	srand(time(NULL));
	Genome res(*this);
	for (size_t j = 0; j < size; j++)
	{
		double rng = (double)(rand() / (RAND_MAX + 1.));
		if (rng > 0.995)
		{
			*(res.genes + j) = (double)(rand() / (RAND_MAX + 1.));
		}

	}
	return res;
}

Genome Genome::Mate(const Genome& A, const Genome& B)
{
	assert(A.size == B.size);
	srand(time(NULL));
	Genome res(A.size);

	for (size_t i = 0; i < A.size; i++)
	{
		double rng = (double)(rand() / (RAND_MAX + 1.));
		if (rng < 0.5)
		{
			*(res.genes + i) = *(A.genes + i);
		}
		else
		{
			*(res.genes + i) = *(B.genes + i);
		}
	}
	return res;
}

std::vector<Snake> Genome::Parents(std::vector<Snake> generation)
{
	Snake father;
	Snake mother;

	for (size_t i = 0; i < generation.size(); i++)
	{
		if (father.fitness < generation[i].fitness) { mother = father; father = generation[i]; }
	}
	std::vector<Snake> parents(0);
	parents.push_back(father);
	parents.push_back(mother);
	return parents;
}

void Genome::GenomeToWeights(Network& net)
{
	double* begin = genes;
	size_t pos = 0;

	for (size_t pos = 0; pos < net.layer - 1; pos++)
	{
		for (size_t i = 0; i < net.weights[pos].Height * net.weights[pos].Width; i++)
		{
			*(net.weights[pos].values + i) = *(i + begin);
		}
		begin += net.weights[pos].Height * net.weights[pos].Width;
	}
}

Genome Genome::NetToGenome(const Network& net)
{
	Genome res;
	res.size = 0;
	for (size_t i = 0; i < net.layer - 1; i++)
	{
		res.size += net.weights[i].Height * net.weights[i].Width;
	}

	size_t begin = 0;
	res.genes = new double[res.size];

	for (size_t i = 0; i < net.layer - 1; i++)
	{
		for (size_t j = 0; j < net.weights[i].Width * net.weights[i].Height; j++)
		{
			*(res.genes + begin + j) = *(net.weights[i].values + j);
		}
		begin += net.weights[i].Width * net.weights[i].Height;
	}

	return res;
}


Snake Genome::Train(Zone& zone, size_t pop)
{
	std::vector<Snake> gen;
	Snake snake(zone);

	for (size_t j = 0; j < pop; j++)
	{
		gen.push_back(snake);
		gen[j].Play(zone);
		std::cout << "Generation initiale" << " Serpent numero : " << j << " Fitness : " << gen[j].fitness << std::endl;
	}

	std::vector<Snake> parents = Parents(gen);
	Genome papagen(parents[0].brain);
	Genome mamangen(parents[1].brain);
	Genome filsgen = Mate(papagen, mamangen);
	Snake fils(zone);
	filsgen.GenomeToWeights(fils.brain);
	return fils;
}

Snake Genome::Train(Zone& zone, size_t pop, size_t generations)
{
	
	std::vector<std::vector<Snake>> gen;
	std::vector<Snake> gen0;
	std::vector<Snake> parents;
	Genome papagen;
	Genome mamangen;
	Genome filsgen;
	Snake first(zone);
	first = Train(zone, pop);
	for (size_t j = 0; j < pop; j++) { gen0.push_back(first); }
	gen.push_back(gen0);

	for (size_t i = 0; i < generations; i++)
	{
		gen0.clear();

		for (size_t j = 0; j < pop; j++)
		{
			gen[i][j].Play(zone);
			std::cout << "Generation : " << i << " Serpent numero : " << j << " Fitness : " << gen[i][j].fitness << std::endl;
		}

		parents = Parents(gen[i]);
		papagen = NetToGenome(parents[0].brain);
		mamangen = NetToGenome(parents[1].brain);
		filsgen = Mate(papagen, mamangen);

		for (size_t j = 0; j < pop; j++)
		{
			filsgen.Mutate().GenomeToWeights(first.brain);
			gen0.push_back(first);
		}

		gen.push_back(gen0);
		std::cout << "FITNESS :" << gen[i + 1][0].fitness << std::endl;
		std::cout << "Generation numero " << i + 1 << " terminee." << std::endl;

	}
	
	return Parents(gen[generations - 1])[0];
}