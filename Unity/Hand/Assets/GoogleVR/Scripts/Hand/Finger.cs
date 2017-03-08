using UnityEngine;
using System.Collections;

public class Finger : MonoBehaviour {

    public enum FingerType
    {
        FINGER_THUMB =0,
        FINGER_INDEX,
        FINGER_MIDDLE,
        FINGER_RING,
        FINGER_LITTLE
    }

    const int vectorNum = 7;
    public FingerType type;
    GameObject[] rotateCenters = new GameObject[3];//three rotated joint
    GameObject[] fingerParts = new GameObject[3];//three parts of a finger
    GameObject refer;
    Vector3[] rotateVectors = new Vector3[vectorNum];//three vectors of three rotated joint
    float[] maxRotateAngles = {90.0f,120.0f,60.0f};//three rotated joints' max rotated angle
    const int angleTypeNums = 6;//seven states of bend intensity
    int[] bendState = {0,0,0};//record current bend state
    const float interval = 0.05f;//the glove's frequency of detect one change,50 ms
    float[] angleRecords = { 0.0f, 0.0f, 0.0f };
    //some vars that playing animation need
    int[] toBendState = { 0, 0, 0 };
    int[] animation_times = { 0, 0, 0 };

    int fingerState = 0;//1->bend
    int lastKey = -1;

	// Use this for initialization
	void Start () {
        switch (type)
        {
            case FingerType.FINGER_THUMB:
                rotateCenters[0] = GameObject.Find("wrist");
                fingerParts[0] = GameObject.Find("thumb");
                maxRotateAngles[0] = 30.0f;
                maxRotateAngles[1] = 40.0f;
                maxRotateAngles[2] = 90.0f;
                fingerParts[1] = fingerParts[0].transform.Find("part1").gameObject;
                fingerParts[2] = fingerParts[1].transform.Find("part2").gameObject;
                rotateCenters[1] = fingerParts[0].transform.Find("joint1").gameObject;
                rotateCenters[2] = fingerParts[1].transform.Find("joint2").gameObject;
                break;
            case FingerType.FINGER_INDEX:
                rotateCenters[0] = GameObject.Find("indexJoint");
                fingerParts[0] = GameObject.Find("index");
                fingerParts[1] = fingerParts[0].transform.Find("part1").gameObject;
                fingerParts[2] = fingerParts[1].transform.Find("part2").gameObject;
                rotateCenters[1] = fingerParts[0].transform.Find("joint1").gameObject;
                rotateCenters[2] = fingerParts[1].transform.Find("joint2").gameObject;
                break;
            case FingerType.FINGER_MIDDLE:
                rotateCenters[0] = GameObject.Find("middleJoint");
                fingerParts[0] = GameObject.Find("middle");
                fingerParts[1] = fingerParts[0].transform.Find("part1").gameObject;
                fingerParts[2] = fingerParts[1].transform.Find("part2").gameObject;
                rotateCenters[1] = fingerParts[0].transform.Find("joint1").gameObject;
                rotateCenters[2] = fingerParts[1].transform.Find("joint2").gameObject;              
                break;
            case FingerType.FINGER_RING:
                rotateCenters[0] = GameObject.Find("ringJoint");
                fingerParts[0] = GameObject.Find("ring");
                fingerParts[1] = fingerParts[0].transform.Find("part1").gameObject;
                fingerParts[2] = fingerParts[1].transform.Find("part2").gameObject;
                rotateCenters[1] = fingerParts[0].transform.Find("joint1").gameObject;
                rotateCenters[2] = fingerParts[1].transform.Find("joint2").gameObject;  
                break;
            case FingerType.FINGER_LITTLE:
                rotateCenters[0] = GameObject.Find("littleJoint");
                fingerParts[0] = GameObject.Find("little");
                fingerParts[1] = fingerParts[0].transform.Find("part1").gameObject;
                fingerParts[2] = fingerParts[1].transform.Find("part2").gameObject;
                rotateCenters[1] = fingerParts[0].transform.Find("joint1").gameObject;
                rotateCenters[2] = fingerParts[1].transform.Find("joint2").gameObject;
                break;
        }
	}
	
