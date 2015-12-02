using UnityEngine;
using System.Collections;

public class EnemyState {
	
	public EnemyState(){}
	public EnemyState(EnemyStateMachine machine)
	{
		this.machine = machine;
		InitState();
	}
	
	public void SetMachine(EnemyStateMachine machine){this.machine = machine;}
	
	protected EnemyStateMachine machine;

	public virtual void InitState()
	{}
	
	public virtual void StateUpdate()
	{}
	
	public virtual void StateFixedUpdate()
	{}
	
	public virtual void StateExit()
	{}
	
	public virtual void StateEnter()
	{}

	public virtual void Restart()
	{
		StateExit();
		StateEnter();
	}
	
	public virtual string StateType()
	{return this.GetType().ToString();}
}
