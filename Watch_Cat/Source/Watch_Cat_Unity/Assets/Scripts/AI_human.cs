using UnityEngine;
using System.Collections;

public class AI_human : AIStateMachine {

	public GameObject healthBar;
	private Vector3 velocity = Vector3.zero;
	private float health = 100f;
	private float healthLoseRate = 10f;

	private bool isUnderAttack = false;
	private bool isBeingTarget = false;
	private bool isBeingCharged = false;

	public float timeFlexibility = 2f;
	public float stopTime = 3f;
	public float walkTime = 3f;

	private float healthBarLength;


	public float Health
	{
		get{return health;}
	}
	public bool IsUnderAttack
	{
		get{return isUnderAttack;}
		set{isUnderAttack = value;}
	}
	public bool IsBeingTarget
	{
		get{return isBeingTarget;}
		set{isBeingTarget = value;}
	}
	public bool IsBeingCharged
	{
		get{return isBeingCharged;}
		set{isBeingCharged = value;}
	}

	void Start()
	{
		this.AddStates("positive", "negative", "haunted");
		this.AddRelationsForState("positive", "negative");
		this.AddRelationsForState("negative", "positive", "haunted");

		this.CurState = this.stateByID("positive");

		this.SetToRandomVelocity ();

		healthBarLength = healthBar.transform.localScale.x;
	}

	void Update()
	{
		UpdateBehaviors ();
		TransitionCheck ();
		WarpCheck ();
		UpdateHealthBar();
	}

	void UpdateBehaviors()
	{
		this.transform.position += velocity * Time.deltaTime;
		if(this.CurState == this.stateByID("negative"))
		{
			GameManager.LoseScore();
		}
		if(this.CurState == this.stateByID("positive"))
		{
			GameManager.AddScore();
		}

		if(isUnderAttack)
		{
			if(health>=0f)
				health -= healthLoseRate * Time.deltaTime;
		}
		if(isBeingCharged){
			if(health < 100f)
				health += healthLoseRate * Time.deltaTime;
		}
	}

	void TransitionCheck()
	{
		if(this.CurState == this.stateByID("positive"))
		{
			if(this.health < 50f)
			{
				this.TransitToState("negative");
			}
		}

		if(this.CurState == this.stateByID("negative"))
		{
			if(this.health > 50f)
			{
				this.TransitToState("positive");
			}
			else if(this.health < 0)
			{
				this.TransitToState("haunted");
				this.GetComponent<Renderer>().material.color = Color.black;
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

	void UpdateHealthBar()
	{
		if(healthBar){
			Vector3 offset = healthBar.transform.localScale;
			healthBar.transform.localScale = new Vector3(health/100f * healthBarLength, offset.y, offset.z) ;
		}
		if(health <= 50f){
			healthBar.GetComponent<Renderer>().material.color = Color.black;
		}else if(health >50f){
			this.GetComponent<Renderer>().material.color = Color.yellow;
			healthBar.GetComponent<Renderer>().material.color = Color.white;
		}
	}
	
	void SetToRandomVelocity()
	{
		CancelInvoke();
		velocity = RandomVelocity ();
		Invoke ("SetToZeroVelocity", walkTime + Random.Range(-timeFlexibility, timeFlexibility));
	}

	public void SetToZeroVelocity()
	{
		CancelInvoke();
		velocity = Vector3.zero;
		Invoke ("SetToRandomVelocity", stopTime + Random.Range(-timeFlexibility, timeFlexibility));
	}

	Vector3 RandomVelocity()
	{
		float speed = Random.Range (-5f, 5f);

		return (new Vector3 (speed, 0, 0));
	}

	void OnGUI()
	{
	}
}
