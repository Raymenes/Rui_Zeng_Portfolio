using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.UI;

public class GameManager : MonoBehaviour {

	#region Singleton Struct
	//=============This makes sure only one instance of the manager ever exists.================//
	private static GameManager _instance;
	public static GameManager instance	//Just dont GameManager.instance to get the right GameManager.
	{
		get { return _instance ?? (_instance = new GameObject("GameManager").AddComponent<GameManager>());}
	}
	
	void Awake()
	{
		EnemyGroup = new List<GameObject>();

		if(_instance == null)
		{
			_instance = this;
		}
		else
		{
			if(this != _instance)
				Destroy(this.gameObject);
		}
		InitPrefabs();

	}
	//=========================================================================================//
	#endregion

	#region Game Running
	bool isGameOver = false;
	bool isGamePause = false;
	public bool IsGamePaused {get {return isGamePause;}}
	void Start () 
	{
		Screen.fullScreen = true;
		Cursor.visible = false;
		LoadHUD();
		SpawnNextWave();
	}
	
	void Update () {
		if(Input.GetKeyDown(KeyCode.Escape)){
//			Application.LoadLevel(0);
			PauseGame();
		}
		CheckForSpawn();
		if(PlayerAct.instance.isOutOfArena()){
			DeathGameOver();
		}
		if(!isGameOver)
		{
			ShakeUpdate();
		}
	}

	public void PauseGame(){
		if(!isGameOver)
		{
			//resume to play state
			if(isGamePause ) {
				PlayerAct.instance.gameObject.GetComponent<LineRenderer>().enabled = false;
				Time.timeScale = 1;
				WipeMessage();
			}
			else{
				Time.timeScale = 0;
				UpdateMessage("Game Paused.", 1f);
			}
			Quit_Btn.SetActive(!isGamePause);
			isGamePause = ! isGamePause;
		}
	}

	public void DeathGameOver()
	{
		Quit_Btn.SetActive(true);
		isGameOver = true;
		Time.timeScale = 0;
		UpdateMessage("Game Over!", 1f);
	}

	public void QuitGame(){
		Time.timeScale = 1;
		Application.LoadLevel(0);
	}

	#endregion

	#region Enemy Management
	private List<GameObject> EnemyGroup;
	public List<GameObject> SpawnPoints;

	public enum EnemyType {GroundMelee, GroundRange, AirRange, Boss};

	public void Register(GameObject enemy){
		EnemyGroup.Add(enemy);
		UpdateEnemyWaveInfo();
	}
	
	public void Deregister(GameObject enemy){
		EnemyGroup.Remove(enemy);
		UpdateEnemyWaveInfo();
	}

	//====================================================//
	#region Spawning
	public int wave_index = 0;
	private int basic_enemy_per_wave = 12;
	public int enemy_next_wave;
	private float lastSpawnTime;

	int CalculateNextWaveEnemyNum(){
		enemy_next_wave = basic_enemy_per_wave + (wave_index+(wave_index-1))*7;
		return enemy_next_wave;
	}
	
	//get called per update to decide whether or not to spawn next wave
	void CheckForSpawn()
	{
		if(EnemyGroup.Count >= 40)
			return;
		if(EnemyGroup.Count <= 7 || (Time.timeSinceLevelLoad-lastSpawnTime) >= (GameStat.MaxWaveDuration*wave_index))
		{
			SpawnNextWave();
		}
	}
	
	void SpawnNextWave()
	{
		lastSpawnTime = Time.timeSinceLevelLoad;
		++wave_index;
		CalculateNextWaveEnemyNum();
		UpdateEnemyWaveInfo();
		
		int airEnemyNum = (int)((enemy_next_wave*3f)/10f);
		int groundEnemyNum = enemy_next_wave - airEnemyNum;
		
		int a = 0;
		
		for(int i = 0; i < enemy_next_wave; ++i){
			GameObject p = SpawnPoints[Random.Range(0, SpawnPoints.Count)];
			Vector3 random = new Vector3(Random.Range(-1.5f, -1.5f), Random.Range(-1.5f, -1.5f), 0);
			Vector3 spawnPos = p.transform.position + random; spawnPos.y = 0f;
			if(a < airEnemyNum)
			{
				GameObject enemy = Instantiate(Resources.Load(ResourcePath.AirEnemy_path), spawnPos, Quaternion.identity) as GameObject;
				EnemyStateMachine machine = enemy.GetComponent<EnemyStateMachine>();
				machine.mType = EnemyType.AirRange;
				++a;
			}
			else
			{
				GameObject enemy = Instantiate(Resources.Load(ResourcePath.GroundEnemy_path), spawnPos, Quaternion.identity) as GameObject;
				EnemyStateMachine machine = enemy.GetComponent<EnemyStateMachine>();
				machine.mType = EnemyType.GroundRange;
			}
		}
	}
	#endregion

