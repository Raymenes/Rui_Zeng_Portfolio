using UnityEngine;
using System.Collections;

public class MenuBarBehavior : MonoBehaviour {

	void Start () 
	{
	
	}
	
	void Update () 
	{
	
	}

	void OnMouseDown ()
	{
		Debug.Log("mouse clicked button!");
		if (gameObject.name == "MenuBar1")
		{
			Application.LoadLevel("GameMode1");
		}
		if (gameObject.name == "MenuBar2")
		{
			Application.LoadLevel("GameMode2");
		}
		if (gameObject.name == "MenuBar3")
		{
			Application.LoadLevel("GameMode4");
		}
		if (gameObject.name == "MenuBar4")
		{
			Application.LoadLevel("Instruction");
		}
		if (gameObject.name == "ResumeButton")
		{
			GameObject PauseMenu = GameObject.Find("PauseMenu");
			PauseMenu.SetActive(false);
			Time.timeScale = 1.0f;
		}
		if (gameObject.name == "ExitButton")
		{
			Application.LoadLevel("Menu");
			Time.timeScale = 1.0f;

		}

	}
}
