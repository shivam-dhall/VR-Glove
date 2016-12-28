VR-Glove
=============
This a project which uses arduino and MPU6050 to make a VR-Glove, and use unity3d to make a vitual hand model to simulate the gesture
of the right hand in the reality.<br><br>
#Goal
We know the VR-Glove detects the position of hand with the kinect, which is limited by the light and the erection of one or even more kinect
devices. Therefore, I generate an idea that if we can make a dependent VR-Glove, it can be excellent. So i try to make a VR-Glove based on 
`Acceleration`, `Gyroscope` and `Bending Sensor`(DIY), to simulate the gesture of the hand in the reality.<br>
Here is one video about [Gait Tracking](http://x-io.co.uk/gait-tracking-with-x-imu/).<br><br>
#Architecture
![image](https://github.com/Mrtriste/VR-Glove/blob/master/raw/picture/Architecture.png)  <br>
There are three parts mainly:<br>
1.Arduino: it connects with one JY-901 module and ten bending sensors(for ten joints of a hand), and then send these data by ESP8266 wifi module every 100ms.<br>
2.Cloud Server: it receive the data from the glove and then pre-process these data. After that, it send these data to the VR Scene at the
smart phone.<br>
3.Smart Phone: it receive the data from the cloud server and then adjust the gesture of the hand model in the VR Scene.<br><br>
#Arduino
to be post<br>
#Cloud Server
to be post<br>
#Smart Phone
to be post<br>
