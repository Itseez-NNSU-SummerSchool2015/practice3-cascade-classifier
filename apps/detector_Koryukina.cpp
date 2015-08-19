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

    //Load detector.
	CascadeClassifier detector;
	detector.load(detector_file);

    if (!image_file.empty())
    {
        // Detect objects on image.
		Mat image;
		vector<Rect> detections;
		image = imread(image_file, CV_LOAD_IMAGE_COLOR);
		detector.detectMultiScale(image, detections);
		drawDetections(detections, red,image);
		imshow(image_file, image);
		waitKey();
    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.
		VideoCapture video;
		video.open(video_file); 
		CV_Assert(video.isOpened());
		Mat frame;
		video >> frame;
	    namedWindow("video_detector",CV_WINDOW_AUTOSIZE);
		char key = 0;
	
	    for(; key != 27;)
		{
			video >> frame;
			vector<Rect> detections;
			detector.detectMultiScale(frame, detections);
			drawDetections(detections, red, frame);
			if (!frame.empty())
			{
				imshow("video_detector", frame);
			}
			key = waitKey(10);
		}

    }
    else if (use_camera)
    {
        // Detect objects on a live video stream from camera.
		VideoCapture cap(0); // open the default camera
		if(!cap.isOpened())  // check if we succeeded
			return -1;
		Mat frame;
		cap >> frame;
	    namedWindow("camera_detector",1);
		char key = 0;
	    for(; key != 27;)
		{
			cap >> frame; 
			vector<Rect> detections;
			detector.detectMultiScale(frame, detections);
			drawDetections(detections, red, frame);
			imshow("camera_detector", frame);
			key = waitKey(10);
		}

    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



