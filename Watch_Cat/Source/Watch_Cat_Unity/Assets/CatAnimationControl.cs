using UnityEngine;
using System.Collections;

public class CatAnimationControl : MonoBehaviour {
	public Animator myAnimator;

	private bool facingRight = true;

	// Use this for initialization
	
	// Update is called once per frame
	void Update () 
	{
		if(Input.GetKeyDown(KeyCode.Space))
		{
			//myAnimator.parameters[0] = true;
		}
	}
}
