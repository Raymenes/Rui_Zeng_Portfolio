using UnityEngine;
using System.Collections;

public class FlashyEffect : MonoBehaviour {

	public Color[] colors;
	int id = 0;
	public float frequency = 2f;

	void Start () {
		if(colors.Length > 1 && frequency > 0) InvokeRepeating("SwitchColor", 0f, frequency);
	}
	
	// Update is called once per frame
	void Update () {

	}

	void SwitchColor()
	{
		id++;
		if(id >= colors.Length){
			id = 0;
		}
		GetComponent<Renderer>().material.color = colors[id];
	}

	public void SetColor(Color co)
	{
		foreach(Renderer r in transform.GetComponentsInChildren<Renderer>())
		{
			r.material.color = co;
		}
	}
}
