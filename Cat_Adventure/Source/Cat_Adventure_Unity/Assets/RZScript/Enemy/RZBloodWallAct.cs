using UnityEngine;
using System.Collections;

public class RZBloodWallAct : MonoBehaviour {

	//was public
	float normalSpeed = 20f;
	float retreatSpeed = 15f;
	float chasingSpeed;
	//
	float currentSpeed;
	Vector3 currentDirection;
	Vector3 velocity;
	GameObject player;
	float startOffset;
	float currentOffset;

	void Awake ()
	{
		normalSpeed = RzSpeedManager.BloodNormalSpeed;
		retreatSpeed = RzSpeedManager.BloodRetreatSpeedOffset;
		chasingSpeed = RzSpeedManager.BloodChaseSpeedOffset;
	}

	void Start () 
	{
		currentSpeed = normalSpeed;
		currentDirection = Vector3.right;
		player = GameObject.FindGameObjectWithTag("Player");
		startOffset = player.transform.position.x - this.transform.position.x;
	}
	
	void Update () 
	{
		currentOffset = player.transform.position.x - this.transform.position.x;

		velocity = currentSpeed * currentDirection;

		if (currentOffset < startOffset)
		{
			currentSpeed = normalSpeed - retreatSpeed;
		}

		else if ((currentOffset - startOffset) >1f)
		{
			currentSpeed = normalSpeed + chasingSpeed;
		}
		else
			currentSpeed = normalSpeed;
		
		this.transform.Translate(velocity*Time.deltaTime, Space.World);
	}
}
