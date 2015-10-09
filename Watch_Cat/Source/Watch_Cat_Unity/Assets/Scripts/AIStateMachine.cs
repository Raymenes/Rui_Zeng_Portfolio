using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class AIStateMachine : MonoBehaviour
{
	public AIState CurState { get; set;}
	public AIState PrevState { get; set;}

	private List<AIState> stateList = new List<AIState>();
	

	//This method should be called once only before any states are added to the state machine
	public void AddStates(params string[] IDList)
	{
		foreach (string id in IDList)
		{
			this.stateList.Add(new AIState(id));
		}
	}

	public void TransitToState(string ID)
	{
		AIState stateTo = CurState.findDescendantbyID (ID);
		if(stateTo != null)
		{
			Debug.Log("Transition from state[" + CurState.ID + "] to [" + ID + "]");
			PrevState = CurState;
			CurState = stateTo;
		}
		else
		{
			Debug.Log("Failed transition from state[" + CurState.ID + "] to [" + ID + "]");
		}
	}

	public void TransitToNextState()
	{
		if (CurState.AdjacentStateList_To.Count > 0) 
		{
			TransitToState(CurState.AdjacentStateList_To[0].ID);
		}
	}

	public void AddRelationsForState(string stateID, params string[] toAdd)
	{
		AIState parentState = stateByID (stateID);
		foreach(string id in toAdd)
		{
			AIState aChild = stateByID(id);
			parentState.AddChild(aChild);
		}

	}

	public AIState stateByID(string ID)
	{
		foreach(AIState s in stateList)
		{
			if(s.ID == ID)
			{
				return s;
			}
		}

		Debug.Log ("AIStateMachine: State[" + ID + "]Not Found!");
		return null;
	}

	public bool isOnState(string ID)
	{
		if(CurState.ID == ID)
		{
			return true;
		}
		else
		{
			return false;
		}
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

