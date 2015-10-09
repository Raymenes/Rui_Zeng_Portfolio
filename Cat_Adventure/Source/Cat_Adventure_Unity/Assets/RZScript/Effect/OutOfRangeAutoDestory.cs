using UnityEngine;
using System.Collections;

public class OutOfRangeAutoDestory : MonoBehaviour {

	GameObject player;
	float distanceToPlayer;
	public float range = 180f;
	
	void Start () 
	{
		player = GameObject.FindGameObjectWithTag("Player");
	}
	
	void Update () 
	{
		distanceToPlayer = player.transform.position.x - transform.position.x;
		if (distanceToPlayer > range)
		{
			Destroy(gameObject);
		}
	}
}
