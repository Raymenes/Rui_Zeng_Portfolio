using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AI_ghost : AIStateMachine {
	public float searchRadius = 8f;
	public float chasingSpeed = 1.5f;
	public float attackTime = 5f;
	public float timeCantAttack = 3f;
	public float timeFlexibility = 2f;

	public float roamingTime = 4f;
	public float stopTime = 1.5f;

	private float velocity = 0f;
	private GameObject[] humans;
	private GameObject target;
	private bool canChase = true;
	private Vector3 destination;


	void Start()
	{
		this.AddStates("normal", "chasing", "attacking");
		this.AddRelationsForState("normal", "chasing");
		this.AddRelationsForState("chasing", "attacking");
		this.AddRelationsForState("attacking", "normal");

		this.CurState = this.stateByID("normal");

		humans = GameObject.FindGameObjectsWithTag("Human"); 
		destination = this.transform.position;
		this.SetToRandomVelocity ();

	}
	
	void Update()
	{
		UpdateBehaviors ();
		TransitionCheck ();
		WarpCheck ();
	}
	
	void UpdateBehaviors()
	{
		if(this.isOnState("normal"))
		{
			this.transform.position = Vector3.MoveTowards(transform.position, destination, velocity*Time.deltaTime);
		}
		else if(this.isOnState("chasing"))
		{

			this.transform.position = Vector2.Lerp(this.transform.position, ModTargetPosition(target), 
			                                       Time.deltaTime * chasingSpeed);
		}
		else if(this.isOnState("attacking"))
		{
			this.transform.position = ModTargetPosition(target);
		}
	}

	Vector3 ModTargetPosition(GameObject targetObj)
	{
		Vector3 targetPos = targetObj.transform.position;
		targetPos.y += targetObj.transform.localScale.y * 0.5f;
		return targetPos;
	}
	
	void TransitionCheck()
	{
		if(this.isOnState("normal"))
		{
			foreach(GameObject h in humans)
			{
				if(Vector2.Distance(this.transform.position, h.transform.position) <= searchRadius && canChase)
				{
					if(h.GetComponent<AI_human>().IsBeingTarget == false){
						target = h;
						target.GetComponent<AI_human>().IsBeingTarget = true;
						this.TransitToNextState();
						canChase = false;
						break;
					}
				}
			}
		}
		else if(this.isOnState("chasing"))
		{
			if(Vector2.Distance(this.transform.position, ModTargetPosition(target)) <= 0.3f)
			{
				target.GetComponent<AI_human>().IsUnderAttack = true;
				this.TransitToNextState();
				TimerToNormal();
			}
		}

	}

	void WarpCheck()
	{
		float wrapBoundary = Mathf.Abs(GameManager.Get().GetWorldBottomLeftPoint().x+gameObject.transform.localScale.x);
			if (Mathf.Abs (this.transform.position.x) > wrapBoundary)
		{
			this.transform.position = new Vector3(-Mathf.Sign(this.transform.position.x) * wrapBoundary,
			                                      this.transform.position.y,
			                                      this.transform.position.z);
		}
	}

	void TimerToNormal()
	{
		//SpawnGhostLocal ();
		Invoke ("ToNormal", attackTime + Random.Range(-timeFlexibility, timeFlexibility));
	}

	void ToNormal()
	{
		this.TransitToState("normal");
		target.GetComponent<AI_human>().IsUnderAttack = false;
		target.GetComponent<AI_human>().IsBeingTarget = false;
		Invoke ("SetCanChase", timeCantAttack + Random.Range(-timeFlexibility, timeFlexibility));
	}

	void SetCanChase()
	{
		canChase = true;
	}
	
	void SetToRandomVelocity()
	{
		velocity = RandomVelocity ();
		Vector3 screen = GameManager.Get().GetWorldBottomLeftPoint();
		destination = new Vector3(Random.Range(screen.x, -screen.x), Random.Range(screen.y, -screen.y), destination.z);
		Invoke ("SetToZeroVelocity", roamingTime + Random.Range(-timeFlexibility, timeFlexibility));
	}
	
	void SetToZeroVelocity()
	{
		velocity = 0f;
		Invoke ("SetToRandomVelocity", stopTime);
	}
	
	float RandomVelocity()
	{
		float speed = Random.Range (0f, 5f);
		
		return speed;
	}


	void SpawnGhostLocal()
	{
		GameObject newGhost = GameObject.Instantiate (this, this.transform.position, Quaternion.identity) as GameObject;
	}


}
