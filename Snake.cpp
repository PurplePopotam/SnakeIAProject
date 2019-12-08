#include "Snake.h"
#include <cstdlib> 
#include <ctime>
#include <iostream>
#include "Windows.h"
#include "stdio.h"
#include <random>
#include <thread>
#include <chrono>


Zone::Zone()
{
	width = 0;
	height = 0;
}

Zone::Zone(const Zone& other)
{
	height = other.height;
	width = other.width;
	Fruit = other.Fruit;
	
	zone = other.zone;

}

Zone& Zone::operator=(const Zone& other)
{

	height = other.height;
	width = other.width;
	zone = other.zone;
	return *this;
}

Zone::Zone(size_t _width, size_t _height)
{
	width = _width;
	height = _height;
	std::string _zone(height * width, ' ');
	zone = _zone;
	
	for (size_t i = 0; i < width * height; i++)
	{

		if (i >= 3 * width && i < width * (height - 1) && i % width == 0)
		{
			zone[i] = '|';
			zone[i - 1] = '|';
		}

		else if (i < width || (2 * width <= i && i < 3 * width) || (height-1)*width <= i && i <= height*width)
		{
			zone[i] = '=';
		}


		zone[3 * width - 1] = '='; //lazy solutions to small details
		zone[width * (height - 1) - 1] = '|';
	}
	
	size_t x = 2 + (size_t)((width - 4) * rand() / (RAND_MAX + 1.0));
	size_t y = 4 + (size_t)((height - 6) * rand() / (RAND_MAX + 1.0));
	Fruit.push_back(x);
	Fruit.push_back(y);
	zone[x + y * width] = '%';
	
}

void Zone::Clear()
{
	for (size_t i = 0; i < width * height; i++)
	{

		if (i >= 3 * width && i < width * (height - 1) && i % width == 0)
		{
			zone[i] = '|';
			zone[i - 1] = '|';
		}

		else if (i < width || (2 * width <= i && i < 3 * width) || (height - 1) * width <= i && i <= height * width)
		{
			zone[i] = '=';
		}

		else
		{
			zone[i] = ' ';
		}
	}
	zone[3 * width - 1] = '='; //lazy solutions to small details
	zone[width * (height - 1) - 1] = '|';
}

void Zone::Display()
{
	std::cout << zone << std::endl;
}

void Zone::SpawnFruit(size_t x, size_t y)
{
	zone[Fruit[0] + Fruit[1] * width] = ' ';
	Fruit[0] = x;
	Fruit[1] = y;
	zone[Fruit[0] + Fruit[1] * width] = '%';
}

void Zone::SpawnFruit()
{
	zone[Fruit[0] + Fruit[1] * width] = ' ';

	std::uniform_int_distribution<int> xdistribution(4, width - 5);
	std::uniform_int_distribution<int> ydistribution(6, height - 5);

	Fruit[0] = xdistribution(FruitGen);
	Fruit[1] = ydistribution(FruitGen);

	while (zone[Fruit[0] + Fruit[1] * width] == 'O' || zone[Fruit[0] + Fruit[1] * width] == '@')
	{
		Fruit[0] = xdistribution(FruitGen);
		Fruit[1] = ydistribution(FruitGen);
	}

	zone[Fruit[0] + Fruit[1] * width] = '%';
}

Snake::Snake()
{
	size_t _sizes[] = { 24, 16, 3 };
	brain = Network(3, _sizes);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	direction = 'S';
	Matrix _surroundings(24, 1);
	Surroundings = _surroundings;
	Surroundings = Matrix::uniform(Surroundings, 0);
}

