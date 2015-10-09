using UnityEngine;
using System.Collections;

public class RzBrickAutoDestroy : MonoBehaviour {

	GameObject player;
	float distanceToPlayer;

	void Start () 
	{
		player = GameObject.FindGameObjectWithTag("Player");
	}
	
	void Update () 
	{

		distanceToPlayer = player.transform.position.x - transform.position.x;

		//float range = transform.parent.GetComponent<RzBrickGenerator>().GetBrickAutoDestroyRange();
		float range = 300f;

		if (distanceToPlayer > range)
		{
			if (transform.parent.GetComponent<RzBrickGenerator>().GetBrickGeneratorMode().Equals("static"))
				Destroy(gameObject);
				//gameObject.transform.position = new Vector3(player.transform.position.x+range, transform.position.y, 0f);
		}

	}
}
