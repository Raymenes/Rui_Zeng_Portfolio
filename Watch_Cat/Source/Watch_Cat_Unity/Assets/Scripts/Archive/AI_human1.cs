using UnityEngine;
using System.Collections;

public class AI_human1 : AIStateMachine {

	private Vector3 velocity = Vector3.zero;

	void Start()
	{
		this.AddStates("normal", "haunted");
		this.AddRelationsForState("normal", "haunted");
		this.AddRelationsForState("haunted", "normal");

		this.CurState = this.stateByID("normal");

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
		this.transform.position += velocity * Time.deltaTime;
	}

	void TransitionCheck()
	{
		if(Input.GetKeyDown(KeyCode.Space))
		{
			if(this.isOnState("normal"))
			{
				this.TransitToState("haunted");
				this.CancelInvoke();
				this.velocity = Vector3.left * 10f;

			}	
		}
		
		if(Input.GetKeyDown(KeyCode.A))
		{
			if(this.isOnState("haunted"))
			{
				this.TransitToState("normal");
				SetToRandomVelocity();
			}
		}
	}

	void WarpCheck()
	{
		if (Mathf.Abs (this.transform.position.x) > 10.6f)
		{
			this.transform.position = new Vector3(-Mathf.Sign(this.transform.position.x) * 10.55f,
			                                      this.transform.position.y,
			                                      this.transform.position.z);
		}
	}
	
	void SetToRandomVelocity()
	{
		velocity = RandomVelocity ();
		Invoke ("SetToZeroVelocity", 3f);
	}

	void SetToZeroVelocity()
	{
		velocity = Vector3.zero;
		Invoke ("SetToRandomVelocity", 3f);
	}

	Vector3 RandomVelocity()
	{
		float speed = Random.Range (-5f, 5f);

		return (new Vector3 (speed, 0, 0));
	}
}
