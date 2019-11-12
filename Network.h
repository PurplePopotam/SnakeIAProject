#pragma once
#include "Matrix.h"
#include <vector>
#include <cmath>

class Network
{

	friend class Genome;

public:

	Network();
	Network(const Network& other);
	Network(size_t layer, size_t* sizes);
	
	void FeedForward(const Matrix& input); //feedforward the input, update activations  
	Matrix GetOutput(); //return the last member of activations
	std::vector<Matrix> GetWeights(); //return the weight matrices
	Network& operator=(const Network& other);
	void DisplayWeights(); 
	void DisplayActivations();

	static std::vector<Network> Batch(size_t layer, size_t* sizes, size_t number); //creates "number" networks based on the same template

private:
	std::vector<Matrix> weights;
	std::vector<Matrix> bias;
	std::vector<Matrix> activations;
	size_t layer;


};