using UnityEngine;
using System.Collections;

public class PlayerMovementMode2 : MonoBehaviour {
	
	public bool red;
	
	public Vector3 originPosition = new Vector3(-11f, 0f, 0f);
	public Vector3 yDifference = new Vector3(0f, 2f, 0f);
	Vector3[] positionArray = new Vector3[5];
	int nowPosition = 2;
	
	string player1UpKey = "w";
	string player1DownKey = "s";
	
	string player2UpKey = "i";
	string player2DownKey = "k";
	
	string player1FireKey = "d";
	string player2FireKey = "j";
	
	string player1LoadKey = "a";
	string player2LoadKey = "l";
	
	string UpKey;
	string DownKey;
	string FireKey;
	string LoadKey;
	Vector3 bulletPosition;
	Vector3 bulletAdjustMent;
	
	int bulletStarIndex;
	int bulletSquareIndex;
	int bulletCircleIndex;
	
	string bulletType;
	string next1BulletType;
	string next2BulletType;

	string[] shapeName = new string[3];

	GameObject bullet;
	GameObject starBullet;
	GameObject squareBullet;
	GameObject circleBullet;


	GameObject displayNext1Bullet;
	GameObject displayNext2Bullet;
	
	GameObject bulletIndicator;

	public bool chaosRandomLoad;
	public bool predicatableRandomLoad;
	public bool predictableFixedLoad;
	public bool manualLoad;

	void Start () 
	{
		if (gameObject.name == "Player1")
		{
			red = true;
			transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
		}
		if (gameObject.name == "Player2")
		{
			red = false;
			transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
		}
		
		positionArray[2] = originPosition;
		positionArray[1] = positionArray[2]-yDifference;
		positionArray[0] = positionArray[1]-yDifference;
		positionArray[3] = positionArray[2]+yDifference;
		positionArray[4] = positionArray[3]+yDifference;
		
		if (red)
		{
			UpKey = player1UpKey;
			DownKey = player1DownKey;
			FireKey = player1FireKey;
			LoadKey = player1LoadKey;
			bulletAdjustMent = Vector3.right;
			bulletIndicator = GameObject.Find("Player1BulletIndicator");
			displayNext1Bullet = GameObject.Find("Player1Next1Bullet");
			displayNext2Bullet = GameObject.Find("Player1Next2Bullet");
		}
		else
		{
			UpKey = player2UpKey;
			DownKey = player2DownKey;
			FireKey = player2FireKey;
			LoadKey = player2LoadKey;
			bulletAdjustMent = Vector3.left;
			bulletIndicator = GameObject.Find("Player2BulletIndicator");
			displayNext1Bullet = GameObject.Find("Player2Next1Bullet");
			displayNext2Bullet = GameObject.Find("Player2Next2Bullet");
		}
		
		starBullet = Resources.Load<GameObject>("StarBullet");
		squareBullet = Resources.Load<GameObject>("SquareBullet");
		circleBullet = Resources.Load<GameObject>("CircleBullet");

		bullet = squareBullet;
		bulletType = "square";
		InitNext12Bullet();
		//InvokeRepeating("PredicatableNextBulletLoad", 4f, 4f);
	}
	
	void Update () 
	{

		if (Input.GetKeyDown(DownKey))
		{
			if (nowPosition > 0)
			{
				nowPosition -= 1;
			}
		}
		if (Input.GetKeyDown(UpKey))
		{
			if (nowPosition < 4)
			{
				nowPosition += 1;
			}
		}
		//print("now position is " +nowPosition);
		if (Input.GetKeyDown(LoadKey))
		{
			if (chaosRandomLoad)
			{}
			else if (predicatableRandomLoad)
			{
				PredicatableNextBulletLoad();
			}
			else if (predictableFixedLoad)
			{
				FixedNextBulletLoad();
			}
		}
		
		transform.position = positionArray[nowPosition];
		
		if (Input.GetKeyDown(FireKey))
		{
			GameObject myBullet;
			myBullet = Instantiate(bullet, transform.position+bulletAdjustMent, Quaternion.identity)
				as GameObject;
			myBullet.name = bulletType;
			myBullet.GetComponent<BulletMovement>().SetBulletIdentity(FireKey);
			myBullet.GetComponent<BulletMovement>().SetBulletShape(bulletType);
		}

		if (manualLoad)
		{
			if (Input.GetKeyDown(KeyCode.Alpha1))
			{
				bulletType = "square";
				bullet = squareBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("square");
			}
			if (Input.GetKeyDown(KeyCode.Alpha3))
			{
				bulletType = "star";
				bullet = starBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("star");
				
			}
			if (Input.GetKeyDown(KeyCode.Alpha2))
			{
				bulletType = "circle";
				bullet = circleBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("circle");
			}
		}
		
	}

	int GetRandNum013 ()
	{
		return ((int)Random.Range(0, 100f))%3;
	}

	void RandomNextBulletLoad ()
	{
		string nowShape;
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";
		do
		{
			nowShape = shapeName[GetRandNum013()];
		}while (nowShape == bulletType);
		
		bulletType= nowShape;

		if (bulletType == "square")
		{
			bullet = squareBullet;
		}
		else if (bulletType == "circle")
		{
			bullet = circleBullet;
		}
		else if (bulletType == "star")
		{
			bullet = starBullet;
		}

		bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType(bulletType);
	}

	void PredicatableNextBulletLoad ()
	{
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";
		string temp = next2BulletType;
		do
		{
			next2BulletType = shapeName[GetRandNum013()];
		}while (next2BulletType == temp);
		bulletType = next1BulletType;
		next1BulletType = temp;
		bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType(bulletType);
		displayNext1Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next1BulletType);
		displayNext2Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next2BulletType);
		if (bulletType == "square")
		{
			bullet = squareBullet;
		}
		else if (bulletType == "circle")
		{
			bullet = circleBullet;
		}
		else if (bulletType == "star")
		{
			bullet = starBullet;
		}
	}

	void FixedNextBulletLoad ()
	{
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";
		string lastNext2 = next2BulletType;
		string nextNext2 = bulletType;


		bulletType = next1BulletType;
		next2BulletType = nextNext2;
		next1BulletType = lastNext2;
		bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType(bulletType);
		displayNext1Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next1BulletType);
		displayNext2Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next2BulletType);
		if (bulletType == "square")
		{
			bullet = squareBullet;
		}
		else if (bulletType == "circle")
		{
			bullet = circleBullet;
		}
		else if (bulletType == "star")
		{
			bullet = starBullet;
		}
	}

	void InitNext12Bullet()
	{
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";

		if (predictableFixedLoad)
		{
			next1BulletType = "circle";
			next2BulletType = "star";
		}

		else
		{
			do
			{
				next1BulletType = shapeName[GetRandNum013()];
			}while (next1BulletType == bulletType);
			
			if (next1BulletType == "star")
			{
				next2BulletType = "circle";
			}
			else if (next1BulletType == "circle")
			{
				next2BulletType = "star";
			}
		}

		/*Debug.Log("next 1 bullet type: " + next1BulletType);
		Debug.Log("next 2 bullet type: " + next2BulletType);
		Debug.Log(displayNext1Bullet);
		Debug.Log(displayNext2Bullet);*/

		displayNext1Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next1BulletType);
		displayNext2Bullet.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(next2BulletType);

	}

}