void Snake::Move(Zone& zone)
{
	movesleft--;
	movesdone++;
	std::vector<size_t> h(0);
	size_t x = 0;
	size_t y = 0;
	zone.zone[body[size - 1][0] + zone.width * body[size - 1][1]] = ' ';
	zone.zone[body[0][0] + zone.width * body[0][1]] = 'O';
	
	switch (direction)
	{
	case 'W':
		body[size - 1].pop_back();
		x = body[0][0] - 1;
		h.push_back(x);
		y = body[0][1];
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'N':
		body[size - 1].pop_back();
		x = body[0][0];
		h.push_back(x);
		y = body[0][1] - 1;
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'E':
		body[size - 1].pop_back();
		x = body[0][0] + 1;
		h.push_back(x);
		y = body[0][1];
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;

	case 'S':
		body[size - 1].pop_back();
		x = body[0][0];
		h.push_back(x);
		y = body[0][1] + 1;
		h.push_back(y);
		body.insert(body.begin(), h);
		zone.zone[body[0][0] + zone.width * body[0][1]] = '@'; break;
	}
}

void Snake::Decide(Zone& zone)
{
	brain.FeedForward(Surroundings);
	Matrix output(3, 1);
	output = brain.GetOutput();

	double max = -100;
	size_t decision = 0;

	if (movesleft == 0) { dead = true; zone.Clear(); }

	for (size_t i = 0; i < 3; i++)
	{
		if (max <= output.at(i, 0))
		{
			max = output.at(i, 0);
			decision = i;
		}
	}

	if(decision == 0)
	{
		if (direction == 'W') {direction = 'S';}
		else if (direction == 'N') { direction = 'W';}
		else if (direction == 'E') {direction = 'N';}
		else{ direction = 'E';}
	}

	else if (decision == 2)
	{
		if (direction == 'W') { direction = 'N';}
		else if (direction == 'N') {direction = 'E';}
		else if (direction == 'E'){direction = 'S';}
		else {direction = 'W';}
	}

	switch (direction)
	{
	case 'W':
		if (zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
		}break;
	}
}

