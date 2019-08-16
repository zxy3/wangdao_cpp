#include "Threadpool.h"
#include "WorkerThread.h"

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

Threadpool::Threadpool(size_t threadNum, size_t queSize)
	: _threadNum(threadNum), _queSize(queSize), _taskque(queSize), _isExit(false)
{
	// 默认是要退出的
	// 只是开辟了空间，才可以调用子线程
	// 此时没有元素，因此不能使用下标运算符
	_threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{
	if (!_isExit)
		stop();
}

void Threadpool::start()
{
	// 开启线程池的运行，让子线程跑起来
	// 通过循环，创建线程对象
	for (size_t idx = 0; idx != _threadNum; ++idx)
	{
		// 用一个unique_ptr指针托管
		unique_ptr<Thread> thread(new WorkerThread(*this));
		_threads.push_back(std::move(thread));
		//_threads.push_back(thread);
	}
	// 让每个线程对象，拿到任务，跑起来
	for (auto &thread : _threads)
	{
		thread->start();
	}
}

void Threadpool::stop()
{
	if (!_isExit)
	{

		while (!_taskque.empty())
		{
			::sleep(1);
		}

		_isExit = true;
		_taskque.wakeup();
		for (auto &thread : _threads)
		{
			thread->join();
		}
	}
}

void Threadpool::addTask(Task *task)
{
	_taskque.push(task);
}

Task *Threadpool::getTask()
{
	return _taskque.pop();
}

//每一个子线程要完成的任务, 其运行在
//WorkerThread::run方法中
void Threadpool::threadfunc()
{
	while (!_isExit)
	{
		// 拿到任务 getTask拿到taslkque的pull方法
		Task *task = getTask();
		if (task)
			task->process(); //当任务执行的速度过快
							 //在还没有将_isExit设置为true之前，每一个子线程
							 //已经阻塞在了getTask()方法之上；
	}
}

} //end of namespace wd
