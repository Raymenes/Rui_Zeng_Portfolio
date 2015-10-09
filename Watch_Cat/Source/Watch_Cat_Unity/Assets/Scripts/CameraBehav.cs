using UnityEngine;
using System.Collections;

public class CameraBehav : MonoBehaviour {

	public Camera MainCam;
	public GameObject Cat;

	Vector3 initPos;
	Vector3 currPos;
	Vector3 nextPos;
	
	public float boundaryRatio = 0.67f;
	float lerpSpeed = 10f;
	float lerpDistance;
	float totalLerpTime;
	float currLerpTime;
	public bool lerping = false;

	void Start () {
		initPos = MainCam.transform.position;
		currPos = initPos;

	}
	
	void Update () 
	{
		if(!lerping)
		{
			if ( ( Camera.main.WorldToViewportPoint(Cat.transform.position).x > 0.5f * (1f+boundaryRatio)) 
			    || ( Camera.main.WorldToViewportPoint(Cat.transform.position).x < 0.5f * (1f-boundaryRatio) )  )
			{
				CatBehav.movementStatus catStatus = Cat.GetComponent<CatBehav>().GetCatStatus(); 

				if( catStatus == CatBehav.movementStatus.sitting 
				   || catStatus == CatBehav.movementStatus.dead )
				{
					lerping = true;
					nextPos = new Vector3(Cat.transform.position.x, currPos.y, currPos.z);
					lerpDistance = nextPos.x - currPos.x;
					totalLerpTime = Mathf.Abs( lerpDistance / lerpSpeed );
					currLerpTime = 0f;
				}
			}
		}

		if (lerping)
		{
			currLerpTime += Time.deltaTime;
			MainCam.transform.position = Vector3.Lerp(MainCam.transform.position, nextPos, currLerpTime/totalLerpTime);
			if(currLerpTime >= totalLerpTime)
			{
				MainCam.transform.position = nextPos;
				currPos = MainCam.transform.position;
				lerpDistance = 0f;
				totalLerpTime = 0f;
				lerping = false;
			}
		}
	}
}
