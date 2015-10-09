using UnityEngine;
using System.Collections;

public class RzBirdAct : MonoBehaviour {

	public bool birdStraight = true, birdHoming = false;
	float normalSpeed;
	Vector3 velocity;
	Vector3 birdDirection;
	GameObject birdHead;
	GameObject player;
	bool meetCat = false;
	public float triggerRange = 60f;

	void Start () 
	{
		normalSpeed = RzSpeedManager.BirdSpeed;

		player = GameObject.FindWithTag("Player");

		//need to adjust to the sprite facing direction
		Vector3 heading;
		float distance;

		heading = transform.eulerAngles;
		distance = heading.magnitude;

		birdDirection = heading/distance;
		//print("bird direction is" + birdDirection);

		//birdDirection = Vector3.left;

		//print();
		//print(transform.eulerAngles.x);
		//print(transform.eulerAngles.y);
		//print(transform.eulerAngles.z);

		birdHead = transform.GetChild(0).gameObject;

	}
	
	void Update () 
	{
		float distanceToPlayer = transform.position.x - player.transform.position.x;
		//Debug.Log(distanceToPlayer);

		if ((transform.position.x - player.transform.position.x) < triggerRange)
		{
			meetCat = true;
		}
		if ((transform.position.x - player.transform.position.x) < -180f)
		{
			meetCat = false;
		}


		if (birdStraight && meetCat)
		{
		velocity = normalSpeed * GetDirectionToPoint(birdHead.transform.position);
		this.transform.Translate(velocity*Time.deltaTime, Space.World);
		}
	}
	Vector3 GetDirectionToPoint (Vector3 point)
	{
		Vector3 heading = point - this.transform.position;
		float distance = heading.magnitude;
		Vector3 direction = heading/distance;
		return direction;
	}
}
