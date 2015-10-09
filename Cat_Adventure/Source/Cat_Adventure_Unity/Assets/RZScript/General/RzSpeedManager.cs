using UnityEngine;
using System.Collections;

public class RzSpeedManager : MonoBehaviour {

	public static float CatNormalSpeed =15f;
	public static float CatBurstSpeedOffset =15f;
	public static float CatBurstSpeedDuration =1.5f;
	public static float BirdSpeed =20f;
	public static float BloodNormalSpeed =15f;
	public static float BloodRetreatSpeedOffset = 5f;
	public static float BloodChaseSpeedOffset =5;

	void Start () 
	{
	
	}
	
	void Update () 
	{
		if(Input.GetKeyDown(KeyCode.Escape))
		{
			Application.LoadLevel("RzTest");
		}
	}
}
