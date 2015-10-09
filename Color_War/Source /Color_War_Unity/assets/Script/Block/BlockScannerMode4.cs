using UnityEngine;
using System.Collections;

public class BlockScannerMode4 : MonoBehaviour {
	
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
	public float invadeInterval = 5f;
	public float invadeAccelerationRate = 0.2f;
	public float minInvadeInterval = 0.7f;
	float invadeTimer;

	int lostCount = 0;

	
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
		InvokeRepeating("CreatRandomNumberForBlockSwitching", 0f, 5f);

		StoreDoubleBlockArray();
		
		//Debug.Log(CheckSingleBlockColor(0,1));
		invadeTimer = Time.timeSinceLevelLoad;
	}
	
	void Update () 
	{
		
		if (Time.timeSinceLevelLoad - invadeTimer >= invadeInterval)
		{
			FixedRateRandomInvade();
			invadeTimer = Time.timeSinceLevelLoad;
		}

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
		else if (blocks[rowIndex][columnIndex].tag.Equals("colorNeutral"))
		{
			blockColor = "white";
		}
		else 
			blockColor = "";
		
		return blockColor;
	}

	void ChangeSingleBlockColor (int rowIndex, int columnIndex, string blockColor)
	{
		if (blockColor == "red")
		{
			blocks[rowIndex][columnIndex].transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
			blocks[rowIndex][columnIndex].tag = "color1";
		}
		else if (blockColor == "blue")
		{
			blocks[rowIndex][columnIndex].transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			blocks[rowIndex][columnIndex].tag = "color2";
		}
		else if (blockColor == "white")
		{
			blocks[rowIndex][columnIndex].transform.GetComponent<Renderer>().material.color = Color.white;
			blocks[rowIndex][columnIndex].tag = "colorNeutral";
		}
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
	

	
	int GetRandNum013 ()
	{
		return ((int)Random.Range(0, 99f))%3;
	}

	int GetRandNum01234 ()
	{
		return ((int)Random.Range(1f, 100f))%5;
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

	void FixedRateRandomInvade ()
	{
		int dice;
		dice = GetRandNum01234();
		switch(dice)
		{
		case 0:
			InvadeRowOne();
			break;
		case 1:
			InvadeRowTwo();
			break;
		case 2:
			InvadeRowThree();
			break;
		case 3:
			InvadeRowFour();
			break;
		case 4:
			InvadeRowFive();
			break;
		default:
			InvadeRowThree();
			break;
		}
		if (invadeInterval > minInvadeInterval)
			invadeInterval -= invadeAccelerationRate;
		else
			invadeInterval = minInvadeInterval;
	}

	void InvadeRowOne ()
	{
		InvadeRow(0, true, false, false);
	}
	void InvadeRowTwo ()
	{
		InvadeRow(1, true, false, false);
	}
	void InvadeRowThree ()
	{
		InvadeRow(2, true, false, false);
	}
	void InvadeRowFour ()
	{
		InvadeRow(3, true, false, false);
	}
	void InvadeRowFive ()
	{
		InvadeRow(4, true, false, false);
	}

	void InvadeRow (int rowIndex, bool byColor, bool byShape, bool bySpin)
	{
		string[] rowColor = new string[columnNumber];
		for (int i =0; i <columnNumber; i++)
		{
			rowColor[i] = CheckSingleBlockColor(rowIndex, i);
		}
		if (byColor)
		{
			if (CheckRowStatus(rowIndex) == "allRed")
			{
				if (lostCount < 6)
				{
					BlueHP.GetComponent<HPBehaviorMode4Tset>().SetHPDestroy(lostCount);
					lostCount++;
				}
			}
			else
			{
				for (int i = 4; i >0; i--)
				{
					ChangeSingleBlockColor(rowIndex, i, CheckSingleBlockColor(rowIndex, i-1));
				}
				ChangeSingleBlockColor(rowIndex, 0, "red");
			}

		}
	}
}
