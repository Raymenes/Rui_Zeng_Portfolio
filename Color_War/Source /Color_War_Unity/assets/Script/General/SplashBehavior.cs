using UnityEngine;
using System.Collections;

public class SplashBehavior : MonoBehaviour {

	// Use this for initialization
	void Start () 
	{
		Debug.Log(GetComponent<ParticleSystem>().duration);
		Destroy(gameObject,1f+GetComponent<ParticleSystem>().duration);
	}
	
	// Update is called once per frame
	void Update () 
	{
		Debug.Log(GetComponent<ParticleSystem>().duration);

	}
}
