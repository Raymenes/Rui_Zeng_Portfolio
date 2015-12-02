using UnityEngine;
using System.Collections;

public class EnemyAct : MonoBehaviour {

	//Enemy State
	public enum behav{idle, roam, think, attack, flee};
	behav myState = behav.idle;
	behav lastState;
	behav nextState;
	//Enemy Type
	GameManager.EnemyType myType;

	//----Public Editables----//
	float speed = 8f;
	float arenaRadius = 200f;
	float shootDistance = 10f;
	float gravityScale = 9.8f;
	float enemyShootSpeed = 15f;
	//------------------------//

	//----Private Variables---//
	public GameObject player;
	Vector3 destination;
	//------------------------//

	void Start () {
		//register objects
		player = GameObject.Find("Player");
		GameObject ground = GameObject.Find("Ground");
		GameManager.instance.Register(this.gameObject);

		//reset position
		transform.position = new Vector3(transform.position.x,
		                                 0.5f,
		                                 transform.position.z);

		//start roaming
		destination = this.transform.position;
		nextState = behav.roam;
		SwitchState();

		//define type
		myType = GameManager.EnemyType.GroundRange;

	}

	#region State Machine
	void Update () {

		switch(myState)
		{
		case behav.idle:
			UpdateIdel();
			break;
		case behav.attack:
			UpdateAttack();
			break;
		case behav.roam:
			UpdateRoam();
			break;
		case behav.think:
			UpdateThink();
			break;
		case behav.flee:
			UpdateFlee();
			break;
		}
	}

	void SwitchState()
	{
		if(myState != nextState)
		{
			lastState = myState;
			ExitState();
			
			switch(nextState)
			{
			case behav.idle:
				OnEnterIdle();
				break;
			case behav.attack:
				OnEnterAttack();
				break;
			case behav.roam:
				OnEnterRoam();
				break;
			case behav.think:
				OnEnterThink();
				break;
			case behav.flee:
				OnEnterFlee();
				break;
			}
		}
	}

	void ExitState()
	{
		switch(myState)
		{
		case behav.idle:
			OnExitIdel();
			break;
		case behav.attack:
			OnExitAttack();
			break;
		case behav.roam:
			OnExitRoam();
			break;
		case behav.think:
			OnExitThink();
			break;
		case behav.flee:

			break;
		}
	}

	void ResumeLastState()
	{
		nextState = lastState;
		SwitchState();
	}
	#endregion

	#region Idle
	void OnEnterIdle()
	{
		myState = behav.idle;
	}

	void OnExitIdel()
	{}

	void UpdateIdel()
	{}
	#endregion

	#region Attack
	bool canShoot = true;
	float shootCoolDown = 2f;
	float lastShootTime = 0f;

	void OnEnterAttack()
	{
		GameManager.instance.AddAttackingEnemy();
		myState = behav.attack;
		this.GetComponent<Renderer>().material.color = Color.red;
//		Invoke("OnEnterRoam", 4f);
	}

	void OnExitAttack()
	{
		GameManager.instance.RemoveAttackingEnemy();
	}

	void UpdateAttack()
	{
		if(player){
			if(myType == GameManager.EnemyType.GroundMelee){
				MoveTowardPlayer();
			}
			else if(myType == GameManager.EnemyType.GroundRange){
				if(!canShoot & (Time.timeSinceLevelLoad - lastShootTime)>shootCoolDown)
				{
					canShoot = true;
				}

				float disToPlayer = Mathf.Abs(player.transform.position.x - this.transform.position.x);
	
				if(disToPlayer > shootDistance){
					MoveTowardPlayer();
				}else{
					float heightToPlayer = Mathf.Abs(player.transform.position.y - this.transform.position.y);
//					ShootTowardPlayer(CalculateForce(disToPlayer, heightToPlayer, gravityScale));
					Vector3 shootDir = (player.transform.position - this.transform.position).normalized;
					shootDir.y += Random.Range(0f, 0.5f);
					ShootTowardPlayer(shootDir * enemyShootSpeed);

				}
			}
		}
	}

	void MoveTowardPlayer()
	{
		//need to add in function to handle hitting walls
		RaycastUpdate();

		destination = new Vector3(player.transform.position.x, destination.y, player.transform.position.z);
		transform.LookAt(destination);
		this.transform.position = Vector3.MoveTowards(this.transform.position, destination, speed*Time.deltaTime);
	}