	public int attackingEnemy = 0;
	public bool CanAttack(){return (GameStat.maxAttackingEnemy - attackingEnemy) > 0; }

	public void AddAttackingEnemy(){++attackingEnemy;}
	public void RemoveAttackingEnemy(){--attackingEnemy;}

	List<GameObject> FindEnemyInRange(Vector3 position, float radius)
	{
		List<GameObject> ret = new List<GameObject>();
		foreach(GameObject enemy in EnemyGroup)
		{
			Vector3 distance = enemy.transform.position - position;
			if( distance.magnitude < radius ) {
				ret.Add(enemy);
			}
		}
		return ret;
	}

	public int NumOfAliveEnemy()
	{return EnemyGroup.Count;}

	#endregion

	#region UI & Game State
	int mScore = 0;

	int coinScore = 3;

	public Text PopMessage;
	public Text ScorePoint;
	public Text Ball_Info;
	public Text Wave_Info;
	public Text Enemy_Info;
	public Slider EnergyBar;
	public Slider HealthBar;
	public GameObject Quit_Btn;
	
	public Color BulletColor;
	public Color ExplodeColor;
	public Color FreezeColor;
	public Color ShotgunColor;
	public Color currBallColor;

	public void SetObjectColor(GameObject obj, Color color)
	{
		obj.GetComponent<Renderer>().material.color = color;
	}
	
	void LoadHUD()
	{
		PopMessage = GameObject.Find("PopMessage").GetComponent<Text>();
		ScorePoint = GameObject.Find("ScorePoint").GetComponent<Text>();
		EnergyBar = GameObject.Find("EnergyBar").GetComponent<Slider>();
		HealthBar = GameObject.Find("HealthBar").GetComponent<Slider>();
		Ball_Info = GameObject.Find("Ball_Info").GetComponent<Text>();
		Wave_Info = GameObject.Find("Wave_Info").GetComponent<Text>();
		Enemy_Info = GameObject.Find("Enemy_Info").GetComponent<Text>();

		Quit_Btn = GameObject.Find("quit_btn");
		Quit_Btn.SetActive(false);

		HealthBar.value = mHealth;
	}

	void UpdateEnemyWaveInfo()
	{
		if(Enemy_Info)
			Enemy_Info.text = "Enemy Alive: " + EnemyGroup.Count.ToString();
		if(Wave_Info)
			Wave_Info.text = "Wave: " + wave_index.ToString();
	}

	public void AddScore(int i){
		mScore += i;
		if(ScorePoint)
			ScorePoint.text = "Score: " + mScore.ToString();
	}

	public void KillEnemyScore(){
		mScore += GameStat.killEnemyScore;
		if(ScorePoint)
			ScorePoint.text = "Score: " + mScore.ToString();
	}

	public void UpdateBall(BallBehav.BallType type, int number){
		string ballName = "Unknown";
		string ballNum = number.ToString();
		switch(type){
		case BallBehav.BallType.bullet:
			currBallColor = BulletColor;
			ballName = "Bullet";
			ballNum = "Infinite";
			break;
		case BallBehav.BallType.explode:
			currBallColor = ExplodeColor;
			ballName = "Bomb";
			break;
		case BallBehav.BallType.freeze:
			currBallColor = FreezeColor;
			ballName = "Gravity";
			break;
		case BallBehav.BallType.shotgun:
			currBallColor = ShotgunColor;
			ballName = "Shotgun";
			break;
		}
		if(Ball_Info)
			Ball_Info.text = "Ammo:  " + ballName + " - " + ballNum;
	}

	public Color messageColor1 = Color.red;
	public Color messageColor2 = Color.white;
	bool isMessageColor1 = true;
	int popMessage_NumOfFlash = 5;
	float popMessage_FlashInterval = 0.15f;

	public void UpdateMessage(string message){
		UpdateMessage(message, GameStat.MessageLifetime);
	}
	
	public void UpdateMessage(string message, float duration){
		for(int i = 1; i <= popMessage_NumOfFlash && i*popMessage_FlashInterval < duration; ++i)
		{
			Invoke("HighlightMessage", i * popMessage_FlashInterval);
		}
		if(PopMessage)
			PopMessage.text = message;
		Invoke("WipeMessage", duration);
	}

