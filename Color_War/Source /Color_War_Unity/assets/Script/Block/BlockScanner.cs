using UnityEngine;
using System.Collections;

public class BlockScanner : MonoBehaviour {

	public GameObject UpperLeftBlock;
	public int rowNumber;
	public int columnNumber;

	Material player1Block;
	Material player2Block;

	int childrenNumber;

	GameObject[][] blocks;
	GameObject[] blocksToPutIn;

	GameObject RedHP, BlueHP;

	int lastRandomNumber;

	void Start () 
	{
		RedHP = GameObject.Find("RedHP");
		BlueHP = GameObject.Find("BlueHP");

		//player1Block = Resources.Load<Material>("Red");
		//player2Block = Resources.Load<Material>("Blue");

		//Debug.Log("player 1 color is " + player1Block);
		//Debug.Log("player 2 color is " + player2Block);

		childrenNumber =  transform.childCount;
		StoreChildBlock();
		InvokeRepeating("CreatRandomNumberForBlockSwitching", 0f, 3f);

		StoreDoubleBlockArray();

		//Debug.Log(CheckSingleBlockColor(0,1));
	}
	
	void Update () 
	{

		SendMessageToHP(0, CheckRowStatus(0));
		SendMessageToHP(1, CheckRowStatus(1));
		SendMessageToHP(2, CheckRowStatus(2));
		SendMessageToHP(3, CheckRowStatus(3));
		SendMessageToHP(4, CheckRowStatus(4));

		/*if (Input.GetKeyDown(KeyCode.Alpha1))
		{
			ChangeRowColorTo(1, "white");
		}*/
	}

	void StoreChildBlock ()
	{
		blocksToPutIn = new GameObject[childrenNumber];
		for (int i = 0; i < childrenNumber; i++)
		{
			//print("For loop: " + transform.GetChild(i).gameObject);
			blocksToPutIn[i] = transform.GetChild(i).gameObject;
			//Debug.Log(blocksToPutIn[i].transform.position);
		}
	}

	void StoreDoubleBlockArray ()
	{
		blocks = new GameObject[rowNumber][];
		for(int i =0; i < rowNumber; i++)
		{
			blocks[i] = new GameObject[columnNumber];
		}

		for (int k = 0; k < childrenNumber;)
		{
			for (int i =0; i < rowNumber; i++)
			{
				for (int j =0; j < columnNumber; j++)
				{
					blocks[i][j] = blocksToPutIn[k];
					k++;
					//print ("this is " + blocks[i][j]);
					//Debug.Log(blocks[i][j].transform.position);
				}
			}
		}

	}

	//return true if same color
	string CheckRowStatus (int rowIndex)
	{
		string rowStatus = "differentColor";
		string firstBlockColor = CheckSingleBlockColor(rowIndex,0);

		for (int i = 1; i < columnNumber; i++)
		{
			if (firstBlockColor != CheckSingleBlockColor(rowIndex,i))
			{
				return rowStatus;
			}
		}
		if (firstBlockColor == "red")
		{
			rowStatus = "allRed";
			//Debug.Log("row color is same color " + rowIndex + rowStatus);
			return rowStatus;
		}
		else if(firstBlockColor == "blue")
		{
			rowStatus = "allBlue";
			//Debug.Log("row color is same color " + rowIndex + rowStatus);
			return rowStatus;
		}
		else if(firstBlockColor == "white")
		{
			rowStatus = "allWhite";
			//Debug.Log("row color is same color " + rowIndex + rowStatus);
			return rowStatus;
		}
		else
		{
			rowStatus = "error";
			return rowStatus;
		}

	}

	string CheckSingleBlockColor (int rowIndex, int columnIndex)
	{
		string blockColor;

		//Debug.Log("color of the block we are checking is " + blocks[rowIndex][columnIndex].transform.renderer.material);

		if (blocks[rowIndex][columnIndex].tag.Equals("color1"))
		{
			blockColor = "red";
		}
		else if (blocks[rowIndex][columnIndex].tag.Equals("color2"))
		{
			blockColor = "blue";
		}
		else
		{
			blockColor = "other";
		}

		return blockColor;
	}

	void ChangeRowColorTo (int rowIndex, string rowColor)
	{
		if (rowColor.Equals("red"))
		{
			for (int i = 0; i < columnNumber; i++)
			{
				blocks[rowIndex][i].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
				blocks[rowIndex][i].tag = "color1";
			}
		}
		else if (rowColor.Equals("blue"))
		{
			for (int i = 0; i < columnNumber; i++)
			{
				blocks[rowIndex][i].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
				blocks[rowIndex][i].tag = "color2";
			}
		}
		else if (rowColor.Equals("white"))
		{
			for (int i = 0; i < columnNumber; i++)
			{
				blocks[rowIndex][i].transform.GetComponent<Renderer>().material.color = Color.white;
				blocks[rowIndex][i].tag = "colorNeutral";
			}
		}

	}

	void SendMessageToHP(int rowIndex, string rowStatus)
	{

		if (rowStatus == "allRed")
		{
			ChangeRowColorTo(rowIndex, "white");
			BlueHP.GetComponent<HPBehavior>().SetHPColor(rowIndex, "red");

		}
		else if (rowStatus == "allBlue")
		{
			ChangeRowColorTo(rowIndex, "white");
			RedHP.GetComponent<HPBehavior>().SetHPColor(rowIndex, "blue");

		}
	}

	int GetRandNum013 ()
	{
		return ((int)Random.Range(0, 100f))%3;
	}

	void CreatRandomNumberForBlockSwitching ()
	{
		int nowRandomNum;

		do
		{
			nowRandomNum = GetRandNum013();
		}while (nowRandomNum == lastRandomNumber);

		lastRandomNumber = nowRandomNum;
	}

	public int ReadLastRandomNumber ()
	{
		return lastRandomNumber;

	}
}
