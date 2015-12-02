using UnityEngine;
using System.Collections;

public class BallBehav : MonoBehaviour {

	public enum OwnerType{player, enemy}
	public OwnerType mOwner = OwnerType.player;

	//Editable Variable
	float lifeTime = 7f;

	//Regular Variable
	public enum BallType{none = 0, bullet = 1, explode = 2, freeze = 3, shotgun = 4};
	BallType mType = BallType.bullet;
	public BallType MyType{get{return mType;} set{mType = value;}}
	float spawnTime;

	void Start () {
		spawnTime = Time.timeSinceLevelLoad;
	}


	void Update () {
		if((Time.timeSinceLevelLoad - spawnTime) > lifeTime){
			Destroy(this.gameObject);
		}
	}

	#region Play Effect
	void Melt()
	{
		GameObject effect;
		switch(mType)
		{
		case BallType.bullet:
			GameManager.instance.SpawnBallEffect(GameManager.instance.Splash_prefab, mType, transform.position, EffectStat.bulletLifetime);
			break;
		case BallType.explode:
			GameManager.instance.SpawnBallEffect(GameManager.instance.Explosion_prefab, mType, transform.position, EffectStat.explodeLifetime);
			break;
		case BallType.freeze:
			GameManager.instance.SpawnBallEffect(GameManager.instance.Electric_prefab, mType, transform.position, EffectStat.freezeLifetime);
			GameManager.instance.SpawnBallEffect(GameManager.instance.DarkFog_prefab, mType, transform.position, EffectStat.freezeLifetime);
			break;
		case BallType.shotgun:
			GameManager.instance.SpawnBallEffect(GameManager.instance.Splash_prefab, mType, transform.position, EffectStat.bulletLifetime);
			break;
		}
		Destroy(this.gameObject);
	}

	void OnCollisionEnter(Collision col){
		if(!col.gameObject.tag.Equals("Ball") && !col.gameObject.tag.Equals("Bullet"))
		{
			if(col.gameObject.tag.Equals("enemy")  && mOwner == OwnerType.player){
				col.gameObject.GetComponent<EnemyStateMachine>().End();
			}
			GameObject effect;
			{
				switch(mType)
				{
				case BallType.bullet:
					GameManager.instance.SpawnBallEffect(GameManager.instance.Splash_prefab, mType, transform.position, EffectStat.bulletLifetime);
					Destroy(this.gameObject);
					break;
				case BallType.explode:
					GameManager.instance.SpawnBallEffect(GameManager.instance.Explosion_prefab, mType, transform.position, EffectStat.explodeLifetime);
					Destroy(this.gameObject);
					break;
				case BallType.freeze:
					break;
				case BallType.shotgun:
					GameManager.instance.SpawnBallEffect(GameManager.instance.Splash_prefab, mType, transform.position, EffectStat.bulletLifetime);
					Destroy(this.gameObject);
					break;
				}
			}
		}

		if(col.gameObject.tag.Equals("Player") && mOwner == OwnerType.enemy){
			GameManager.instance.AddDamage();
			Destroy(this.gameObject);
		}

		if(!this.gameObject.tag.Equals("Player"))
		{
			if(col.gameObject.tag.Equals("Ground")){
				Melt();
			}

		}
	}

	void OnTriggerEnter(Collider col)
	{
		if(col.gameObject.tag.Equals("Player") && mOwner == OwnerType.enemy){
			GameManager.instance.AddDamage();
			Destroy(this.gameObject);
		}
	}

	#endregion
}
