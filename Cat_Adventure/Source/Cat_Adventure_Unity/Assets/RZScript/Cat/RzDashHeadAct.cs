using UnityEngine;
using System.Collections;

public class RzDashHeadAct : MonoBehaviour {

	public float splashLifeTime = 20f;

	void Start () 
	{
	
	}
	
	void Update () 
	{
	
	}

	void OnTriggerEnter2D (Collider2D col)
	{
		if (col.gameObject.tag.Equals("Enemy"))
		{
			Destroy(col.gameObject);
			GameObject Splash = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Splash"), 
			                                                    new Vector3(transform.position.x, transform.position.y, -5f), Quaternion.identity);
			Destroy(Splash, splashLifeTime);
		}

		if (col.gameObject.tag.Equals("DestroyBrick"))
		{


			Destroy(col.gameObject);
			GameObject Splash = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("Splash"), 
			                                                        new Vector3(transform.position.x, transform.position.y, -5f), Quaternion.identity);
			Destroy(Splash, splashLifeTime);
		}
	}
}
