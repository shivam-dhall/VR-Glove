using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PopFountain : MonoBehaviour {
    GameObject[] pops = new GameObject[9];
    bool isStart = false;
	// Use this for initialization
	void Start () {
        int i = 0;
        foreach (Transform child in this.transform)
        {
            pops[i++] = child.gameObject;
        }
        
	}


	
	// Update is called once per frame
	void Update () {
        if (isStart)
        {
            if (EventTypeCnt.isEqualAndFirst(0, 1))
                StartCoroutine(PlayFirstFountain());
            if (EventTypeCnt.isEqualAndFirst(0, 5))
                StartCoroutine(PlayPopFountains(new int[] { 6, 8 }, 2, 4));
            if (EventTypeCnt.isEqualAndFirst(0, 6))
                StartCoroutine(PlayPopFountains(new int[] { 0, 2 }, 2, 2));
        }
	}

    public void Init()
    {
        isStart = true;
    }

    public void End()
    {
        isStart = false;
        for (int i = 0; i < pops.Length; ++i)
            StopPopFountain(i);
    }

    private IEnumerator PlayFirstFountain()
    {
        PlayPopFountain(4);
        yield return new WaitForSeconds(6);
        StopPopFountain(4);
    }


    private IEnumerator PlayPopFountains(int[] arr, int size, float delay)
    {
        for (int i = 0; i < size; ++i)
            PlayPopFountain(arr[i]);
        yield return new WaitForSeconds(delay);
        for (int i = 0; i < size; ++i)
            StopPopFountain(arr[i]);
    }

    private void PlayPopFountain(int i)
    {
        StopPopFountain(i);
        pops[i].GetComponent<ParticleSystem>().Play();

    }

    private void StopPopFountain(int i)
    {
        if (pops[i].GetComponent<ParticleSystem>().isPlaying)
            pops[i].GetComponent<ParticleSystem>().Stop();
    }
}
