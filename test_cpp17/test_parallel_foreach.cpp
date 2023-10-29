#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <execution>
#include <thread>
#include <mutex>
using namespace std;

double total = 0.;
std::mutex m;

//////////////////////////////////////////////////////////////////////////
void monte_carlo_count_pi(int i2)
{
	srand((unsigned int)time(NULL));
	int nbTests = 10000000;
	double c = 0;
	for (int i = 0; i < nbTests; i++)
	{
		double x = (double)rand() / (RAND_MAX);
		double y = (double)rand() / (RAND_MAX);
		if (x * x + y * y <= 1.0)
			c++;
	}

	double dPi = 4.*c / (double)nbTests;
	{
		std::lock_guard<std::mutex> guard(m);
		total += dPi;
	}
}
//////////////////////////////////////////////////////////////////////////
int main()
{
	cout << "running:" << endl;
	int timeMonothread,iNbThread = std::thread::hardware_concurrency();
	cout << "estimated number of thread= " << iNbThread << endl;

	for (int iNbThreads = 1; iNbThreads < 16; iNbThreads++)
	{
		total = 0;
		vector<int> val;

		for (int i = 0; i < iNbThreads; i++)
			val.push_back(0);

		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		
		std::for_each(std::execution::par_unseq, val.begin(), val.end(), monte_carlo_count_pi );
		end = std::chrono::system_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds> 	(end - start).count();
		auto iSharedTimeByThread = elapsed_ms / iNbThreads;
		std::cout << endl << "nb_thread=" << iNbThreads << " pi= " << total / iNbThreads << " time/crible=" << iSharedTimeByThread << "ms" << endl;
	
		if (iNbThreads == 1)
			timeMonothread = (int)elapsed_ms;

		if (iNbThreads == iNbThread)
			cout << "efficienty=" << timeMonothread / (double)(iSharedTimeByThread*iNbThread)*100. << endl;
	}

	return 0;
}
