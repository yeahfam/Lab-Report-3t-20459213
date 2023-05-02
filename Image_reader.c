#include <stdio.h>

#include "opencv_aee.hpp"
#include "main.hpp"     // You can use this file for declaring defined values and functions

#include "pi2c.h"


Pi2c car(0x22); // Configure the I2C interface to the Car as a global variable


Pi2c arduino(0x04);

double anglefinder (Mat frameHSV)
{
            double pos1x, pos1y,pos2x,pos2y;
            double angle, angle1, angle2;
            int pixelcount=0;
            int x=0;
            int centy=(frameHSV.size().height)/2;
            int y=centy;

            while (pixelcount<=5)
            {
                //cout<<"test";
                //cout<<x;
                if ((frameHSV.at<Vec3b>(y, x)[0])==255)
                {
                    if ((frameHSV.at<Vec3b>(y,x)[1])==255)
                    {
                        if ((frameHSV.at<Vec3b>(y,x)[2])==255)
                        {
                            //cout<<"yes!";
                            pixelcount =pixelcount+1;
                        }
                    }
                }
                x=x+1;
            }
            int centx=(frameHSV.size().width)/2;
            //centy=frameHSV.size().height;
            int distanceFromCent=centx-x;
            //printf(distanceFromCent);
            //printf("\n");
            angle2=(distanceFromCent/4);
            return angle2;
}


void setup(void)
{
    setupCamera(320, 240);
      // Enable the camera for OpenCV

}

int main( int argc, char** argv )
{
    setup();    // Call a setup function to prepare IO and devices

    int lowH = 85, highH = 140, lowS = 81, highS = 255, lowV = 32, highV = 255;

    bool Blue=false, Green=false, Red=false, Black=false, Yellow=false;

    int width1, height1;

    cv::namedWindow("Photo");   // Create a GUI window called photo

    //int16_t leftmotor=0;
    //int16_t rightmotor=0;
    int16_t angle=80;

    arduino.i2cWriteArduinoInt(angle);
    //arduino.i2cWriteArduinoInt(rightmotor);
    //arduino.i2cWriteArduinoInt(angle);

    while(1)    // Main loop to perform image processing
    {
        Mat frame;

        while(frame.empty())
            frame = captureFrame(); // Capture a frame from the camera and store in a new matrix variable

        width1= frame.size().width;
        height1= frame.size().height;

        //Mat cropped_image = frame(Range((height1/5),(height1-height1/5)), Range((width1/5),(width1-width1/5)));

        //cv::imshow("Photo", cropped_image); //Display the image in the window

        cv::Mat cropped_image;
        cv::flip(frame, cropped_image, -1);

        Mat frameHSV;       // Convert the frame to HSV and apply the limits
        cvtColor(cropped_image, frameHSV, COLOR_BGR2HSV);
        inRange(frameHSV, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV),  frameHSV);


        Mat greenHSV;       // Convert the frame to HSV and apply the limits
        cvtColor(cropped_image, greenHSV, COLOR_BGR2HSV);
        inRange(greenHSV, Scalar(64, 117, 40), Scalar(80, 255, 255),  greenHSV);

        Mat redHSV;       // Convert the frame to HSV and apply the limits
        cvtColor(cropped_image, redHSV, COLOR_BGR2HSV);
        inRange(redHSV, Scalar(173, 72, 0), Scalar(179, 255, 255),  redHSV);

        Mat blackHSV;       // Convert the frame to HSV and apply the limits
        cvtColor(cropped_image, blackHSV, COLOR_BGR2HSV);
        inRange(blackHSV, Scalar(0, 0, 0), Scalar(90, 255, 60),  blackHSV);

        Mat yellowHSV;       // Convert the frame to HSV and apply the limits
        cvtColor(cropped_image, yellowHSV, COLOR_BGR2HSV);
        inRange(yellowHSV, Scalar(22, 36, 53), Scalar(37, 255,255),  yellowHSV);

        Mat finalframe;
        cvtColor(cropped_image, finalframe, COLOR_BGR2HSV);




        int BluePixels =0;
        int GreenPixels =0;
        int RedPixels =0;
        int BlackPixels = 0;
        int YellowPixels=0;
        BluePixels=countNonZero(frameHSV);
        GreenPixels=countNonZero(greenHSV);
        RedPixels=countNonZero(redHSV);
        BlackPixels=countNonZero(blackHSV);
        YellowPixels=countNonZero(yellowHSV);

        if (BluePixels-GreenPixels-RedPixels-BlackPixels-YellowPixels>0){
            Blue=true;
            printf("Blue is the Colour of the Object\n");
            finalframe= frameHSV;
            //imshow("HSV Tester",finalframe);
        }
        else if (GreenPixels-BluePixels-RedPixels-BlackPixels-YellowPixels>0){
            Green=true;
            printf("Green is the Colour of the Object\n");
            finalframe=greenHSV;
            //imshow("HSV Tester",finalframe);
        }
        else if (RedPixels-BluePixels-GreenPixels>0){
            Red=true;
            printf("Red is the Colour of the Object\n");
            finalframe=redHSV;
            //imshow("HSV Tester",finalframe);
        }

         else if (BlackPixels-RedPixels-BluePixels-GreenPixels-YellowPixels>0){
            Black=true;
            printf("Black is the Colour of the Object\n");
            finalframe=blackHSV;
            //imshow("HSV Tester",finalframe);
        }

         else if (YellowPixels-BlackPixels-RedPixels-BluePixels-GreenPixels>0){
            Yellow=true;
            printf("Yellow is the Colour of the Object\n");
            finalframe=yellowHSV;
            //imshow("HSV Tester",finalframe);
        }

        Mat finalframe1 = finalframe(Range(((finalframe.size().height)/2), (finalframe.size().height)), Range(0,(finalframe.size().width)));

        angle=anglefinder(finalframe1);

        angle=80-angle;

        arduino.i2cWriteArduinoInt(angle);

        imshow("HSV Tester", finalframe1);







        cv::imshow("Photo", cropped_image); //Display the image in the window

        int key = cv::waitKey(1);   // Wait 1ms for a keypress (required to update windows)

        key = (key==255) ? -1 : key;    // Check if the ESC key has been pressed
        if (key == 27)
            break;
      }

      closeCV();  // Disable the camera and close any windows

      return 0;
}
