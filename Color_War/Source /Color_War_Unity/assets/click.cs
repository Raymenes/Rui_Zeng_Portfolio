﻿using UnityEngine;
using System.Collections;

public class click : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () 
	{
		if(Input.GetKeyDown(KeyCode.Return))
		{
			//Destroy(gameObject);
		}
	}

	void OnMouseDown ()
	{
		Destroy(gameObject);
	}
}