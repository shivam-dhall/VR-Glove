using UnityEngine;
using System.Collections;

public class Hand : MonoBehaviour {

    public GameObject hand;
    int xSpeed = 10;
    int ySpeed = 10;
    int zSpeed = 10;

	// Use this for initialization
	void Start () {
	    
	}
	
	// Update is called once per frame
	void Update () {
	
	}

    //use the gyro to show the rotation of xyz axis
    void SetXYZRotation(int x, int y, int z)
    {
        if(Index.IsReady())
            hand.transform.rotation = Quaternion.Slerp(hand.transform.)

    }

    void PlayAnimation()
    {

    }

    void GetKeyEvent()
    {
        if (Input.GetKey(KeyCode.Alpha8))//up
        {

        }
        else if (Input.GetKey(KeyCode.Alpha2))//down
        {

        }
        else if (Input.GetKey(KeyCode.Alpha4))//left
        {

        }
        else if (Input.GetKey(KeyCode.Alpha6))//right
        {

        }
        else if (Input.GetKey(KeyCode.Alpha7))//forward
        {

        }
        else if (Input.GetKey(KeyCode.Alpha9))//back
        {

        }
    }
}
