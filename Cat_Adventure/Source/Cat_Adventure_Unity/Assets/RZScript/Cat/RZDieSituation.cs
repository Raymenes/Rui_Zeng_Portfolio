using UnityEngine;
using System.Collections;

public class RZDieSituation : MonoBehaviour {

	bool catDead = false;
	public TextMesh dieText;

	void Start () 
	{
		dieText.text = "";
	}
	
	void Update () 
	{
		//test

		if (catDead)
		{
			dieText.text = "Cat Died!";
			if(Input.GetMouseButtonDown(0))
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;
				CatRevive(mousePosition);
			}
		}
		//
	}
	void OnTriggerStay2D(Collider2D col)
	{
		if (col.gameObject.tag.Equals("HazardBrick"))
		{
			CatDie();
		}
	}

	void OnTriggerEnter2D(Collider2D col)
	{
		if (col.gameObject.tag.Equals("Deadline"))
		{
			//print ("killed by deadline!");
			CatDie();
		}

		if (col.gameObject.tag.Equals("Enemy"))
		{
			//print ("killed by deadline!");
			CatDie();
		}

		if (col.gameObject.tag.Equals("HazardBrick"))
		{
			CatDie();
		}

		if (col.gameObject.tag.Equals("DestroyBrick"))
		{
			CatPause();
		}


	}

	void OnTriggerExit2D(Collider2D col)
	{
		CatResume();
	}

	void CatDie()
	{
		gameObject.GetComponent<Renderer>().enabled = false;
		this.GetComponent<CircleCollider2D> ().enabled = false;
		this.GetComponent<RzCatBehavior>().enabled = false;
		catDead = true;
	}

	void CatRevive (Vector3 revivePosition)
	{
		dieText.text = "";
		gameObject.transform.position = revivePosition;
		gameObject.GetComponent<Renderer>().enabled = true;
		this.GetComponent<CircleCollider2D> ().enabled = true;
		this.GetComponent<RzCatBehavior>().enabled = true;
		catDead = false;
	}

	void CatPause ()
	{
		this.GetComponent<RzCatBehavior>().enabled = false;
	}

	void CatResume ()
	{
		this.GetComponent<RzCatBehavior>().enabled = true;
	}
}
