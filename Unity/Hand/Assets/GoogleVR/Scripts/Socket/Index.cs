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
        data = System.Text.Encoding.Default.GetString(a);
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
