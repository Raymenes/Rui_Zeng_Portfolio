using UnityEngine;
using System.Collections;

public class EnemyStateMachine : MonoBehaviour {


	//===============States=================//
	private EnemyState mState;
	public EnemyState_Attack mAttackState;
	public EnemyState_Roam mRoamState;
	private string mCurrStateInfo;
	//======================================//
	private Color mLastColor;
	private Color mCurrColor;

	private GameObject player; 
	private CharacterController controller;
	public float currHealth;
	public GameManager.EnemyType mType;
	public GameObject mBody;
	public GameObject mCannon;

	//==============Property================//
	public Vector3 _position {get{return transform.position;}}
	public GameObject _player {get {return player;}}
	public CharacterController _controller{get{return controller;}}
	public float _currHealth {get{return currHealth;}}
	public string _mCurrStateInfo {get{return mCurrStateInfo;}}
	//======================================//

	#region Timer Related
	private bool isAttackOnCoolDown = true;
	public bool IsAttackOnCoolDown {get{return isAttackOnCoolDown;}}
	private DelegateTimer mAttackCoolDownTimer;

	private bool isFreeze = false;
	private DelegateTimer mFreezeTimer;

	private void InitTimers()
	{
		mAttackCoolDownTimer = new DelegateTimer(AttackCoolDownOff, EnemyStat.AttackStateCoolDown);
		mFreezeTimer = new DelegateTimer(FreezeOff, EnemyStat.FreezeDuration);
		mFreezeTimer.isPaused = true;
	}

	private void TickTimers()
	{
		mAttackCoolDownTimer.Tick(Time.deltaTime);
		mFreezeTimer.Tick(Time.deltaTime);
	}

	public void AttackCoolDownOn()
	{
		isAttackOnCoolDown = true;
		mAttackCoolDownTimer.ResetTime();
		mAttackCoolDownTimer.Activate();
	}

	private void AttackCoolDownOff()
	{
		isAttackOnCoolDown = false;
	}
	#endregion

	#region Init Machine
	void Start () 
	{
		InitVariables();
		InitStates();
		InitTimers();
	}
	
	void InitStates()
	{
		mAttackState = new EnemyState_Attack(this);
		mRoamState = new EnemyState_Roam(this);

		mState = mRoamState;
		mState.StateEnter();
		mLastColor = Color.blue;
		mCurrColor = Color.blue;
	}

	void InitVariables()
	{
		player = PlayerAct.instance.gameObject;
		controller = GetComponent<CharacterController>();

		GameObject ground = GameObject.Find("Ground");
		GameManager.instance.Register(this.gameObject);
		
//		mType = GameManager.EnemyType.AirRange;
	}
	#endregion

	#region Running Machine
	
	void Update () 
	{
		TickTimers();
		if(!isFreeze){
			mState.StateUpdate();
		}
	}
	#endregion

	#region switching state
	public void SwitchState(string stateType)
	{
		if ( mCurrStateInfo == stateType) return;
		
		if(stateType == mAttackState.StateType()){
			if(mBody)
			{
				SwitchColor(Color.red);
//				mBody.GetComponent<FlashyEffect>().SetColor(Color.red);
			}
			SwitchState(mAttackState);
		}
		else if(stateType == mRoamState.StateType()){
			if(mBody)
			{
				SwitchColor(Color.blue);
//				mBody.GetComponent<FlashyEffect>().SetColor(Color.blue);
			}

			SwitchState(mRoamState);
		}
		else{
			Debug.LogError("No state of " + stateType);
		}
	}
	
	private void SwitchState(EnemyState nextState)
	{
		if(nextState != null)
		{
			mCurrStateInfo = nextState.StateType();
			mState.StateExit();
			mState = nextState;
			mState.StateEnter();
		}
		else
		{
			Debug.LogError("Invalid Enemy State!");
		}
	}
	#endregion

	#region Movement Function
	public bool IsFacingWall()
	{
		float range = 1.5f;
		Vector3 castStartPoint = transform.position + Vector3.up;
		Debug.DrawRay(castStartPoint, transform.forward, Color.red);

		RaycastHit hitInfo;
		if(Physics.SphereCast(_position,range,Vector3.zero, out hitInfo))
		{
			if(hitInfo.collider.tag == "environment"){
				return true;
			}
		}
		if(Physics.Raycast(castStartPoint, transform.forward, out hitInfo, range)){
			if(hitInfo.collider.tag == "environment"){
				return true;
			}
		}
		return false;
	}

	public bool CheckIsPlayerVisibile()
	{
		float range = 1f;
//		Debug.DrawLine(transform.position, player.transform.position, Color.yellow);
		Vector3 dir = player.transform.position - transform.position;
		RaycastHit[] hitInfo = Physics.RaycastAll(transform.position, dir.normalized, dir.magnitude);
		foreach(RaycastHit h in hitInfo)
		{
			if(h.collider.tag == "environment"){
				return false;
			}
		}

		return true;
	}

	//get called (by state script) per frame to move the enemy
	public void Move(Vector3 direction, float speed){
		controller.Move(direction * speed * Time.deltaTime);
	}
	//similar as above
	//return true if reached destination
	public bool MoveTowards(Vector3 destination,float speed){
		Vector3 direction = (destination - _position).normalized;
		if(Vector3.Distance(_position, destination) <= 1.0f){
			return true;
		}
		else{
			Move(direction, speed);
			return false;
		}
	}
	#endregion

	#region Interaction
	public void End()
	{
		BurstPowerUp();
        GameObject deathBlood = Instantiate(Resources.Load("Effect/BloodSplash"), transform.position, transform.rotation) as GameObject;
		deathBlood.GetComponent<Renderer>().material.color = mCurrColor;
        mState.StateExit();
		GameManager.instance.Deregister(this.gameObject);
		GameManager.instance.KillEnemyScore();
		Destroy(this.gameObject);
	}
	
	public void Freeze(float duration){
		if(mBody)
		{
			SwitchColor(Color.black);
//			mBody.GetComponent<FlashyEffect>().SetColor(Color.black);
		}
		isFreeze = true;
		mFreezeTimer.ResetTime();
		mFreezeTimer.Activate();
	}

	void FreezeOff()
	{
		isFreeze = false;
		SwitchColor(mLastColor);
	}

	void BurstPowerUp()
	{
		float roll = Random.Range(0f, 1f);
		if(roll < EnemyStat.LeavePowerUpChance)
		{
			GameObject powerup = Instantiate(Resources.Load(ResourcePath.PowerUp_path), 
			                                 transform.position + 0.5f*Vector3.up, Quaternion.identity) as GameObject;
			powerup.GetComponent<PowerUpAct>().SetLifeTime(EnemyStat.LeavePowerUpTime);
		}

	}

	void SwitchColor(Color color)
	{
		mLastColor = mCurrColor;
		mCurrColor = color;
		mBody.GetComponent<FlashyEffect>().SetColor(color);
	}
	#endregion

	#region Attack Function

	#endregion
}
