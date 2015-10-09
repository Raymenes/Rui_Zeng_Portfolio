using UnityEngine;
using System.Collections;

public class MessageStorage : MonoBehaviour {

	string player1Name, player2Name;
	public Color color1;
	public Color color2;
	public static Color player1Color, player2Color;



	void Start () 
	{
		player1Name = "Player 1";
		player2Name = "Player 2";
	}
	void Awake() 
	{
		player1Color = color1;
		player2Color = color2;
		DontDestroyOnLoad(transform.gameObject);
	}

	void Update () 
	{
		 if (Input.GetKeyDown(KeyCode.Escape))
		{
			//Application.LoadLevel("Menu");
		}
	}
	public void SetPlayer1Name (string passIn)
	{
		player1Name = passIn;
	}
	public void SetPlayer2Name (string passIn)
	{
		player2Name = passIn;
	}
	public string GetPlayer1Name ()
	{
		return player1Name;
	}
	public string GetPlayer2Name ()
	{
		return player2Name;
	}
}
