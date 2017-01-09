using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using clinet;  

public class Index : MonoBehaviour {
    static string data = "";
    static bool isReady = false;
    private MyTcpIpClient client = null;

    void Awake()
    {
        
    }  

	// Use this for initialization
	void Start () {
        //Debug.Log("begin connect");
        client = new MyTcpIpClient();
        client.Connect();
        //Debug.Log("end connect");
	}
	
	// Update is called once per frame
	void Update () {
        //Debug.Log("begin receive data");
        client.ReceiveData();
        //Debug.Log("end receive data");
	}

    static public void SetData(byte[]a)
    {
        isReady = false;
        data = System.Text.Encoding.Default.GetString(a);
        //Debug.Log("---------");
        Debug.Log(data[0]+" "+data[data.Length-1]);
        isReady = true;
    }

    static public void print(string s)
    {
        Debug.Log(s);
    }

    static public void println(string s)
    {
        Debug.Log(s+'\n');
    }
}
