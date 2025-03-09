#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <execution>
#include <thread>
#include <chrono>
using namespace std;

double total = 0.;
std::mutex m;

//////////////////////////////////////////////////////////////////////////
void monte_carlo_count_pi()
{
	srand(time(NULL));
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
	int timeMonothread,iNbThreadHardware = std::thread::hardware_concurrency();
	cout << "number of hardware thread= " << iNbThreadHardware << endl;

	for (int iNbThreads = 1; iNbThreads < 16; iNbThreads++)
	{
		total = 0.;

		auto start = std::chrono::system_clock::now();
		
		// run all threads and wait jobs end
		vector<thread> allThreads;		
		for (int iT = 0; iT < iNbThreads; iT++)
			allThreads.push_back(thread(monte_carlo_count_pi));
		for (int iT = 0; iT < iNbThreads; iT++)
			allThreads[iT].join();

		
		auto end = std::chrono::system_clock::now();
		int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds> 	(end - start).count();
		int iSharedTimeByThread = elapsed_ms / iNbThreads;
		std::cout << endl << "nb_thread=" << iNbThreads << " pi= " << total / iNbThreads << " time/crible=" << iSharedTimeByThread << "ms" << endl;
	
		if (iNbThreads == 1)
			timeMonothread = elapsed_ms;

		if (iNbThreads == iNbThreadHardware)
			cout << "efficienty=" << timeMonothread / (double)(iSharedTimeByThread*iNbThreadHardware)*100. << endl;
	}

	return 0;
}
