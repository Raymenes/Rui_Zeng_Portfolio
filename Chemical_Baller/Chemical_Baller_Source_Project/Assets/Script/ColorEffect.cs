using UnityEngine;
using System.Collections;

public class ColorEffect : MonoBehaviour {

	public Color[] colors;
	public float changeSpeed;
	int currId = 0;
	int nextId = 1;
	float t = 0f;

	void Start () {

	}
	
	void Update () {
//		GetComponent<Renderer>().material.color = colors[3];

		GetComponent<Renderer>().material.color = Color.Lerp(colors[currId], colors[nextId], t);
		t += changeSpeed/10f * Time.deltaTime;
		if(GetComponent<Renderer>().material.color == colors[nextId])
		{
			currId = nextId;
			++nextId;
			if(nextId == colors.Length){
				nextId = 0;
			}
			t = 0;
		}
	}
}
