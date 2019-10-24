#pragma once

class Matrix
{
public:
	Matrix(); //constructeur par d�faut
	Matrix(size_t a, size_t b); //cr�er une matrice sans initialiser de valeurs
	Matrix(size_t a, size_t b, double inf, double sup); //cr�er une matrice remplis de double al�atoire compris entre inf et sup
	Matrix(size_t a, size_t b, double* tab); //cr�er une matrice � partir d'un tableau de double
	Matrix(const Matrix& other); //constructeur par copie 
	~Matrix(); //destructeur

	double at(size_t i, size_t j) const; //acc�s � une case de la matrice
	double& at(size_t i, size_t j); //permet l'affectation 
	double getHeight();
	double getWidth();
	void setValues(double* tab); //remplie la matrice � partir d'un tableau de valeur

	bool operator==(const Matrix& other);
	Matrix operator+(const Matrix& other) const;
	Matrix& operator=(const Matrix& other);
	Matrix operator*(const Matrix& other) const; //produit termes � termes
	Matrix& operator+=(const Matrix& other);
	Matrix& operator-=(const Matrix& other);

	static Matrix dot(const Matrix& A, const Matrix& B); //produit matriciel
	static Matrix uniform(const Matrix& A, double val);
	static Matrix sigmoid(const Matrix& A);
	void display(); //affiche les valeurs de la matrice de mani�re naturelle

private:
	size_t Height, Width;
	double* values;

};