using UnityEngine;
using System.Collections;

public class MyEvent : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        this.GetComponent<BeatDetection>().CallBackFunction = MyCallbackEventHandler;
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void MyCallbackEventHandler(BeatDetection.EventInfo eventInfo)
    {
        EventTypeCnt.AddEventType((int)eventInfo.messageInfo);
        //SideFountain.AddEventType((int)eventInfo.messageInfo);

        switch (eventInfo.messageInfo)
        {
            case BeatDetection.EventType.Energy:

                Debug.Log("Energy");
                break;
            case BeatDetection.EventType.HitHat:
                Debug.Log("Hithat");
                break;
            case BeatDetection.EventType.Kick:
                Debug.Log("Kick");
                break;
            case BeatDetection.EventType.Snare:
                Debug.Log("Snare");
                break;
        }
    }

}
