using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Catch : MonoBehaviour {

    public GameObject hand;
    Vector3 posDiff;
    bool isCatch = false;
    int isntcatchCnt = 0;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        if (Hand.IsCatch())
        {
            if (!isCatch)
            {
                this.transform.parent = hand.transform;
                //this.GetComponent<SphereCollider>().enabled = false;
                this.GetComponent<Rigidbody>().useGravity = false;
                this.transform.localPosition = new Vector3(0, -2.0f, 3.3f);
                isCatch = true;
            }
        }
        else
        {
            ++isntcatchCnt;
        }

        if (isntcatchCnt > 1)
        {
            this.transform.parent = null;
            this.GetComponent<Rigidbody>().useGravity = true;
            isCatch = false;
            this.GetComponent<SphereCollider>().enabled = true;
            isntcatchCnt = 0;
        }
            
	}
}
