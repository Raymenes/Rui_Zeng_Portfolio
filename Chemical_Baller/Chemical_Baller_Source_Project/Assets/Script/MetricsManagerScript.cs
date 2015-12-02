using UnityEngine;
using System.Collections;
using System.IO;

public class MetricsManagerScript : MonoBehaviour
{

    string createText = "";

    public int launchNum = 0, jumpNum = 0, bulletNum = 0, bombNum = 0, freezeNum = 0, shotgunNum = 0, totalNum = 0;

    void Start() { }
    void Update() { totalNum = bulletNum + bombNum + freezeNum; }

    //When the game quits we'll actually write the file.
    void OnApplicationQuit()
    {
        GenerateMetricsString();
        string time = System.DateTime.UtcNow.ToString(); string dateTime = System.DateTime.Now.ToString(); //Get the time to tack on to the file name
        print("before: " + time);
        time = time.Replace("/", "-"); //Replace slashes with dashes, because Unity thinks they are directories..
        time = time.Replace(":", "-");
        string reportFile = "GameName_Metrics_" + time + ".txt";
        File.WriteAllText(reportFile, createText);
        //In Editor, this will show up in the project folder root (with Library, Assets, etc.)
        //In Standalone, this will show up in the same directory as your executable
    }

    void GenerateMetricsString()
    {
        createText =
            "Number of times player launched: " + launchNum + '\n' +
            "Number of times player jumped:  " + jumpNum + '\n' +
            "Number of times bullet used: " + bulletNum + '\n' +
            "Number of times bombs used: " + bombNum + '\n' +
            "Number of times freeze used: " + freezeNum + '\n' +
            "Total number of bullets used: " + totalNum + '\n';


    }

    //Add to your set metrics from other classes whenever you want
    public void AddToLaunch(int amtToAdd)
    {
        launchNum += amtToAdd;
    }
    public void AddToJump(int amtToAdd)
    {
        jumpNum += amtToAdd;
    }
    public void AddToBullet(int amtToAdd)
    {
        bulletNum += amtToAdd;
    }
    public void AddToBomb(int amtToAdd)
    {
        bombNum += amtToAdd;
    }
    public void AddToFreeze(int amtToAdd)
    {
        freezeNum += amtToAdd;
    }
	public void AddToShotgun(int amtToAdd)
	{
		shotgunNum += amtToAdd;
	}
}