void Snake::See(Zone& zone) //returns the distance to anything if there's anything (wall/body/fruit)
{
	double dist = 0;
	Surroundings = Matrix::uniform(Surroundings, 0);
	
	for (size_t i = 0; i < body[0][0]; i++) //West 
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(18, 0) = 1; break;
			case 'E': Surroundings.at(12, 0) = 1; break;
			case 'S': Surroundings.at(6, 0) = 1; break;
			case 'W':  Surroundings.at(0, 0) = 1; break;
			}
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(19, 0) = 1 - double(dist) / zone.width; break;
			case 'E': Surroundings.at(13, 0) = 1 - double(dist) / zone.width; break;
			case 'S': Surroundings.at(7, 0) = 1 - double(dist) / zone.width; break;
			case 'W': Surroundings.at(1, 0) = 1 - double(dist) / zone.width; break;
			}
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(20, 0) = 1 - double(dist) / zone.width; break;
			case 'E': Surroundings.at(14, 0) = 1 - double(dist) / zone.width; break;
			case 'S': Surroundings.at(8, 0) = 1 - double(dist) / zone.width; break;
			case 'W': Surroundings.at(2, 0) = 1 - double(dist) / zone.width; break;
			}
		}
	}
	
	for (size_t i = zone.width - 1; i > body[0][0]; i--) //East
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(6, 0) = 1; break;
			case 'E': Surroundings.at(0, 0) = 1; break;
			case 'S': Surroundings.at(18, 0) = 1; break;
			case 'W': Surroundings.at(12, 0) = 1; break;
			}
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(7, 0) = 1 - double(dist) / zone.width; break;
			case 'E': Surroundings.at(1, 0) = 1 - double(dist) / zone.width; break;
			case 'S': Surroundings.at(19, 0) = 1 - double(dist) / zone.width; break;
			case 'W': Surroundings.at(13, 0) = 1 - double(dist) / zone.width; break;
			}
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = sub(i, body[0][0]);
			switch (direction)
			{
			case 'N': Surroundings.at(8, 0) = 1 - double(dist) / zone.width; break;
			case 'E': Surroundings.at(2, 0) = 1 - double(dist) / zone.width; break;
			case 'S': Surroundings.at(20, 0) = 1 - double(dist) / zone.width; break;
			case 'W': Surroundings.at(14, 0) = 1 - double(dist) / zone.width; break;
			}
		}
	}
	
	for (size_t i = 2; i < body[0][1]; i++) //North
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(0, 0) = 1; break;
			case 'E': Surroundings.at(18, 0) = 1; break;
			case 'S': Surroundings.at(12, 0) = 1; break;
			case 'W': Surroundings.at(6, 0) = 1; break;
			}
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(1, 0) = 1 - double(dist) / zone.height; break;
			case 'E': Surroundings.at(19, 0) = 1 - double(dist) / zone.height; break;
			case 'S': Surroundings.at(13, 0) = 1 - double(dist) / zone.height; break;
			case 'W': Surroundings.at(7, 0) = 1 - double(dist) / zone.height; break;
			}
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '=')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(2, 0) = 1 - double(dist) / zone.height; break;
			case 'E': Surroundings.at(20, 0) = 1 - double(dist) / zone.height; break;
			case 'S': Surroundings.at(14, 0) = 1 - double(dist) / zone.height; break;
			case 'W': Surroundings.at(8, 0) = 1 - double(dist) / zone.height; break;
			}
		}
	}

	for (size_t i = zone.height - 1; i > body[0][1]; i--) //South
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(12, 0) = 1; break;
			case 'E': Surroundings.at(6, 0) = 1; break;
			case 'S': Surroundings.at(0, 0) = 1; break;
			case 'W': Surroundings.at(18, 0) = 1; break;
			}
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(13, 0) = 1 - double(dist) / zone.height; break;
			case 'E': Surroundings.at(7, 0) = 1 - double(dist) / zone.height; break;
			case 'S': Surroundings.at(1, 0) = 1 - double(dist) / zone.height; break;
			case 'W': Surroundings.at(19, 0) = 1 - double(dist) / zone.height; break;
			}
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '=')
		{
			dist = sub(i, body[0][1]);
			switch (direction)
			{
			case 'N': Surroundings.at(14, 0) = 1 - double(dist) / zone.height; break;
			case 'E': Surroundings.at(8, 0) = 1 - double(dist) / zone.height; break;
			case 'S': Surroundings.at(2, 0) = 1 - double(dist) / zone.height; break;
			case 'W': Surroundings.at(20, 0) = 1 - double(dist) / zone.height; break;
			}
		}
	} 
	/*
	dist = 0;
	size_t i = 1;
	
	while (dist == 0 || i < min(zone.width - body[0][0], body[0][1] - 3)) //North-east
	{
		if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '%')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(3, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(21, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(15, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(9, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == 'O')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(4, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(22, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(16, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(10, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '=' || zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '|')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(5, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(23, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(17, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(11, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		i++;
	}
	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(body[0][0], zone.height - body[0][1])) //South-West
	{
		if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '%')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(15, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(9, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(3, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(21, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == 'O')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(16, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(10, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(4, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(22, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '=' || zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '|')
		{ 
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(17, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(11, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(5, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(23, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		i++;
	}
	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(body[0][0], body[0][1] - 3)) //North-West
	{
		if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '%')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(21, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(15, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(9, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(3, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == 'O')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(22, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(16, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(10, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(4, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '=' || zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '|')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(23, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(17, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(11, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(5, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		i++;
	}

	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(zone.width - body[0][0], zone.height - body[0][1])) //South-East
	{
		if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '%')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(9, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(3, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(21, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(15, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == 'O')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(10, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(4, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(22, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(16, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '=' || zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '|')
		{
			dist = sub(body[0][1], i) + sub(body[0][0], i);
			switch (direction)
			{
			case 'N': Surroundings.at(11, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'E': Surroundings.at(5, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'S': Surroundings.at(23, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			case 'W': Surroundings.at(17, 0) = 1 - double(dist) / (zone.height + zone.width); break;
			}
		}
		i++;
	}*/
}

