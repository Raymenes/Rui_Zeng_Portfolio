using UnityEngine;
using System.Collections;

public class RzCatBehavior : MonoBehaviour {

	string movingPattern;
	string listOfPattern = "Line" + "Stay" + "Wrap";
	/*public*/ float normalSpeed = 10f;
	float currentSpeed;
	Vector3 currentDirection;
	Vector3 velocity;
	GameObject interactBlock;
	Vector3 lastPosition;
	bool onAnotherButton = false;
	public bool constantSpeedTowardRight = true;
	string blockOnHold;
	string lastInteractBlockName;
	bool isBurst = false;
	/*public*/ float burstDuration = 1.5f;
	/*public*/ float burstAmount = 10f;

	void Awake ()
	{
		normalSpeed = RzSpeedManager.CatNormalSpeed;
		burstAmount = RzSpeedManager.CatBurstSpeedOffset;
		burstDuration = RzSpeedManager.CatBurstSpeedDuration;
	}

	void Start () 
	{
		movingPattern = "Line";
		currentDirection = Vector3.right;
		currentSpeed = normalSpeed;
		lastPosition = Vector3.left;
	}




//****************** Game Running **************************
	void Update () 
	{
		if (movingPattern.Equals("Line"))
		{
			//velocity = GetLineDirection(currentSpeed);
			velocity = currentSpeed * currentDirection;
			if (constantSpeedTowardRight)
			{
				if (velocity.x > 0)
					velocity = new Vector3(currentSpeed, velocity.y, 0f);
				if (velocity.x<0)
					velocity = new Vector3(-currentSpeed, velocity.y, 0f);
			}
				
			this.transform.Translate(velocity*Time.deltaTime, Space.World);
			FaceAlongTrail();

			if(Input.GetMouseButtonDown(1))
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				currentDirection = GetDirectionToPoint(mousePosition);

				Instantiate(Resources.Load<GameObject>("RedDot"), mousePosition, Quaternion.identity);
				//FacePoint(mousePosition);
			}
		}


		if (movingPattern.Equals("Stay"))
		{
			if (interactBlock == null)
			{
				if (lastInteractBlockName == "Tri")
				{
					Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
					mousePosition.z = 0f;
					currentDirection = GetDirectionToPoint(mousePosition);
					movingPattern = "Line";
				}
				else
					movingPattern = "Line";
			}

			StayInBlock(interactBlock);

			if(interactBlock.tag.Equals("Ring"))
			{
				currentDirection = Vector3.right;
				FaceNewDirection(Vector3.right);
			}
			if(interactBlock.tag.Equals("Tri"))
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				FacePoint(mousePosition);
				//ObjectFacePoint(interactBlock, mousePosition);
				interactBlock.GetComponent<RzPlaceable>().SetTriReadyToShoot(true);
			}


			if(Input.GetMouseButtonDown(0) && onAnotherButton == false && blockOnHold=="None")
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				if (movingPattern.Equals("Stay"))
				{
					if (interactBlock.tag.Equals("Ring"))
					{
						movingPattern = "Line";
						Destroy(interactBlock, 1f);
					}
					if (interactBlock.tag.Equals("Tri"))
					{
						isBurst = true;
						Invoke("EndBurst", burstDuration);

						currentDirection = GetDirectionToPoint(mousePosition);
						movingPattern = "Line";
						interactBlock.GetComponent<RzPlaceable>().SetTriReadyToShoot(false);
						Destroy(interactBlock, 1f);
					}
				}
			}
		}
		if (movingPattern.Equals("Wrap"))
		{
			//gameObject.SetActive(false);
			//DisableCollider();
			gameObject.GetComponent<Renderer>().enabled = false;
			Debug.Log ("Wrap!");
			if(Input.GetMouseButtonDown(0) && onAnotherButton== false)
			{

				Debug.Log ("mouse clicked!");
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				GameObject releaseCross = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Cross"), 
				             mousePosition, Quaternion.identity);
				releaseCross.GetComponent<RzPlaceable> ().enabled = false;


				releaseCross.GetComponent<BoxCollider2D> ().enabled = false;

				gameObject.transform.position = mousePosition;
				Destroy(releaseCross, 1f);
				Destroy(interactBlock, 1f);
				movingPattern = "Line";
				gameObject.GetComponent<Renderer>().enabled = true;
			}
		}

		//test
		/*
		if (Input.GetKeyDown(KeyCode.B))
		{
			isBurst = true;
			Invoke("EndBurst", burstDuration);
		}
		*/

		if (isBurst)
		{
			SpeedBurst(burstAmount);
		}
	}






