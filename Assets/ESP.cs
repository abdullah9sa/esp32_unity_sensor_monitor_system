using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;
using UnityEngine.Networking;

public class ESP : MonoBehaviour
{
    
    // slider gameobject
    public Slider slider;
    public Text tempText;
    public Text humText;
    public Text threshHold;
    //connection status text
    public Text conn;

    string data;
    string tempData;
    string humData;

    bool alarmOn;
    string alarmm;
    public GameObject alarm;
    float x=0;
    String tempValue;
        
    void Start(){
        InvokeRepeating("Updater", 1.0f, 1f);
    }

    void Updater()
    {
        tempValue = slider.value.ToString();
        StartCoroutine(getRequest("http://192.168.1.8/setdata/?temp="+tempValue));
    }

    IEnumerator getRequest(string uri)
    {
        yield return new WaitForSeconds(x);
        UnityWebRequest uwr = UnityWebRequest.Get(uri);
        yield return uwr.SendWebRequest();

        if (!uwr.isNetworkError)
        {
            conn.text = " Connected";
            conn.color = Color.green;
            data = uwr.downloadHandler.text;
            tempData = data.Split('-')[0];
            humData = data.Split('-')[1];
            alarmm = data.Split('-')[2];
            print("tempdata : " + tempData + " C");
            print("humData : " + humData + " %");
            tempText.text = tempData  + " c";
            humText.text = humData + " %";

            if(alarmm == "tr"){
                alarm.SetActive(true);
                tempValue = "40.00";
                slider.value = 40;
            }
            else
            {
                alarm.SetActive(false);
            }
            Debug.Log("Received: " + uwr.downloadHandler.text);
        }

    }

    public void ChangeTempValue(){
        //changes slider indicator text
        threshHold.text = "Alarm Temp. : "+ slider.value.ToString("00.00") + " C";
    }

    public void Quit()
    {
        Application.Quit();
    }

}
