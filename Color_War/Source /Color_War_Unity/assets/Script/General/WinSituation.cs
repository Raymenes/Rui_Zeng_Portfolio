using UnityEngine;
using System.Collections;

public class WinSituation : MonoBehaviour {

	bool GameEneded = false;
	GameObject Player1, Player2;
	string message;
	TextMesh tm;

	string player1Name, player2Name;
	GameObject PauseMenu; 

	void Start () 
	{
		ReadPlayersName();
		GameEneded = false;
		Player1 = GameObject.Find("Player1");
		Player2 = GameObject.Find("Player2");
		tm = gameObject.GetComponent<TextMesh>();
		PauseMenu = GameObject.Find("PauseMenu");
		PauseMenu.SetActive(false);

	}

	void Awake ()
	{

	}
	
	void Update () 
	{
		if (Input.GetKeyDown(KeyCode.Escape))
		{
			//Application.LoadLevel("Menu");
			PauseMenu.SetActive(true);
			Time.timeScale = 0.0f;
		}


		if (GameEneded)
		{
			//Time.timeScale = 0.0f;
			//Player1.SetActive(false);
			//Player2.SetActive(false);

			/////
			/// need revision
			if(Player1 != null)
			{
				if (Player1.GetComponent<PlayerMovement>() != null)
				{
					Player1.GetComponent<PlayerMovement>().enabled = false;
					Player2.GetComponent<PlayerMovement>().enabled = false;
				}
				if (Player1.GetComponent<PlayerMovementMode2>() != null)
				{
					Player1.GetComponent<PlayerMovementMode2>().enabled = false;
					Player2.GetComponent<PlayerMovementMode2>().enabled = false;
					
				}
			}
			////

			tm.text = message;
			Invoke("RestartGame", 5f);
			//get pause menu
		}
	}

	public void SetWinner (string winner)
	{

		if (winner.Equals("player1"))
		{
			GameEneded = true;
			message = player1Name + " Wins!";
		}
		else if (winner.Equals("player2"))
		{
			GameEneded = true;
			message = player2Name + " Wins!";
		}
		else if (winner.Equals("lose"))
		{
			GameEneded = true;
			message = "You Lost!";
		}
	}

	void ReadPlayersName()
	{
		GameObject Messenger = GameObject.Find("Messenger");
		if (Messenger != null)
		{
			player1Name = Messenger.GetComponent<MessageStorage>().GetPlayer1Name();
			player2Name = Messenger.GetComponent<MessageStorage>().GetPlayer2Name();
		}
		else
		{
			player1Name = "Player 1";
			player2Name = "Player 2";
		}

	}

	void RestartGame ()
	{
		Application.LoadLevel("Menu");
	}
}
