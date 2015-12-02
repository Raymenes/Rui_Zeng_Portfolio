using UnityEngine;
using System.Collections;

public class ImageDisplay : MonoBehaviour
{
    public Texture[] tex; // Holds all the splashscreens
    public float faderSpeed;// sets the fading speed
    private float fader;    // holds the actual fade value
    private Texture splash; // holds the currently displayed splash.
    public int nextLevelToLoad; // holds the next level in build to load

    IEnumerator Start()
    {
        for (int i = 0; i < tex.Length; i++)
        {
            yield return StartCoroutine(FadeSplash(tex[i]));
        }
        splash = null;
        Application.LoadLevel(nextLevelToLoad);
    }

    IEnumerator FadeSplash(Texture pSplash)
    {
        // Set to initial state and assign image
        fader = 0;
        splash = pSplash;

        // Fade in
        while (fader < 1)
        {
            yield return null;
            fader += Time.deltaTime * faderSpeed;
        }
        fader = 1;

        // Keep displayed for # seconds
        yield return new WaitForSeconds(2.0f);

        // Fade out
        while (fader > 0)
        {
            yield return null;
            fader -= Time.deltaTime * faderSpeed;
        }
        fader = 0;
    }

    void OnGUI()
    {
        if (splash != null)
        {
            GUI.color = new Color(1, 1, 1, fader);
            GUI.DrawTexture(new Rect(0, 0, Screen.width, Screen.height), splash);
            GUI.color = new Color(1, 1, 1, 1);
        }
    }
}
