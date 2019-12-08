#include "Genome.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <random>
#include <algorithm>

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
	for (size_t i = 0; i < size; i++) {std::cout << *(genes + i) << " ";}
	std::cout << std::endl << std::endl;
}

Genome Genome::Mutate(const Genome& other, double mrate, double delta)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 2);

	Genome res(other);
	for (size_t j = 0; j < res.size; j++)
	{
		double rng = (double)(rand() / (RAND_MAX + 1.));
		if (rng < mrate) {
			if (distribution(generator) == 1) { *(res.genes + j) += delta; }
			else { *(res.genes + j) -= delta; }
		}
	}
	return res;
}

std::vector<Snake> Genome::Select(std::vector<Snake> generation, double percent)
{
	std::vector<Snake> besties(0);
	Snake snake;
	struct {
		bool operator()(const Snake& a, const Snake& b) const
		{
			return a.fitness < b.fitness;
		}
	} comp;

	std::sort(generation.begin(), generation.end(), comp);
	size_t number = (size_t)(percent * generation.size());
	for (size_t j = generation.size() - number; j < generation.size(); j++) { besties.push_back(generation[j]); }
	return besties;
}

Genome Genome::Mate(const Genome& A, const Genome& B)
{
	Genome res;
	res.size = A.size;
	res.genes = new double[res.size];
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, A.size);
	size_t pivot = distribution(generator);
	for (size_t j = 0; j < pivot; j++) {*(res.genes + j) = *(A.genes + j);}
	for (size_t j = pivot; j < res.size; j++) { *(res.genes + j) = *(B.genes + j); }
	
	return res;
}

void Genome::GenomeToBias(Network& net)
{
	double* begin = genes;

	for (size_t pos = 0; pos < net.layer - 1; pos++)
	{
		for (size_t i = 0; i < net.bias[pos].Height * net.bias[pos].Width; i++)
		{
			*(net.bias[pos].values + i) = *(i + begin);
		}
		begin += net.bias[pos].Height * net.bias[pos].Width;
	}
}

void Genome::GenomeToWeights(Network& net)
{
	double* begin = genes;

	for (size_t pos = 0; pos < net.layer - 1; pos++)
	{
		for (size_t i = 0; i < net.weights[pos].Height * net.weights[pos].Width; i++)
		{
			*(net.weights[pos].values + i) = *(i + begin);
		}
		begin += net.weights[pos].Height * net.weights[pos].Width;
	}
}

Genome Genome::BiasToGenome(const Network& net)
{
	Genome res;
	res.size = 0;
	for (size_t i = 0; i < net.layer - 1; i++)
	{
		res.size += net.bias[i].Height * net.bias[i].Width;
	}

	size_t begin = 0;
	res.genes = new double[res.size];

	for (size_t i = 0; i < net.layer - 1; i++)
	{
		for (size_t j = 0; j < net.bias[i].Width * net.bias[i].Height; j++)
		{
			*(res.genes + begin + j) = *(net.bias[i].values + j);
		}
		begin += net.bias[i].Width * net.bias[i].Height;
	}

	return res;
}

Genome Genome::WeightsToGenome(const Network& net)
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

Network Genome::Train(Zone& zone, size_t pop, size_t generations, bool affichage)
{
	std::default_random_engine generator;
	std::vector<std::vector<Snake>> gen;
	std::vector<Snake> gen0;
	Snake best;

	for (size_t j = 0; j < pop; j++) {Snake snake; gen0.push_back(snake);}

	gen.push_back(gen0);
	zone.Clear();

	for (size_t j = 0; j < generations; j++)
	{
		for (size_t i = 0; i < pop; i++)
		{
			gen[j][i].Play(zone);
			std::cout << "Generation : " << j << " Serpent : " << i << " Score : " << gen[j][i].growcount << std::endl;
			zone.SpawnFruit();
		}
		gen0 = Select(gen[j], 0.5);
		best = gen0[gen0.size() - 1];
		gen.push_back(gen0);
		std::uniform_int_distribution<int> distribution(0, gen0.size()-1);

		for (size_t i = 0; i < pop - gen0.size(); i++)
		{
			size_t x = distribution(generator);
			size_t y = distribution(generator);
			Snake fils;
			Genome xwgen = WeightsToGenome(gen[j + 1][x].brain);
			Genome ywgen = WeightsToGenome(gen[j + 1][y].brain);
			Genome xbgen = BiasToGenome(gen[j + 1][x].brain);
			Genome ybgen = BiasToGenome(gen[j + 1][y].brain);
			Genome wfils = Mutate(Mate(xwgen, ywgen), 0.9, 0.01);
			Genome bfils = Mutate(Mate(xbgen, ybgen), 0.9, 0.01);
			wfils.GenomeToWeights(fils.brain);
			wfils.GenomeToBias(fils.brain);
			gen[j + 1].push_back(fils);
		}

		std::cout << "GENERATION " << j << " TERMINEE" << "MEILLEUR FITNESS " << best.GetFitness() << std::endl;

		if (affichage)
		{
			Snake player;
			player.PutBrain(best.brain);
			player.PlayToShow(zone);
			zone.SpawnFruit();
		 }		
	}

	struct {
		bool operator()(const Snake& a, const Snake& b) const
		{
			return a.fitness < b.fitness;
		}
	} comp;

	std::sort(gen[generations-1].begin(), gen[generations-1].end(), comp);
	std::cout << "Le meilleur agent a eu un fitness de   " << gen[generations - 1][pop - 1].fitness << std::endl;
	return best.brain;
}