using UnityEngine;
using System.Collections;

public class RZFlashMode2 : MonoBehaviour {

	float alphaValue1 = 0;
	float alphaValue2 = 1f;
	public float smooth = 1;

	SpriteRenderer sr;
	float LerpAlpha;

	float r, g, b, a;


	void Start () 
	{
		sr = gameObject.GetComponent<SpriteRenderer>();
		r = sr.color.r;
		g = sr.color.g;
		b = sr.color.b;
		a = sr.color.a;
		LerpAlpha = 0;
	}
	
	void Update () 
	{
		//print(sr.color.a);

		if (sr.color.a <= alphaValue1 +0.01f)
			LerpAlpha = alphaValue2;
		if (sr.color.a >= alphaValue2 -0.01f)
			LerpAlpha = alphaValue1;
		
		a = Mathf.Lerp(sr.color.a, LerpAlpha, smooth*Time.deltaTime);
		sr.color = new Color(r, g, b, a);


		/*while (sr.color.a > 0)
		{
			a -= 0.00000000000000000000000000000000001f;

			sr.color = new Color(r, g, b, a);
		}*/
	}
}
