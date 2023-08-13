import cv2
from ultralytics import YOLO
from time import sleep
import paho.mqtt.client as paho

def on_publish(client, userdata, mid):
    print("published meassage "+str(mid))
client = paho.Client()
client.on_publish = on_publish
#broker.mqttdashboard.com
client.connect('broker.emqx.io', 1883)
client.loop_start()

#counter variables for each class
sparCt=0
crowCt=0
peaCt=0
pigCt=0
parCt=0
notCt=0
# Load the YOLOv8 model
model = YOLO('yolov8s_custom.pt')

# Open the video file
video_path = "test1.mp4"
address="http://192.168.173.201:8080/video"
cap = cv2.VideoCapture(0) #video_path

# Loop through the video frames
while cap.isOpened():
    # Read a frame from the video
    success, frame = cap.read()

    if success:
        # Run YOLOv8 inference on the frame
        results = model(frame,conf=0.6,save=True)

        # Visualize the results on the frame
        annotated_frame = results[0].plot()

        # Display the annotated frame
        cv2.imshow("YOLOv8 Inference", annotated_frame)
        what=str(results[0].boxes.cls)
        print(what)
        what=what[-4]
        if what=='0':
            print("it is crow")
            crowCt+=1
            if (crowCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "0", qos=1)
                sleep(0.5)
        elif what=='1':
            print('it is sparrow')
            sparCt+=1
            if (sparCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "1", qos=1)
                sleep(0.5)
        elif what=='2':
            print('it is peacock')
            peaCt+=1
            if (peaCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "2", qos=1)
                sleep(0.5)
        elif what=='3':
            print('it is pigeon')
            pigCt+=1
            if (pigCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "3", qos=1)
                sleep(0.5)
        elif what=='4':
            print('it is parrot')
            parCt+=1
            if (parCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "4", qos=1)
                sleep(0.5)
        else:
            #once nothing is detected on frame
            print('not detected')
            notCt+=1
            if (notCt%2)==0:
                #Publishing to mqtt broker
                mid = client.publish('yat/1', "N", qos=1)
                sleep(0.5)
                sparCt=0
                crowCt=0
                peaCt=0
                pigCt=0
                parCt=0
        
        # Break the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break
    else:
        # Break the loop if the end of the video is reached
        break

# Release the video capture object and close the display window
cap.release()
cv2.destroyAllWindows()
