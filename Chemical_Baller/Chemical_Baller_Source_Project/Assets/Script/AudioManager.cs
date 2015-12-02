using UnityEngine;
using System.Collections;

public class AudioManager : MonoBehaviour {

	#region Singleton Struct
	//=============This makes sure only one instance of the manager ever exists.================//
	private static AudioManager _instance;
	public static AudioManager instance	//Just dont GameManager.instance to get the right GameManager.
	{
		get { return _instance ?? (_instance = new GameObject("AudioManager").AddComponent<AudioManager>());}
	}
	
	void Awake()
	{
		if(_instance == null)
			_instance = this;
		else
			if(this != _instance)
				Destroy(this.gameObject);
	}
	//=========================================================================================//
	#endregion

	public AudioSource BgmPlayer_layer1;
	public AudioSource BgmPlayer_layer2;
	public AudioSource BgmPlayer_layer3;
	public AudioSource PlayerSound;

	public AudioClip MathFull;
	public AudioClip StringFull;
	public AudioClip SynthFull;

	public AudioClip BGM_Layer1;
	public AudioClip BGM_Layer2;
	public AudioClip BGM_Layer3;

	public AudioClip AmmoPickUp;
	public AudioClip NormalBulletSound;
	public AudioClip ShotGunSound;
	public AudioClip FreezeBulletSound;
	public AudioClip LaunchSound;

	public float bgmVolume = 0.5f;

	void Start () 
	{

		BgmPlayer_layer1.loop = false;
		BgmPlayer_layer1.clip = StringFull;
		BgmPlayer_layer1.Play();
	}
	
	void Update () 
	{
		//finished current clip
		if(!BgmPlayer_layer1.isPlaying)
		{
			if(BgmPlayer_layer1.clip == StringFull)
			{
				BgmPlayer_layer1.clip = BGM_Layer1;
				BgmPlayer_layer2.clip = BGM_Layer2;
				BgmPlayer_layer3.clip = BGM_Layer3;

				BgmPlayer_layer1.loop = true;
				BgmPlayer_layer2.loop = true;
				BgmPlayer_layer3.loop = true;
//				BgmPlayer_layer1.Play();
				BgmPlayer_layer2.Play();
				BgmPlayer_layer3.Play();
			}
		}

		int enemyNum = GameManager.instance.NumOfAliveEnemy();

		if(enemyNum < 15)
		{
			BgmPlayer_layer1.volume = bgmVolume;
			BgmPlayer_layer2.volume = 0f;
			BgmPlayer_layer3.volume = 0f;
		}
		else if(enemyNum < 25)
		{
			BgmPlayer_layer1.volume = bgmVolume;
			BgmPlayer_layer2.volume = bgmVolume;
			BgmPlayer_layer3.volume = 0f;
		}
		else 
		{
			BgmPlayer_layer1.volume = bgmVolume;
			BgmPlayer_layer2.volume = bgmVolume;
			BgmPlayer_layer3.volume = bgmVolume;
		}
	}

	public void PlayBulletSound()
	{
		if(PlayerSound.clip != NormalBulletSound)
			PlayerSound.clip = NormalBulletSound;
		PlayerSound.Play();
	}

	public void PlayShotgunSound()
	{
		if(PlayerSound.clip != ShotGunSound)
			PlayerSound.clip = ShotGunSound;
		PlayerSound.Play();
	}

	public void PlayPickUpSound()
	{
		if(PlayerSound.clip != AmmoPickUp)
			PlayerSound.clip = AmmoPickUp;
		PlayerSound.Play();
	}

	public void PlayFreezeBulletSound()
	{
		if(PlayerSound.clip != FreezeBulletSound)
			PlayerSound.clip = FreezeBulletSound;
		PlayerSound.Play();
	}

	public void PlayLaunchSound()
	{
		if(PlayerSound.clip != LaunchSound)
			PlayerSound.clip = LaunchSound;
		PlayerSound.Play();
	}
}
