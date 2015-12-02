using UnityEngine;
using System.Collections;

public class TouchTracker : MonoBehaviour {

	public GameObject Player;

	Vector2 mousePosition;

	void Start () 
	{
	
	}
	
	void OnMouseDown()
	{
		mousePosition = Input.mousePosition;
	}
}
