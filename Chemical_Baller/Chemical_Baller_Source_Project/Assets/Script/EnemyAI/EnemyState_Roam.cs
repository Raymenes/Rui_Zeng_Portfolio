using UnityEngine;
using System.Collections;

public class EnemyState_Roam : EnemyState {

	public EnemyState_Roam(){}
	public EnemyState_Roam(EnemyStateMachine machine)
	{this.machine = machine; InitState();}

	Vector3 destination;

	#region Timer Related
	#endregion
	
	#region Running State
	public override void InitState()
	{
		destination = machine._position;
	}
	
	public override void StateUpdate()
	{
		if(!machine.IsAttackOnCoolDown & GameManager.instance.CanAttack()){
			if(machine.CheckIsPlayerVisibile() ){
				machine.SwitchState(machine.mAttackState.StateType());
			}
		}
		else{
			MoveAround();
		}
	}
	
	public override void StateFixedUpdate()
	{}
	
	public override void StateExit()
	{}
	
	public override void StateEnter()
	{
		GetDestination();
	}
	#endregion

	#region Helper Function
	void GetDestination()
	{
		Vector3 new_destination;
		bool legitDestination;
		Vector3 detect_dir = machine.transform.forward;
		float radius = 10f;
		
		int i = 0;
		//try 5 times to get a destination
		//that is not blocked by any wall
		do{
			legitDestination = true;
			
			new_destination = new Vector3(Random.Range(destination.x - 10f, destination.x + 10f), 
			                              machine._position.y, 
			                              Random.Range(destination.z - 10f, destination.z + 10f));
			
			if(new_destination.sqrMagnitude > EnemyStat.ArenaRadius*EnemyStat.ArenaRadius){
				legitDestination = false;
			}
			RaycastHit hitInfo;
			Vector3 distance = (new_destination-machine._position);
			if(Physics.Raycast(machine._position, distance.normalized, out hitInfo, distance.magnitude)){
				if( hitInfo.collider.tag == "environment"){
					legitDestination = false;
					i++;
				}
			}
		}while(!legitDestination && i < 5);
		
		destination = new_destination;
		machine.transform.LookAt(destination);
	}

	void MoveAround()
	{
		if(machine.IsFacingWall()){
			GetDestination();
		}
		else{
			//the following function returns a boolean that indicates arrived or not
			if(machine.MoveTowards(destination, EnemyStat.MoveSpeed))
			{
				GetDestination();
			}
		}
	}
	#endregion
}