	void ShootTowardPlayer(Vector3 force)
	{
		if(canShoot && force != Vector3.zero){
			GameObject initBall = Instantiate(Resources.Load("Prefab/Enemy_Ball"), this.transform.position + Vector3.up, Quaternion.identity) 
				as GameObject;
			initBall.GetComponent<Rigidbody>().AddForce(force, ForceMode.VelocityChange);

			canShoot = false;
			this.GetComponent<AudioSource>().Play();
			lastShootTime = Time.timeSinceLevelLoad;
			nextState = behav.roam;
			Invoke( "SwitchState", 1.5f);
		}
	}

	Vector3 CalculateForce (float d, float h, float g)
	{
		float shootScale = (2*d*d - d*g)/(2*h);
		if (shootScale > 0)
		{
			Vector3 playerGroundPos = player.transform.position;
			playerGroundPos.y = this.transform.position.y;
			Vector3 flatDirToPlayer = (playerGroundPos - this.transform.position).normalized;
			float flatScale = (shootScale*shootScale) / (flatDirToPlayer.x * flatDirToPlayer.x + flatDirToPlayer.z * flatDirToPlayer.z);
			flatScale = Mathf.Sqrt(flatScale);
			flatDirToPlayer *= flatScale;
			Vector3 shootDir = flatDirToPlayer;
			shootDir.y = shootScale;
			return shootDir;
		}
		else
			return Vector3.zero;
	}
	#endregion

	#region Roam
	void OnEnterRoam()
	{
		myState = behav.roam;
		if(GameManager.instance.CanAttack()){
			nextState = behav.attack;
		}else{
			nextState = behav.roam;
		}

		int roamTime = Random.Range(1, 9);
		Invoke("SwitchState", roamTime);
		
		this.GetComponent<Renderer>().material.color = Color.white;
		GetDestination();
	}

	void OnExitRoam()
	{}

	void UpdateRoam()
	{
		RaycastUpdate();
		this.transform.position = Vector3.MoveTowards(this.transform.position, destination, speed*Time.deltaTime);
		if(this.transform.position == destination){
			GetDestination();
		}
	}

	void GetDestination()
	{
		Vector3 new_destination;
		bool legitDestination;
		Vector3 detect_dir = transform.forward;
		float radius = 10f;

		int i = 0;
		do{
			legitDestination = true;

			new_destination = new Vector3(Random.Range(destination.x - 10f, destination.x + 10f), 
			                              destination.y, 
			                              Random.Range(destination.z - 10f, destination.z + 10f));

			if(new_destination.sqrMagnitude > arenaRadius*arenaRadius){
				legitDestination = false;
			}
			RaycastHit hitInfo;
			Vector3 distance = (new_destination-transform.position);
			if(Physics.Raycast(transform.position, distance.normalized, out hitInfo, distance.magnitude)){
				if( hitInfo.collider.tag == "environment"){
					legitDestination = false;
					i++;
				}
			}
		}while(!legitDestination && i < 5);

		destination = new_destination;
		transform.LookAt(destination);
	}

	#endregion

	#region Think
	void OnEnterThink()
	{
		myState = behav.think;
	}

	void OnExitThink()
	{}

	void UpdateThink()
	{}
	#endregion

	#region Flee
	void OnEnterFlee()
	{
		myState = behav.flee;
	}

	void OnExitFlee()
	{}

	void UpdateFlee()
	{}
	#endregion


	#region Interact

	void RaycastUpdate()
	{
		float range = 1f;
		Debug.DrawRay(transform.position, transform.forward*range, Color.yellow);
		RaycastHit hitInfo;
		if(Physics.Raycast(transform.position, transform.forward, out hitInfo, range)){
			if(hitInfo.collider.tag == "environment"){
				OnHittingWall();
			}
		}
	}

	void OnHittingWall()
	{
		destination = this.transform.position;
		GetDestination();
		CancelInvoke();
		if(myState != behav.roam){
			nextState = behav.roam;
			SwitchState();
		}
	}


	void OnCollisionEnter(Collision col)
	{
		if(col.gameObject.tag == "environment"){
			OnHittingWall();
		}
	}

//	public void End()
//	{
//		ExitState();
//		GameManager.instance.Deregister(this.gameObject);
//		GameManager.instance.KillEnemyScore();
//		Destroy(this.gameObject);
//	}
//
//	public void Freeze(float duration){
//		nextState = behav.idle;
//		this.GetComponent<Renderer>().material.color = Color.black;
//		SwitchState();
//		CancelInvoke();
//		Invoke("ResumeLastState", duration);
//	}
	#endregion

}
