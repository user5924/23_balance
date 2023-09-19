// 2_3_tree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
using  namespace std;

#include "2_3_tree.h"


void sleep(_int64 time)
{
	_int64 start = clock();
	_int64 finish = start + CLOCKS_PER_SEC * time / 1000;

	while (clock() < finish);
}

int main()
{
	srand(time(0));
	two_three_tree <float> test;

	int max = 200000;
	int min = 0;
	int i;
	char* buff;
	fstream f;
	f.open("output.txt", ios::out | ios::trunc);
	vector <char*> folder;

	for (i = 0; i < max; i += 1)
	{
		buff = rand_str(10);
		folder.push_back(copy_str(buff));
		test.add(buff, i);
		delete[] buff;
	}

	///for (i = 0; i < folder.size(); i += 1)
		//delete []folder[i];
	//folder.clear();
	//test.clear();
	//test.post(f);

	///buff = int_to_str(456);
	//cout << test.get_value(buff);
	//free(buff);
	for (i = 0; i < max * 2; i += 1)
	{
		int first = rand() % max;
		int second = rand() % max;
		swap(folder[first], folder[second]);
	}
	for (i = 0; i < max; i += 1)
	{
		test.remove(folder[i]);
		free(folder[i]);
	}
	folder.clear();

	sleep(1000);

	test.post(f);
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
