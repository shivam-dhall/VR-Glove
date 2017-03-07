using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SideFountain : MonoBehaviour {
    GameObject[] sides = new GameObject[40];

    bool isStart = false;
    int selfState = 0;
    
	// Use this for initialization
	void Start () {
        GameObject origin = GameObject.Find("side");
        sides[0] = origin;
        sides[1] = Instantiate(origin);
        Vector3 v = sides[0].transform.position;
        v.x = -v.x;
        sides[1].transform.position = v;
        for (int i = 2; i < 40; ++i)
        {
            sides[i] = Instantiate(sides[i - 2]);
            Vector3 vv = sides[i].transform.position;
            vv.z += 15;
            sides[i].transform.position = vv;
        }
	}
	
	// Update is called once per frame
	void Update () {
        if (isStart)
        {
            if (Input.GetKeyDown(KeyCode.L))
            {
                StartCoroutine(PlaySideFountains());
                ++selfState;
            }
        }
	}

    public void Init()
    {
        isStart = true;
    }

    public void End()
    {
        isStart = false;
    }

    private IEnumerator PlaySideFountains()
    {
        for (int i = 0; i < 20; ++i)
        {
            PlaySideFountain(i*2);
            PlaySideFountain(i * 2+1);
            yield return new WaitForSeconds(0.05f);
        }
            
    }

    private void PlaySideFountain(int i)
    {
        StopSideFountain(i);
        sides[i].GetComponent<ParticleSystem>().Play();

    }

    private void StopSideFountain(int i)
    {
        if (sides[i].GetComponent<ParticleSystem>().isPlaying)
            sides[i].GetComponent<ParticleSystem>().Stop();
    }

}
