using UnityEngine;

using System.Collections;

public class DelegateTimer{
	
	public delegate void MyDelegate();
	private MyDelegate mDelegate;
	private float mTime;
	private float mRemainTime = 0;
	private bool isLooping = false;
	public bool isFinished = false;
	public bool isPaused = false;
	
	private DelegateTimerKey mTimerKey = null;
	public DelegateTimerKey _mTimerKey {get {return mTimerKey;}}
	
	public DelegateTimer(DelegateTimerKey key, MyDelegate d, float remainTime){
		mTimerKey = key;
		mDelegate = d;
		mTime = remainTime;
		mRemainTime = remainTime;
	}
	
	public DelegateTimer(MyDelegate d, float remainTime){
		mDelegate = d;
		mTime = remainTime;
		mRemainTime = remainTime;
	}
	
	public DelegateTimer(DelegateTimerKey key, MyDelegate d, float remainTime, bool looping){
		mTimerKey = key;
		mDelegate = d;
		mRemainTime = remainTime;
		mTime = remainTime;
		isLooping = looping;
	}
	
	public void AddDelegate(MyDelegate d)
	{
		mDelegate += d;
	}
	
	public void Call(){
		mDelegate();
	}
	
	
	public void ResetTime(){
		mRemainTime = mTime;
	}
	
	public void ResetTime(float newTime){
		mTime = newTime;
		mRemainTime = mTime;
	}
	
	public void Activate(){
		isPaused = false;
		isFinished = false;
	}
	
	public void Tick(float deltaTime)
	{
		if(!isFinished && !isPaused)
		{
			mRemainTime -= deltaTime;
			if(mRemainTime < 0f)
			{
				if(isLooping){
					mRemainTime = mTime;
					mDelegate();
				}
				else{
					mDelegate();
					mRemainTime = 0f;
					isFinished = true;
				}
			}
		}
	}
}

public class DelegateTimerKey
{
	private static int totalKeyNum = 0;
	private int mKeyNum;
	public int _mKeyNum {get {return mKeyNum;} }
	public DelegateTimerKey(){mKeyNum = totalKeyNum++;}
	public bool isEqual(DelegateTimerKey key){ return (mKeyNum == key._mKeyNum);}
};
