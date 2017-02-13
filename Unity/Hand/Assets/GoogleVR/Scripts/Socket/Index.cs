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
    static private Vector3 shifting = new Vector3(0, 0, 0);
    static private bool isInit = false;

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
        //isReady = false;
        string s = "";
        byte[] temp = new byte[4];
        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 4;++j )
                temp[j] = a[i * 4 + (3-j)];
            dataArray[i] = System.BitConverter.ToInt32(temp, 0);
            s += (dataArray[i] + " ");
           
        }
        data = System.Text.Encoding.Default.GetString(a);
        //Debug.Log("data:" + s);
        //Debug.Log(data[0]+" "+data[data.Length-1]);
        if (!isInit)
        {
            Hand.SetReferRotation(new Vector3(dataArray[3]/100,dataArray[5]/100,dataArray[4]/100));
            //Debug.Log("refer:" + referRotation.x + "," + referRotation.y + "," + referRotation.z);
            isInit = true;
        }
        shifting.x = dataArray[0] / 100;
        shifting.y = dataArray[2] / 100;
        shifting.z = dataArray[1] / 100;

        rotation.x = dataArray[3]/100;
        rotation.y = dataArray[5]/100;
        //rotation.y = dataArray[21];
        rotation.z = dataArray[4]/100;
        //rotation += referRotation;
        isReady = true;
        Debug.Log("end");
    }

    static public bool getIsInit()
    {
        return isInit;
    }

    static public void setIsInit(bool i)
    {
        isInit = i;
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
        //isReady = false;
        return rotation;
    }

    static public Vector3 getShifting()
    {
        return shifting;
    }

}
