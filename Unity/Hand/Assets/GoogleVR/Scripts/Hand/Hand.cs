using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Hand : MonoBehaviour {
    public enum ShiftingType{
        X,
        Y,
        Z
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
    static private Vector3 referRotation = new Vector3(0, 0, 0);
    bool isInit = false;
    float lastCameraY = 0.0f;
    float lastRcvY = 0.0f;
    static float scale = 0.01f;

	// Use this for initialization
	void Start () {


	}

	
	// Update is called once per frame
	void Update () {

        SetXYZRotation();
        SetXYZPosition();
        
        //float x = (float)(hand.transform.forward.x * scale);
        //Vector3 v = new Vector3((float)(hand.transform.forward.x * scale),
        //    (float)(hand.transform.forward.y * scale), (float)(hand.transform.forward.z * scale));
        //hand.transform.position += v;

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

    public void SetXYZPosition()
    {
        if (Input.GetKey(KeyCode.Q))//<- X
        {
            
        }
        else if (Input.GetKey(KeyCode.W))//X->
        {
            
        }
        else if (Input.GetKey(KeyCode.A))//<- Y
        {
            
        }
        else if (Input.GetKey(KeyCode.S))//Y->
        {
            
        }
        else if (Input.GetKey(KeyCode.Z))//<- Z
        {
            
        }
        else if (Input.GetKey(KeyCode.X))//Z->
        {
            
        }
            

        if (Index.IsReady())
        {
            Vector3 s = Index.getShifting();
            //hand.transform.position += getShiftVec(s.x, ShiftingType.X);
            Index.ResetShifting();
            //hand.transform.localPosition;
            //hand.transform.position += getShiftVec(s.y, ShiftingType.Y);
            //hand.transform.position += getShiftVec(s.z, ShiftingType.Z);
        }
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

    Vector3 getShiftVec(float distance, ShiftingType type)
    {
        Vector3 v = new Vector3(0, 0, 0);
        switch(type){
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
