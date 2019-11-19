#pragma once

class Matrix
{
	friend class Genome;
	friend class Network;
public:
	Matrix(); 
	Matrix(size_t a, size_t b); //creates a matrix with undefined values
	Matrix(size_t a, size_t b, double inf, double sup); //creates a matrix filled with numbers between inf and sup
	Matrix(size_t a, size_t b, double* tab); //creates a matrix from a double array
	Matrix(const Matrix& other); 
	~Matrix(); 

	double at(size_t i, size_t j) const; //access to an element of the matrix
	double& at(size_t i, size_t j); 
	double getHeight();
	double getWidth();
	void setValues(double* tab); //fill the matrix from a double array

	bool operator==(const Matrix& other);
	Matrix operator+(const Matrix& other) const;
	Matrix& operator=(const Matrix& other);
	Matrix operator*(const Matrix& other) const; //term to term (or hadamard) product
	Matrix& operator+=(const Matrix& other); 
	Matrix& operator-=(const Matrix& other);

	static Matrix dot(const Matrix& A, const Matrix& B); //dot product
	static Matrix uniform(const Matrix& A, double val);
	static Matrix sigmoid(const Matrix& A);
	static Matrix arctan(const Matrix& A);
	void display(); //displays the matrix

private:
	size_t Height, Width;
	double* values;

};