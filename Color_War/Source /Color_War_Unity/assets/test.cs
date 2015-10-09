using UnityEngine;
using System.Collections;

public class test : MonoBehaviour {

	public Camera cam;
	// Use this for initialization
	void Start () 
	{
		transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
	}
	
	// Update is called once per frame
	void Update () 
	{
		transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
		if (Input.GetMouseButtonDown(0))
		{
			Vector3 mousePos = Input.mousePosition;
			mousePos.z = 10f;
			mousePos = Camera.main.ScreenToWorldPoint(mousePos);
			Debug.Log (mousePos);

			Instantiate(Resources.Load<GameObject>("BlueSplashParticles"), mousePos, Quaternion.identity);
		}
	}
}