void Snake::Grow(Zone& zone)
{
	std::vector<size_t> h(0);
	switch (direction)
	{
	case 'W':
		if (zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '%')
		{
			size++;
			growcount++;
			hasgrown = 1;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] - 1);
			h.push_back(body[0][1]);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
			zone.SpawnFruit();
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '%')
		{
			size++;
			growcount++;
			hasgrown = 1;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] - 1));
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
			zone.SpawnFruit();
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '%')
		{
			size++;
			growcount++;
			hasgrown = 1;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] + 1);
			h.push_back(body[0][1]);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
			zone.SpawnFruit();
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '%')
		{
			size++;
			growcount++;
			hasgrown = 1;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] + 1));
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
			zone.SpawnFruit();
		}break;

	}
}

void Snake::Play(Zone& zone)
{
	/*
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,4);
	size_t dir = distribution(generator);

	switch (dir)
	{
	case 1:
		direction = 'W'; break;
	case 2:
		direction = 'N'; break;
	case 3:
		direction = 'E'; break;
	case 4:
		direction = 'S'; break;
	};
	*/

	size_t x = zone.width / 4;
	size_t y = 5;
	direction = 'E';

	std::vector<size_t> head;
	head.push_back(x);
	head.push_back(y);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	body.push_back(head);
	zone.zone[head[0] + head[1] * zone.width] = '@';

	std::vector<size_t> part(2, 0);

	switch (direction)
	{
	case 'W':
		part[0] = body[0][0] + 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;

	case 'N':
		part[0] = body[0][0];
		part[1] = body[0][1] + 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;

	case 'E':
		part[0] = body[0][0] - 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;


	case 'S':
		part[0] = body[0][0];
		part[1] = body[0][1] - 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;
	}
	
	while (not(dead))
	{
		See(zone);
		Decide(zone);
		if (not(dead))
		{
			Move(zone);
		}
		Grow(zone);

		//fitness = movesdone * movesdone * 2^(growcount);
		fitness = growcount;
	}
}

void Snake::PlayToShow(Zone& zone)
{
	/*
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 4);
	size_t dir = distribution(generator);
	size = 4;
	
	switch (dir)
	{
	case 1:
		direction = 'W'; break;
	case 2:
		direction = 'N'; break;
	case 3:
		direction = 'E'; break;
	case 4:
		direction = 'S'; break;
	};

	/*size_t x = 6 + (size_t)((zone.width - 10) * rand() / (RAND_MAX + 1.0));
	size_t y = 8 + (size_t)((zone.height - 11) * rand() / (RAND_MAX + 1.0));*/

	size = 4;
	size_t x = zone.width / 4;
	size_t y = 5;
	direction = 'E';

	std::vector<size_t> head;
	head.push_back(x);
	head.push_back(y);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	body.push_back(head);
	zone.zone[head[0] + head[1] * zone.width] = '@';

	std::vector<size_t> part(2, 0);

	switch (direction)
	{
	case 'W':
		part[0] = body[0][0] + 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;

	case 'N':
		part[0] = body[0][0];
		part[1] = body[0][1] + 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]++;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;

	case 'E':
		part[0] = body[0][0] - 1;
		part[1] = body[0][1];
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[0]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;


	case 'S':
		part[0] = body[0][0];
		part[1] = body[0][1] - 1;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O';
		part[1]--;
		body.push_back(part);
		zone.zone[part[0] + part[1] * zone.width] = 'O'; break;
	}

	
	HANDLE hconsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hconsole);
	DWORD dwbyteswritten = 0;

	while (not(dead))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		std::wstring zone_ = std::wstring(zone.zone.begin(), zone.zone.end());
		const wchar_t* screen = zone_.c_str();
		WriteConsoleOutputCharacter(hconsole, screen, zone.width * zone.height, { 0,0 }, &dwbyteswritten);

		See(zone);
		Decide(zone);
		Grow(zone);
		if (not(dead))
		{
			Move(zone);
		}
	}
}