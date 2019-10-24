#pragma once
#include "Matrix.h"
#include <vector>
#include <cmath>

class Network
{
public:

	Network();
	Network(const Network& other);
	Network(size_t layer, size_t* sizes);
	
	void FeedForward(const Matrix& input);
	Matrix GetOutput();
	std::vector<Matrix> GetWeights();
	void DisplayWeights();
	void DisplayActivations();


private:
	std::vector<Matrix> weights;
	std::vector<Matrix> bias;
	std::vector<Matrix> activations;
	size_t layer;

};