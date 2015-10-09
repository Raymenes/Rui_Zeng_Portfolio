using UnityEngine;
using System.Collections;

public class HPBehavior : MonoBehaviour {

	int childrenNumber;
	GameObject[] HPBlocks;
	int[] HPStatus;
	public int HPIdentity;
	public int lostNumber = 3;

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
			if (HPIdentity == 1)
				HPBlocks[i].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
			else if (HPIdentity == 2)
				HPBlocks[i].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			//Debug.Log(blocksToPutIn[i].transform.position);
		}
	}

	public void SetHPColor (int HPIndex, string color)
	{
		if (color == "red")
		{
			switch (HPStatus[HPIndex])
			{
			case 0:
				HPStatus[HPIndex] = 1;
				HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = Color.white;
				HPBlocks[HPIndex].tag = "colorNeutral";
				break;
			case 1:
				HPStatus[HPIndex] = 2;
				HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
				HPBlocks[HPIndex].tag = "color1";
				break;
			case 2:
				break;
			default:
				Debug.Log("ERROR!");
				break;
			}
		}
		else if (color == "blue")
		{
			switch (HPStatus[HPIndex])
			{
			case 0:
				HPStatus[HPIndex] = 1;
				HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = Color.white;
				HPBlocks[HPIndex].tag = "colorNeutral";
				break;
			case 1:
				HPStatus[HPIndex] = 2;
				HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
				HPBlocks[HPIndex].tag = "color2";
				break;
			case 2:
				break;
			default:
				Debug.Log("ERROR!");
				break;
			}
		}
		else if (color == "white")
		{
			HPStatus[HPIndex] = 1;
			HPBlocks[HPIndex].transform.GetComponent<Renderer>().material.color = Color.white;
			HPBlocks[HPIndex].tag = "colorNeutral";
		}
			
	}



	void DetermineWinner ()
	{
		if (HPIdentity == 1)
		{
			if (CheckPlayerHPOccupied() >= 3)
			{
				//Debug.Log("Player 2 Won");
				GameController.GetComponent<WinSituation>().SetWinner("player2");
			}
		}

		if (HPIdentity == 2)
		{
			if (CheckPlayerHPOccupied() >= 3)
			{
				//Debug.Log("Player 1 Won");
				GameController.GetComponent<WinSituation>().SetWinner("player1");
			}
		}
	}

	int CheckPlayerHPOccupied ()
	{
		int blue = 0;
		int red = 0;
		int white = 0;

		if (HPBlocks[0].tag.Equals("color1"))
			red += 1;
		else if (HPBlocks[0].tag.Equals("color2"))
			blue += 1;

		if (HPBlocks[1].tag.Equals("color1"))
			red += 1;
		else if (HPBlocks[1].tag.Equals("color2"))
			blue += 1;

		if (HPBlocks[2].tag.Equals("color1"))
			red += 1;
		else if (HPBlocks[2].tag.Equals("color2"))
			blue += 1;

		if (HPBlocks[3].tag.Equals("color1"))
			red += 1;
		else if (HPBlocks[3].tag.Equals("color2"))
			blue += 1;

		if (HPBlocks[4].tag.Equals("color1"))
			red += 1;
		else if (HPBlocks[4].tag.Equals("color2"))
			blue += 1;


		if(HPIdentity == 1)
			return blue;
		else
			return red;
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
