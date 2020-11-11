#include <iostream>
#include <fstream>
#include <vector>
#include <thread>

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
			throw new exception("can't calculate a det of non-square matrix");

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

void detWrapper(const Matrix& m, double& det) 
{
	det = Matrix::det(m);
}

void sqsum(int iTread, int iTN, double* arr, int size, double& sum) {
	for (int i = iTread; i < size; i += iTN) {
		//*sum += arr[i] * arr[i];
		sum += arr[i] * arr[i];
	}
}


void solve(Matrix& m) 
{
	thread* thr[5];
	double x[5];
	double sum[5];

	for (int i = 0; i < 5; ++i)
	{
		Matrix temp = m.excludeCol(i);

		//temp.print();
		//cout << endl;
		//thr[i] = new std::thread{ sqsum, i, 5, x, 5, std::ref(sum[i]) };
		thr[i] = new thread{ detWrapper, temp, ref(x[i])};
	}

	for (int i = 0; i < 5; ++i)
	{
		thr[i]->join();
		delete thr[i];
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
	//int argc = 2;
	//string argv[] = { "asd", "C:\\Users\\maxma\\Desktop\\Ass\\task03\\tests\\test1.txt" };

	if (argc != 2) {
		throw new invalid_argument("Only one parameter - path to test - expected");
	}
	//cout << "Input format - four strings, each contains coeffs\n For example, for {i} string it is 'ai1 ai2 ai3 ai4 bi'\n";

	ifstream in(argv[1]);
	Matrix m(5, 4);
	
	if (!in.is_open())
		throw new exception("Can't get the file!");

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 5; ++j)
			in >> m(i, j);

	cout << "Input coeffs:\n";
	m.print();
	cout << endl;
	//cout << "---\n";
	//m.excludeCol(-1).print();

	solve(m);
}