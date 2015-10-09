using UnityEngine;
using System.Collections;

public class PlayerMovement : MonoBehaviour {
	
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

	string player1PickKey = "a";
	string player2PickKey = "l";

	string UpKey;
	string DownKey;
	string FireKey;
	string PickKey;
	Vector3 bulletPosition;
	Vector3 bulletAdjustMent;

	int bulletStarIndex;
	int bulletSquareIndex;
	int bulletCircleIndex;

	string bulletType;

	GameObject bullet;
	GameObject starBullet;
	GameObject squareBullet;
	GameObject circleBullet;
	
	GameObject bulletIndicator;


	
	void Start () 
	{

		if (gameObject.name == "Player1")
		{
			red = true;
			Debug.Log ("player is player 1 ");
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
			PickKey = player1PickKey;
			bulletAdjustMent = Vector3.right;
			bulletIndicator = GameObject.Find("Player1BulletIndicator");

		}
		else
		{
			UpKey = player2UpKey;
			DownKey = player2DownKey;
			FireKey = player2FireKey;
			PickKey = player2PickKey;
			bulletAdjustMent = Vector3.left;
			bulletIndicator = GameObject.Find("Player2BulletIndicator");

		}

		starBullet = Resources.Load<GameObject>("StarBullet");
		squareBullet = Resources.Load<GameObject>("SquareBullet");
		circleBullet = Resources.Load<GameObject>("CircleBullet");

		bullet = squareBullet;
		bulletType = "square";
	}
	
	void Update () 
	{
		if (gameObject.name == "Player1")
		{
			transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
		}
		if (gameObject.name == "Player2")
		{
			transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
		}

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
		if (Input.GetKeyDown(PickKey))
		{
			if (nowPosition == bulletSquareIndex)
			{
				bulletType = "square";
				bullet = squareBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("square");
			}
			if (nowPosition == bulletStarIndex)
			{
				bulletType = "star";
				bullet = starBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("star");

			}
			if (nowPosition == bulletCircleIndex)
			{
				bulletType = "circle";
				bullet = circleBullet;
				bulletIndicator.GetComponent<BulletIndicatorBehavior>().SetBulletType("circle");
			}

		}
		
		transform.position = positionArray[nowPosition];

		if (Input.GetKeyDown(FireKey))
		{
			GetComponent<AudioSource>().Play();
			GameObject myBullet;
			myBullet = Instantiate(bullet, transform.position+bulletAdjustMent, Quaternion.identity)
						as GameObject;
			myBullet.name = bulletType;
			myBullet.GetComponent<BulletMovement>().SetBulletIdentity(FireKey);
			myBullet.GetComponent<BulletMovement>().SetBulletShape(bulletType);
		}


	}

	public void SetPlayerBulletSelection (int starPosition, int squarePosition, int circlePosition)
	{
		bulletStarIndex = starPosition;
		bulletSquareIndex = squarePosition;
		bulletCircleIndex = circlePosition;
	}
}
