using UnityEngine;
using System.Collections;

public class Hand : MonoBehaviour {

    float ex, ey, ez;
    //用于记录计算结果  
    float qx, qy, qz, qw;
    float PIover180 = 0.0174532925f;//常量 

    public GameObject hand;
    public GameObject camera;
    public GameObject cube;
    int xSpeed = 10;
    int ySpeed = 10;
    int zSpeed = 10;
    int cnt = 0;
    int z_cnt = 1;

	// Use this for initialization
	void Start () {


	}

	
	// Update is called once per frame
	void Update () {
        //SetXYZRotation();
        //if (z_cnt % 30 == 0)
        {
            TestXYZRotation();
            z_cnt = 1;
        }
        ++z_cnt;
        //SetXYZRotation();
	}

    void TestXYZRotation()
    {
        Vector3 r = Index.getRotation(); //new Vector3(90, cnt, 0);
        cnt++;
        float angle = r.y;
        r.y = 0;
        //hand.transform.localEulerAngles = r;
        //Debug.Log("rotate to:" + r.x + "," + r.y + "," + r.z);
        ex = -r.x;
        ey = r.y;
        ez = -r.z;

        ex = ex * PIover180 / 2.0f;
        ey = ey * PIover180 / 2.0f;
        ez = ez * PIover180 / 2.0f;
        qx = Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) - Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);
        qy = Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Cos(ez) - Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Sin(ez);
        qz = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Sin(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Cos(ez);


        qw = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);

        Quaternion q = new Quaternion(qx, qy, qz, qw);

        //hand.transform.rotation = q;
        float hand_y = hand.transform.localEulerAngles.y;
        cube.transform.rotation = q;
        Vector3 cv = cube.transform.localEulerAngles;
        //Debug.Log("cube:" + cv.x + "," + cv.y + "," + cv.z + " ");
        cv.y = hand_y;
        if (angle != -361&&angle!=0)
        {
            float res = cv.y + (float)(-angle / 5);
            if (res > -45 && res < 45)
            {
                cv.y = res;
                Debug.Log(cnt + "-hand.y+" + (float)(angle / 5));
            }
            else
            {

            }
            
            
        }
            
        hand.transform.localEulerAngles = cv;
        //Vector3 v = hand.transform.localEulerAngles;
        //v.z = -v.z;
        //v.x = -v.x;
        //v.y = -v.y;
        //hand.transform.localEulerAngles = v;
    }

    //use the gyro to show the rotation of xyz axis
    void SetXYZRotation()
    {
        if (Index.IsReady())
        {
            Vector3 r =Index.getRotation();
            //hand.transform.localEulerAngles = r;
            //Debug.Log("rotate to:" + r.x + "," + r.y + "," + r.z);
            ex = r.x;
            ey = r.y;
            ez = r.z;

            ex = ex * PIover180 / 2.0f;
            ey = ey * PIover180 / 2.0f;
            ez = ez * PIover180 / 2.0f;
            qx = Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) - Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);
            qy = Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Cos(ez) - Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Sin(ez);
            qz = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Sin(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Cos(ez);


            qw = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);

            Quaternion q = new Quaternion(qx, qy, qz, qw);

            hand.transform.rotation = q;
            Vector3 v = hand.transform.localEulerAngles;
            v.z= -v.z;
            v.x = -v.x;
            v.y = -v.y;
            hand.transform.localEulerAngles = v;
        }

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
