#ifndef ThreadPool_
#define Threadpool_

#include <thread>
#include <vector>
#include <functional>

//1st prototype using threads to validate the API

class ThreadPool
{
public:
	ThreadPool(std::function<void(int)> worker)
	{
		_iNbThreadHardware = std::thread::hardware_concurrency();
		_worker = worker;
	}

	~ThreadPool()
	{
		waitAll();
	}

	void addJob(int iArgument)
	{
		// for now, use many threads
		_vThreads.push_back(std::thread(_worker, iArgument));
	}

	void waitAll()
	{
		for (unsigned int i = 0; i < _vThreads.size(); i++)
			_vThreads[i].join();

		_vThreads.clear();
	}

private:
	int _iNbThreadHardware;
	std::vector<std::thread> _vThreads;
	std::function < void(int) > _worker;

};

// parallel loop

void ThreadPool_for(std::function<void(int)> worker, int iFirst, int iEnd )
{
#ifdef THREADPOOL_NO_THREADS
	for (int i = iFirst; i < iEnd; i++)
		worker(i);
#else
	ThreadPool tp(worker);
	for (int i = iFirst; i < iEnd; i++)
		tp.addJob(i);

	tp.waitAll();
#endif
}


#endif