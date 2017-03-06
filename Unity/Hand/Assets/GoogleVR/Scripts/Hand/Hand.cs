using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Hand : MonoBehaviour {
    public enum ShiftingType{
        X=0,Y,Z
    }

    float ex, ey, ez;
    //用于记录计算结果  
    float qx, qy, qz, qw;
    float PIover180 = 0.0174532925f;//常量 

    public GameObject hand;
    public GameObject camera;
    public GameObject cube;
    public GameObject text;
    int xSpeed = 10;
    int ySpeed = 10;
    int zSpeed = 10;
    int cnt = 0;
    int z_cnt = 1;
    static private Vector3 referRotation = new Vector3(0, 180, 0);
    bool isInit = false;
    float lastCameraY = 0.0f;
    float lastRcvY = 0.0f;
    static float scale = 0.1f;
    Vector3 initPosition = new Vector3(0.509f, -0.37f, 0.8f);
    Vector3 negPisition = new Vector3(-0.5f,-0.37f,0.8f);
    Vector3 evenPostion = new Vector3(0.509f,0.4f,1.2f);
    float dist = 5.0f;
    static int[] fingerStates = new int[5] { 0, 0, 0, 0, 0 };
    static Vector3 handPos;

    int xCnt = 0;
    int zCnt = 0;

	// Use this for initialization
	void Start () {
        
    }
	
	// Update is called once per frame
	void Update () {
        SetXYZRotation();
        SetXYZPosition();
        handPos = hand.transform.position;
        //float z = hand.transform.localEulerAngles.z;
        //if(z>180)
        //    z = 360-z;
        //if ( z> 50)
        //    --xCnt;
        //else if (z < -50)
        //    ++xCnt;
        //else
        //    xCnt = 0;

        //float x = hand.transform.localEulerAngles.x;
        //if (x > 180)
        //    x = 360 - x;
        //if (x > 50)
        //    ++zCnt;
        //else if (x < -50)
        //    --zCnt;
        //else
        //    zCnt = 0;

        //Debug.Log("xCnt:" + xCnt + " " + "hand.z:" + z);

        //if (Mathf.Abs(xCnt) > 20)
        //{
        //    Vector3 v = hand.transform.right;//getShiftVec(dist * xCnt / (Mathf.Abs(xCnt)), ShiftingType.X);
        //    v.x *= (scale * dist*xCnt / (Mathf.Abs(xCnt)));
        //    hand.transform.localPosition += v;
        //}

        //if (Mathf.Abs(zCnt) > 20)
        //{
        //    Vector3 v = hand.transform.forward;// getShiftVec(dist * zCnt / (Mathf.Abs(zCnt)), ShiftingType.Z);
        //    v.z *= (scale * dist * zCnt / (Mathf.Abs(zCnt)));
        //    hand.transform.position+=v;
        //}


	}

    public void SetXYZPosition()
    {
        if (Index.IsReady())
        {
            Vector3 s = Index.getShifting();
            Vector3 tempPos = hand.transform.localPosition;
            Vector3 add = new Vector3(0, 0, 0);

            for (int i = 0; i < 3; ++i)
            {
                s[i] *= scale;
                if (tempPos[i] + s[i] < negPisition[i]){
                    if (s[i] > 0)
                        add[i] = s[i];
                    else
                        add[i] = negPisition[i] - tempPos[i];
                }
                    
                else if (tempPos[i] + s[i] > evenPostion[i]){
                    if (s[i] < 0)
                        add[i] = s[i];
                    else
                        add[i] = evenPostion[i] - tempPos[i];
                }
                else
                    add[i] = s[i];
            }

            //hand.transform.localPosition += add;
            //Debug.Log("add:x "+add.x);

                //Vector3 tempPos = hand.transform.localPosition;
                //Vector3 add = new Vector3(0, 0, 0);
                //for (int i = 0; i < 1; ++i)
                //{

                //}
                //Vector3 add = getShiftVec(s[(int)type], type);
                //hand.transform.localPosition += add;
                //hand.transform.localPosition += getShiftVec(s.x, ShiftingType.X);
                Index.ResetShifting();
            
        }

        if (Input.GetKey(KeyCode.Q))//<- X
        {
            hand.transform.position += getShiftVec(-1, ShiftingType.X);
        }
        else if (Input.GetKey(KeyCode.W))//X->
        {
            hand.transform.position += getShiftVec(1, ShiftingType.X);
        }
        else if (Input.GetKey(KeyCode.A))//<- Y
        {
            hand.transform.position += getShiftVec(-1, ShiftingType.Y);
        }
        else if (Input.GetKey(KeyCode.S))//Y->
        {
            hand.transform.position += getShiftVec(1, ShiftingType.Y);
        }
        else if (Input.GetKey(KeyCode.Z))//<- Z
        {
            hand.transform.position += getShiftVec(-1, ShiftingType.Z);
        }
        else if (Input.GetKey(KeyCode.X))//Z->
        {
            hand.transform.position += getShiftVec(1, ShiftingType.Z);
        }
        else
        {

        }
    }



    Vector3 getShiftVec(float distance, ShiftingType type)
    {
        Vector3 v = new Vector3(0, 0, 0);
        switch (type)
        {
            case ShiftingType.X:
                v = hand.transform.right;
                break;
            case ShiftingType.Y:
                v = hand.transform.up;
                break;
            case ShiftingType.Z:
                v = hand.transform.forward;
                break;
        }

        v.x *= (scale * distance);
        v.y *= (scale * distance);
        v.z *= (scale * distance);
        Debug.Log("shiftVec:" + v.x);//+ " " + v.y + " " + v.z
        return v;
    }


    //use the gyro to show the rotation of xyz axis
    void SetXYZRotation()
    {
        float angle = camera.transform.localEulerAngles.y - lastCameraY;
        if (Index.IsReady())
        {
            Vector3 r =Index.getRotation();
            Vector3 rr = getReferRotation();
            if (isInit)
            {
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
                cube.transform.rotation = q;

                Vector3 v = cube.transform.localEulerAngles;
                float change = v.y - lastRcvY;
                //v += rr;
                //float temp = v.z;
                //v.z = v.x;
                //v.x = -temp;
                //v.y = -v.y;
                float yyy = camera.transform.localEulerAngles.y;
                //Debug.Log("v.y:" + v.y + " camera.y:" + yyy + " after v.y:" + (camera.transform.localEulerAngles.y + v.y));
                ////Vector3 cameraVec = camera.transform.localEulerAngles;
                ////v.y += (camera.transform.localEulerAngles.y);

                ////if(System.Math.Abs(yyy)>1)
                ////    text.GetComponent<Text>().text = 
                ////    ("v.y:" + (v.y + 360) % 360 + "\ncam.y:" + (yyy + 360) % 360)+"\nchu:"+(v.y/yyy)+" jian:"+(v.y-yyy);

                float minus = yyy - lastCameraY;
                if (System.Math.Abs(minus) > 0.1)
                {
                    //float change = hand.transform.localEulerAngles.y - (getReferRotation().y + v.y);
                    //referRotation.y += change;
                    //Debug.Log("not change:" + lastRcvY + " " + v.y + " " + change);
                    
                }
                else
                {
                    //Debug.Log("change:" + lastRcvY + " " + v.y + " " + change);
                    Vector3 t_v = new Vector3(-v.z, hand.transform.localEulerAngles.y - change, v.x);
                    hand.transform.localEulerAngles = t_v;

                }
                lastRcvY = v.y;
                
                
            }
        }
        
        lastCameraY = camera.transform.localEulerAngles.y;

    }

    Vector3 getReferRotation()
    {
        if (Index.getIsInit()&&!isInit)
        {
            isInit = true;
            ex = referRotation.x;
            ey = referRotation.y;
            ez = referRotation.z;

            ex = ex * PIover180 / 2.0f;
            ey = ey * PIover180 / 2.0f;
            ez = ez * PIover180 / 2.0f;
            qx = Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) - Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);
            qy = Mathf.Cos(ex) * Mathf.Sin(ey) * Mathf.Cos(ez) - Mathf.Sin(ex) * Mathf.Cos(ey) * Mathf.Sin(ez);
            qz = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Sin(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Cos(ez);
            qw = Mathf.Cos(ex) * Mathf.Cos(ey) * Mathf.Cos(ez) + Mathf.Sin(ex) * Mathf.Sin(ey) * Mathf.Sin(ez);

            Quaternion q = new Quaternion(qx, qy, qz, qw);
            cube.transform.rotation = q;
            Vector3 v = cube.transform.localEulerAngles;

            lastRcvY = v.y;
            //Debug.Log("first:" + lastRcvY);

            referRotation.x = -v.x;
            referRotation.y = -v.y;
            referRotation.z = -v.z;
        }
        return referRotation;
    }

    static public void SetReferRotation(Vector3 v)
    {
        referRotation = v;
    }

    static public void SetFingerState(int i, int v)
    {
        fingerStates[i] = v;
    }

    static public bool IsCatch()
    {
        int sum = 0;
        for(int i=0;i<5;++i)
            sum+=fingerStates[i];
        return sum == 5;
    }

    static public Vector3 GetHandPos()
    {
        return handPos;
    }

    static public void PlayStar()
    {

    }



}
