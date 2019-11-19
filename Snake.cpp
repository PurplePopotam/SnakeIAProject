#include "Snake.h"
#include <cstdlib> 
#include <ctime>
#include <iostream>



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
	
	size_t x = 2 + (size_t)((width - 6) * rand() / (RAND_MAX + 1.0));
	size_t y = 4 + (size_t)((height - 5) * rand() / (RAND_MAX + 1.0));

	while (zone[x + y * width] == 'O' || zone[x + y * width] == '@')
	{
		x = 2 + (size_t)((width - 6) * rand() / (RAND_MAX + 1.0));
		y = 4 + (size_t)((height - 5) * rand() / (RAND_MAX + 1.0));
	}
	
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

void Zone::SpawnFruit()
{
	zone[Fruit[0] + Fruit[1] * width] = ' ';
	Fruit[0] = 2 + (size_t)((width - 4) * rand() / (RAND_MAX + 1.0));
	Fruit[1] = 4 + (size_t)((height - 6) * rand() / (RAND_MAX + 1.0));

	while (zone[Fruit[0] + Fruit[1] * width] == 'O' || zone[Fruit[0] + Fruit[1] * width] == '@')
	{
		Fruit[0] = 2 + (size_t)((width - 4) * rand() / (RAND_MAX + 1.0));
		Fruit[1] = 4 + (size_t)((height - 6) * rand() / (RAND_MAX + 1.0));
	}
	zone[Fruit[0] + Fruit[1] * width] = '%';
}

Snake::Snake()
{
	fitness = 0;
	size_t _sizes[] = { 24, 10, 3 };
	brain = Network(3, _sizes);
	std::vector<std::vector<size_t>> _body(0);
	body = _body;
	direction = 'S';
	Matrix _surroundings(24, 1);
	Surroundings = _surroundings;
	Surroundings = Matrix::uniform(Surroundings, 0);
}

Snake::Snake(Zone& zone)
{
	fitness = 0;
	size = 4;
	size_t dir = 1 + (size_t)(4 * rand() / (RAND_MAX + 1.0));
	size_t _sizes[] = {24, 10, 3};

	brain = Network(3, _sizes);
	Matrix _surroundings(24, 1);
	Surroundings = _surroundings;
	Surroundings = Matrix::uniform(Surroundings, 0);
	
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
	size_t x = 50;
	size_t y = 12;
	

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

}

void Snake::Move(Zone& zone)
{
	movesleft--;
	fitness++;
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
	double max = 0;
	size_t decision = 0;

	for (size_t i = 0; i < 3; i++)
	{
		if (max < output.at(i, 0))
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
			zone.SpawnFruit();
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
			zone.SpawnFruit();
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
			zone.SpawnFruit();
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
			zone.SpawnFruit();
		}break;
	}

}

