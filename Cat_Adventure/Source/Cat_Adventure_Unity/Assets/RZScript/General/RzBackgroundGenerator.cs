using UnityEngine;
using System.Collections;

public class RzBackgroundGenerator : MonoBehaviour {

	Vector3 BgGeneratePosition = new Vector3 (0f, 0f, 0f);
	GameObject[] Bgs;
	public float BgOffset = 194.7f;
	public int maxBgNum = 8;
	GameObject player;

	void Start () 
	{
		BgGeneratePosition = new Vector3(BgGeneratePosition.x + BgOffset, 0f, 0f);
	}
	
	void Update () 
	{
		GenerateBackground();
	}

	void GenerateBackground ()
	{
		//Bgs = GameObject.FindGameObjectsWithTag("Background");

		if (transform.childCount < maxBgNum)
		{
			GameObject Bg = (GameObject)GameObject.Instantiate (Resources.Load<GameObject> ("LowPolyBackground"), 
			                                                    BgGeneratePosition, Quaternion.identity);
			Bg.transform.parent = gameObject.transform;
			BgGeneratePosition = new Vector3(BgGeneratePosition.x + BgOffset, 0f, 0f);
		}
	}
}
