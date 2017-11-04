#include <chrono>
#include <math.h>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>

using namespace std::chrono;
//Функция для расчета интеграла
//с использованием cilk_plus
double integrate(const int N, const int high, const int low) {

	const double step = (double)(high - low) / N;

	cilk::reducer_opadd<double> reduce;
	reduce.set_value(0);

	cilk_for(int i = 0; i < N - 1; ++i)
	{
		reduce += 5 / sqrt(8 - 4 * (low + i*step)*(low + i*step));//считаем заданный интеграл
	}

	double result = reduce.get_value()*step;
	return result;

}
//Функция для расчета интеграла
//Последовательный код
double integrateS(const int N, const int high, const int low) {

	const double step = (double)(high - low) / N;

	double sum = 0;

	for(int i = 0; i < N - 1; ++i)
	{
		sum += 5 / sqrt(8 - 4 * (low + i*step)*(low + i*step));//считаем заданный интеграл
	}


	return sum*step;

}

int main() {

	const int high = 1;
	const int low = -1;
	const int N = 1000000;
	double sum = 0;

	//Переменные для измереняи времени
	duration<double> duration;
	double elapsedtime, elapsedtime_;// Переменная для вывода времени
	
	high_resolution_clock::time_point start = high_resolution_clock::now();
	double res = integrate(N, high, low);
	high_resolution_clock::time_point end = high_resolution_clock::now();
	duration = end - start;
	elapsedtime = duration.count(); // Расчет времени вычислений 

	high_resolution_clock::time_point start_ = high_resolution_clock::now();
	sum = integrateS(N, high, low);
	high_resolution_clock::time_point end_ = high_resolution_clock::now();
	duration = end_ - start_;
	elapsedtime_ = duration.count();// Расчет времени вычислений 

	//Вывод результата
	printf("Parallel code: \n");
	printf("Result = %lf, Elapsed time: %lf second\n", res, elapsedtime);
	printf("Serial code: \n");
	printf("Result = %lf, Elapsed time: %lf second", sum, elapsedtime_);

	getchar();
	return 0;
}
