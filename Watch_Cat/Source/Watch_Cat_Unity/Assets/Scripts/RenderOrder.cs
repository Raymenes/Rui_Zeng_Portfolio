using UnityEngine;
using System.Collections;

public class RenderOrder : MonoBehaviour {

	void Start () 
	{
		if(this.tag.Equals("Ghost")){
			this.gameObject.GetComponent<Renderer>().material.renderQueue = 100;
		}
		else if(this.tag.Equals("Human")){
			this.gameObject.GetComponent<Renderer>().material.renderQueue = 100;
		}
		else if(this.tag.Equals("Player")){
			this.gameObject.GetComponent<Renderer>().material.renderQueue = 100;
		}
		else if(this.tag.Equals("Environment")){
			this.gameObject.GetComponent<Renderer>().material.renderQueue = 100;
		}
		else if(this.tag.Equals("Background")){
			this.gameObject.GetComponent<Renderer>().material.renderQueue = 100;
		}
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
