#pragma once
#include "Matrix.h"
#include <vector>
#include <cmath>

class Network
{
public:

	Network();
	~Network();
	Network(const Network& other);
	Network(size_t layer, size_t* sizes);




private:
	std::vector<Matrix> weights;
	std::vector<Matrix> bias;
	std::vector<Matrix> activations;


};