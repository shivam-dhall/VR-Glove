using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EventTypeCnt : MonoBehaviour {
    static int[] eventsCnt = new int[4] { 0, 0, 0, 0 };
    static bool[] isFirst = new bool[] { false, false,false,false};
    string[] s = new string[] { "energy:", "kick:", "snare:", "hithat:" };
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        for (int i = 1; i < 2;++i )
            if (isFirst[i])
                Debug.Log(s[i] + eventsCnt[i]);
	}

    static public int GetCnt(int type)
    {
        return eventsCnt[type];
    }

    static public bool isEqualAndFirst(int type,int i)
    {
        if (isFirst[type] && eventsCnt[type] == i)
            return true;
        return false;
    }

    static public bool isBetweenAndFirst(int type, int i1,int i2)
    {
        if (isFirst[type] && eventsCnt[type] >= i1&&eventsCnt[type]<=i2)
            return true;
        return false;
    }

    static public void AddEventType(int i)
    {
            eventsCnt[i]++;
            isFirst[i] = true;

    }

    static public void SetFirst(int type,bool b)
    {
        isFirst[type] = b;
    } 
}
