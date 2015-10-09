using UnityEngine;
using System.Collections;

public class BulletIndicatorBehavior : MonoBehaviour {

	string currentBulletType;
	SpriteRenderer sr;
	Sprite starShape, squareShape, circleShape;

	void Start () 
	{
		currentBulletType = "square";
		transform.GetComponent<Renderer>().material.color = Color.black;

		squareShape = Resources.Load<Sprite>("Square");
		starShape = Resources.Load<Sprite>("Star");
		circleShape = Resources.Load<Sprite>("Circle");

		sr = GetComponent<SpriteRenderer>();
	}
	
	void Update () 
	{
		if (currentBulletType.Equals("star"))
			sr.sprite = starShape;
		else if (currentBulletType.Equals("square"))
			sr.sprite = squareShape;
		else if (currentBulletType.Equals("circle"))
			sr.sprite = circleShape;

	}

	public void SetBulletType(string type)
	{
		if (type.Equals("square"))
			currentBulletType = type;
		else if (type.Equals("circle"))
			currentBulletType = type;
		else if (type.Equals("star"))
			currentBulletType = type;
	}

	public string GetBulletType()
	{
		return currentBulletType;
	}
}
