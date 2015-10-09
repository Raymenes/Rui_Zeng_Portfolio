using UnityEngine;
using System.Collections;

public class CatBehav : MonoBehaviour {

	GameObject myHuman;

	LineRenderer lineRenderer;
	public GameObject killEffect;
	public GameObject deathEffect;

	Vector3 worldMousePos;
	Vector3 screenMousePos;
	Vector3 dirToMouse;
	Vector3 shootDir;
	Vector3 currentVelocity;
	Vector3 initialVelocity;
	Vector3 gravity;
	public Vector3 respawnPosition;

	float shootSpeed;
	public float minSpeed = 12f;
	public float maxSpeed = 20f;
	public float chargeSpeed = 3f;
	float startTime;
	float gravityScale = 3.0f;
	
	public enum movementStatus {shooting, flying, sitting, dead};
	movementStatus catStatus = movementStatus.sitting;
	
	bool canShoot = true;
	bool isAlive = true;

	//Animation specific varaibles
	private bool facingRight = true;
	public Animator myAnimator;


	void Start () 
	{
		gravity = new Vector3(0f, -gravityScale);
		lineRenderer = GetComponent<LineRenderer>();
		catStatus = movementStatus.flying;
	}
	
	void Update ()  
	{
		if(isAlive)
		{
			screenMousePos = Input.mousePosition;
			screenMousePos.z = -Camera.main.transform.position.z;
			worldMousePos = Camera.main.ScreenToWorldPoint(screenMousePos);
			dirToMouse = worldMousePos - transform.position;
			dirToMouse /= dirToMouse.magnitude;
			
			canShoot = checkIfCanShoot(transform.position, dirToMouse, 0.2f);
			Projectile();
			WarpCheck();
		}
	}

	void FixedUpdate ()
	{
		if(currentVelocity.x > 0 && facingRight)
			Flip ();
		else if(currentVelocity.x < 0 && !facingRight)
			Flip();
	}

	void OnCollisionExit2D (Collision2D col)
	{
		if (col.gameObject.tag == "Human"){
			ExitHuman();
		}
	}

	void OnCollisionEnter2D (Collision2D col)
	{
		if(isAlive)
		{
			if (col.gameObject.tag == "Environment" && catStatus == movementStatus.flying){
				catStatus = movementStatus.sitting;
			}
			
			else if (col.gameObject.tag == "Hazard"){
				print("collide with hazard!");
				Die();
			}

			else if (col.gameObject.tag == "killable"){
//				catStatus = movementStatus.sitting;
				print("collide with killable!");
				Kill(col.gameObject);
			}

			else if (col.gameObject.tag == "Human"){
				ExitHuman();
				myHuman = col.gameObject;
				catStatus = movementStatus.sitting;
				if(myHuman.GetComponent<AI_human>()){
					myHuman.GetComponent<AI_human>().SetToZeroVelocity();
					myHuman.GetComponent<AI_human>().IsBeingCharged = true;
				}
				this.transform.parent = col.gameObject.transform;
			}

			else if (col.gameObject.tag == "Ghost")
			{
				Vector2 colPoint = col.contacts[0].point;
//				Debug.Log( col.contacts[0].point);
				if(transform.position.y >  col.gameObject.transform.position.y  && catStatus==movementStatus.flying){
					Kill(col.gameObject);
					GameManager.KillGhost();
				}else{
					Die();
				}
			}
		}
	}