//****************** Collide **************************
	void OnTriggerEnter2D(Collider2D col)
	{
		if(col.gameObject.tag.Equals("Ring"))
		{
			movingPattern = "Stay";
			interactBlock = col.gameObject;
			lastInteractBlockName = col.gameObject.tag;
		}

		if(col.gameObject.tag.Equals("Cross"))
		{
			movingPattern = "Wrap";
			interactBlock = col.gameObject;
			lastInteractBlockName = col.gameObject.tag;
			//WaitToWarp();
		}

		if(col.gameObject.tag.Equals("Square") || col.gameObject.tag.Equals("Brick"))
		{
			LinearBounce(col);
			if (col.gameObject.tag.Equals("Square"))
			{
				lastInteractBlockName = col.gameObject.tag;
				interactBlock = col.gameObject;
				Destroy(interactBlock, 1f);
			}
		}

		if(col.gameObject.tag.Equals("Tri"))
		{
			lastInteractBlockName = col.gameObject.tag;
			interactBlock = col.gameObject;
			movingPattern = "Stay";
			//LinearBounce(col);
			//WaitingToShoot();
		}
	}
//****************** Direction **************************
	/*Vector3 GetLineDirection (float speed)
	{
		LinearDirection = new Vector3(LinearDirection.x,
		                              LinearDirection.y,
		                              0);
		
		return LinearDirection.normalized * speed;
	}*/
	Vector3 GetDirectionToPoint (Vector3 point)
	{
		Vector3 heading = point - this.transform.position;
		float distance = heading.magnitude;
		Vector3 direction = heading/distance;
		return direction;
	}
	void FacePoint (Vector3 point)
	{
		Vector3 direction = GetDirectionToPoint(point);
		//LinearDirection = direction;
		FaceNewDirection(direction);
	}
	void FaceNewDirection(Vector3 newDirction)
	{
		float sign = Mathf.Sign (Vector3.Dot (newDirction, this.transform.up));
		float angle = Vector3.Angle (this.transform.right, newDirction);
		angle = angle * sign;
		
		this.transform.Rotate (new Vector3 (0, 0, angle));
	}
	void FaceAlongTrail ()
	{
		if(lastPosition != null)
		{
			FaceNewDirection(velocity);
			lastPosition = this.transform.position;
			
		}
	}

//****************** Square Behavior **************************

	void LinearBounce(Collider2D col)
	{
		float angle = angleToOriginHorizon (this.transform.position - col.gameObject.transform.position);
		DisableCollider();
		
		
		if((angle > 45 && angle <= 135)||(angle > 225 && angle <= 315))
		{
			//Collided top or Bottom side
			
			this.currentDirection = new Vector3(currentDirection.x,
			                                   -currentDirection.y,
			                                  0);
		}
		else
		{
			//Collided right or left side
			
			this.currentDirection = new Vector3(-currentDirection.x,
			                                   currentDirection.y,
			                                   currentDirection.z);
			
		}
		
		Invoke ("EnableCollider", 0.1f);
	}
//*************************************************************

	void StayInBlock (GameObject block)
	{
		this.transform.position = block.transform.position;
	}

//********************** Triangle Behavior ********************

	void SpeedBurst (float burstAmount)
	{
		currentSpeed = normalSpeed + burstAmount;
		if(movingPattern == "Stay" && interactBlock.tag == "Ring")
		{
			movingPattern = "Line";
		}
		GameObject DashHead = transform.Find("DashHead").gameObject;
		DashHead.SetActive(true);
	}

	void EndBurst ()
	{
		currentSpeed = normalSpeed;
		isBurst = false;
		GameObject DashHead = transform.Find("DashHead").gameObject;
		DashHead.SetActive(false);
	}




//********************** Math *********************************
	float angleToOriginHorizon(Vector3 theLine)
	{
		//the line should be from the center to the point so Line  = point - center
		
		float sign = Mathf.Sign (Vector3.Dot (theLine, Vector3.up));
		float angle = Vector3.Angle (theLine, Vector3.right);
		angle = angle * sign ;
		
		if(angle < 0 )
		{
			angle = 360 + angle;
		}
		
		
		return angle;		
	}
//*************************************************************
	public void SetBlockOnHold (string blockName)
	{
		blockOnHold = blockName;
	}

	public void CheckMouseOnButton (bool yesOrNo)
	{
		onAnotherButton = yesOrNo;
	}

	public string GetPlayerMovementMode ()
	{
		return movingPattern;
	}

	void DisableSprite (GameObject thingOff)
	{
		thingOff.GetComponent<Renderer>().enabled = false;
	}

	void EnableSprite (GameObject thingOff)
	{
		thingOff.GetComponent<Renderer>().enabled = true;
	}

	void EnableCollider()
	{
		this.GetComponent<CircleCollider2D> ().enabled = true;
	}
	void DisableCollider()
	{
		this.GetComponent<CircleCollider2D> ().enabled = false;
	}

}
