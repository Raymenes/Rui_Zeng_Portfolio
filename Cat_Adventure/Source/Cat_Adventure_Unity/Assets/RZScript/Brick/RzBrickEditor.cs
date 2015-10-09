using UnityEngine;
using System.Collections;

public class RzBrickEditor : MonoBehaviour {
	
	public int rightNumber, leftNumber, upNumber, downNumber;
	public float brickSideLength = 4.47f;
	Vector3 generatePosition;

	void Start () 
	{

		if (rightNumber > 0)
		{
			generatePosition = this.transform.position;
			generatePosition.x += brickSideLength;
			for(int i = 0; i < rightNumber; i++)
			{
				GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (gameObject.name), 
				                                                       generatePosition, Quaternion.identity);
				brick.transform.parent = gameObject.transform;

				generatePosition.x += brickSideLength;

			}
		}
		if (leftNumber > 0)
		{
			generatePosition = this.transform.position;
			generatePosition.x -= brickSideLength;
			for(int i = 0; i < leftNumber; i++)
			{
				GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (gameObject.name), 
				                                                       generatePosition, Quaternion.identity);
				brick.transform.parent = gameObject.transform;
				
				generatePosition.x -= brickSideLength;
				
			}
		}
		if (upNumber > 0)
		{
			generatePosition = this.transform.position;
			generatePosition.y += brickSideLength;
			for(int i = 0; i < upNumber; i++)
			{
				GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (gameObject.name), 
				                                                       generatePosition, Quaternion.identity);
				brick.transform.parent = gameObject.transform;
				
				generatePosition.y += brickSideLength;
				
			}
		}
		if (downNumber > 0)
		{
			generatePosition = this.transform.position;
			generatePosition.y -= brickSideLength;
			for(int i = 0; i < downNumber; i++)
			{
				GameObject brick = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> (gameObject.name), 
				                                                       generatePosition, Quaternion.identity);
				brick.transform.parent = gameObject.transform;
				
				generatePosition.y -= brickSideLength;
				
			}
		}
	}
	
	void Update () 
	{
	
	}
}