	// Update is called once per frame
	void Update () {
        //if(type == FingerType.FINGER_THUMB)
        GetKeyEvent();
        if (fingerState == 0)
            PlayAnimation(1, 0, 5);
        else
            PlayAnimation(1, 6, 5);


        //int state = Index.getFingerState((int)type);
        //if (fingerState != state)
        //{
        //    Hand.SetFingerState((int)type, state);
        //    fingerState = state;
        //    if (fingerState == 0){
        //        PlayAnimation(1, 0, 5);
        //    }

        //    else
        //    {
        //        PlayAnimation(1, 6, 5);
        //    }
               
        //}



        for (int i = 0; i < 3; ++i)
        {
            //everytime we change the part's position, we need to update the rotateVector
            switch (type)
            {
                case FingerType.FINGER_THUMB:
                    rotateVectors[0] = rotateCenters[0].transform.forward;
                    rotateVectors[1] = rotateCenters[1].transform.right;
                    rotateVectors[2] = rotateCenters[2].transform.right;
                    break;
                case FingerType.FINGER_INDEX:
                    rotateVectors[0] = rotateCenters[0].transform.forward + 0.2f * rotateCenters[0].transform.right;
                    rotateVectors[1] = rotateCenters[1].transform.forward +0.3f * rotateCenters[1].transform.right;
                    rotateVectors[2] = rotateCenters[2].transform.forward +0.3f * rotateCenters[2].transform.right;
                    break;
                case FingerType.FINGER_MIDDLE:
                    rotateVectors[0] = rotateCenters[0].transform.forward;
                    rotateVectors[1] = rotateCenters[1].transform.forward ;
                    rotateVectors[2] = rotateCenters[2].transform.forward ;
                    break;
                case FingerType.FINGER_RING:
                    rotateVectors[0] = rotateCenters[0].transform.forward;
                    rotateVectors[1] = rotateCenters[1].transform.forward - 0.15f * rotateCenters[1].transform.right;
                    rotateVectors[2] = rotateCenters[2].transform.forward - 0.13f * rotateCenters[2].transform.right;
                    break;

                case FingerType.FINGER_LITTLE:
                    rotateVectors[0] = rotateCenters[0].transform.forward - 0.2f * rotateCenters[0].transform.right;
                    rotateVectors[1] = rotateCenters[1].transform.forward-0.5f * rotateCenters[1].transform.right;
                    rotateVectors[2] = rotateCenters[2].transform.forward - 0.7f * rotateCenters[2].transform.right;
                    break;
            }
            if (toBendState[i] > bendState[i])
            {
                float per = Time.deltaTime / (animation_times[i] * 0.05f);
                float angle = per * (toBendState[i] - bendState[i]) * maxRotateAngles[i] / angleTypeNums;
                if (angleRecords[i] + angle >= (toBendState[i] * maxRotateAngles[i] / angleTypeNums))
                {
                    bendState[i] = toBendState[i];
                    angle = (toBendState[i] * maxRotateAngles[i] / angleTypeNums) - angleRecords[i];
                    angleRecords[i] = (toBendState[i] * maxRotateAngles[i] / angleTypeNums);
                }
                else
                    angleRecords[i] += angle;

                fingerParts[i].transform.RotateAround(rotateCenters[i].transform.position, rotateVectors[i], angle);
            }
            else if (toBendState[i] < bendState[i])
            {
                float per = Time.deltaTime / (animation_times[i] * 0.05f);
                float angle = per * (toBendState[i] - bendState[i]) * maxRotateAngles[i] / angleTypeNums;
                if (angleRecords[i] + angle <= (toBendState[i] * maxRotateAngles[i] / angleTypeNums))
                {
                    bendState[i] = toBendState[i];
                    angle = (toBendState[i] * maxRotateAngles[i] / angleTypeNums) - angleRecords[i];
                    angleRecords[i] = (toBendState[i] * maxRotateAngles[i] / angleTypeNums);
                }
                else
                    angleRecords[i] += angle;  

                fingerParts[i].transform.RotateAround(rotateCenters[i].transform.position, rotateVectors[i], angle);
            }
        }
	}

