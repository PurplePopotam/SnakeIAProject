#pragma once

class Matrix
{
public:
	Matrix(); //constructeur par défaut
	Matrix(size_t a, size_t b); //créer une matrice sans initialiser de valeurs
	Matrix(size_t a, size_t b, double inf, double sup); //créer une matrice remplis de double aléatoire compris entre inf et sup
	Matrix(size_t a, size_t b, double* tab); //créer une matrice à partir d'un tableau de double
	Matrix(const Matrix& other); //constructeur par copie 
	~Matrix(); //destructeur

	double at(size_t i, size_t j) const; //accès à une case de la matrice
	double& at(size_t i, size_t j); //permet l'affectation 
	double getHeight();
	double getWidth();
	void setValues(double* tab); //remplie la matrice à partir d'un tableau de valeur

	bool operator==(const Matrix& other);
	Matrix operator+(const Matrix& other) const;
	Matrix& operator=(const Matrix& other);
	Matrix operator*(const Matrix& other) const; //produit termes à termes
	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);

	static Matrix dot(const Matrix& A, const Matrix& B); //produit matriciel
	static Matrix uniform(const Matrix& A, double val);
	static Matrix sigmoid(const Matrix& A);
	void display(); //affiche les valeurs de la matrice de manière naturelle

private:
	size_t Height, Width;
	double* values;

};