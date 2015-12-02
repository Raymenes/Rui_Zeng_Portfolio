using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class MainMenu : MonoBehaviour {

//    public Button startButton;
//    public Button quitButton;

	bool isInstructionOn = false;
	public GameObject instruction_img1;
	public GameObject instruction_img2;
	public GameObject Credit_img;
	public GameObject StartText;
	public GameObject QuitText;
	public GameObject CreditText;
	public GameObject Logo;
	public GameObject How_To_Play_Text;

    void Start()
    {
		instruction_img1 = GameObject.Find("Instruction_img");
		instruction_img2 = GameObject.Find("Instruction_img2");
		StartText = GameObject.Find("StartText");
		QuitText = GameObject.Find("QuitText");
		CreditText = GameObject.Find("CreditText");
		Credit_img = GameObject.Find("Credit_img");
		How_To_Play_Text = GameObject.Find("How_To_Play");
		Logo = GameObject.Find("Logo");

		instruction_img1.SetActive(isInstructionOn);
		instruction_img2.SetActive(isInstructionOn);
		Credit_img.SetActive(false);
    }

    public void StartLevel()
    {
        Application.LoadLevel(2);
    }

    public void QuitGame()
    {
        Application.Quit();
    }

	public void ToggleInstruction()
	{
		isInstructionOn = !isInstructionOn;
		instruction_img1.SetActive(isInstructionOn);
		instruction_img2.SetActive(isInstructionOn);
		StartText.SetActive(!isInstructionOn);
		QuitText.SetActive(!isInstructionOn);
		CreditText.SetActive(!isInstructionOn);
	}

	public void ShowCredit()
	{
		instruction_img1.SetActive(false);
		instruction_img2.SetActive(false);
		StartText.SetActive(false);
		QuitText.SetActive(false);
		CreditText.SetActive(false);
		How_To_Play_Text.SetActive(false);
		Credit_img.SetActive(true);
		Logo.SetActive(false);
	}

	public void BackToMainMenu()
	{
		instruction_img1.SetActive(false);
		instruction_img2.SetActive(false);
		Credit_img.SetActive(false);

		Logo.SetActive(true);
		StartText.SetActive(true);
		QuitText.SetActive(true);
		How_To_Play_Text.SetActive(true);
		CreditText.SetActive(true);
	}


}
