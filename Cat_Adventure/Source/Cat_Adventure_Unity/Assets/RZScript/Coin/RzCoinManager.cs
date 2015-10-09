using UnityEngine;
using System.Collections;

public class RzCoinManager : MonoBehaviour {

	public TextMesh coinRecordText;
	int coinCollect;
	GameObject[] Coins;
	int coinNumber;
	int totalCoinNumber;
	public int goalCoinNumber = 3;

	void Start () 
	{
		totalCoinNumber = transform.childCount;
		StoreCoins();
		coinCollect = 0;
	}
	
	void Update () 
	{
		coinRecordText.text = "Coins: " + coinCollect + "/" + goalCoinNumber + "/" + totalCoinNumber;
		StoreCoins();
		if(coinNumber > 0)
		{
			Coins[0].GetComponent<RzCoinAct>().SetCurrentTarget();
		}
		if (coinNumber > 1)
		{
		Coins[1].GetComponent<RzCoinAct>().SetNextTarget();
		}

	}

	void StoreCoins()
	{
		coinNumber =  transform.childCount;
		Coins = new GameObject[coinNumber];
		for (int i = 0; i < coinNumber; i++)
		{
			Coins[i] = transform.GetChild(i).gameObject;
		}
	}

	public void CollectOneCoin ()
	{
		coinCollect +=1;
	}

	public void LostOneCoin()
	{
		totalCoinNumber -= 1;
	}
}
