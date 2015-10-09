#include "GameTimers.h"
#include <iostream>

GameTimerManager::GameTimerManager()
	:mNextTimerId(0)
	,mAreTimersTicking(false)
{

}

void GameTimerManager::Tick(float deltaTime)
{
    mAreTimersTicking = true;
    {
        auto t = mActiveTimers.begin();
        while ( t != mActiveTimers.end() )
        {
            if (t->second.mStatus != Cleared)
            {
                t->second.mRemainingTime = t->second.mRemainingTime - deltaTime;
                
                if (t->second.mRemainingTime <= 0.0f)
                {
                    t->second.mDelegate->Execute();
                    if (t->second.mIsLooping) {
                        t->second.mRemainingTime = t->second.mDuration;
                    }
                    else{
                        t->second.mStatus = Cleared;
                        mClearedTimers.push_back(t->first);
                    }
                }
            }
            t++;
        }
    }
    
    for (auto th : mClearedTimers)
    {
        auto t = mActiveTimers.find(th);
        RemoveFromObjMap(t->second.mObj, t->first);
        mActiveTimers.erase(t);
    }
    
    mClearedTimers.clear();
    
    auto t = mPendingTimers.begin();
    while (t != mPendingTimers.end())
    {
        mActiveTimers.emplace(t->first, t->second);
        t++;
    }
    
    mPendingTimers.clear();
    
    mAreTimersTicking = false;
}

void GameTimerManager::ClearTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		// We can just remove this from pending timers
		RemoveFromObjMap(iter->second.mObj, handle);
		mPendingTimers.erase(iter);
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Cleared;
			mClearedTimers.push_back(handle);
		}
	}
}

void GameTimerManager::PauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Paused;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Paused;
		}
	}
}

void GameTimerManager::UnpauseTimer(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mStatus = Pending;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mStatus = Active;
		}
	}
}

float GameTimerManager::GetRemainingTime(const TimerHandle& handle)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		return iter->second.mRemainingTime;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			return iter->second.mRemainingTime;
		}
	}

	// Unknown timer
	return -1.0f;
}

void GameTimerManager::AddTime(const TimerHandle& handle, float time)
{
	// Is this pending?
	auto iter = mPendingTimers.find(handle);
	if (iter != mPendingTimers.end())
	{
		iter->second.mRemainingTime += time;
	}
	else
	{
		iter = mActiveTimers.find(handle);
		if (iter != mActiveTimers.end())
		{
			iter->second.mRemainingTime += time;
		}
	}
}

void GameTimerManager::ClearAllTimers(Object* obj)
{
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		for (auto& t : iter->second)
		{
			ClearTimer(t);
		}

		mObjToHandlesMap.erase(iter);
	}
}

void GameTimerManager::SetTimerInternal(TimerHandle& outHandle, Object* obj, TimerDelegatePtr delegate, float duration, bool looping)
{
    outHandle.mValue = mNextTimerId;
    mNextTimerId++;
    TimerInfo timerInfo;
    timerInfo.mDelegate = delegate;
    timerInfo.mHandle = outHandle;
    timerInfo.mObj = obj;
    timerInfo.mDuration = duration;
    timerInfo.mRemainingTime = duration;
    timerInfo.mIsLooping = looping;

    
    if (mAreTimersTicking) {
        timerInfo.mStatus = Pending;
        mPendingTimers.emplace(outHandle, timerInfo);
    }
    else{
        timerInfo.mStatus = Active;
        mActiveTimers.emplace(outHandle, timerInfo);
    }
    
    AddToObjMap(obj, outHandle);
}

void GameTimerManager::AddToObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	// Do we already know of this object?
	if (iter != mObjToHandlesMap.end())
	{
		iter->second.emplace(handle);
	}
	else
	{
		std::unordered_set<TimerHandle> temp;
		temp.emplace(handle);
		mObjToHandlesMap.emplace(obj, temp);
	}
}

void GameTimerManager::RemoveFromObjMap(Object* obj, const TimerHandle& handle)
{
	auto iter = mObjToHandlesMap.find(obj);
	if (iter != mObjToHandlesMap.end())
	{
		auto handleIter = iter->second.find(handle);
		if (handleIter != iter->second.end())
		{
			iter->second.erase(handleIter);
		}
	}
}
