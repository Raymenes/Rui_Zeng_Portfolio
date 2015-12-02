using UnityEngine;
using System.Collections;

public class ScoreText : MonoBehaviour {

    // Use this for initialization
    float destroyTime = 5.0f;
    int speed = 90;
    void Start()
    {
        StartCoroutine(FloatScore());
    }

    // Update is called once per frame
    void Update () {
        float y = Time.deltaTime * speed;
        transform.Translate(0, y, 0);
	}

    IEnumerator FloatScore()
    {
        yield return new WaitForSeconds(destroyTime);
        Destroy(gameObject);
    }
}
    