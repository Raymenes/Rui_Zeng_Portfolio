using UnityEngine;
using System.Collections;

public class PlayerBulletSpecification : MonoBehaviour {

	Sprite starShape, squareShape, circleShape;
	
	SpriteRenderer sr;

	void Awake ()
	{
		squareShape = Resources.Load<Sprite>("Square");
		starShape = Resources.Load<Sprite>("Star");
		circleShape = Resources.Load<Sprite>("Circle");
		sr = GetComponent<SpriteRenderer>();
		

	}

	void Start () 
	{
	}
	
	void Update () 
	{
		//sr.sprite = circleShape;
	}
	public void ChangeBulletShapeTo (string shape)
	{
		if (shape.Equals("square"))
		{
			sr.sprite = squareShape;
		}
		if (shape.Equals("circle"))
		{
			sr.sprite = circleShape;
		}
		if (shape.Equals("star"))
		{
			sr.sprite = starShape;
		}
		
	}
}
