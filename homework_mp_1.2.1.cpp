//Задание 2
// 05.03.2024
//Параллельные вычисления

//Напишите программу для расчёта суммы двух векторов.
//Распараллельте эту программу на 2, 4, 8 и 16 потоков.
//Определите, какое количество потоков даёт самый быстрый результат.
//Сравните результаты выполнения для массивов из 1 000, 10 000, 100 000 и 1 000 000 элементов.
//Время выполнения для каждого варианта сведите в таблицу и выведите её в консоль.
//Первый запущенный поток должен вывести на экран доступное количество аппаратных ядер.

#include <iostream>
#include <thread>
#include <execution>
#include <algorithm>
#include <random>
#include <chrono>
#include <vector>

//const int size = 1000;

// делаем в один поток
auto sum_vec1(std::vector<int> vec1, std::vector<int> vec2, int size)
{
	std::vector<int> sum_vec(size);
	// здесь мы замеряем время старта
	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < size; ++i)
	{
		sum_vec[i] = vec1[i] + vec2[i];

	}
	// здесь мы замеряем время завершения работы
	auto end = std::chrono::steady_clock::now();
	// здесь время работы программы от начала до конца
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto run_time = diff.count();

	return run_time;
}

//делаем в два потока

int run_time_2_streams = 0;

void sum_vec_2_streams(std::vector<int> vec1, std::vector<int> vec2, int size, int begin, int end)
{
	// создаём вектор, куда будем класть сумму двух векторов
	std::vector<int> sum_vec(size);
	// здесь мы замеряем время старта
	auto start = std::chrono::steady_clock::now();

	for (int i = begin; i < end; ++i)
	{
		sum_vec[i] = vec1[i] + vec2[i];

	}
	// здесь мы замеряем время завершения работы
	auto finish = std::chrono::steady_clock::now();
	// здесь время работы программы от начала до конца
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	auto run_time21 = diff.count();

	// кладём время в глобальную переменную
	run_time_2_streams = run_time21 + run_time_2_streams;
}

//делаем в четыре потока

int run_time_4_streams = 0;

void sum_vec_4_streams(std::vector<int> vec1, std::vector<int> vec2, int size, int begin, int end)
{
	std::vector<int> sum_vec(size);
	// здесь мы замеряем время старта
	auto start = std::chrono::steady_clock::now();

	for (int i = begin; i < end; ++i)
	{
		sum_vec[i] = vec1[i] + vec2[i];

	}
	// здесь мы замеряем время завершения работы
	auto finish = std::chrono::steady_clock::now();
	// здесь время работы программы от начала до конца
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	auto run_time41 = diff.count();

	// кладём время в глобальную переменную
	run_time_4_streams = run_time41 + run_time_4_streams;
}

int run_time_8_streams = 0;

void sum_vec_8_streams(std::vector<int> vec1, std::vector<int> vec2, int size, int begin, int end)
{
	//создаём вектор - сумму дву векторов
	std::vector<int> sum_vec(size);
	// здесь мы замеряем время старта
	auto start = std::chrono::steady_clock::now();

	for (int i = begin; i < end; ++i)
	{
		sum_vec[i] = vec1[i] + vec2[i];
	}
	// здесь мы замеряем время завершения работы
	auto finish = std::chrono::steady_clock::now();
	// здесь время работы программы от начала до конца
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	auto run_time8 = diff.count();

	// кладём время в глобальную переменную
	run_time_8_streams = run_time8 + run_time_8_streams;
}

// функция заполнения вектора случайными числами

auto random_vec(int size)
{
	std::vector<int> vec(size);
	std::mt19937 gen;
	std::uniform_int_distribution<int> dist(0, size);
	std::generate(std::execution::par, vec.begin(), vec.end(), [&]() {
		return dist(gen);
		});

	return vec;
}

