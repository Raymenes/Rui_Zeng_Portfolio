using UnityEngine;
using System.Collections;

public class RzBlockManager : MonoBehaviour {

	public string readyBlock = "None";
	string aboutToReady = "";
	bool receivedSignal;
	string readyBlockList = "Ring" + "Square" + "Cross" + "Tri";
	bool onAnotherButton = false;
	string playerMovementMode;

	public bool testMode = false;
	GameObject player;


	void Start () 
	{
		//test
		//readyBlock = "Ring";
		//test
		player = GameObject.FindWithTag("Player");
		receivedSignal = false;
	}
	
	void Update () 
	{
		//test keyboard control
		if (Input.GetKeyDown(KeyCode.Alpha1))
		{
			readyBlock = "Ring";
		}
		if (Input.GetKeyDown(KeyCode.Alpha2))
		{
			readyBlock = "Square";
		}
		if (Input.GetKeyDown(KeyCode.Alpha3))
		{
			readyBlock = "Cross";
		}
		if (Input.GetKeyDown(KeyCode.Alpha4))
		{
			readyBlock = "Tri";
		}


		ReadPlayerMovementMode();
		player.GetComponent<RzCatBehavior>().SetBlockOnHold(readyBlock);

		if (receivedSignal && !Input.anyKey)
		{
			Debug.Log("signal received!");


				readyBlock = aboutToReady;

			receivedSignal = false;
		}
		/* Drag Effect
		if (Input.GetMouseButtonUp(0))
		{
			if(readyBlock != "None")
			{
				Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
				mousePosition.z = 0f;

				GameObject aBlock = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (readyBlock), 
				                                                        mousePosition, Quaternion.identity);
				aBlock.GetComponent<RzPlaceable> ().isButton = false;
				aBlock.GetComponent<RzFollowPlayer>().enabled = false;
				aBlock.gameObject.name = "clone" + readyBlock;
				readyBlock = "None";

			}

		}
		*/ 
		if (Input.GetMouseButtonDown(0) && !onAnotherButton && playerMovementMode!="Wrap")
		{
			Vector3 mousePosition = Camera.main.ScreenToWorldPoint (Input.mousePosition);
			mousePosition.z = 0f;

			if(readyBlock != "None" && !onAnotherButton)
			{
				GameObject existingCloneBlock = GameObject.Find("clone" + readyBlock);
				
				if (existingCloneBlock != null)
				{
					Destroy(existingCloneBlock);
				}
				GameObject aBlock = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (readyBlock), 
				                                                        mousePosition, Quaternion.identity);
				aBlock.GetComponent<RzPlaceable> ().isButton = false;
				aBlock.GetComponent<RzFollowPlayer>().enabled = false;
				aBlock.gameObject.name = "clone" + readyBlock;
				readyBlock = "None";
			}
			
		}
	}

	public void SetReadyBlock (string blockName)
	{
		if (playerMovementMode != "Wrap")
		{
			receivedSignal = true;
			Debug.Log("set ready block is " + blockName);
			aboutToReady = blockName;
		}
		else
			receivedSignal = false;


	}

	public void ReadPlayerMovementMode ()
	{
		playerMovementMode = player.GetComponent<RzCatBehavior>().GetPlayerMovementMode();
		//print(playerMovementMode);
	}

	public string GetReadyBlock ()
	{
		return readyBlock;
	}

	public void CheckMouseOnButton (bool yesOrNo)
	{
		onAnotherButton = yesOrNo;
	}

	/*bool ifMouseOnAnotherButton(Vector3 mousePos)
	{
		if (mousePos.x < -65f && mousePos.x > -85f)
		{
			if (mousePos.y < 10 && mousePos.y > -40)
			{
				return true;
				Debug.Log("mouse on another button!");
			}
			else 
				return false;
		}

		else 
			return false;
	}*/

}
