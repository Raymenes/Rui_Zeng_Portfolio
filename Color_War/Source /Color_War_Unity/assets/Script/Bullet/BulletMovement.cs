﻿using UnityEngine;
using System.Collections;

public class BulletMovement : MonoBehaviour {

	bool redBullet;
	Vector3 bulletDirection;
	public float speed; 
	Vector2 bulletEndLine; // representing a line

	Vector2 player1BulletEndLine = new Vector2(3f, 0);
	Vector2 player2BulletEndLine = new Vector2(-3f, 0);

	float endAdjust = 0.1f;
	float distanceToEnd;

	Material player1BlockMaterial;
	Material player2BlockMaterial;

	string bulletShape;

	int GameMode;
	GameObject GlobalDefine;

	void Start () 
	{
		player1BlockMaterial = Resources.Load<Material>("Red");
		player2BlockMaterial = Resources.Load<Material>("Blue");
		SpecifyBulletVariables();
		GlobalDefine = GameObject.Find("GlobalDefine");
		GameMode = GlobalDefine.GetComponent<GameManager>().GetGameMode();

		//bulletShape = "square";

	}
	
	void Update () 
	{

		CalculateBulletEnd();

		transform.Translate(bulletDirection*speed*Time.deltaTime);

	}

	public void SetBulletIdentity (string FireKey)
	{
		if (FireKey == "d")
			redBullet = true;
		if (FireKey == "j")
			redBullet = false;


	}

	void SpecifyBulletVariables ()
	{
		if (redBullet)
		{
			bulletDirection = Vector3.right;
			bulletEndLine = player1BulletEndLine;
			gameObject.tag = "color1";
			transform.GetComponent<Renderer>().material.color = GameManager.player1Color;

			//transform.renderer.material.color = Color.red;
		}
		else
		{
			bulletDirection = Vector3.left;
			bulletEndLine = player2BulletEndLine;
			gameObject.tag = "color2";
			transform.GetComponent<Renderer>().material.color = GameManager.player2Color;
			//transform.renderer.material.color = Color.blue;
		}

	}

	void CalculateBulletEnd ()
	{
		if (bulletEndLine.x == 0)
		{
			distanceToEnd = Mathf.Abs(transform.position.y - bulletEndLine.y);
		}
		if (bulletEndLine.y == 0)
		{
			distanceToEnd = Mathf.Abs(transform.position.x - bulletEndLine.x);
		}
		if(distanceToEnd <= 0.1f)
		{
			Destroy(gameObject);
		}
	}

	void OnTriggerEnter2D (Collider2D other)
	{
		if (other.gameObject.name == "Blocks")
		{
			string theBlockShape = other.gameObject.GetComponent<BlockTypeSwitch>().GetBlockShape();
			GameObject theBlock = other.gameObject;
			Debug.Log(theBlock);
			Debug.Log("the block shape is " + theBlockShape);
			Debug.Log("the bullet shape is " + bulletShape);
			BulletBlockInteraction(other, theBlock, theBlockShape);
		}
	}

	void BulletBlockInteraction (Collider2D other, GameObject block, string blockShape)
	{
		//get the block spin status
		//if (bulletColor != blockColor)
		/*   if (bulletShape == blockShape)
		 * 		bullet occupied the block
		 * 	 else do nothing
		 * 	
		 *if (bulletColor = blockColor)
		 *	 if(spinning)
		 *		pass through
		 *	 if (!spinning)
		 *		if (bulletShape == blockShape)
		 *			spin
		 *		else
		 *			destroy Bullet
		 * 		check if shape match and then occupy or self-destroy
		 */
		if (GameMode == 3 )
		{
			bool spinning = other.gameObject.GetComponent<BlockTypeSwitch>().GetSpinStatus();
			
			if (gameObject.tag != block.tag)
			{
				if (bulletShape == blockShape)
				{
					if (gameObject.tag == "color1")
					{
						other.gameObject.GetComponent<BlockTypeSwitch>().ChangeBlockColorTo("red");
						CallSplashEffect("red", other.gameObject.transform.position);
						Destroy(gameObject);
						GameManager.currentScore += 1;
					}
					else if (gameObject.tag == "color2")
					{
						other.gameObject.GetComponent<BlockTypeSwitch>().ChangeBlockColorTo("blue");
						CallSplashEffect("blue", other.gameObject.transform.position);
						Destroy(gameObject);
						GameManager.currentScore += 1;
					}
				}
				else
					Destroy(gameObject);
			}
			else if (gameObject.tag == block.tag)
			{
				if (spinning)
				{
					
				}
				else
				{
					if (bulletShape == blockShape)
					{
						other.gameObject.GetComponent<BlockTypeSwitch>().SetSpinStatus(true);
					}
					else
						Destroy(gameObject);
				}
			}
		}

		if (GameMode == 1 || GameMode == 2)
		{
			if (bulletShape == blockShape)
			{

				if (gameObject.tag == "color1" && other.gameObject.tag != "color1")
				{
					other.gameObject.GetComponent<BlockTypeSwitch>().ChangeBlockColorTo("red");
					//Instantiate(Resources.Load<GameObject>("RedSplashParticles"), 
					            //other.gameObject.transform.position, Quaternion.identity);
					CallSplashEffect("red", other.gameObject.transform.position);
					Destroy(gameObject);
				}
				if (gameObject.tag == "color2" && other.gameObject.tag != "color2")
				{
					other.gameObject.GetComponent<BlockTypeSwitch>().ChangeBlockColorTo("blue");
					//Instantiate(Resources.Load<GameObject>("BlueSplashParticles"), 
					            //other.gameObject.transform.position, Quaternion.identity);
					CallSplashEffect("blue", other.gameObject.transform.position);
					Destroy(gameObject);
				}
			}
				
			else 
				Destroy(gameObject);
		}



	}

	public void SetBulletShape (string shape)
	{
		bulletShape = shape;
	}

	public void SetGameMode(int currentGameMode)
	{
		GameMode = currentGameMode;
	}

	void CallSplashEffect (string color, Vector3 SplashPosition)
	{
		if (color == "red")
		Instantiate(Resources.Load<GameObject>("RedSplashParticles"), 
		            SplashPosition, Quaternion.identity);
		else if (color == "blue")
		Instantiate(Resources.Load<GameObject>("BlueSplashParticles"), 
			        SplashPosition, Quaternion.identity);
	}

}