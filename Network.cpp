#include "Network.h"
#include <cassert>

Network::Network()
{
	std::vector<Matrix> weights;
	std::vector<Matrix> bias;
	std::vector<Matrix> activations;
	size_t layer = 0;
}

Network::Network(const Network& other)
{
	layer = other.layer;
	weights = other.weights;
	bias = other.bias;
	activations = other.activations;
}

Network::Network(size_t _layer, size_t* sizes)
{
	layer = _layer;
	size_t* current = sizes;

	for (size_t i = 0; i < layer - 1; i++, current++)
	{
		bias.push_back(Matrix::uniform(Matrix(*(current + 1), 1), 0)); //biais � z�ro pour le moment
		weights.push_back(Matrix(*(current + 1), *current, 0, 1)); //poids initialis�s arbitrairement entre 0 et 1
	}
}

void Network::FeedForward(const Matrix& input)
{
	activations.push_back(Matrix::sigmoid(Matrix::dot(weights[0], input)));
	for (size_t i = 0; i < layer - 2; i++)
	{
		activations.push_back(Matrix::sigmoid(Matrix::dot(weights[i + 1], activations[i])));
	}
	
}

Matrix Network::GetOutput()
{
	return activations[layer - 2];
}

std::vector<Matrix> Network::GetWeights()
{
	return weights;
}


void Network::DisplayWeights()
{
	for (size_t i = 0; i < layer - 1; i++)
	{
		weights[i].display();
	}
}

void Network::DisplayActivations()
{
	for (size_t i = 0; i < layer - 1; i++)
	{
		activations[i].display();
	}
}