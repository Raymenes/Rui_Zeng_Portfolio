using UnityEngine;
using System.Collections;

public class BlockTypeSwitch : MonoBehaviour {
	
	string currentBlockColor;
	string currentBlockShape;

	Sprite starShape, squareShape, circleShape;

	SpriteRenderer sr;

	int shapeIndex = 0;
	string[] shapeName = new string[3];

	string lastShape;
	public bool RanDomShapeSwitchingForAll;
	public bool RandomShapeSwitchingForEach;
	public bool RoutinShapeSwitching;

	public bool spinning;

	float nowAngle = 0f;
	public float spinDuration = 5f;
	public float RandomEachSwitchInterval = 5f;
	float spinSpeed = 500f;
	float nowSpinSpeed;
	
	void Start () 
	{
		currentBlockShape = "square";

		squareShape = Resources.Load<Sprite>("Square");
		starShape = Resources.Load<Sprite>("Star");
		circleShape = Resources.Load<Sprite>("Circle");

		ConsistantColorAndTag();
		lastShape = "square";
		sr = GetComponent<SpriteRenderer>();

		if (RoutinShapeSwitching)
		{
			InvokeRepeating("AutoShapeSwitching", 0f, 3f);
		}
		if (RanDomShapeSwitchingForAll)
		{
			InvokeRepeating("RandomShapeSwitching", 0.1f, 3f);
		}
		if (RandomShapeSwitchingForEach)
		{
			InvokeRepeating("RandomShuffleAll", 0.1f, RandomEachSwitchInterval);
		}
		/*if (spinning)
		{
			InvokeRepeating("SpinSprite", 0f, 0.1f);

		}*/
	}
	
	void Update () 
	{
		ConsistantColorAndTag();
		if (spinning)
		{
			Debug.Log("spinning is " + spinning);
			SpinSprite();
			//audio.Play();
			///
			//Invoke("TurnOffSpin", spinDuration);
			///this is the problem
		}


		/*if(Input.GetKeyDown(KeyCode.Alpha1))
		{
			AutoShapeSwitching();
			print("1 PRESSED!");
		}*/

	}

	public void ChangeBlockColorTo (string color)
	{
		if (color.Equals("red") || color.Equals("Red") || color.Equals("RED"))
		{
			this.transform.GetComponent<Renderer>().material.color = GameManager.player1Color;
			currentBlockColor = "red";
			gameObject.tag = "color1";
		}
		if (color.Equals("blue") || color.Equals("Blue") || color.Equals("BLUE"))
		{
			this.transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			currentBlockColor = "blue";
			gameObject.tag = "color2";
		}
		if (color.Equals("white") || color.Equals("White") || color.Equals("WHITE"))
		{
			this.transform.GetComponent<Renderer>().material.color = Color.white;
			currentBlockColor = "white";
			gameObject.tag = "colorNeutral";
		}

	}

	public void ChangeBlockShapeTo (string shape)
	{
		if (shape.Equals("square"))
		{
			currentBlockShape = shape;
			sr.sprite = squareShape;
		}
		if (shape.Equals("circle"))
		{
			currentBlockShape = shape;
			sr.sprite = circleShape;
		}
		if (shape.Equals("star"))
		{
			currentBlockShape = shape;
			sr.sprite = starShape;
		}
		
	}

	public string GetBlockShape ()
	{
		return currentBlockShape;
	}

	public string GetCurrentBlockStatus (string colorOrShape)
	{
		if (colorOrShape.Equals("color"))
			return currentBlockColor;
		else if (colorOrShape.Equals("shape"))
			return currentBlockShape;
		else
			return "incorrect parameter!";
	}

	void ConsistantColorAndTag()
	{
		if (gameObject.tag.Equals("color1"))
			ChangeBlockColorTo("red");
		if (gameObject.tag.Equals("color2"))
			ChangeBlockColorTo("blue");
		if (gameObject.tag.Equals("colorNeutral"))
			ChangeBlockColorTo("white");
	}

	int GetRandNum013 ()
	{
		return ((int)Random.Range(0, 100f))%3;
	}

	void RandomShapeSwitching ()
	{
		string nowShape;
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";
		int tempIndex = transform.GetComponentInParent<BlockScanner>().ReadLastRandomNumber();
		nowShape = shapeName[tempIndex];
		
		ChangeBlockShapeTo(nowShape);

	}

	void RandomShuffleAll()
	{
		string nowShape;
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";
		do
		{
			nowShape = shapeName[GetRandNum013()];
		}while (nowShape == lastShape);
		
		lastShape = nowShape;
		
		ChangeBlockShapeTo(nowShape);
	}

	void AutoShapeSwitching ()
	{
		shapeName[0] = "square";
		shapeName[1] = "circle";
		shapeName[2] = "star";

		ChangeBlockShapeTo(shapeName[shapeIndex]);
		if(shapeIndex == 2)
		{
			shapeIndex = 0;
		}
		else
			shapeIndex++;
	}

	void SpinSprite ()
	{
		/*
		float SpinAngle = 22.5f;
		nowAngle += SpinAngle;
		transform.rotation = Quaternion.AngleAxis(nowAngle, Vector3.up);
		*/
		transform.Rotate(Vector3.up * spinSpeed * Time.deltaTime);
	}

	void TurnOnSpin ()
	{
		spinning = true;
	}

	void TurnOffSpin ()
	{
		spinning = false;
		///this is the problem
		transform.rotation = Quaternion.AngleAxis(0f, Vector3.up);
	}

	public bool GetSpinStatus ()
	{
		return spinning;
	}

	public void SetSpinStatus (bool booleanInput)
	{
		spinning = booleanInput;
		if (booleanInput)
		{
			Invoke("TurnOffSpin", spinDuration);
			GetComponent<AudioSource>().Play();
		}
	}
}
