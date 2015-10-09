using UnityEngine;
using System.Collections;

public class RzRoutinPath : MonoBehaviour {

	public Vector3[] path = new Vector3[10];
	public float speed =5;
	public int pathBegin = 0;
	public int pathEnd = 1;
	
	Vector3 originPosition;
	
	void Start () 
	{
		pathEnd = pathEnd+1;
		
		originPosition = transform.position;
		for (int i = 0; i < pathEnd; i++)
		{
			path[i] = path[i] + originPosition;
		}
	}
	
	void Update () 
	{
		MoveTowardPoint(path[pathBegin]);
		
		if (transform.position == path[pathBegin])
		{
			pathBegin = pathBegin + 1;
		}
		
		if (pathBegin == pathEnd)
		{
			pathBegin = 0;
		}
		
	}
	
	void MoveTowardPoint (Vector3 point)
	{
		bool arrived = false;
		
		transform.position = Vector3.MoveTowards(transform.position, point, speed * Time.deltaTime);
		if (transform.position == point)
		{
			arrived = true;
		}
		
		else
			arrived = false;
		
	}
}
