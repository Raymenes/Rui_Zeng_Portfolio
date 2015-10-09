using UnityEngine;
using System.Collections;

public class RandomizeBulletSelection : MonoBehaviour {

	public int playerIndex;
	// 0, 2, 4
	// 0, 1, 3
	Random rand = new Random();
	int randomNumber;
	int position1;
	int position2;
	int position3;

	string shape1, shape2, shape3;

	Sprite starShape, squareShape, circleShape;
	SpriteRenderer sr;
	GameObject playerBulletStar, playerBulletSquare, playerBulletCircle;
	
	GameObject player1, player2;

	void Start () 
	{
		player1 = GameObject.Find("Player1");
		player2 = GameObject.Find("Player2");

		//GetRandomPositions();

		InvokeRepeating("GetRandomPositions", 0f, 3f);


	}
	
	void Update () 
	{





	}

	int GetRandNum013 ()
	{
		return ((int)Random.Range(0, 100f))%3;
	}

	void GetRandomPositions ()
	{
		position1 = GetRandNum013();
		do 
		{
			position2 = GetRandNum013();
		}while(position2 == position1);
		do 
		{
			position3 = GetRandNum013();
		}while(position3 == position1 || position3 == position2);

		position1 = position1*2;
		position2 = position2*2;
		position3 = position3*2;

		//print ("position one is " + position1.ToString());
		//print ("position two is " + position2.ToString());
		//print ("position three is " + position3.ToString());

		if (position1 == 0)
			shape1 = "star";
		if (position2 == 0)
			shape1 = "square";
		if (position3 == 0)
			shape1 = "circle";
		if (position1 == 2)
			shape2 = "star";
		if (position2 == 2)
			shape2 = "square";
		if (position3 == 2)
			shape2 = "circle";
		if (position1 == 4)
			shape3 = "star";
		if (position2 == 4)
			shape3 = "square";
		if (position3 == 4)
			shape3 = "circle";
		if (playerIndex == 1)
		{
			player1.GetComponent<PlayerMovement>().SetPlayerBulletSelection(position1, position2, position3);
			playerBulletStar = GameObject.Find("Player1BulletStar");
			playerBulletSquare = GameObject.Find("Player1BulletSquare");
			playerBulletCircle = GameObject.Find("Player1BulletCircle");
			
		}
		else if(playerIndex == 2)
		{
			player2.GetComponent<PlayerMovement>().SetPlayerBulletSelection(position1, position2, position3);
			playerBulletStar = GameObject.Find("Player2BulletStar");
			playerBulletSquare = GameObject.Find("Player2BulletSquare");
			playerBulletCircle = GameObject.Find("Player2BulletCircle");
		}
		playerBulletStar.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(shape3);
		playerBulletSquare.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(shape2);
		playerBulletCircle.GetComponent<PlayerBulletSpecification>().ChangeBulletShapeTo(shape1);

	}
}
