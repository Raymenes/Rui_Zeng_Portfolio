using UnityEngine;
using System.Collections;

public class HPBehaviorMode4Tset : MonoBehaviour {

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
		DetermineWinner();
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
			if (i==0 || i==1 || i==2)
			{
				HPBlocks[i].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			}
			else if (i==3 || i==4 || i==5)
				HPBlocks[i].transform.GetComponent<Renderer>().material.color = Color.white;
			else if (i==6 || i==7 || i==8)
				HPBlocks[i].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;

			//Debug.Log(blocksToPutIn[i].transform.position);
		}
	}
	void DetermineWinner ()
	{
		
		if (transform.childCount <= 3)
		{
			GameController.GetComponent<WinSituation>().SetWinner("lose");
			print("you lost!");
		}
	}

	public void SetHPDestroy (int HPIndex)
	{
		Destroy(HPBlocks[HPIndex]);
	}
}