int main()
{
	setlocale(LC_ALL, "ru");
	std::cout << "Количество аппаратных ядер: " << std::thread::hardware_concurrency() << std::endl;
	int size = 1000;	
	
	// создаём первый вектор
	std::vector<int> vec1 = random_vec(size);
	// создаём второй вектор
	std::vector<int> vec2 = random_vec(size);

	// Запускаем в один поток
	double run_time1 = sum_vec1(vec1, vec2, size);

	// Запускаем в два потока
	int begin = 0;
	int end = size / 2;
	std::thread rt_1_2_1k(sum_vec_2_streams, vec1, vec2, size, begin,end);

	begin = size / 2;
	end = size;
	std::thread rt_2_2_1k(sum_vec_2_streams, vec1, vec2, size, begin, end);

	rt_1_2_1k.join();
	rt_2_2_1k.join();

	// Объявляем массив для хранения времени параллельного исполнения в два потока

	int* time_2_streams = new int[4];
	time_2_streams[0] = run_time_2_streams;

	// запускаем 4 потока

	begin = 0;
	end = size / 4;
	std::thread rt_1_4_1k(sum_vec_4_streams, vec1, vec2, size, begin, end);
	begin = size / 4;
	end = size / 2;
	std::thread rt_2_4_1k(sum_vec_4_streams, vec1, vec2, size, begin, end);
	begin = size / 2;
	end = 3 * size / 4;
	std::thread rt_3_4_1k(sum_vec_4_streams, vec1, vec2, size, begin, end);
	begin = 3 * size / 4;
	end = size;
	std::thread rt_4_4_1k(sum_vec_4_streams, vec1, vec2, size, begin, end);

	rt_1_4_1k.join();
	rt_2_4_1k.join();
	rt_3_4_1k.join();
	rt_4_4_1k.join();

	// Объявляем массив для хранения времени параллельного исполнения в четыре потока
	int* time_4_streams = new int[4];

	// Сохраняем текущее значение времени исполнения в массив
	time_4_streams[0] = run_time_8_streams;

	// запускаем 8 потоков

	begin = 0;
	end = 1 * size / 8;
	std::thread rt_1_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 1 * size / 8;
	end = 2 * size / 8;
	std::thread rt_2_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 2 * size / 8;
	end = 3 * size / 8;
	std::thread rt_3_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 3 * size / 8;
	end = 4 * size /8;
	std::thread rt_4_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 4 * size / 8;;
	end = 5 * size / 8;
	std::thread rt_5_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 5 * size / 8;
	end = 6 * size / 8;
	std::thread rt_6_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 6 * size / 8;
	end = 7 * size / 8;
	std::thread rt_7_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	begin = 7 * size / 8;
	end = size;
	std::thread rt_8_8_1k(sum_vec_8_streams, vec1, vec2, size, begin, end);
	rt_1_8_1k.join();
	rt_2_8_1k.join();
	rt_3_8_1k.join();
	rt_4_8_1k.join();
	rt_5_8_1k.join();
	rt_6_8_1k.join();
	rt_7_8_1k.join();
	rt_8_8_1k.join();


	// Объявляем массив для хранения времени параллельного исполнения в восемь потоков
	int* time_8_streams = new int[4];

	// Сохраняем текущее значение времени исполнения в массив
	time_8_streams[0] = run_time_8_streams;

	size = 10000;

	// создаём третий вектор
	std::vector<int> vec3 = random_vec(size);
	// создаём четвёртый вектор
	std::vector<int> vec4 = random_vec(size);
	double run_time2 = sum_vec1(vec3, vec4, size);
	
	// Запускаем в два потока
	begin = 0;
	end = size / 2;
	std::thread rt_1_2_10k(sum_vec_2_streams, vec3, vec4, size, begin, end);
	begin = size / 2;
	end = size;
	std::thread rt_2_2_10k(sum_vec_2_streams, vec3, vec4, size, begin, end);

	rt_1_2_10k.join();
	rt_2_2_10k.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_2_streams[1] = run_time_2_streams;

	// запускаем 4 потока

	begin = 0;
	end = size / 4;
	std::thread rt_1_4_10k(sum_vec_4_streams, vec3, vec4, size, begin, end);
	begin = size / 4;
	end = size / 2;
	std::thread rt_2_4_10k(sum_vec_4_streams, vec3, vec4, size, begin, end);
	begin = size / 2;
	end = 3 * size / 4;
	std::thread rt_3_4_10k(sum_vec_4_streams, vec3, vec4, size, begin, end);
	begin = 3 * size / 4;
	end = size;
	std::thread rt_4_4_10k(sum_vec_4_streams, vec3, vec4, size, begin, end);

	rt_1_4_10k.join();
	rt_2_4_10k.join();
	rt_3_4_10k.join();
	rt_4_4_10k.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_4_streams[1] = run_time_4_streams;

	// запускаем 8 потоков

	begin = 0;
	end = 1 * size / 8;
	std::thread rt_1_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 1 * size / 8;
	end = 2 * size / 8;
	std::thread rt_2_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 2 * size / 8;
	end = 3 * size / 8;
	std::thread rt_3_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 3 * size / 8;
	end = 4 * size / 8;
	std::thread rt_4_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 4 * size / 8;;
	end = 5 * size / 8;
	std::thread rt_5_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 5 * size / 8;
	end = 6 * size / 8;
	std::thread rt_6_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 6 * size / 8;
	end = 7 * size / 8;
	std::thread rt_7_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	begin = 7 * size / 8;
	end = size;
	std::thread rt_8_8_10k(sum_vec_8_streams, vec3, vec4, size, begin, end);
	rt_1_8_10k.join();
	rt_2_8_10k.join();
	rt_3_8_10k.join();
	rt_4_8_10k.join();
	rt_5_8_10k.join();
	rt_6_8_10k.join();
	rt_7_8_10k.join();
	rt_8_8_10k.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_8_streams[1] = run_time_8_streams;

	size = 100000;
	// создаём пятый вектор
	std::vector<int> vec5 = random_vec(size);
	// создаём шестой вектор
	std::vector<int> vec6 = random_vec(size);
	int run_time3 = sum_vec1(vec5, vec6, size);

	// Запускаем в два потока
	
	begin = 0;
	end = size / 2;
	std::thread rt_1_2_100k(sum_vec_2_streams, vec5, vec6, size, begin, end);
	begin = size / 2;
	end = size;
	std::thread rt_2_2_100k(sum_vec_2_streams, vec5, vec6, size, begin, end);

	rt_1_2_100k.join();
	rt_2_2_100k.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_2_streams[2] = run_time_4_streams;

	// запускаем 4 потока

	begin = 0;
	end = size / 4;
	std::thread rt_1_4_100k(sum_vec_4_streams, vec5, vec6, size, begin, end);
	begin = size / 4;
	end = size / 2;
	std::thread rt_2_4_100k(sum_vec_4_streams, vec5, vec6, size, begin, end);
	begin = size / 2;
	end = 3 * size / 4;
	std::thread rt_3_4_100k(sum_vec_4_streams, vec5, vec6, size, begin, end);
	begin = 3 * size / 4;
	end = size;
	std::thread rt_4_4_100k(sum_vec_4_streams, vec5, vec6, size, begin, end);

	rt_1_4_100k.join();
	rt_2_4_100k.join();
	rt_3_4_100k.join();
	rt_4_4_100k.join();


	// Сохраняем текущее значение времени исполнения в массив
	time_4_streams[2] = run_time_4_streams;
	
	// запускаем 8 потоков

	begin = 0;
	end = 1 * size / 8;
	std::thread rt_1_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 1 * size / 8;
	end = 2 * size / 8;
	std::thread rt_2_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 2 * size / 8;
	end = 3 * size / 8;
	std::thread rt_3_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 3 * size / 8;
	end = 4 * size / 8;
	std::thread rt_4_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 4 * size / 8;;
	end = 5 * size / 8;
	std::thread rt_5_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 5 * size / 8;
	end = 6 * size / 8;
	std::thread rt_6_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 6 * size / 8;
	end = 7 * size / 8;
	std::thread rt_7_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	begin = 7 * size / 8;
	end = size;
	std::thread rt_8_8_100k(sum_vec_8_streams, vec5, vec6, size, begin, end);
	rt_1_8_100k.join();
	rt_2_8_100k.join();
	rt_3_8_100k.join();
	rt_4_8_100k.join();
	rt_5_8_100k.join();
	rt_6_8_100k.join();
	rt_7_8_100k.join();
	rt_8_8_100k.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_8_streams[2] = run_time_8_streams;

	size = 1000000;
	// создаём седьмой вектор
	std::vector<int> vec7 = random_vec(size);
	// создаём восьмой вектор
	std::vector<int> vec8 = random_vec(size);
	int run_time4 = sum_vec1(vec7, vec8, size);

	// Запускаем в два потока

	begin = 0;
	end = size / 2;
	std::thread rt_1_2_1m(sum_vec_2_streams, vec7, vec8, size, begin, end);
	begin = size / 2;
	end = size;
	std::thread rt_2_2_1m(sum_vec_2_streams, vec7, vec8, size, begin, end);

	rt_1_2_1m.join();
	rt_2_2_1m.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_2_streams[3] = run_time_2_streams;

	// запускаем 4 потока

	begin = 0;
	end = size / 4;
	std::thread rt_1_4_1m(sum_vec_4_streams, vec7, vec8, size, begin, end);
	begin = size / 4;
	end = size / 2;
	std::thread rt_2_4_1m(sum_vec_4_streams, vec7, vec8, size, begin, end);
	begin = size / 2;
	end = 3 * size / 4;
	std::thread rt_3_4_1m(sum_vec_4_streams, vec7, vec8, size, begin, end);
	begin = 3 * size / 4;
	end = size;
	std::thread rt_4_4_1m(sum_vec_4_streams, vec7, vec8, size, begin, end);

	rt_1_4_1m.join();
	rt_2_4_1m.join();
	rt_3_4_1m.join();
	rt_4_4_1m.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_4_streams[3] = run_time_4_streams;

	// запускаем 8 потоков

	begin = 0;
	end = 1 * size / 8;
	std::thread rt_1_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 1 * size / 8;
	end = 2 * size / 8;
	std::thread rt_2_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 2 * size / 8;
	end = 3 * size / 8;
	std::thread rt_3_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 3 * size / 8;
	end = 4 * size / 8;
	std::thread rt_4_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 4 * size / 8;;
	end = 5 * size / 8;
	std::thread rt_5_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 5 * size / 8;
	end = 6 * size / 8;
	std::thread rt_6_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 6 * size / 8;
	end = 7 * size / 8;
	std::thread rt_7_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	begin = 7 * size / 8;
	end = size;
	std::thread rt_8_8_1m(sum_vec_8_streams, vec7, vec8, size, begin, end);
	rt_1_8_1m.join();
	rt_2_8_1m.join();
	rt_3_8_1m.join();
	rt_4_8_1m.join();
	rt_5_8_1m.join();
	rt_6_8_1m.join();
	rt_7_8_1m.join();
	rt_8_8_1m.join();

	// Сохраняем текущее значение времени исполнения в массив
	time_8_streams[3] = run_time_8_streams;
	std::cout << "      " << " Время выполнения " << std::endl;
	std::cout << "Размер вектора " << " 1000   " << " 10000   " << "100000  " << "100000 " << std::endl;
	std::cout << "Один поток      " << run_time1 << " ms      " << run_time2 << " ms  " << run_time3 << " ms    " << run_time4 << " ms\n";
	std::cout << "Два потока      " << time_2_streams[0] << " ms      " << time_2_streams[1] << " ms  " << time_2_streams[2] << " ms    "
	<< time_2_streams[3] << " ms\n";
	std::cout << "Четыре потока   " << time_4_streams[0] << " ms      " << time_4_streams[1] << " ms  " << time_4_streams[2] << " ms    " 
	<< time_4_streams[3] << " ms\n";
	std::cout << "Восемь потоков  " << time_8_streams[0] << " ms      " << time_8_streams[1] << " ms  " << time_8_streams[1] << " ms    "
	<< time_4_streams[3] << " ms\n";
	delete [] time_2_streams;
	return 0;
}