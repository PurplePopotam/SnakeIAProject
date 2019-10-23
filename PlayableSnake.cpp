#include <iostream>
#include <windows.h> //Pour le buffer/la fenêtre
#include <vector> //Pour les objets snake, fruit, obstacle etc
#include <thread> //Pour "mettre en pause" le jeu afin d'avoir un framerate ~ constant
#include <chrono> //pour thread 
#include <cstdlib> //pour un générateur de nombre aléatoire
#include <ctime> //pour initialiser le générateur de nombre aléatoire


int nScreenWidth = 120;
int nScreenHeight = 30;

bool bGameOver = 0;
bool bSnakeGrew = 0;
int score = 0;

std::vector<int> coordFruit;
bool bFruit = true;

bool bDroite = false;
bool bGauche = false;
bool bDroitePrec = false;
bool bGauchePrec = false;

std::vector<std::vector<int>> snake(0);
int tailleSnake = 4;
char direction = 'W';


//Créé une zone de jeu plus réduite que l'écran
void Setup(wchar_t* screen)
{
	bGameOver = 0;
	tailleSnake = 4;

	char word[] = { 'S','N','A','K','E',' ','G','A','M','E' };

	//Bordures
	for (int i = 0; i < nScreenHeight * nScreenWidth - 1; i++)
	{
		if (i < nScreenWidth || (2 * nScreenWidth <= i && i < 3 * nScreenWidth))
		{
			screen[i] = '=';
		}

		else
		{
			screen[i] = ' ';
		}
	}

	//Titre du jeu, Score
	for (int i = 0; i <= 9; i++)
	{
		screen[nScreenWidth + 54 + i] = word[i];
	};


	//(Re-)Initialise la position de Snake et sa direction
	std::vector<std::vector<int>> newSnake(1);
	snake = newSnake;
	std::vector<int> tete(2, 55);
	tete[1] = 20;
	snake[0] = tete;
	std::vector<int> corp1(2, 56);
	corp1[1] = 20;
	snake.push_back(corp1);
	std::vector<int> corp2(2, 57);
	corp2[1] = 20;
	snake.push_back(corp2);
	std::vector<int> corp3(2, 58);
	corp3[1] = 20;
	snake.push_back(corp3);

	direction = 'W';

	//Dessine Snake
	for (int i = 0; i < tailleSnake; i++)
	{
		int coord = snake[i][0] + nScreenWidth * snake[i][1];

		if (i == 0)
		{
			screen[coord] = '@';
		}

		else
		{
			screen[coord] = 'O';
		}

	}

	//(Re-)Génère le premier fruit
	if (bFruit)
	{
		std::vector<int> newFruit;
		int x = 1 + (int)(nScreenWidth * rand() / (RAND_MAX + 1.0));
		int y = 4 + (int)((nScreenHeight - 3) * rand() / (RAND_MAX + 1.0));
		coordFruit = newFruit;
		coordFruit.push_back(x);
		coordFruit.push_back(y);
		bFruit = false;
	}

}

//Ecran de fin de partie
void EndScreen(wchar_t* screen)
{
	char word1[] = { 'G','A','M', 'E', ' ', 'O', 'V', 'E', 'R' };
	char word2[] = { 'P','O','U','R',' ','R','E','J','O','U','E','R',' ','T','A','P','E','R',' ','E','S','P','A','C','E' };

	for (int i = 0; i < nScreenHeight * nScreenWidth; i++)
	{
		screen[i] = ' ';
	}
	for (int i = 0; i < 9; i++)
	{
		screen[10 * nScreenWidth + 55 + i] = word1[i];
	}

	for (int i = 0; i < 25; i++)
	{
		screen[12 * nScreenWidth + 50 + i] = word2[i];
	}

}

