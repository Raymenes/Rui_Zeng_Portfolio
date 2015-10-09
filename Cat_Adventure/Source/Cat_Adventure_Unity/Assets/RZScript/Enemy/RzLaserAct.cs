using UnityEngine;
using System.Collections;

public class RzLaserAct : MonoBehaviour {

	GameObject laserParticles;
	float laserLength;
	public float squareLength = 2f;
	float scaleYvalue;
	bool readyToCollide = true;

	void Start () 
	{
		laserParticles = transform.Find("BloodLaser").gameObject;
		//print (laserParticles.transform.position);
		Vector3 lengthOffset = laserParticles.transform.position - this.transform.position;
		laserLength = 2 * lengthOffset.magnitude;
		scaleYvalue = transform.localScale.y;
	}
	
	void Update () 
	{
		if(Input.GetKeyDown(KeyCode.Y))
			transform.localScale = new Vector3(transform.localScale.x, transform.localScale.y/2, 1f);
	}

	void OnTriggerEnter2D(Collider2D col)
	{
		if (col.gameObject.tag.Equals("Square"))
		{
			if(readyToCollide)
			{
				Vector3 lengthOffset = col.gameObject.transform.position - this.transform.position;
				float newLength =  squareLength+lengthOffset.magnitude;
				transform.localScale = new Vector3(transform.localScale.x, scaleYvalue * (newLength/laserLength), 1f);

				//readyToCollide = false
			}

			//problem

			//

			//laserLength = newLength;
		}
	}
	void OnTriggerExit2D(Collider2D col)
	{
		if (col.gameObject.tag.Equals("Square"))
		{
			//readyToCollide = true;
			float newLength = Mathf.Abs(transform.position.y - col.gameObject.transform.position.y) - squareLength ;
			transform.localScale = new Vector3(transform.localScale.x, scaleYvalue, 1f);

		}
	}
}
