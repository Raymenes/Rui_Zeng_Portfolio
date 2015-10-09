using UnityEngine;
using System.Collections;

public class GameManager : MonoBehaviour {

	public Color color1;
	public Color color2;

	public static Color player1Color, player2Color;
	public int GameMode = 1;
	public TextMesh Score;
	public int scoreAdd = 5;
	public static int currentScore;

	void Awake ()
	{
		if (GameObject.Find("Messenger"))
		{
			player1Color = MessageStorage.player1Color;
			player2Color = MessageStorage.player2Color;
		}
		else
		{
			player1Color = color1;
			player2Color = color2;
		}

	}

	void Start () 
	{
		if (GameMode == 3)
		{
			currentScore = 0;
			Score.text = "SCORE: " + currentScore.ToString();
			//InvokeRepeating("AddScore", 1f, 1f);
		}
	}
	
	void Update () 
	{
		if (GameMode == 3)
		{
			Score.text = "SCORE: " + currentScore.ToString();
		}
	}

	public int GetGameMode ()
	{
		return GameMode;
	}

	public void AddScore ()
	{
		currentScore += scoreAdd;
		Score.text = currentScore.ToString();
	}


}
