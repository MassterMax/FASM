#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

class Matrix // Класс матриц
{
private:
	int n, m; // n columns and m rows
	vector<double> data;

public:
	Matrix(int cols, int rows)
	{
		n = cols;
		m = rows;
		data = vector<double>(n * m);
	}

	double& operator()(int i, int j)
	{
		return data[i * n + j];
	}

	double operator()(int i, int j) const
	{
		return data[i * n + j];
	}

	void print() const
	{
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j)
				cout << this->operator()(i, j) << "\t";
			cout << endl;
		}
	}

	bool isSquare() const { return n == m && n > 0; }
	int getN() const { return n; }
	int getM() const { return m; }

	Matrix excludeCol(int q) const // returns original matrix without column q
	{
		Matrix temp(getN() - 1, getM());
		int i = 0, j = 0; // here we save indexes of elements in temp

		if (q == -1)
			q = n - 1;

		for (int row = 0; row < m; ++row)
		{
			for (int col = 0; col < n; ++col)
			{
				if (col != q)
				{
					temp(i, j) = this->operator()(row, col);
					j++;

					if (j == n - 1)
					{
						j = 0;
						i++;
					}
				}
			}
		}
		return temp;
	}

	Matrix cofactor(int p, int q) const
	{
		Matrix temp(getN() - 1, getM() - 1);
		int i = 0, j = 0; // here we save indexes of elements in temp

		for (int row = 0; row < m; ++row)
		{
			for (int col = 0; col < n; ++col)
			{
				if (row != p && col != q)
				{
					temp(i, j) = this->operator()(row, col);
					j++;

					if (j == n - 1)
					{
						j = 0;
						i++;
					}
				}
			}
		}
		return temp;
	}

	static double det(const Matrix& m)
	{
		if (!m.isSquare())
			throw new exception();

		double D = 0;
		int n = m.getN();

		if (n == 1)
			return m(0, 0);

		Matrix temp(n - 1, n - 1);
		int sign = 1;

		for (int j = 0; j < n; j++)
		{
			temp = m.cofactor(0, j);
			D += sign * m(0, j) * Matrix::det(temp);
			sign = -sign;
		}

		return D;
	}
};

void solve(Matrix& m)
{
	double x[5];

	#pragma omp parallel num_threads(5)
	{
		#pragma omp for
		for (int i = 0; i < 5; ++i)
		{
			Matrix temp = m.excludeCol(i);
			x[i] = Matrix::det(temp);
		}

	}

	if (x[4] == 0)
		cout << "Det(A) = 0! Kramer's method is useless here (the system has 0 or +oo number of solutions)" << endl;
	else
	{
		cout << "x1 = " << -x[0] / x[4] << endl;
		cout << "x2 = " << x[1] / x[4] << endl;
		cout << "x3 = " << -x[2] / x[4] << endl;
		cout << "x4 = " << x[3] / x[4] << endl;
	}

	return;
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		cout << "Your input is incorrect, only one argument (input data path) expected." << endl;
		return -1;
	}

	ifstream in(argv[1]);
	Matrix m(5, 4);

	if (!in.is_open()) {
		cout << "Can't find the file in a directory: " << argv[1] << endl;
		return -1;
	}

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 5; ++j)
			in >> m(i, j);

	cout << "Input coeffs:\n";
	m.print();
	cout << endl;

	solve(m);

	return 0;
}