using UnityEngine;
using System.Collections;

public class LineFountain : MonoBehaviour
{
    private ParticleSystem[] fountains;
    private int index;
    int lCnt = 0;

    bool isStart = false;
    int selfState = 0;

    // Use this for initialization
    void Start()
    {
        fountains = GetComponentsInChildren<ParticleSystem>();
        index = 0;
    }

    public void Init()
    {
        isStart = true;
    }

    public void End()
    {
        isStart = false;
        Component[] children = GetComponentsInChildren<ParticleSystem>();
        foreach (ParticleSystem childParticleSystem in children)
        {
            if (childParticleSystem.isPlaying)
                childParticleSystem.Stop();
        }
        CancelInvoke("mode0");
        CancelInvoke("mode1");
        CancelInvoke("mode2");
    }

    // Update is called once per frame
    void Update()
    {
        if (isStart)
        {
            InitFinger();
            if (Input.GetKeyDown(KeyCode.L)){
                if (selfState == 0)
                    Play1();
                if (selfState== 1)
                    Play2();
                ++selfState;
            }
            
        }

    }

    void InitFinger()
    {
        if (Input.GetKeyDown(KeyCode.Alpha1))//49
        {
            PlayLineFountains(new int[] { 4 }, 1);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha2))//50
        {
            PlayLineFountains(new int[] { 3, 5 }, 2);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha3))
        {
            PlayLineFountains(new int[] { 2, 6 }, 2);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha4))
        {
            PlayLineFountains(new int[] { 1, 7 }, 2);
        }
        else if (Input.GetKeyDown(KeyCode.Alpha5))
        {
            PlayLineFountains(new int[] { 0, 8 }, 2);
        }   
    }

    private void PlayLineFountains(int[] arr, int size)
    {
        for (int i = 0; i < size; ++i)
            PlayLineFountain(arr[i]);
    }

    private void PlayLineFountain(int i)
    {
        StopPopFountain(i);
        fountains[i].Play();
    }

    private void StopPopFountain(int i)
    {
        if (fountains[i].isPlaying)
            fountains[i].Stop();
    }

    void mode0()
    {

    }

    void mode1()
    {
        int count = 0;
        foreach (ParticleSystem child in fountains)
        {
            float tmp = Mathf.Abs((index + count * 4) % 40f - 19.5f) + 4f;

            child.startSpeed = tmp * 7;
            count++;
        }
        index++;

    }

    void mode2()
    {
        int i = index % 9;
        int count = fountains[i].particleCount;
        fountains[i].Emit(count);
        //(fountains[(int)(Random.value * 10f)%10] as ParticleSystem).Emit(500);
        index++;

    }

    public void Play1()
    {
        isStart = true;
        StartCoroutine(PlayDiffFountains1(4f));
    }

    IEnumerator PlayDiffFountains1(float delay)
    {
        index = 0;
        foreach (ParticleSystem child in fountains)
        {
            child.Stop();
        }
        foreach (ParticleSystem child in fountains)
        {
            child.loop = true;
            var shape = child.shape;
            shape.enabled = true;
            shape.angle = 1f;
            shape.arc = 2f;
            child.startSpeed = 150f;

            child.Play();
        }
        InvokeRepeating("mode0", 0, 0.5f);

        yield return new WaitForSeconds(delay);
        CancelInvoke("mode0");

        index = 0;
        foreach (ParticleSystem child in fountains)
        {
            child.Stop();
        }
        foreach (ParticleSystem child in fountains)
        {
            child.loop = true;
            var shape = child.shape;
            shape.enabled = true;
            shape.angle = 1.5f;
            shape.arc = 2f;

            child.Play();
        }
        InvokeRepeating("mode1", 0, 1f / 4f);
    }

    public void Play2()
    {
        isStart = true;

        index = 0;
        foreach (ParticleSystem child in fountains)
        {
            child.Stop();
        }
        foreach (ParticleSystem child in fountains)
        {
            child.loop = false;
            var shape = child.shape;
            shape.enabled = true;
            shape.angle = 5F;
            shape.arc = 0.1F;
            child.startSpeed = 150f;
        }
        InvokeRepeating("mode2", 0, 0.1f);
    }


}
