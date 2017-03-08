using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClickTable : MonoBehaviour {
    GameObject[] cubes = new GameObject[7];

	// Use this for initialization
	void Start () {
        int i = 0;
        foreach (Transform child in this.transform)
        {
            cubes[i++] = child.gameObject;
        }
	}


    public void Activate()
    {
        
        foreach (GameObject obj in cubes)
        {
            obj.GetComponent<Key>().Activate();
        }

    }

    public void InActivate()
    {

        foreach (GameObject obj in cubes)
        {
            obj.GetComponent<MeshRenderer>().enabled = false;
        }

    }
	
	// Update is called once per frame
	void Update () {
		
	}

    public void Init()
    {

    }
}
