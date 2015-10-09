using UnityEngine;
using System.Collections;

public class ColorLerp : MonoBehaviour {

	public Color color1;
	public Color color2;
	Color lerpedColor;
	public float smooth = 2f;
	SpriteRenderer sr;
	
	void Start () 
	{
		sr = gameObject.GetComponent<SpriteRenderer>();
		//transform.renderer.material.color = color1;
		sr.color = color1;

	}
	
	void Update () 
	{

		if (sr.color == color1)
		{
			lerpedColor = color2;
		}
		if (sr.color == color2)
		{
			lerpedColor = color1;
		}
		/*
		if (transform.renderer.material.color == color1)
		{
			lerpedColor = color2;
		}
		if (transform.renderer.material.color == color2)
		{
			lerpedColor = color1;
		}
		*/
		sr.color = Color.Lerp(sr.color, lerpedColor, smooth * Time.deltaTime);
	}

}