//Action en fonction de l'état du buffer
void Logic(wchar_t* screen)
{
	//detection des collisions avec les bordures
	if (snake[0][1] < 4 || snake[0][1] > nScreenHeight || snake[0][0] < 0 || snake[0][0] > nScreenWidth)
	{
		bGameOver = true;
	}

	//detection des collisions avec le reste du snake
	for (int i = 1; i < tailleSnake; i++)
	{
		if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1])
		{
			bGameOver = true;
		}
	}

	//detection d'un fruit mangé, action sur le buffer en conséquence
	if (snake[0][0] == coordFruit[0] && snake[0][1] == coordFruit[1])
	{
		bSnakeGrew = 1;
		score++;
		tailleSnake++;
		std::vector<int> nouv;

		switch (direction)
		{
		case 'W':
			nouv.push_back(snake[0][0] - 1);
			nouv.push_back(snake[0][1]);
			snake.insert(snake.begin(), nouv); break;
		case 'N':
			nouv.push_back(snake[0][0]);
			nouv.push_back(snake[0][1] - 1);
			snake.insert(snake.begin(), nouv); break;
		case 'E':
			nouv.push_back(snake[0][0] + 1);
			nouv.push_back(snake[0][1]);
			snake.insert(snake.begin(), nouv); break;
		case 'S':
			nouv.push_back(snake[0][0]);
			nouv.push_back(snake[0][1] + 1);
			snake.insert(snake.begin(), nouv); break;
		}

		bFruit = true;
	}

	//Génère un nouveau fruit si besoin
	if (bFruit)
	{

		int x = 1 + (int)((float)nScreenWidth * rand() / (RAND_MAX + 1.0));
		int y = 4 + (int)(((float)nScreenHeight - 3) * rand() / (RAND_MAX + 1.0));
		coordFruit[0] = x;
		coordFruit[1] = y;
		bFruit = false;
	}

}

//fait avancer snake en fonction de sa direction, le dessine, dessine le fruit
void UpdateSnake(wchar_t* screen)
{
	//Avance Snake dans le buffer en fonction de la position
	if (direction == 'W')
	{
		snake.pop_back();
		std::vector<int> h;
		h.push_back(snake[0][0] - 1);
		h.push_back(snake[0][1]);
		snake.insert(snake.begin(), h);
	}

	if (direction == 'E')
	{
		snake.pop_back();
		std::vector<int> h;
		h.push_back(snake[0][0] + 1);
		h.push_back(snake[0][1]);
		snake.insert(snake.begin(), h);
	}

	if (direction == 'N')
	{
		snake.pop_back();
		std::vector<int> h;
		h.push_back(snake[0][0]);
		h.push_back(snake[0][1] - 1);
		snake.insert(snake.begin(), h);
	}

	if (direction == 'S')
	{
		snake.pop_back();
		std::vector<int> h;
		h.push_back(snake[0][0]);
		h.push_back(snake[0][1] + 1);
		snake.insert(snake.begin(), h);
	}

	bGauchePrec = false;
	bDroitePrec = false;

	//Nettoie la zone de jeu
	for (int i = nScreenWidth * 3; i < nScreenHeight * nScreenWidth - 1; i++)
	{
		screen[i] = ' ';
	}
	//Dessine le nouveau snake
	for (int i = 0; i < tailleSnake; i++)
	{
		int coord = snake[i][0] + nScreenWidth * snake[i][1];

		if (i == 0)
		{
			screen[coord] = '@';
		}

		else
		{
			screen[coord] = 'O';
		}

	}

	//Dessine le fruit
	screen[coordFruit[1] * nScreenWidth + coordFruit[0]] = '%';

}

//changement de direction en fonction des inputs (gauche/droite)
void Input(wchar_t* screen)
{
	//stockage de l'état des touches gauche droite dans des bools
	bDroite = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
	bGauche = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

	//changement des directions
	if (bDroite && (bDroite != bDroitePrec))
	{
		switch (direction)
		{
		case 'W': direction = 'N'; break;
		case 'N': direction = 'E'; break;
		case 'E': direction = 'S'; break;
		case 'S': direction = 'W'; break;
		}
		bDroitePrec = bDroite;
	}

	if (bGauche && (bGauchePrec != bGauche))
	{
		switch (direction)
		{
		case 'S': direction = 'E'; break;
		case 'W': direction = 'S'; break;
		case 'N': direction = 'W'; break;
		case 'E': direction = 'N'; break;

		}
		bGauchePrec = bGauche;
	}

}

int main()
{
	//Créé un buffer pour l'écran 

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	//boucle de jeu 
	while (1)
	{
		//initalise le générateur de nombre aléatoire au début de chaque partie

		srand(time(NULL));

		//Ecran de début de partie
		Setup(screen);
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));

		while (!bGameOver)
		{
			auto t0 = std::chrono::system_clock::now();
			while (std::chrono::system_clock::now() - t0 < std::chrono::microseconds(200))
			{
				Input(screen);
			}
			Logic(screen);
			UpdateSnake(screen);
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
		}


		while ((0x8000 & GetAsyncKeyState((unsigned char)('\x20'))) == 0)
		{
			EndScreen(screen);
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
		}


	}

	return 0;
}