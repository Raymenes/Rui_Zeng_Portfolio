using UnityEngine;
using System.Collections;

public class RzCoinAct : MonoBehaviour {

	public Color color1;
	public Color color2;
	Color lerpedColor;
	public float smooth = 2f;

	public bool nextTarget = false;
	public bool targetCoin = false;

	GameObject CoinManager;

	void Start () 
	{
		CoinManager = GameObject.Find("CoinManager");
		transform.GetComponent<Renderer>().material.color = color1;
	}
	

	void Update () 
	{
		IndicateTargetCoin();
	}

	void OnTriggerEnter2D(Collider2D col)
	{
		if (col.gameObject.tag.Equals("Deadline"))
		{
			print ("coin killed by deadline!");
			Destroy(gameObject);
			CoinManager.GetComponent<RzCoinManager>().LostOneCoin();
			//send coin manager message	
		}
		if(col.gameObject.tag.Equals("Player"))
		{
			//Debug.Log("coin collide player!");
			if (targetCoin)
			{
				Debug.Log("target coin collide player!");
				CoinManager.GetComponent<RzCoinManager>().CollectOneCoin();
				Destroy(gameObject);
			}
		}
	}

	void ColorLerp ()
	{
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

	void IndicateTargetCoin ()
	{
		if(nextTarget)
		{
			transform.GetComponent<Renderer>().material.color = color1;
		}
		else if (targetCoin)
		{

			ColorLerp();
		}
		else
		{
			transform.GetComponent<Renderer>().material.color = color2;
		}
	}

	public void SetCurrentTarget ()
	{
		targetCoin = true;
		nextTarget = false;
	}

	public void SetNextTarget ()
	{
		nextTarget = true;
	}

	public void MessageCoinManager ()
	{

	}
}
