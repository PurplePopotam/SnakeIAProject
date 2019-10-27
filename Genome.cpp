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

void Genome::Mutate()
{
	srand(time(NULL));
	for (size_t j = 0; j < size; j++)
	{
		double rng = (double)(rand() / (RAND_MAX + 1.));
		if (rng > 0.95)
		{
			*(genes + j) = (double)(rand() / (RAND_MAX + 1.));
		}
	}
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


