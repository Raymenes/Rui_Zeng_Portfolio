using UnityEngine;
using System.Collections;

public class RzColorLerp: MonoBehaviour {
	public Color color1;
	public Color color2;
	Color lerpedColor;
	public float smooth = 2f;

	void Start () 
	{
		transform.GetComponent<Renderer>().material.color = color1;
	}
	
	void Update () 
	{
		//lerpedColor = Color.Lerp(color1, color2, 1f);
		//transform.renderer.material.color = lerpedColor;


		/*
		if(Input.GetKeyDown(KeyCode.Z))
			lerpedColor = color1;
		if(Input.GetKeyDown(KeyCode.C))
			lerpedColor = color2;
		*/
		if (transform.GetComponent<Renderer>().material.color == color1)
		{
			lerpedColor = color2;
		}
		if (transform.GetComponent<Renderer>().material.color == color2)
		{
			lerpedColor = color1;
		}

		transform.GetComponent<Renderer>().material.color = Color.Lerp(transform.GetComponent<Renderer>().material.color, lerpedColor, smooth * Time.deltaTime);
	}
}
