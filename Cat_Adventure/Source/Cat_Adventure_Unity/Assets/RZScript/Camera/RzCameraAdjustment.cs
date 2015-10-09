using UnityEngine;
using System.Collections;

public class RzCameraAdjustment : MonoBehaviour {

	Camera Cam;
	public float CameraSize = 75f;
	GameObject player;
	public float smooth=0.3f;
	string lastPlayerMoveMode;
	bool lerping = false;

	void Start () 
	{
		Cam = Camera.main;
		Cam.orthographicSize = CameraSize;
		player = GameObject.FindGameObjectWithTag ("Player");
	}
	
	void Update () 
	{
		if(GameObject.FindGameObjectWithTag ("Player") != null)
		{
			//should lerp when cat wrap

			if (player.GetComponent<RzCatBehavior>().GetPlayerMovementMode() == "Line" && lastPlayerMoveMode=="Wrap") 
			{
				lerping = true;
				Invoke("TurnOffLerp", 1f);
			}


		}
		LerpCamera();
		lastPlayerMoveMode = player.GetComponent<RzCatBehavior>().GetPlayerMovementMode();
	}

	void LerpCamera ()
	{
		if (lerping)
		{
			this.transform.position = Vector3.Lerp(transform.position, 
			                                       new Vector3(player.transform.position.x,
			            this.transform.position.y,
			            this.transform.position.z), 
			                                       smooth * Time.deltaTime);
		}
		else
		{
			this.transform.position = new Vector3 (player.transform.position.x,
			                                       this.transform.position.y,
			                                       this.transform.position.z);
		}
	}

	void TurnOffLerp ()
	{
		lerping = false;
	}
}
