using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using clinet;  

public class Index : MonoBehaviour {
    static string data = "";
    static bool isReady = false;
    private MyTcpIpClient client = null;
    static private int[] dataArray = new int[22];
    static private Vector3 rotation = new Vector3(0, 0, 0);

    void Awake()
    {
        
    }  

	// Use this for initialization
	void Start () {
        client = new MyTcpIpClient();
        client.Connect();
	}
	
	// Update is called once per frame
	void Update () {
        client.ReceiveData();
	}

    static public void SetData(byte[]a)
    {
        isReady = false;
        string s = "";
        for (int i = 0; i < 22; ++i)
        {
            dataArray[i] = (int)((a[i*4] & 0xFF << 24)
                                | ((a[i*4+1] & 0xFF) << 16)
                                | ((a[i*4+2] & 0xFF) << 8)
                                | ((a[i*4+3] & 0xFF)));
            s += (dataArray[i] + " ");
        }
        data = System.Text.Encoding.Default.GetString(a);
        Debug.Log("data" + s);
        //Debug.Log(data[0]+" "+data[data.Length-1]);
        isReady = true;
    }

    static public void print(string s)
    {
        Debug.Log(s);
    }

    static public bool IsReady()
    {
        return isReady;
    }

    static public Vector3 getRotation()
    {
        return new Vector3(dataArray[19], dataArray[20], dataArray[21]);
    }

}