	//now determine the shoot direction at the momenmt the mouse is clicked down
	//need to update so that shoot direction keeps changing
	//when the mouse button is down and on hold
	void Projectile()
	{
		if(catStatus == movementStatus.sitting)
		{
			//horizontalVelocity = Vector3.zero;
			//verticalVelocity = Vector3.zero;
			currentVelocity = Vector3.zero;
			
			if(Input.GetMouseButtonDown(0) && canShoot == true)
			{

				catStatus = movementStatus.shooting;
				shootSpeed = minSpeed;
			}
		}

		if(catStatus == movementStatus.shooting)
		{
			if(canShoot)
			{
				if(Input.GetMouseButton(0))
				{
					if ((shootSpeed + chargeSpeed * Time.deltaTime) < 20f)
						shootSpeed += chargeSpeed * Time.deltaTime;
					else
						shootSpeed = maxSpeed;
				}
				else
				{
					if( (shootSpeed - chargeSpeed * Time.deltaTime) > 12f)
						shootSpeed -= chargeSpeed * Time.deltaTime;
					else
						shootSpeed = minSpeed;
				}
				
				shootDir = dirToMouse;
				initialVelocity = shootDir * shootSpeed;
				lineRenderer.enabled = true;
				UpdateTrajectory(transform.position, initialVelocity, gravity*10f);
			}
		}
		
		if (Input.GetMouseButtonUp(0) && catStatus == movementStatus.shooting)
		{


			catStatus = movementStatus.flying;
			lineRenderer.enabled = false;
			startTime = Time.timeSinceLevelLoad;
			currentVelocity = shootDir * shootSpeed;

			myAnimator.SetTrigger("isJumpStart");
		}
		if (catStatus == movementStatus.flying)
		{
			//verticalVelocity -= new Vector3(0f, gravityScale);
			if(currentVelocity.y + gravity.y*10f*Time.deltaTime >= -12f){
				currentVelocity += gravity*10f*Time.deltaTime;
			}
		}
		//combinedVelocity = verticalVelocity + horizontalVelocity;
		transform.Translate(currentVelocity*Time.deltaTime, Space.World);

	}

	void Flip()
	{
		facingRight = !facingRight;
		Vector3 aScale = transform.localScale;
		aScale.x *= -1;
		transform.localScale = aScale;
	}
	//now only cast a single raycast to the mouse direction
	//need to update that it casts to up, right, left, down
	bool checkIfCanShoot (Vector2 position, Vector2 dir, float distance)
	{

		RaycastHit2D hitHorizontal = Physics2D.Raycast(position, new Vector2(dir.x, 0f), distance);
		if (hitHorizontal.collider != null)
		{
			return false;
		}
		RaycastHit2D hitVertical = Physics2D.Raycast(position, new Vector2(0f, dir.y), distance);
		if (hitVertical.collider != null)
		{
			return false;
		}

		else
			return true;
	}

	void UpdateTrajectory(Vector3 initialPosition, Vector3 initialVelocity, Vector3 gravity)
	{
		int numSteps = 20; // for example
		float timeDelta = 1.0f / initialVelocity.magnitude; // for example
		
		lineRenderer = GetComponent<LineRenderer>();
		lineRenderer.SetVertexCount(numSteps);
		
		Vector3 position = initialPosition;
		Vector3 velocity = initialVelocity;
		
		for (int i = 0; i < numSteps; ++i)
		{
			lineRenderer.SetPosition(i, position);
			
			position += velocity * timeDelta + 0.5f * gravity * timeDelta * timeDelta;
			velocity += gravity * timeDelta;
		}
	}

	void Die()
	{
		ExitHuman();
		isAlive = false;
		lineRenderer.enabled = false;
		catStatus = movementStatus.dead;
		gameObject.GetComponent<SpriteRenderer>().enabled = false;
		currentVelocity = Vector3.zero;
		deathEffect.GetComponent<ParticleSystem>().Play();
		Invoke("Respawn", 1.5f);
	}

	void Respawn()
	{
		respawnPosition = GameManager.Get().GetWorldBottomLeftPoint();
		respawnPosition.x += gameObject.transform.localScale.x;
		respawnPosition = new Vector3(Random.Range(respawnPosition.x, -respawnPosition.x), 0f, transform.position.z);
		transform.position = respawnPosition;
		catStatus = movementStatus.flying;
		isAlive = true;
		gameObject.GetComponent<SpriteRenderer>().enabled = true;
	}

	void Kill(GameObject other)
	{
		currentVelocity.y = 0f;
		killEffect.GetComponent<ParticleSystem>().Play();
		other.SetActive(false);
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

	public movementStatus GetCatStatus(){ return catStatus; } 

	void ExitHuman(){
		if(myHuman){
			myHuman.GetComponent<AI_human>().IsBeingCharged = false;
			this.transform.parent = null;
			myHuman = null;
		}
	}
}
