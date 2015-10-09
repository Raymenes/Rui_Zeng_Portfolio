using UnityEngine;
using System.Collections;

public class RzGridSystem : MonoBehaviour {

	public float GridWidth = 4.47f;
	public float GridHeight = 4.47f;
	
	float CellWidth = 4.47f;
	float CellHeight = 4.47f;
	
	
	Vector3 movingRod;

	void Start () 
	{
		SnapToGrid ();
	}
	
	
	
	// Update is called once per frame
	void Update () 
	{
		//print ("84.75%4 = " + 84.75f%4f);
	}
	
	//Set the object to the center of a cell
	public void SnapToGrid()
	{

			//this.transform.position = CellCenter (Cell ());
		float xx = this.transform.position.x;
		float yy = this.transform.position.y;

		xx = xx - (xx%4f);
		yy = yy - (yy%4f) +2f;

		Vector3 pos = new Vector3(xx, yy, 0f);

		this.transform.position = pos;
	}
	
	//Cell center of a cell in WORLD_SPACE.
	//Cellcode describes cell coordinates. e.x. 5,4


}
