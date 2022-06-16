#include <iostream>
#include <queue>
#include <list>
#include <csignal>
#include <string>
#include <vector>
#include <thread>
#include <mutex>


struct Request
{
	int requestClass;
	int priority;

	Request(int requestClass, int priority)
	{
		this->requestClass = requestClass;
		this->priority = priority;
	}

	Request() : Request(-1, -1) {}

};

struct Device
{
	int id;
	int group;
	Request request;
	double workTime;
	double startTime;
	bool isWorking = false;

	Device(int id, int group)
	{
		this->id = id;
		this->group = group;
	}

	Device() : Device(-1, -1) {}
};

int queueSize, groupCount, devicesCount;
bool programIsWorking = true;
std::mutex _mutex;
std::thread generatorThread;

std::vector<std::thread> threadDevices;
std::vector<Device> devices;
std::list<Request> requestsList;

const int MIN_TIME_SLEEP_DEV = 250;
const int MAX_TIME_SLEEP_DEV = 3000;

const int MIN_TIME_SLEEP_GEN = 1;
const int MAX_TIME_SLEEP_GEN = 100;
const int THREAD_INF_SLEEP = 1000;


void InitDevices();

void InitThreads();

void EventLoop();

Request DoWork(int typeGroup);

void ThreadWork(Device& dev);

void IOLoop(std::vector<Device>& devs);

void JoinThreads();

Request GetRequest();

int GetRand(int min, int max);

void Terminate(int sigint);

void InitDevices()
{
	for (int i = 0; i < groupCount * devicesCount; i++)
	{
		Device device(i, i / devicesCount);
		devices.push_back(device);
	}
}

void InitThreads()
{
	threadDevices.resize(devices.size());

	for (int i = 0; i < devices.size(); i++)
	{
		threadDevices[i] = std::thread(ThreadWork, std::ref(devices[i]));
	}
}

void EventLoop()
{
	while (programIsWorking)
	{
		bool isAdd = false;
		_mutex.lock();

		if (requestsList.size() != queueSize)
		{
			requestsList.push_back(GetRequest());

			isAdd = true;
		}
		_mutex.unlock();

		if (!isAdd)
			continue;

		std::this_thread::sleep_for(std::chrono::milliseconds((int)GetRand(MIN_TIME_SLEEP_GEN, MAX_TIME_SLEEP_GEN)));
	}
}

Request DoWork(int typeGroup)
{
	std::list<Request>::iterator bestIterator = requestsList.begin();
	Request bestRequest;
	bool isInit = false;

	for (std::list<Request>::iterator it = requestsList.begin(); it != requestsList.end(); it++)
	{
		Request currentRequest = *it;


		if (currentRequest.requestClass != typeGroup)
			continue;

		if (isInit && currentRequest.priority > (*bestIterator).priority)
		{
			bestIterator = it;
		}
		else if (!isInit)
		{
			isInit = true;
			bestIterator = it;
		}

		if ((*bestIterator).priority == 3)
		{
			break;
		}
	}

	if (isInit)
	{
		bestRequest = *bestIterator;
		requestsList.erase(bestIterator);
	}

	return bestRequest;
}

void ThreadWork(Device& dev)
{
	while (programIsWorking)
	{
		bool isEvent = false;

		_mutex.lock();

		if (!dev.isWorking)
		{
			Request bestRequest = DoWork(dev.group);

			if (bestRequest.requestClass != -1)
			{
				isEvent = true;

				double sleepTime = GetRand(MIN_TIME_SLEEP_DEV, MAX_TIME_SLEEP_DEV);

				dev.request = bestRequest;
				dev.workTime = sleepTime;
				dev.startTime = clock();
				dev.isWorking = true;
			}
		}
		else
		{
			double diff = clock() - dev.startTime;
			double restTime = dev.workTime - diff;

			if (restTime <= 0)
			{
				dev.isWorking = false;
			}
		}

		_mutex.unlock();

		if (!isEvent)
			continue;

		std::this_thread::sleep_for(std::chrono::milliseconds((int)(dev.workTime)));
	}
}

void IOLoop(std::vector<Device>& devs)
{
	while (programIsWorking)
	{
		_mutex.lock();
		for (int i = 0; i < devs.size(); i++)
		{
			if (devs[i].isWorking && devs[i].workTime - (clock() - devs[i].startTime) <= 0)
				devs[i].isWorking = false;

			std::cout << "Device info: " << devs[i].id << ". ";
			std::cout << " State: ";
			if (devs[i].isWorking)
			{
				std::cout << "WORKING. ";
				std::cout << "Task type: " << devs[i].request.requestClass;
				std::cout << " Time until end of work: " << devs[i].workTime - (clock() - devs[i].startTime);
			}
			else
			{
				std::cout << "WAITING. ";
			}
			std::cout << std::endl;
		}
		std::cout << "Elements in queue: " << requestsList.size() << std::endl << std::endl << std::endl;

		_mutex.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_INF_SLEEP));
	}
}

void JoinThreads()
{
	for (int i = 0; i < threadDevices.size(); i++)
	{
		threadDevices[i].join();
	}
}

Request GetRequest()
{
	int requestPriority = GetRand(1, 3);
	int requestClass = GetRand(0, groupCount - 1);
	Request request(requestClass, requestPriority);

	return request;
}

int GetRand(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

void Terminate(int sigint)
{
	programIsWorking = false;
}

int main()
{
	signal(SIGINT, Terminate);

	std::cout << "Enter capacity: ";
	std::cin >> queueSize;
	std::cout << "Enter number of groups: ";
	std::cin >> groupCount;
	std::cout << "Enter group size: ";
	std::cin >> devicesCount;

	InitDevices();
	InitThreads();

	std::thread generatorThread(EventLoop);
	std::thread informationThread(IOLoop, std::ref(devices));

	generatorThread.join();
	informationThread.join();
	JoinThreads();

	return 0;
}
