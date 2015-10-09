using UnityEngine;
using System.Collections;

public class RzFollowPlayer : MonoBehaviour {

	Vector3 offset;
	GameObject player;
	float yValue;
	public bool followMode;
	public bool staticMode;

	void Start () 
	{

		player = GameObject.FindGameObjectWithTag("Player");
		offset = transform.position - player.transform.position;
		yValue = transform.position.y;


	}
	
	void Update () 
	{
		gameObject.transform.position = new Vector3(player.transform.position.x + offset.x, yValue, 0f);
	}
}
