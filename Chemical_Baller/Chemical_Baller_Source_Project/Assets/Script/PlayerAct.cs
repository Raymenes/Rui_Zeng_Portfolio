using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PlayerAct : MonoBehaviour {
	#region Singlton Design
	//=============This makes sure only one instance of the manager ever exists.================//
	private static PlayerAct _instance;
	public static PlayerAct instance	//Just do GameManager.instance to get the right GameManager.
	{
		get { return _instance;}
	}
	
	void Awake()
	{
		if(_instance == null)
		{
			_instance = this;
		}
		else
		{
			if(this != _instance)
				Destroy(this.gameObject);
		}
	}
	//=========================================================================================//
	#endregion
	/**************Editable Variables**************/

	/**********************************************/
	#region Variables
	//player state
	enum PlayerState{
		grounded,
		jumping,
		launching,
		falling,
		walled
	}
	PlayerState mState = PlayerState.falling;
	
	//game objects
	LineRenderer lineRenderer;
	GameObject Ball;
	public GameObject Gun_Ball;
	public GameObject Gun_Self;
	public Camera cam;
	public GameObject Gun_Model_Ball;
	public GameObject Gun_Model_Self;
	
	//balls
	bool isBallReady = false;
	bool isSelfReady = false;
	int mBallType = 1; /*{none = 0, bullet = 1, explode = 2, smoke = 3, freeze = 4}*/
	int[] mAvailableBalls;
	
	//player movement
	Vector3 direction;
	Vector3 velocity = Vector3.zero;
	int doubleLaunchChance_max = 1;
	int doubleLaunchChance;
	bool isDoubleLaunch = false;
	
	//energy
	float mEnergy = 100f;
	float normal_jump_energy_cost = 10f;
	float extra_jump_energy_cost = 20f;
	float next_jump_enery_cost;

    //metrics
    public MetricsManagerScript ms;

	#endregion
	
	#region Running Game
	void Start () {
		lineRenderer = GetComponent<LineRenderer>();
		lineRenderer.enabled = false;
		doubleLaunchChance = doubleLaunchChance_max;
		Ball = GameManager.instance.Ball_prefab;
		InitBalls();
	}
	
	void Update () 
	{
		if(!GameManager.instance.IsGamePaused){
			UpdateTrajectory();
			UpdateShooting();
			MovementUpdate();
			UpdateEnergy();
		}
	}
	
	void InitBalls(){
		mAvailableBalls = new int[5];
		mAvailableBalls[1] = 30;
		mAvailableBalls[2] = 25;
		mAvailableBalls[3] = 15;
		mAvailableBalls[4] = 25;
		UpdateBallDisplayInfo();
	}
	#endregion
	
	#region Input
	bool SwithBallCommand1(){
		return  Input.GetKeyDown(KeyCode.RightCommand) || 
				Input.GetKeyDown(KeyCode.RightShift) || 
				Input.GetButtonDown("Switch"); //need to add console input
	}
	bool SwithBallCommand2(){
		return  Input.GetKeyDown(KeyCode.LeftCommand) || 
				Input.GetKeyDown(KeyCode.LeftShift) || 
				Input.GetButtonDown("Switch2"); //need to add console input
	}
	bool ReadyBallCommand(){
		return Input.GetKeyDown(KeyCode.E) || Input.GetMouseButtonDown(0) || Input.GetButtonDown("Shoot");
	}
	
	bool ShootBallCommand(){
		return Input.GetKeyUp(KeyCode.E) || Input.GetMouseButtonUp(0) || Input.GetButtonUp("Shoot");
	}
	
	bool ReadySelfCommand(){
		return Input.GetKey(KeyCode.Q) || Input.GetMouseButton(1) || Input.GetButton("Launch");
	}
	
	bool LaunchSelfCommand(){
		return Input.GetKeyUp(KeyCode.Q) || Input.GetMouseButtonUp(1) || Input.GetButtonUp("Launch");
	}
	
	bool ForwardCommand(){
		return Input.GetKey(KeyCode.W)|| (Input.GetAxis("Vertical2") > 0);
	}
	
	bool BackwardCommand(){
		return Input.GetKey(KeyCode.S) || (Input.GetAxis("Vertical2") < 0);
	}
	
	bool LeftCommand(){
		return Input.GetKey(KeyCode.A) || (Input.GetAxis("Horizontal2") < 0);
	}
	
	bool RightCommand(){
		return Input.GetKey(KeyCode.D) || (Input.GetAxis("Horizontal2") > 0);
	}

	bool JumpCommand(){
		return (Input.GetKeyDown(KeyCode.Space) || Input.GetButtonDown("Jump"));
	}
	#endregion

	#region Update Shooting
	void UpdateShooting()
	{
		if(SwithBallCommand1()){
			SwitchBall1();
		}
		if(SwithBallCommand2()){
			SwitchBall2();
		}
		//Ready to shoot ball
		if(ReadyBallCommand() && !isSelfReady){
			if(mEnergy > 10f){
				if(mAvailableBalls[mBallType] > 0){
					isBallReady = true;
					lineRenderer.enabled = true;
				}else{
					GameManager.instance.UpdateMessage("Out Of Ammo!", 1f);
				}
			}else{
				GameManager.instance.UpdateMessage("Out Of Energy!", 1f);
			}
			
		}
		//Ready to shoot self
		if(ReadySelfCommand() && !isBallReady){
			if( mEnergy > 10f){
				if(collisionCount > 0){
					if(cam.transform.forward.y < 0f && transform.position.y <= transform.localScale.y)
					{
						GameManager.instance.UpdateMessage("Please Aim Higher!", 1f);
						RaycastHit hit;
						if(Physics.Raycast(transform.position, Gun_Self.transform.forward, out hit, 2f)){
							if(isEnvironment(hit.collider.gameObject) )
							{
								isSelfReady = false;
								lineRenderer.enabled = false;
							}
						}
					}else{
						isSelfReady = true;
						lineRenderer.enabled = true;
					}
				}else {
					if (doubleLaunchChance <= 0 ){
						GameManager.instance.UpdateMessage("Out Of Double Launch!", 1f);
					}else if (mEnergy < 20f){
						GameManager.instance.UpdateMessage("Out Of Energy!", 1f);
					}
					else if (doubleLaunchChance > 0 && mEnergy > 20f){
						isSelfReady = true;
						lineRenderer.enabled = true;
						isDoubleLaunch = true;
					}
				}
			}
			else{
				GameManager.instance.UpdateMessage("Out Of Energy!", 1f);
			}
		}
		//Shoot ball
		if(ShootBallCommand() && isBallReady ){
			ShootBall();
		}
		//Shoot self
		if(LaunchSelfCommand() && isSelfReady){
			LaunchPlayer();
			if(isDoubleLaunch)
				--doubleLaunchChance;
			isDoubleLaunch = false;
		}
	}
	#endregion

	#region Shoot & Launch
	void SwitchBall1()
	{
		if(mBallType == 4){
			mBallType = 1;
		}
		else{
			++mBallType;
		}
		UpdateBallDisplayInfo();
	}
	void SwitchBall2()
	{
		if(mBallType == 1){
			mBallType = 4;
		}
		else{
			--mBallType;
		}
		UpdateBallDisplayInfo();
	}
	
	void ShootBall()
	{

		lineRenderer.enabled = false;
		int shootOutNumber = 1;
		Vector3 randomPositionOffset = Vector3.zero;
		Vector3 randomForcenOffset = Vector3.zero;

		//some metrics code
		if (mBallType == 1){
			ms.AddToBullet(1);
			mEnergy -= PlayerStat.ShootBulletCost;
			AudioManager.instance.PlayBulletSound();
		}
		else if (mBallType == 2){
			ms.AddToBomb(1);
			mEnergy -= PlayerStat.ShootBombCost;
			mAvailableBalls[mBallType] -= 1;
			AudioManager.instance.PlayBulletSound();
		}
		else if (mBallType == 3){
			ms.AddToFreeze(1);
			mEnergy -= PlayerStat.ShootFreezeCost;
			mAvailableBalls[mBallType] -= 1;
			AudioManager.instance.PlayFreezeBulletSound();
		}
		else if (mBallType == 4){
			ms.AddToShotgun(1);
			mEnergy -= PlayerStat.ShootShotGunCost;
			mAvailableBalls[mBallType] -= 1;
			shootOutNumber = PlayerStat.ShotGunBulletNum;
			AudioManager.instance.PlayShotgunSound();
		}
		GameObject initBall;
		//actuall spawning balls and shoot them out
		for(int i = 0; i < shootOutNumber; ++i)
		{
			initBall = Instantiate(Ball, Gun_Ball.transform.position + cam.transform.forward*1f + randomPositionOffset, Quaternion.identity) 
				as GameObject;
			GameManager.instance.SetObjectColor(initBall, GameManager.instance.currBallColor);

			Vector3 shootDir = (cam.transform.forward + randomForcenOffset).normalized;
			initBall.GetComponent<Rigidbody>().AddForce(shootDir*PlayerStat.ShootSpeed, ForceMode.VelocityChange);
			initBall.GetComponent<BallBehav>().MyType =  (BallBehav.BallType)mBallType;
			initBall.GetComponent<BallBehav>().mOwner = BallBehav.OwnerType.player;

			randomForcenOffset = new Vector3(Random.Range(-0.35f, 0.35f), Random.Range(-0.15f, 0.15f), 0f);
			randomPositionOffset = new Vector3(Random.Range(-0.3f, 0.3f), Random.Range(-0.15f, 0.15f), 0f);
		}

		UpdateBallDisplayInfo();
		isBallReady = false;
	}
	
	void LaunchPlayer()
	{
		lineRenderer.enabled = false;
		this.GetComponent<Rigidbody>().isKinematic = false;
		this.GetComponent<Rigidbody>().useGravity = true;
		this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
		this.GetComponent<Rigidbody>().AddForce(cam.transform.forward*PlayerStat.LaunchSpeed, ForceMode.VelocityChange);
		isSelfReady = false;
		mEnergy -= PlayerStat.LaunchCost;
		mState = PlayerState.launching;
		AudioManager.instance.PlayLaunchSound();
        ms.AddToLaunch(1);
	}
	#endregion
	
	#region Actual Movement
	bool RaycastHittingWall(Vector3 position, Vector3 direction, float length)
	{
		RaycastHit hit;
		if(Physics.Raycast(position, direction, out hit, length))
		{
			if(isEnvironment(hit.collider.gameObject))
				return true;
		}
		return false;
		
	}
	
	void MovementUpdate(){
		RaycastHit hit;
		if(mState != PlayerState.launching && mState != PlayerState.falling)
		{
			if(ForwardCommand()){
				direction = cam.transform.forward;
				direction.y = 0f;
				if(!RaycastHittingWall(transform.position, direction, PlayerStat.PlayerMoveSpeed * Time.deltaTime * 7f)){
					//					gameObject.GetComponent<Rigidbody>().AddForce(direction * speed * Time.deltaTime, ForceMode.VelocityChange);
					this.transform.Translate(direction * PlayerStat.PlayerMoveSpeed * Time.deltaTime, Space.World);
				}
			}
			else if(LeftCommand()){
				direction = cam.transform.right * -1;
				direction.y = 0f;
				if(!Physics.Raycast(transform.position, direction, out hit, PlayerStat.PlayerMoveSpeed * Time.deltaTime * 7f)){
					this.transform.Translate(direction * PlayerStat.PlayerMoveSpeed * Time.deltaTime, Space.World);
				}
			}
			else if(BackwardCommand())
			{
				direction = cam.transform.forward * -1;
				direction.y = 0f;
				if(!Physics.Raycast(transform.position, direction, out hit, PlayerStat.PlayerMoveSpeed * Time.deltaTime * 7f)){
					this.transform.Translate(direction * PlayerStat.PlayerMoveSpeed * Time.deltaTime, Space.World);
				}
			}
			else if(RightCommand())
			{
				direction = cam.transform.right;
				direction.y = 0f;
				if(!Physics.Raycast(transform.position, direction, out hit, PlayerStat.PlayerMoveSpeed * Time.deltaTime * 7f)){
					this.transform.Translate(direction * PlayerStat.PlayerMoveSpeed * Time.deltaTime, Space.World);
				}
			}
			if (JumpCommand() && mState != PlayerState.jumping){
				this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
				gameObject.GetComponent<Rigidbody>().AddForce(Vector3.up * PlayerStat.JumpSpeed, ForceMode.Impulse);
				mState = PlayerState.jumping;
				mEnergy -= PlayerStat.JumpCost;
                ms.AddToJump(1);
			}
		}
		
	}
	#endregion
	
	#region Collision & Interaction
	GameObject lastTouch;
	public int collisionCount = 0;
	
	void OnCollisionEnter (Collision col)
	{
		if(col.gameObject.tag == "Ball")
		{}
		else if(col.gameObject.tag == "PowerUp")
		{}
		else if(col.gameObject.tag == "Bullet")
		{
			StablizePlayer();
		}
		else 
		{
			if(isEnvironment(col.gameObject)){
				++collisionCount;
				StablizePlayer();
				this.GetComponent<Rigidbody>().useGravity = false;
				lastTouch = col.gameObject;
				mState = PlayerState.walled;

				RestoreDoubleLaunch();
			}
		}
	}
	
	void OnCollisionExit(Collision col)
	{
		
		if(isEnvironment(col.gameObject) ){
			--collisionCount;
			if(collisionCount == 0){
				this.GetComponent<Rigidbody>().useGravity = true;
				this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
				if(mState != PlayerState.jumping && mState != PlayerState.launching){
					mState = PlayerState.falling;
				}
			}
		}
	}
	
	void OnCollisionStay(Collision col)
	{
	}
	
	bool isEnvironment(GameObject obj){
		if(obj.tag.Equals("environment") | obj.tag.Equals("Ground"))
			return true;
		//		while(obj != null){
		//			if(obj.tag.Equals("environment") | obj.tag.Equals("Ground"))
		//				return true;
		//			if(!obj.transform.parent.gameObject)
		//				obj = null;
		//			else
		//				obj = obj.transform.parent.gameObject;
		//		}
		return false;
	}
	#endregion
	
	#region Trajectory
	void DrawTrajectory(Vector3 initialPosition, Vector3 initialVelocity, Vector3 gravity)
	{
		int numSteps = 50; // for example
		float timeDelta = 1.0f / initialVelocity.magnitude; // for example
		
		lineRenderer = GetComponent<LineRenderer>();
		lineRenderer.SetVertexCount(numSteps);
		
		Vector3 lastPosition;
		Vector3 position = initialPosition;
		Vector3 velocity = initialVelocity;
		
		for (int i = 0; i < numSteps; ++i)
		{
			lineRenderer.SetPosition(i, position);
			lastPosition = position;
			position += velocity * timeDelta + 0.5f * gravity * timeDelta * timeDelta;
			velocity += gravity * timeDelta;
			
			Vector3 offSet = position-lastPosition;
			RaycastHit hit;
			
			if(Physics.Raycast(lastPosition, offSet, out hit)){
			}
		}
	}
	
	void UpdateTrajectory()
	{
		if(isBallReady){
			DrawTrajectory(Gun_Ball.transform.position, 
			               cam.transform.forward*PlayerStat.ShootSpeed, 
			               new Vector3(0f, -9.8f, 0f));
		}
		else if(isSelfReady){
			DrawTrajectory(Gun_Self.transform.position, 
			               cam.transform.forward*PlayerStat.LaunchSpeed, 
			               new Vector3(0f, -9.8f, 0f));
		}
	}
	#endregion
	
	#region Player Stat
	
	void UpdateBallDisplayInfo()
	{
		GameManager.instance.UpdateBall((BallBehav.BallType)mBallType, mAvailableBalls[mBallType]);
		foreach(Renderer r in Gun_Model_Ball.GetComponentsInChildren<Renderer>())
		{
			r.material.color = GameManager.instance.currBallColor;
		}
		foreach(Renderer r in Gun_Model_Self.GetComponentsInChildren<Renderer>())
		{
			r.material.color = GameManager.instance.currBallColor;
		}
//		GameManager.instance.SetObjectColor(Gun_Ball, GameManager.instance.currBallColor);
//		GameManager.instance.SetObjectColor(Gun_Self, GameManager.instance.currBallColor);
	}

	void UpdateEnergy()
	{
		if(mEnergy > 100f){
			mEnergy = 100f;
		}
		if(mEnergy < 0){
			mEnergy = 0;
		}
		mEnergy += PlayerStat.EnergyResumeRate * Time.deltaTime;
		GameManager.instance.UpdateEnergy(mEnergy);
	}

	void RestoreDoubleLaunch()
	{
		doubleLaunchChance = doubleLaunchChance_max;
	}

	void StablizePlayer()
	{
		this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeAll;
		this.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotation;
	}

	public bool isOutOfArena()
	{
		return transform.position.y < -10f;
	}
	#endregion

	#region Power Up Related
	public void AddBall(int type, int number){
		int actualType = (int)(Mathf.Clamp(type, 1f, 3f));
		mAvailableBalls[actualType] += number;
		UpdateBallDisplayInfo();
	}

	public void AddBall(int number){
		for(int i = 0; i < mAvailableBalls.Length; ++i)
			mAvailableBalls[i] += number;
		UpdateBallDisplayInfo();
	}
	
	public void AddEnergy(float num)
	{
		mEnergy += num;
	}

	public void AddDoubleLaunchChance(float duration){
		doubleLaunchChance_max += 1;
		Invoke("RemoveDoubleLaunchPowerUp", duration);
		Debug.Log("doubleLaunchChance_max: "+doubleLaunchChance_max);
	}

	void RemoveDoubleLaunchPowerUp()
	{
		doubleLaunchChance_max -= 1;
	}
	#endregion
}
