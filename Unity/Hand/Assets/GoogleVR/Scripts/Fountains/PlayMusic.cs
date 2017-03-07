using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayMusic : MonoBehaviour {
    bool isStart = false;
    int selfState = 0;
    int starCnt = 0;
    bool isStarFinish = false;
    bool isStarBegin = false;
    GameObject hand;
	// Use this for initialization
	void Start () {
        hand = GameObject.Find("HandAgent");
	}

    public void Init()
    {
        isStart = true;
    }

    public void End()
    {
        isStart = false;
    }
	
	// Update is called once per frame
	void Update () {
        if (isStart)
        {
            if (Input.GetKeyDown(KeyCode.Alpha8))
            {
                isStarBegin = true;
            }

            if (isStarBegin && !isStarFinish)
            {
                int inter = 35;
                if (starCnt < inter)
                {
                    Vector3 v = new Vector3(0.2f, 0, 0);
                    hand.transform.localPosition += v;
                }
                else if (starCnt < inter * 2)
                {
                    Vector3 v = new Vector3(-0.18f, -0.18f, 0);
                    hand.transform.localPosition += v;
                }
                else if (starCnt < inter * 3)
                {
                    Vector3 v = new Vector3(0.1f, 0.21f, 0);
                    hand.transform.localPosition += v;
                }
                else if (starCnt < inter * 4)
                {
                    Vector3 v = new Vector3(0.1f, -0.21f, 0);
                    hand.transform.localPosition += v;
                }
                else if (starCnt < inter * 5)
                {
                    Vector3 v = new Vector3(-0.18f, 0.18f, 0);
                    hand.transform.localPosition += v;
                }
                else
                {
                    isStarFinish = true;
                    GameObject.Find("ClickTable").GetComponent<ClickTable>().Activate();
                }

                ++starCnt;
            }
        }
	}
}
