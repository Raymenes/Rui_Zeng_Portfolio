using UnityEngine;
using System.Collections;

public class RZFlash : MonoBehaviour {

	public float flashRate = 10f;

	void Start () 
	{
		flashRate = 1/flashRate;
		Invoke("FlashOff", flashRate);
	}
	
	void Update () 
	{

	}

	void FlashOff ()
	{
			gameObject.GetComponent<Renderer>().enabled = false;
		Invoke("FlashOn", flashRate);
	}

	void FlashOn ()
	{
		gameObject.GetComponent<Renderer>().enabled = true;
		Invoke("FlashOff", flashRate);
	}
}
