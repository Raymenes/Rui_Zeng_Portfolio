using UnityEngine;
using System.Collections;

public class MessageStorage : MonoBehaviour {

	string player1Name, player2Name;
	public Color color1;
	public Color color2;
	public static Color player1Color, player2Color;



	void Start () 
	{
		player1Name = "Player Red";
		player2Name = "Player Blue";
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
	public void SetPlayer1Name (string name)
	{
		player1Name = name;
	}
	public void SetPlayer2Name (string name)
	{
		player2Name = name;
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
