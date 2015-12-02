using UnityEngine;
using System.Collections;

public class EnemyState_Attack : EnemyState {

	public EnemyState_Attack(){}
	public EnemyState_Attack(EnemyStateMachine machine)
	{this.machine = machine; InitState();}

	#region Variable
	bool canShoot = true;
	float lastShootTime = 0f;
	private Vector3 destination;
	int numOfShoot = 0;
	#endregion

	#region Timer Related
	bool isResting = false;
	DelegateTimer mRestAfterAttackTimer;

	void InitTimers()
	{
		mRestAfterAttackTimer = new DelegateTimer(DoneRest, EnemyStat.AfterAttackRestTime);
		mRestAfterAttackTimer.isPaused = true;
	}

	void TickTimers()
	{
		mRestAfterAttackTimer.Tick(Time.deltaTime);
	}

	void StartRest()
	{
		isResting = true;
		mRestAfterAttackTimer.ResetTime();
		mRestAfterAttackTimer.Activate();
	}

	void DoneRest()
	{
		isResting = false;
		machine.SwitchState(machine.mRoamState.StateType());
	}
	#endregion

	#region Running State
	public override void InitState()
	{
		InitTimers();
	}
	
	public override void StateUpdate()
	{
		if(!isResting)
		{
			if(machine._player){
				if(machine.mType == GameManager.EnemyType.GroundMelee){
					MoveToPlayer();
				}
				else if(machine.mType == GameManager.EnemyType.GroundRange 
				        || machine.mType == GameManager.EnemyType.AirRange){
					
					if(!canShoot & (Time.timeSinceLevelLoad - lastShootTime)>EnemyStat.ShootCoolDown){
						canShoot = true;
					}
					
					float disToPlayer = Mathf.Abs(machine._player.transform.position.x - machine._position.x);
					
					if(disToPlayer > EnemyStat.ShootDistance){
						MoveToPlayer();
					}else{
						machine.transform.LookAt(machine._player.transform.position);

						ShootPlayer();
						lastShootTime = Time.timeSinceLevelLoad;
					}
				}
			}
		}
		TickTimers();
	}
	
	public override void StateFixedUpdate()
	{}
	
	public override void StateExit()
	{
		GameManager.instance.RemoveAttackingEnemy();

	}
	
	public override void StateEnter()
	{
		GameManager.instance.AddAttackingEnemy();

		numOfShoot = 0;
	}
	#endregion

	#region Helper Function
	void ShootPlayer()
	{
		float heightToPlayer = Mathf.Abs(machine._player.transform.position.y - machine._position.y);
		Vector3 shootDir = (machine._player.transform.position - machine._position).normalized;
		shootDir.y += Random.Range(0f, 0.5f);

		Vector3 force = shootDir * EnemyStat.EnemyShootSpeed;

		if(force != Vector3.zero)
		{
			Vector3 shootPoint = machine.transform.position + Vector3.up;
			if(machine.mCannon){
				shootPoint = machine.mCannon.transform.position + machine.transform.forward;
			}
			GameObject initBall = GameObject.Instantiate(Resources.Load(ResourcePath.EnemyBall_path), 
			                                             shootPoint, Quaternion.identity) as GameObject;
			initBall.GetComponent<Rigidbody>().AddForce(force, ForceMode.VelocityChange);
			machine.GetComponent<AudioSource>().Play();

			++numOfShoot;

			//switch to roam
			//try to do 1.5 second delay
			if(numOfShoot >= numOfShoot){
				StartRest();
				machine.AttackCoolDownOn();
			}
		}
	}

	void MoveToPlayer()
	{
		//if hit wall
		if(machine.IsFacingWall()){
			machine.SwitchState(machine.mRoamState.StateType());
		}
		else
		{
			machine.transform.LookAt(destination);
			if(machine.mType == GameManager.EnemyType.GroundRange)
			{
				destination = new Vector3(machine._player.transform.position.x, machine._position.y, machine._player.transform.position.z);
			}
			else if(machine.mType == GameManager.EnemyType.AirRange)
			{
				destination = machine._player.transform.position;
			}
			machine.Move((destination-machine._position).normalized, EnemyStat.MoveSpeed);
		}
	}
	#endregion
}
