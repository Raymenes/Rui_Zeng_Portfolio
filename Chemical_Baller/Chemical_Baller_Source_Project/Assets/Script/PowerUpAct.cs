using UnityEngine;
using System.Collections;

public class PowerUpAct : MonoBehaviour {

	public enum powerUpType{ammo = 1, health, launch, score, energy}
	public powerUpType mType;


	public void SetLifeTime(float time)
	{
		Destroy(gameObject, time);
	}

	// Use this for initialization
	void Start () {
		mType = (powerUpType) Random.Range(1, 4);
	}
	
	void Update () {}

	void OnCollisionEnter (Collision col){
		if(col.gameObject.tag == "Ball" || col.gameObject.tag == "Player")
		{
			CollectPowerUp(col.gameObject);
		}
		if(col.gameObject.tag == "Ball")
		{
			OnHitBall();
		}
		if(col.gameObject.tag == "Player")
		{
			OnHitPlayer();
		}
	}

	protected virtual void OnHitBall()
	{}

	protected virtual void OnHitPlayer()
	{}

	void CollectPowerUp(GameObject other){
		if(mType == powerUpType.ammo){
			int addNum = Random.Range(10,20);
			PlayerAct.instance.AddBall(addNum);
			GameManager.instance.UpdateMessage("Get Ammo:  " + addNum + "!");
		}
		else if(mType == powerUpType.health){
			float addNum = Random.Range(15f,30f);
			GameManager.instance.AddHealth(addNum);
			GameManager.instance.UpdateMessage("Restore Health:  " + (int)addNum + "!");
		}
		else if(mType == powerUpType.launch){
			float addNum = Random.Range(7f,15f);
			PlayerAct.instance.AddDoubleLaunchChance(addNum);
			GameManager.instance.UpdateMessage("Triple Launch:  " + (int)addNum + "s!");
		}
		else if(mType == powerUpType.energy){
			float addNum  = Random.Range(20f,50f);
			PlayerAct.instance.AddEnergy(addNum);
			GameManager.instance.UpdateMessage("Fill Energy:  " + addNum + "!");
		}
		else if(mType == powerUpType.score){
			int addNum = Random.Range(15,30);
			GameManager.instance.AddScore(addNum);
			GameManager.instance.UpdateMessage("Score ++:  " + addNum + "!");
		}
		GameManager.instance.SpawnNextPowerUp(other.transform.position, Random.Range(15f, 30f));
		AudioManager.instance.PlayPickUpSound();
		Destroy(gameObject);
	}
}
