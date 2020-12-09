#include <pthread.h>
#include <Windows.h>
#include <semaphore.h>
#include <vector>
#include <string>

sem_t semaphore;
std::vector<int> arr;

int arrSize;

int readersCnt;
int writersCnt;

int iterCnt = 0;
int iterMaxCnt;

int findEl(std::vector<int>* arr, int value)
{
	int m = 0;
	int l = -1;
	int r = arr->size();

	while (r - l > 1)
	{
		m = (r + l) / 2;
		if (arr->at(m) > value)
			r = m;
		else
			l = m;
	}

	return r;
}

void* reader(void* args) {
	int i = *((int*)args);
	srand(i * time(0));

	while (1)
	{
		int delay = 3000 + (rand() % 50) * 100;
		int j = rand() % arrSize;
		Sleep(delay);
		printf("reader%d: db el on pos [%d]: %d, now sleep for %d ms\n", i, j, arr[j], delay);

	}

	return NULL;
}

void* writer(void* args) {
	int i = *((int*)args);
	srand(-i * time(0));

	while (iterCnt < iterMaxCnt)
	{
		sem_wait(&semaphore);

		if (iterCnt < iterMaxCnt)
		{
			iterCnt++;
			int el = (rand() % arrSize) * 2 - arrSize;
			int pos = findEl(&arr, el);
			if (pos == arrSize) pos--;

			printf("writer%d changes db el on pos [%d] from %d to %d\n", i, pos, arr[pos], el);
			arr[pos] = el;

			Sleep(1000 + (rand() % 5) * 500);
		}

		sem_post(&semaphore);
	}

	return NULL;
}

void solve()
{
	printf("Starting database (%d elements):", arrSize);
	for (int i = 0; i < arrSize; ++i)
	{
		arr.push_back(i);
		printf(" %d", i);
	}
	printf("\n");

	pthread_t* p_readers = new pthread_t[readersCnt];
	pthread_t* p_writers = new pthread_t[writersCnt];

	int* readers = new int[readersCnt];
	int* writers = new int[writersCnt];

	sem_init(&semaphore, 0, 1);

	for (int i = 0; i < writersCnt; ++i)
	{
		writers[i] = i + 1;
		pthread_create(&p_writers[i], NULL, writer, (void*)(writers + i));
	}

	for (int i = 0; i < readersCnt; ++i)
	{
		readers[i] = i + 1;
		pthread_create(&p_readers[i], NULL, reader, (void*)(readers + i));
	}

	while (iterCnt < iterMaxCnt)
	{
		Sleep(100);
	}

	delete[] p_readers;
	delete[] p_writers;
	delete[] readers;
	delete[] writers;

	printf("Result database (%d elements):", arrSize);
	for (int i = 0; i < arrSize; ++i)
	{
		printf(" %d", arr[i]);
	}

	printf("\n");
}

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		printf("Error: four parameters expected in format: {readersCnt} {writersCnt} {size of database} {max number of iterations}");
		return 0;
	}

	readersCnt = std::stoi(argv[1]);
	writersCnt = std::stoi(argv[2]);
	arrSize = std::stoi(argv[3]);
	iterMaxCnt = std::stoi(argv[4]);

	if (readersCnt > 100 || readersCnt < 0 || writersCnt > 100 || writersCnt < 0 || arrSize > 100 || arrSize < 0 || iterMaxCnt > 100 || iterMaxCnt < 0)
	{
		printf("Error: all parameters should be integers in range [1, 100]");
		return 0;
	}

	solve();

	return 0;
}