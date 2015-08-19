#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;
using namespace cv;

const char* params =
     "{ h | help     | false | print usage                                   }"
     "{   | detector |       | XML file with a cascade detector              }"
     "{   | image    |       | image to detect objects on                    }"
     "{   | video    |       | video file to detect on                       }"
     "{   | camera   | false | whether to detect on video stream from camera }";


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

    string detector_file = parser.get<string>("detector");
    CV_Assert(!detector_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    // TODO: Load detector.
	CascadeClassifier cclass;
	cclass.load(detector_file);
	CascadeClassifier cs;
	cs.load("./haarcascade_frontalface_default.xml");
	if(cs.empty()) cout<<"lol";
    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
		Mat img = imread(image_file);
		vector<Rect> obj;
		cclass.detectMultiScale(img, obj);
		if (!img.empty())
		{
			namedWindow("Detection");  
			int key;
			drawDetections(obj,Scalar(0,255,255),img);
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
		vector<Rect> obj;
		namedWindow("Detection");
		Mat frame;
		cam >> frame;
		while (cam.isOpened())
		{
			cam >> frame;
			int key;
			cclass.detectMultiScale(frame, obj);
			drawDetections(obj,Scalar(0,255,255),frame);
			cs.load("./cascade.xml");
			cs.detectMultiScale(frame,obj);
			drawDetections(obj,Scalar(255,0,0),frame);
			imshow("Detection",frame);
			key = waitKey(10);
			if (key == 27) break;
		}
    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.
		
		VideoCapture cam(0);
		vector<Rect> obj;
		namedWindow("Detection");
		Mat frame;
		cam >> frame;
		for (;;)
		{
			cam >> frame;
			int key;
			cclass.detectMultiScale(frame, obj);
			drawDetections(obj,Scalar(0,255,255),frame);
			cs.load("./haarcascade_frontalface_default.xml");
			cs.detectMultiScale(frame,obj);
			drawDetections(obj,Scalar(255,255,255),frame);
			cs.load("./cascade.xml");
			cs.detectMultiScale(frame,obj);
			drawDetections(obj,Scalar(255,0,0),frame);
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



