using UnityEngine;
using System.Collections;

public class MenuBehavior : MonoBehaviour {

	public string player1NameToEdit = "Enter Player1's Name";
	public string player2NameToEdit = "Enter Player2's Name";
	string player1Name, player2Name;
	public float rectX1 = 100f, rectY1 = 100f;
	public float rectX2 = 100f, rectY2 = 100f;
	string tempInput;
	bool user1HasHitReturn = false, user2HasHitReturn = false;
	bool enterDone = false;

	bool printed = false;

	void Start () 	
	{
	
	}
	
	void Update () 
	{
		if (Input.GetKeyDown(KeyCode.Space))
		{
			ConfirmTextEnter();
		}

		if (enterDone)
		{
			Invoke("StorePlayersName", 0f);
		}
	}

	public void ConfirmTextEnter ()
	{
		enterDone = true;
	}
		



	void StorePlayersName ()
	{
		if (printed == false)
		{
			player1Name = player1NameToEdit;
			player2Name = player2NameToEdit;
			Debug.Log("Player1's Name is " + player1Name);
			Debug.Log("Player2's Name is " + player2Name);
			printed = true;
			GameObject Messenger = GameObject.Find("Messenger");
			Messenger.GetComponent<MessageStorage>().SetPlayer1Name(player1Name);
			Messenger.GetComponent<MessageStorage>().SetPlayer2Name(player2Name);

		}


	}

	void OnGUI() 
	{
		player1NameToEdit = GUI.TextField(new Rect(rectX1-100, rectY1, 200, 20), player1NameToEdit, 25);
		player2NameToEdit = GUI.TextField(new Rect(rectX2-100, rectY2, 200, 20), player2NameToEdit, 25);

		//Debug.Log("now in the field" + player1NameToEdit);
			
	}
		
		
		
		//player2NameToEdit = GUI.TextField(new Rect(rectX, rectY, 200, 20), player2NameToEdit, 25);


	///use return key to exit text input field
	/*void OnGUI() 
	{
		Event e = Event.current;
		if (e.keyCode == KeyCode.Return)
		{
			player1Name = player1NameToEdit;
			Debug.Log("Player 1 Name Is " + player1Name);
		}
		else if (user1HasHitReturn == false)
		{
			player1NameToEdit = GUI.TextField(new Rect(rectX1, rectY1, 200, 20), player1NameToEdit, 25);
			Debug.Log("now in the field" + player1NameToEdit);

		}



		//player2NameToEdit = GUI.TextField(new Rect(rectX, rectY, 200, 20), player2NameToEdit, 25);
	}*/


}
