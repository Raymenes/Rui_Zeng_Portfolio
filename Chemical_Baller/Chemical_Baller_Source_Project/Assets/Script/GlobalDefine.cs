using UnityEngine;
using System.Collections;

public class ResourcePath 
{
	public static string DarkFog_path = "Effect/DarkFog";
	public static string Splash_path = "Effect/Splash";
	public static string Explosion_path = "Effect/Explosion";
	public static string Electric_path = "Effect/Electric";

	public static string PowerUp_path = "Prefab/PowerUp";
	public static string Ball_path = "Prefab/Ball";
	public static string EnemyBall_path = "Prefab/Enemy_Ball";
	public static string GroundEnemy_path = "Prefab/EnemyModel";
	public static string AirEnemy_path = "Prefab/AirEnemyModel";
}

public class PlayerStat
{
	public static float LaunchSpeed = 25f; /*for player*/
	public static float ShootSpeed = 40f; /*for ball*/
	public static float PlayerMoveSpeed = 10f; /*on ground*/
	public static float LaunchCost = 15f; /*cost per launch*/
	public static float ShootCost = 15f; /*cost per shooting ball*/
	public static float ShootBulletCost = 3f;
	public static float ShootBombCost = 12f;
	public static float ShootFreezeCost = 10f;
	public static float ShootShotGunCost = 10f;
	public static float JumpCost = 15f;
	public static float JumpSpeed = 10f; /*on ground*/
	public static float EnergyResumeRate = 6f; /*resuming this much energy per sec*/
	public static int ShotGunBulletNum = 5;
}

public class EnemyStat
{
	public static float MoveSpeed = 8f;
	public static float ArenaRadius = 200f;
	public static float ShootDistance = 7f;
	public static float GravityScale = 9.8f;
	public static float EnemyShootSpeed = 15f;
	public static float ShootCoolDown = 1.5f;
	public static float AttackStateCoolDown = 6f;
	public static float AfterAttackRestTime = 1.5f;
	public static float FreezeDuration = 3f;
	public static float LeavePowerUpChance = 0.2f;
	public static float LeavePowerUpTime = 3.0f;
	public static float AttackDmg = 10f;
	public static int EnemyMultiShootNum = 3;
	public static float EnemyMultiShootInterval = 0.5f;
}

public class GameStat
{
	public static float MaxWaveDuration = 20f;
	public static float MessageLifetime = 2f;
	public static int killEnemyScore = 10;
	public static int maxAttackingEnemy {get{return 7;}} /* How many enemies can attack at a time */
}

public class EffectStat
{
	public static float bulletLifetime = 1f;
	public static float explodeLifetime = 2.5f;
	public static float freezeLifetime = 4f;
}