#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

const char* params =
     "{ h | help      | false | print usage                                   }"
     "{   | detector1 |       | XML file with a cascade detector              }"
	 "{   | detector2 |       | XML file with a cascade detector              }"
     "{   | image     |       | image to detect objects on                    }"
     "{   | video     |       | video file to detect on                       }"
     "{   | camera    | false | whether to detect on video stream from camera }";


void drawDetections(const vector<Rect>& detections,
                    const Scalar& color,
                    Mat& image)
{
    for (size_t i = 0; i < detections.size(); ++i)
    {
        rectangle(image, detections[i], color, 2);
    }
}

const Scalar red(0, 0, 255);
const Scalar green(0, 255, 0);
const Scalar blue(255, 0, 0);
const Scalar colors[] = {red, green, blue};

int main(int argc, char** argv)
{
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, params);
    // If help flag is present, print help message and exit.
    if (parser.get<bool>("help"))
    {
        parser.printParams();
        return 0;
    }

    string detector1_file = parser.get<string>("detector1");
    CV_Assert(!detector1_file.empty());
	string detector2_file = parser.get<string>("detector2");
    CV_Assert(!detector2_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    // TODO: Load detector.
	CascadeClassifier cclass1;
	cclass1.load(detector1_file);
	CascadeClassifier cclass2;
	cclass2.load(detector2_file);
	CascadeClassifier cs;
	cs.load("./haarcascade_frontalface_default.xml");
	if(cs.empty()) cout<<"lol";
    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
		Mat img = imread(image_file);
		vector<Rect> obj1;
		vector<Rect> obj2;
		cclass1.detectMultiScale(img, obj1);
		cclass2.detectMultiScale(img, obj2);
		if (!img.empty())
		{
			namedWindow("Detection");  
			int key;
			drawDetections(obj1,red,img);
			drawDetections(obj2,green,img);
			while (key!=27)
			{
				imshow("Detection",img);
				key = waitKey();
			}
		}
    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.
		VideoCapture cam(video_file);
		vector<Rect> obj1;
		vector<Rect> obj2;
		namedWindow("Detection");
		Mat frame;
		cam >> frame;
		while (cam.isOpened())
		{
			cam >> frame;
			int key;
			cclass1.detectMultiScale(frame, obj1);
			cclass2.detectMultiScale(frame, obj2);
			drawDetections(obj1,red,frame);
			drawDetections(obj1,green,frame);
			imshow("Detection",frame);
			key = waitKey(10);
			if (key == 27) break;
		}
    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.
		
		VideoCapture cam(0);
		vector<Rect> obj1;
		vector<Rect> obj2;
		namedWindow("Detection");
		Mat frame;
		cam >> frame;
		for (;;)
		{
			cam >> frame;
			int key;
			cclass1.detectMultiScale(frame, obj1);
			cclass2.detectMultiScale(frame, obj2);
			drawDetections(obj1,red,frame);
			drawDetections(obj2,green,frame);
			cs.detectMultiScale(frame,obj1);
			drawDetections(obj1,Scalar(255,255,255),frame);
			imshow("Detection",frame);
			key = waitKey(10);
			if (key == 27) break;
		}
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