    //play the rotation of one part of a finger
    //note:the part3 is influenced by the rotation of part2
    //times means the times of 50ms
    void PlayAnimation(int partIndex,int finalState,int times){
        toBendState[partIndex] = finalState;
        animation_times[partIndex] = times;
        if (type==FingerType.FINGER_THUMB&&partIndex == 1)
        {
            PlayAnimation(0,finalState,times);
            PlayAnimation(2, finalState/2, times);
        }
        if (type != FingerType.FINGER_THUMB && partIndex ==1)
        {
            PlayAnimation(0, finalState, times);
            PlayAnimation(2, finalState, times);
        }
            
       
    }

    void GetKeyEvent()
    {
        //Debug.Log("-----------");
        int x = -1;
        if (Input.GetKey(KeyCode.Alpha1))//49
             x = (int)KeyCode.Alpha1;
        else if (Input.GetKey(KeyCode.Alpha2))//50
            x = (int)KeyCode.Alpha2;
        else if (Input.GetKey(KeyCode.Alpha3))
            x = (int)KeyCode.Alpha3;
        else if (Input.GetKey(KeyCode.Alpha4))
            x = (int)KeyCode.Alpha4;
        else if (Input.GetKey(KeyCode.Alpha5))
            x = (int)KeyCode.Alpha5;
   
        if (x != -1)
        {
            if (lastKey != x)
            {
                Debug.Log("x:" + x);
                int _x = x - 49;
                if (_x == 0)//1
                {
                    int[] arr = new int[5] { 1, 0, 1, 1, 1 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 1)//2
                {
                    int[] arr = new int[5] { 1, 0, 0, 1, 1 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 2)//3
                {
                    int[] arr = new int[5] { 1, 1, 0, 0, 0 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 3)//4
                {
                    int[] arr = new int[5] { 1, 0, 0, 0, 0 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 4)//5
                {
                    int[] arr = new int[5] { 0, 0, 0, 0, 0 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 5)//6
                {
                    int[] arr = new int[5] { 1, 0, 0, 0, 1 };
                    SetFingerState(arr, 5);
                }
                else if (_x == 6)//7
                {
                    int[] arr = new int[5] { 1, 1, 1, 1, 1 };
                    SetFingerState(arr, 5);
                }
            }
            lastKey = x;
        }
            
            
        //int x = -1;
        //if (Input.GetKey(KeyCode.A))
        //    x = (int)KeyCode.A;
        //else if (Input.GetKey(KeyCode.B))
        //    x = (int)KeyCode.B;
        //else if (Input.GetKey(KeyCode.C))
        //    x = (int)KeyCode.C;
        //else if (Input.GetKey(KeyCode.D))
        //    x = (int)KeyCode.D;
        //else if (Input.GetKey(KeyCode.E))
        //    x = (int)KeyCode.E;
        //else if (Input.GetKey(KeyCode.F))
        //    x = (int)KeyCode.F;
        //else if (Input.GetKey(KeyCode.G))
        //    x = (int)KeyCode.G;
        //else if (Input.GetKey(KeyCode.H))
        //    x = (int)KeyCode.H;
        //else if (Input.GetKey(KeyCode.I))
        //    x = (int)KeyCode.I;
        //else if (Input.GetKey(KeyCode.J))
        //    x = (int)KeyCode.J;
        //else if (Input.GetKey(KeyCode.K))
        //    x = (int)KeyCode.K;
        //else if (Input.GetKey(KeyCode.L))
        //    x = (int)KeyCode.L;
        //else if (Input.GetKey(KeyCode.M))
        //    x = (int)KeyCode.M;
        //else if (Input.GetKey(KeyCode.N))
        //    x = (int)KeyCode.N;

        //x = x - 97;//ABCDEFG  HIJKLMN
        //if (x >= 0 && x <= 13)
        //{
        //    if (type == FingerType.FINGER_THUMB)
        //        PlayAnimation(1, x%7, 5);
        //    else
        //    {
        //        //int x1 = x / 7;
        //        int x2 = x % 7;
        //        PlayAnimation(1, x2, 5);
        //    }
        //}
    }

    void SetFingerState(int[]arr,int size)
    {
        for (int i = 0; i < size; ++i)
            if ((int)type == i)
                fingerState = arr[i];
    }
}
