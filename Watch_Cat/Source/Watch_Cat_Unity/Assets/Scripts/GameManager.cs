using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class GameManager : MonoBehaviour {

	public GameObject gameInfo;

	void UpdateGameInfo(){
		gameInfo.GetComponent<Text>().text = 
				"Human Alive:" + human_alive.ToString() + "\n" +
				"Ghost Killed:" + ghost_killed.ToString() + "\n" +
				"Died:" + death.ToString() + "\n" +
				"Score:" + score.ToString();
	}	

	#region Static Variable & Function
	public static int numOfGm = 0;
	public static GameManager gm;

	static int human_total;
	static int human_alive;
	static int human_goal;
	static int ghost_killed;
	static int ghost_goal;
	static int level;
	static int death = 0;
	static float score;
	static int score_goal;
	static float score_addRate;
	static float score_loseRate;
	static bool beatLevel;
	
	public static GameManager Get(){return gm;}
	
	public static void KillGhost(){
		ghost_killed += 1;
	}
	
	public static void HauntHuman(){
		human_alive -= 1;
	}
	
	public static void AddScore(){
		score += score_addRate * Time.deltaTime;
	}
	
	public static void LoseScore(){
		score -= score_loseRate * Time.deltaTime;
	}
	
	public static bool IsLevelUnlock(){
		return beatLevel;
	}
	#endregion
	
	#region Game Running
	void Start () 
	{
		numOfGm += 1;
		if(numOfGm > 1)
			throw new UnityException("There is more than one GameManager");
		
		level = 1;
		ghost_killed = 0;
		ghost_goal = 10;
		human_total = 5;
		human_alive = 5;
		human_goal = 1;
		score = 0;
		score_goal = 100;
		beatLevel = false;
		gm = this;
		
		GenerateNextLevel();
	}
	
	void Update () 
	{
		UpdateGameInfo();
		if(human_alive < human_goal){
			//game lost
		}
		if(ghost_killed > ghost_goal){
			//level finished
			if((int)score > score_goal){
				//level beat
				beatLevel = true;
				LevelUp();
			}
		}
	}
	#endregion
	
	#region Procedure Generation
	
	public GameObject buildingBlock;
	public GameObject ghost;
	public GameObject human;
	Vector3 worldBottomLeftPoint;
	public float groundLevelY = -3f;
	public int numOfBuilding = 5;
	public int minBuildingHeight = 3;
	public int maxBuildingHeight = 9;
	public float buildingMinDistance = 3f;
	public float buildingMaxDistance = 5f;

	public static void LevelUp(){
		level += 1;
		score = 0;
		score_goal += 25;
		ghost_goal += 1;
	}

	public Vector3 GetWorldBottomLeftPoint() { return worldBottomLeftPoint; }


	private void GenerateNextLevel(){
		Debug.Log("worldBottomLeftPoint: " + Camera.main.ViewportToWorldPoint(new Vector3(0f, 0f, 10f)));
		
		worldBottomLeftPoint = Camera.main.ViewportToWorldPoint(new Vector3(0f, 0f, 10f));
		
		float gap = Random.Range(0, buildingMinDistance);
		Vector3 initPoint = new Vector3(worldBottomLeftPoint.x, groundLevelY, 0f);
		initPoint.x += gap;
		
		GameObject environment = GameObject.Find("Environment");
		if(!environment){
			environment = this.gameObject;
		}
		
		//Generate Buildings
		for (int i = 0; i < numOfBuilding; ++i)
		{
			GameObject building = new GameObject("building" + i.ToString());
			building.transform.parent = environment.transform;
			int buildingHeight = Random.Range(minBuildingHeight, maxBuildingHeight);
			for(int j = 0; j < buildingHeight; ++j)
			{
				GameObject block = GameObject.Instantiate(buildingBlock, initPoint, Quaternion.identity) as GameObject;
				block.transform.parent = building.transform;
				initPoint.y += 1f;
			}
			
			initPoint.y = groundLevelY;
			initPoint.x += Random.Range(buildingMinDistance, buildingMaxDistance);
		}
		
		//Spawn Ghost and Human
		float humanGroundLevel = groundLevelY + (human.transform.localScale.y-1f) * 0.5f;
		GameObject humans = GameObject.Find("Humans");
		if(!humans){
			humans = this.gameObject;
		}
		for (int i = 0; i < human_total; ++i){
			Vector3 randomPos = new Vector3(Random.Range(worldBottomLeftPoint.x, -worldBottomLeftPoint.x), humanGroundLevel, 0f);
			GameObject humanClone = GameObject.Instantiate(human, randomPos, Quaternion.identity) as GameObject;
			humanClone.transform.parent = humans.transform;
		}

		float ghostGroundLevel = groundLevelY + (ghost.transform.localScale.y-1f) * 0.5f;
		GameObject Ghosts = GameObject.Find("Ghosts");
		if(!Ghosts){
			Ghosts = this.gameObject;
		}
		for (int i = 0; i < human_total; ++i){
			Vector3 randomPos = new Vector3(
				Random.Range(worldBottomLeftPoint.x, -worldBottomLeftPoint.x), 
				Random.Range(ghostGroundLevel, -worldBottomLeftPoint.y), 
				0f);
			GameObject ghostClone = GameObject.Instantiate(ghost, randomPos, Quaternion.identity) as GameObject;
			ghostClone.transform.parent = Ghosts.transform;
		}
	}
	#endregion
}
