using UnityEngine;
using System.Collections;

public class EffectAct : MonoBehaviour {

	BallBehav.BallType type = BallBehav.BallType.none;

	void Start () {}
	
	void Update () {}

	public void SetLifeTimer(float duration)
	{
		Destroy(this.gameObject, duration);
	}

	public void SetType(BallBehav.BallType type)
	{this.type = type;}

	void OnTriggerEnter(Collider col)
	{
		if(col.gameObject.tag.Equals("enemy")){
			switch(type)
			{
			case BallBehav.BallType.bullet:
				break;
			case BallBehav.BallType.explode:
				col.gameObject.GetComponent<EnemyStateMachine>().End();
				break;
			case BallBehav.BallType.freeze:
				col.gameObject.GetComponent<EnemyStateMachine>().Freeze(2f);
				break;
//			case BallBehav.BallType.smoke:
//				col.gameObject.GetComponent<EnemyStateMachine>().Freeze(2f);
//				break;
			}
		}
	}
}
