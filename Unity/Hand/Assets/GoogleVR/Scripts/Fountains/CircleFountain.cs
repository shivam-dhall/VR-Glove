using UnityEngine;
using System.Collections;

public class CircleFountain : MonoBehaviour
{

    private Component[] fountains;

    private int direction;

    private float maxTime = 4f;
    private float curTime;

    bool isStart = false;
    int selfState = 0;

    private Vector3[] rotate = new Vector3[] {
		Vector3.left, Vector3.right, Vector3.right, Vector3.left,
		Vector3.up, Vector3.down, Vector3.down, Vector3.up};

    // Use this for initialization
    void Start()
    {
        fountains = GetComponentsInChildren<ParticleSystem>();
        Debug.Log(fountains.Length);
        direction = 0;
        curTime = 0;
    }

    public void Init()
    {
        isStart = true;
        Component[] children = GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem childParticleSystem in children)
        {
            if (!childParticleSystem.isPlaying)
                childParticleSystem.Play();
        }
    }

    public void End()
    {
        isStart = false;
        Component[] children = GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem childParticleSystem in children)
        {
            if (childParticleSystem.isPlaying)
                childParticleSystem.Stop();
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (isStart)
        {
            foreach (ParticleSystem child in fountains)
            {
                child.transform.Rotate(rotate[direction] * 6f * Time.deltaTime);

                if (child.startSpeed < 50f + 55f)
                {
                    child.startSpeed += 3f * Time.deltaTime;
                    child.startLifetime += 0.2f * Time.deltaTime;
                }
            }

            curTime += Time.deltaTime;
            if (curTime > maxTime)
            {
                direction = (direction + 1) % 8;
                curTime = 0;
            }
            
        }


    }


}