void Snake::See(Zone& zone)
{
	double dist; //returns 1 for no object in this direction, a normalized distance if there's anything (wall/body/fruit)

	for (size_t i = 0; i < body[0][0]; i++) //West 
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(0, 0) = zone.width - dist ;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(1, 0) = zone.width - dist;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(2, 0) = zone.width - dist;
		}
	}
	
	for (size_t i = zone.width - 1; i > body[0][0]; i--) //East
	{
		if (zone.zone[i + zone.width * body[0][1]] == '%')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(3, 0) = zone.width - dist;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == 'O')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(4, 0) = zone.width - dist;
		}
		else if (zone.zone[i + zone.width * body[0][1]] == '|')
		{
			dist = sub(i, body[0][0]);
			Surroundings.at(5, 0) = zone.width - (double)dist;
		}
	}
	
	for (size_t i = 2; i < body[0][1]; i++) //North
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(6, 0) = zone.height - dist;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(7, 0) = zone.height - dist;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '=')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(8, 0) = zone.height - dist;
		}
	}

	for (size_t i = zone.height - 1; i > body[0][1]; i--) //South
	{
		if (zone.zone[body[0][0] + zone.width * i] == '%')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(9, 0) = zone.height - dist;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == 'O')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(10, 0) = zone.height - dist;
		}
		else if (zone.zone[body[0][0] + zone.width * i] == '=')
		{
			dist = sub(i, body[0][1]);
			Surroundings.at(11, 0) = zone.height - dist;
		}
	}
	
	int b = body[0][1] + body[0][0];
	dist = 0;
	size_t i = 1;
	while (dist == 0 || i < min(zone.width - body[0][0], body[0][1] - 3)) //North-east
	{
		if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '%')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(12, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == 'O')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(13, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '=' || zone.zone[body[0][0] + i + zone.width * (body[0][1] - i)] == '|')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(14, 0) = zone.height + zone.width - dist;
		}
		i++;
	}
	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(body[0][0], zone.height - body[0][1])) //South-West
	{
		if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '%')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(15, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == 'O')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(16, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '=' || zone.zone[body[0][0] - i + zone.width * (body[0][1] + i)] == '|')
		{ 
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(17, 0) = zone.height + zone.width - dist;
		}
		i++;
	}
	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(body[0][0], body[0][1] - 3)) //North-West
	{
		if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '%')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(18, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == 'O')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(19, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '=' || zone.zone[body[0][0] - i + zone.width * (body[0][1] - i)] == '|')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(20, 0) = zone.height + zone.width - dist;
		}
		i++;
	}
	
	dist = 0;
	i = 1;
	while (dist == 0 || i < min(zone.width - body[0][0], zone.height - body[0][1])) //South-East
	{
		if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '%')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(21, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == 'O')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(22, 0) = zone.height + zone.width - dist;
		}
		else if (zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '=' || zone.zone[body[0][0] + i + zone.width * (body[0][1] + i)] == '|')
		{
			dist = sub(body[0][1], i / zone.width) + sub(body[0][0], i % zone.width);
			Surroundings.at(23, 0) = zone.height + zone.width - dist;
		}
		i++;
	}

}

void Snake::Check(Zone& zone)
{
	if (movesleft == 0) { dead = true; zone.Clear(); zone.SpawnFruit(); };

	switch (direction)
	{
	case 'W':
		if (zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] - 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true; 
			zone.Clear();
			zone.SpawnFruit();
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == 'O')
		{
			dead = true;
			zone.Clear();
			zone.SpawnFruit();
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '|' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '=' || zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == 'O')
		{
			dead = true; 
			zone.Clear();
			zone.SpawnFruit();
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '|' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '=' || zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == 'O')
		{
			dead = true; 
			zone.Clear();
			zone.SpawnFruit();
		}break;
	}
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
			fitness += 100;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] - 1);
			h.push_back(body[0][1]);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'N':
		if (zone.zone[body[0][0] + (body[0][1] - 1) * zone.width] == '%')
		{
			size++;
			fitness +=100;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] - 1));
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'E':
		if (zone.zone[body[0][0] + 1 + body[0][1] * zone.width] == '%')
		{
			size++;
			fitness += 100;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0] + 1);
			h.push_back(body[0][1]);
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	case 'S':
		if (zone.zone[body[0][0] + (body[0][1] + 1) * zone.width] == '%')
		{
			size++;
			fitness += 100;
			movesleft = 200;
			zone.zone[body[0][0] + body[0][1] * zone.width] = 'O';
			h.push_back(body[0][0]);
			h.push_back((body[0][1] + 1));
			body.insert(body.begin(), h);
			zone.zone[body[0][0] + body[0][1] * zone.width] = '@';
		}break;

	}
}

void Snake::Play(Zone& zone)
{
	while (not(dead))
	{
		See(zone);
		Grow(zone);
		Decide(zone);
		if (not(dead))
		{
			Move(zone);
		}
	}
}

void Snake::PlayToShow(Zone& zone)
{
	while (not(dead))
	{
		See(zone);
		Grow(zone);
		Decide(zone);
		if (not(dead))
		{
			Move(zone);
		}
		zone.Display();

	}
}