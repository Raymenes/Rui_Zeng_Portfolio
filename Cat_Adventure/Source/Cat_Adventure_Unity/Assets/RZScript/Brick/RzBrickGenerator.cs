using UnityEngine;
using System.Collections;

public class RzBrickGenerator : MonoBehaviour {

	public GameObject UpCenterBrick, UpCenterNextBrick, UpLeftBrick, UpRightBrick, DownCenterBrick;
	public float brickSideLength;
	public float brickScreenHeight;
	public float brickScreenWidth;

	Vector3 offset;
	GameObject player;
	float yValue;
	public bool followMode;
	public bool staticMode;
	public float brickOutOfSightRange = 1000f;
	public int maxBrickNum = 250;

	Vector3 playerStartPosition;
	Vector3 BgGeneratePosition = new Vector3 (0f, 0f, 0f);
	
	Vector3 generatePositionTop, generatePositionBottom;

	void Start () 
	{
		brickSideLength = 4f;
		brickScreenHeight = UpCenterBrick.transform.position.y - DownCenterBrick.transform.position.y;
		brickScreenWidth = UpRightBrick.transform.position.x - UpLeftBrick.transform.position.x;
		/*
		GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Brick"), 
		                                                        this.transform.position, Quaternion.identity);
		brick.transform.parent = gameObject.transform;*/
		GenerateRow("top");
		GenerateRow("bottom");


		player = GameObject.FindGameObjectWithTag("Player");
			offset = transform.position - player.transform.position;
			yValue = transform.position.y;

		playerStartPosition = player.transform.position;
		generatePositionTop = UpRightBrick.transform.position;
		generatePositionBottom = new Vector3(UpRightBrick.transform.position.x, DownCenterBrick.transform.position.y, 0f);
	}
	
	void Update () 
	{
		if (followMode)
		{
			gameObject.transform.position = new Vector3(player.transform.position.x + offset.x, yValue, 0f);
		}
		if (staticMode)
		{

		}
		KeepGenerating();

	}


	void GenerateRow(string row)
	{
		Vector3 generatePosition;
		if (row.Equals("top"))
			generatePosition = UpLeftBrick.transform.position;
		else 
			generatePosition = new Vector3(UpLeftBrick.transform.position.x, DownCenterBrick.transform.position.y, 0f);
		while (generatePosition.x < (UpRightBrick.transform.position.x+brickOutOfSightRange))
		{
			GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Brick"), 
			                                                       generatePosition, Quaternion.identity);
			brick.transform.parent = gameObject.transform;
			generatePosition.x += brickSideLength;
		}
	}

	void KeepGenerating ()
	{
		if (transform.childCount <= maxBrickNum)
		{
			generatePositionTop.x += brickSideLength;
			generatePositionBottom.x += brickSideLength;
			playerStartPosition = player.transform.position;
			GameObject brickTop = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Brick"), 
			                                                       generatePositionTop, Quaternion.identity);
			brickTop.transform.parent = gameObject.transform;
			GameObject brickBottom = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Brick"), 
			                                                       generatePositionBottom, Quaternion.identity);
			brickBottom.transform.parent = gameObject.transform;
		}
	}

	public float GetBrickAutoDestroyRange()
	{
		float result = brickOutOfSightRange + brickScreenWidth/2;
		return result;
	}

	public string GetBrickGeneratorMode ()
	{
		if (followMode)
			return "follow";
		else
			return "static";
	}

	public float GetBrickScreenWidth ()
	{
		return brickScreenWidth;
	}
	public float GetBrickScreenHeight ()
	{
		return brickScreenHeight;
	}
	public float GetBrickSideLength ()
	{
		return brickSideLength;
	}
}
