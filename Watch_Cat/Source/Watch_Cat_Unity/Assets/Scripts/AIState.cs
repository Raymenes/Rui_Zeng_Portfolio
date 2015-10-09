using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AIState {

	private string id = "";
	public string ID {get{ return id;}}
	public List<AIState> AdjacentStateList_To;

	public AIState(string AI_ID)
	{
		id = AI_ID;
		AdjacentStateList_To = new List<AIState> ();
	}

	public AIState findDescendantbyID(string ID)
	{
		foreach(AIState s in AdjacentStateList_To)
		{
			if(s.ID == ID)
			{
				return s;
			}
		}

		Debug.Log ("AIState " + this.ID + ": State Not Found!");
		return null;
	}

	public void AddChild(AIState child)
	{
		this.AdjacentStateList_To.Add (child);
	}



}

/*
public class AIStateTransition {

	private AIState m_StateFrom; 
	private AIState m_StateTo;
	private delegate void OnTransitionDel();

	public AIStateMachine StateMachine { get; set;}
	public OnTransitionDel onTransition;

	public void DoTransit()
	{

	}

}
*/