	void HighlightMessage()
	{
		if(PopMessage)
		{
			if(isMessageColor1){
				PopMessage.color = messageColor2;
			}else{
				PopMessage.color = messageColor1;
			}
			isMessageColor1 = !isMessageColor1;
		}
	}

	public void WipeMessage()
	{
		if(PopMessage)
		{
			PopMessage.text = "";
			PopMessage.color = messageColor1;
		}
	}

	private float shakeDuration = 0.3f;//in sec
	private float shakeIntensity = 0.1f;
	private float sI = 0;
	private Quaternion originRotation;

	//Shake the camera. Duration as in seconds
	public void CameraShake(float intensity, float duration)
	{
		shakeDuration = duration;
		shakeIntensity = intensity;
		if (sI <= 0)
		{
			originRotation = Camera.main.transform.rotation;
		}
		sI = intensity;
	}

	void ShakeUpdate()
	{
		if (sI > 0)
		{
			Camera.main.transform.rotation = new Quaternion(
				originRotation.x + UnityEngine.Random.Range(-sI, sI) * .2f,
				originRotation.y + UnityEngine.Random.Range(-sI, sI) * .2f,
				originRotation.z + UnityEngine.Random.Range(-sI, sI) * .2f,
				originRotation.w + UnityEngine.Random.Range(-sI, sI) * .2f);
			sI -= shakeIntensity / (1 / Time.deltaTime * shakeDuration);
		}
	}

	#endregion

	#region Player State  
	float mHealth = 100f;

	public void AddHealth(float num){
		mHealth += num;
		if (mHealth > 100f){
			mHealth = 100f;
		}
		if(HealthBar)
			HealthBar.value = mHealth;
	}
	
	public void UpdateEnergy(float num){
		if(EnergyBar)
			EnergyBar.value = num;
	}

	public void AddDamage(){
		mHealth -= EnemyStat.AttackDmg;
		if(HealthBar){
			HealthBar.value = mHealth;
		}
		UpdateMessage("Hit By Enemy!");
		if(mHealth < 0f)
		{
			DeathGameOver();
		}else{
			//this is causing a weird bug to move the gun out of place
//			CameraShake(0.2f, 0.3f);
		}
	}


	void GameOver()
	{
		//go back to menu
		Application.LoadLevel(0);
	}

	#endregion

	#region ObjectLoading
	GameObject explosion_prefab;
	GameObject splash_prefab;
	GameObject electric_prefab;
	GameObject darkFog_prefab;
	GameObject powerUp_prefab;
	GameObject ball_prefab;

	Vector3 nextPowerUpPos;

	public GameObject Explosion_prefab {get{return explosion_prefab;}}
	public GameObject Splash_prefab {get{return splash_prefab;}}
	public GameObject Electric_prefab {get{return electric_prefab;}}
	public GameObject DarkFog_prefab {get{return darkFog_prefab;}}
	public GameObject PowerUp_prefab {get{return powerUp_prefab;}}
	public GameObject Ball_prefab {get{return ball_prefab;}}

	void InitPrefabs()
	{
		explosion_prefab = Resources.Load(ResourcePath.Explosion_path) as GameObject;
		splash_prefab = Resources.Load(ResourcePath.Splash_path) as GameObject;
		electric_prefab = Resources.Load(ResourcePath.Electric_path) as GameObject;
		darkFog_prefab = Resources.Load(ResourcePath.DarkFog_path) as GameObject;
		powerUp_prefab = Resources.Load(ResourcePath.PowerUp_path) as GameObject;
		ball_prefab = Resources.Load(ResourcePath.Ball_path) as GameObject;;
	}

	public void SpawnBallEffect(GameObject effect, BallBehav.BallType type, Vector3 position, float duration)
	{
		GameObject result = GameObject.Instantiate(effect, position, Quaternion.identity) as GameObject;
		result.GetComponent<EffectAct>().SetLifeTimer(duration);
		result.GetComponent<EffectAct>().SetType(type);
		if(type == BallBehav.BallType.freeze)
		{
			Vector3 pos = result.transform.position;
			result.transform.position = new Vector3 (pos.x, 0.01f, pos.z);
			result.GetComponent<Renderer>().material.color = FreezeColor;
		}
	}

	public void SpawnNextPowerUp(Vector3 position, float delayTime)
	{
		nextPowerUpPos = position;
		if(powerUp_prefab){
			Invoke("OnSpawnPowerUp", delayTime);
		}
	}
	
	void OnSpawnPowerUp()
	{
		GameObject.Instantiate(powerUp_prefab, nextPowerUpPos, Quaternion.identity);
	}
	#endregion
}
