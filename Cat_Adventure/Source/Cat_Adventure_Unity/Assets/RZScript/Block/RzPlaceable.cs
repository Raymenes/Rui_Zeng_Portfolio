using UnityEngine;
using System.Collections;

public class RzPlaceable : MonoBehaviour {

	public bool isButton = false;
	public bool canDrag = true;
	bool isPlaced;
	bool onDrag = false;
	Vector3 offset = new Vector3 (0, 0, 0);
	//GameObject GM;
	GameObject BrickManager;
	GameObject BlockManager;
	GameObject player;
	float screenWidth;
	bool TriReadyToShoot = false;

	bool MouseOnButton = false;

	public Sprite whiteSprite;
	public Sprite colorSprite;
	SpriteRenderer sr;

	void Start () 
	{

		BrickManager = GameObject.Find("BrickManager");
		BlockManager = GameObject.Find("GameManager");
		player = GameObject.FindGameObjectWithTag("Player");

		sr = GetComponent<SpriteRenderer>();
	}
	
	void Update () 
	{
		/*/test
		if (gameObject.tag == "Ring")
		{
			if (Input.GetKeyDown(KeyCode.W))
			{
				sr.sprite = whiteSprite;
			}
			if (Input.GetKeyDown(KeyCode.C))
			{
				sr.sprite = colorSprite;
			}
		}
		*/
		HighlightSelectedButton();

		screenWidth = BrickManager.GetComponent<RzBrickGenerator>().GetBrickScreenWidth();

		if (!isButton)
		{
			if ((player.transform.position.x - transform.position.x) > screenWidth)
			{
				Destroy(gameObject);
			}

			if (TriReadyToShoot && gameObject.tag.Equals("Tri"))
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				FacePoint(mousePosition);
			}
		}
	}

	void OnMouseDown ()
	{
		if (isButton)
		{
			//Debug.Log ("mouse clicked " +this.gameObject.tag);

			BlockManager.GetComponent<RzBlockManager>().SetReadyBlock(this.gameObject.tag);

				//RzBlockManager.readyBlock = this.gameObject.tag;
		}

	}

	void OnMouseOver ()
	{
		if (isButton)
		{
			//print(gameObject.name);
			MouseOnButton = true;
			BlockManager.GetComponent<RzBlockManager>().CheckMouseOnButton(true);
			player.GetComponent<RzCatBehavior>().CheckMouseOnButton(true);
		}

	}

	void HighlightSelectedButton ()
	{
		string nowSelectedButton = BlockManager.GetComponent<RzBlockManager>().GetReadyBlock();
		if (isButton)
		{
			if (nowSelectedButton == gameObject.tag)
			{
				sr.sprite = whiteSprite;
			}
			else
				sr.sprite = colorSprite;
		}
	}
	void OnMouseExit()
	{
		if (isButton)
		{
			//print("mouse on nothing");
			MouseOnButton = false;
			BlockManager.GetComponent<RzBlockManager>().CheckMouseOnButton(false);
			player.GetComponent<RzCatBehavior>().CheckMouseOnButton(false);
		}
	}

	void OnTriggerEnter2D(Collider2D col)
	{
		if(col.gameObject.tag.Equals("Brick"))
		{
			//print("Block Over Brick");
			Destroy(gameObject);
		}
	}

	public void SetTriReadyToShoot (bool yesOrNo)
	{
		TriReadyToShoot = yesOrNo;
	}


	void FacePoint (Vector3 point)
	{
		Vector3 heading = point - this.transform.position;
		float distance = heading.magnitude;
		Vector3 direction = heading/distance;

		//LinearDirection = direction;
		float sign = Mathf.Sign (Vector3.Dot (direction, this.transform.up));
		float angle = Vector3.Angle (this.transform.right, direction);
		angle = angle * sign;
		
		this.transform.Rotate (new Vector3 (0, 0, angle));
	}


	/*
	void OnMouseDrag()
	{
		if (canDrag == true)
		{
			Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
			mousePosition.z = 0f;

			if (onDrag == false)
			{
				offset = -mousePosition + this.transform.position;
				onDrag = true;
			}
			if (this.isButton == true) 
			{
				gameObject.GetComponent<RzFollowPlayer>().enabled = true;

				GameObject aClone = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (this.gameObject.tag), 
				                                                        this.transform.position, Quaternion.identity);
				aClone.GetComponent<RzPlaceable> ().isButton = true;
				
			}
			if (this.isButton == false)
			{
				gameObject.GetComponent<RzFollowPlayer>().enabled = false;
			}

			this.transform.position = new Vector3 (mousePosition.x + offset.x, mousePosition.y + offset.y, 0f);
		}

	}

	void OnMouseUp ()
	{
		if (onDrag == true)
		{
			this.isButton = false;
			onDrag = false;
			offset = new Vector3 (0, 0, 0);
		}

	}
	*/
}
