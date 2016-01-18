#include "ParallelTask.h"



ParallelTask::ParallelTask()
	:mThread(&ParallelTask::runTask,this),mFinished(false)
{
}

void ParallelTask::execute()
{
	mFinished = false;
	mElapsedTime.restart();
	mThread.launch();
}

bool ParallelTask::isFinished()
{
	sf::Lock lock(mMutex);
	return mFinished;
}

float ParallelTask::getCompletion()
{
	sf::Lock lock(mMutex);

	return mElapsedTime.getElapsedTime().asSeconds()/10.f;// 100% at 10 seconds of elapsed time
}

void ParallelTask::runTask()
{
	//just to see the loading, 10 sec of stall
	bool ended = false;
	while (!ended)
	{
		sf::Lock lock(mMutex);
		if (mElapsedTime.getElapsedTime().asSeconds() >= 10.f)
			ended = true;
	}

	{// mFinished may be accessed from multiple threads, protect it. the brackets help to free as soon as possible
		sf::Lock lock(mMutex);
		mFinished = true;
	}
}


