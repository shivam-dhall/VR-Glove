using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Key : MonoBehaviour {
    bool isClick = false;
    bool isActivate = false;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnTriggerEnter(Collider e)
    {
        if (isActivate&&!isClick)
        {
            isClick = true;
            StartCoroutine(Down());
        }
            
    }

    public void Activate()
    {
        isActivate = true;
        this.GetComponent<MeshRenderer>().enabled = true;
    }

    IEnumerator Down()
    {
        Vector3 v = this.transform.position;
        v.y -= 0.5f;
        this.transform.position = v;
        this.GetComponent<AudioSource>().Play();
        yield return new WaitForSeconds(0.3f);
        v.y += 0.5f;
        this.transform.position = v;
        isClick = false;
    }
}
