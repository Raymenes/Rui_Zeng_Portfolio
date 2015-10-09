using UnityEngine;
using System.Collections;

public class HPBehaviorMode4 : MonoBehaviour {
	
	int childrenNumber;
	GameObject[] HPBlocks;
	int[] HPStatus;

	
	GameObject GameController;
	
	void Start () 
	{
		childrenNumber =  transform.childCount;
		StoreChildBlock();
		
		
		GameController = GameObject.Find("GlobalDefine");
	}
	
	void Update () 
	{
		/*if (HPIdentity == 1)
			Debug.Log("red hp now has " + CheckPlayerHPRemains());
		else
			Debug.Log("blue hp now has " + CheckPlayerHPRemains());*/
//		Debug.Log(GameController);
		DetermineWinner();
		
		//print(CheckSingleHPColor(0));
		
	}
	
	void StoreChildBlock ()
	{
		HPBlocks = new GameObject[childrenNumber];
		HPStatus = new int[childrenNumber];
		for (int i = 0; i < childrenNumber; i++)
		{
			//print("For loop: " + transform.GetChild(i).gameObject);
			HPStatus[i] = 0;
			HPBlocks[i] = transform.GetChild(i).gameObject;

			HPBlocks[i].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			//Debug.Log(blocksToPutIn[i].transform.position);
		}
	}
	
	public void SetHPColor (int HPIndex, string color)
	{
		if (HPStatus[HPIndex] == 0)
		{
			HPStatus[HPIndex]++;
			HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = Color.white;
			HPBlocks[HPIndex].tag = "colorNeutral";
		}

		if (HPStatus[HPIndex] == 1)
		{
			HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
			HPBlocks[HPIndex].tag = "color1";
		}
		/*
		if (color == "red")
		{
			HPBlocks[HPIndex].transform.renderer.material.color = GameManager.player1Color;
			HPBlocks[HPIndex].tag = "color1";

		}

		else if (color == "white")
		{
			HPBlocks[HPIndex].transform.renderer.material.color = Color.white;
			HPBlocks[HPIndex].tag = "colorNeutral";
		}*/
		
	}
	
	
	
	void DetermineWinner ()
	{

		if (CheckSingleHPColor(0)=="red" && CheckSingleHPColor(1)=="red" && CheckSingleHPColor(2)=="red")
		{
			GameController.GetComponent<WinSituation>().SetWinner("lose");
			print("you lost!");
		}
	}
	

	
	string CheckSingleHPColor (int HPIndex)
	{
		if (HPBlocks[HPIndex].tag.Equals("color1"))
			return "red";
		else if (HPBlocks[HPIndex].tag.Equals("color2"))
			return "blue";
		else if (HPBlocks[HPIndex].tag.Equals("colorNeutral"))
			return "white";
		else
			return "unkown";
	}
}
