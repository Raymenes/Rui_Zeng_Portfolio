using UnityEngine;
using System.Collections;

public class RzFlashMode3 : MonoBehaviour {

	bool PowerOn = false;
	
	void Start () 
	{
		PowerOn = false;
		InvokeRepeating("Flash", 1f, 2f);
		gameObject.tag = "HazardBrick";	
	}
	
	void Update () 
	{
		
		if (PowerOn == false)
		{
			transform.GetComponent<Renderer>().material.color = Color.white;
			gameObject.tag = "Nothing";
		}
		if (PowerOn == true)
		{
			transform.GetComponent<Renderer>().material.color = Color.red;
			gameObject.tag = "HazardBrick";	
			
		}
		
	}
	
	void Flash ()
	{
		if (PowerOn == false)
		{
			PowerOn = true;
		}
		else
		{
			PowerOn = false;
		}
		
	}

	/*
	void OnTriggerStay2D(Collider2D col) 
	{
		if (col.gameObject.tag == "Player")
		{
			//Debug.Log("Kill Cat!");
			
			if (PowerOn == true)
			{
				Destroy(col.gameObject);
				
			}
		}
	}*/
}
