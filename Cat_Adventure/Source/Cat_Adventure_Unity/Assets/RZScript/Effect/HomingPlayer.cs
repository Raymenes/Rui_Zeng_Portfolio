using UnityEngine;
using System.Collections;

public class HomingPlayer : MonoBehaviour {

	float bulletStartTime;
	public float bulletStraightToCurveTimer = 0.35f;
	public float bulletFlyTimer = 1.2f;
	public float homingComparativeSpeed = 2f;
	float bulletHomingTime; 
	float currentSpeed;
	float normalSpeed;

	Vector3 movement;
	Vector3 startPosition;
	GameObject target;
	
	float homingSensitive;
	float homingSensitiveVeryMin = 0.1f;
	public float homingSensitiveMin = 3f;
	public float homingSensitiveMax = 6f;
	float timeOfHomingAcceleration = 1.6f; 
	
	//float timeOfSpeedAcceleration = 1f;

	//float minSpeed = 5f;
	//float maxSpeed;
	

	

	
	
	void Start () 
	{

		bulletStartTime = Time.timeSinceLevelLoad;
		startPosition = gameObject.transform.position;
		
		homingSensitive = homingSensitiveMin;
		currentSpeed =  RzSpeedManager.CatNormalSpeed + 2f;

		//currentSpeed = minSpeed;
		//maxSpeed = speed;



		target = GameObject.FindGameObjectWithTag("Player");


	}
	
	void Update () 
	{
		/*if (currentSpeed <= maxSpeed)
		{
			currentSpeed += (maxSpeed-minSpeed)/timeOfSpeedAcceleration * Time.deltaTime;
		}*/

		bulletHomingTime = Time.timeSinceLevelLoad - bulletStartTime;
		
		if (bulletHomingTime >= bulletStraightToCurveTimer && bulletHomingTime <= (bulletStraightToCurveTimer + bulletFlyTimer))
		{
			if (homingSensitive <= homingSensitiveMax)
			{
				homingSensitive += (homingSensitiveMax-homingSensitiveMin)/timeOfHomingAcceleration * Time.deltaTime;
			}
			
			Vector3 thisToTarget = target.transform.position - gameObject.transform.position;
			float sign = Mathf.Sign (Vector3.Dot(thisToTarget, gameObject.transform.up));
			float angle = Vector3.Angle(gameObject.transform.right, thisToTarget);
			angle = angle * sign;
			Vector3 rotation = new Vector3(0f, 0f, angle);
			transform.Rotate(rotation * Time.deltaTime * homingSensitive);
		}
		transform.Translate (Vector3.right * Time.deltaTime * currentSpeed);
		
	}
	
	
	void OnCollisionEnter2D (Collision2D other)
	{

		
		if (other.gameObject.tag == "!@#$")
			Destroy(gameObject);
		

		
	}
	

}
