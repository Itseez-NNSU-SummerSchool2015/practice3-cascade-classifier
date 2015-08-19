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

	CascadeClassifier ccl;

	ccl.load(detector_file);

    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
		
		//Mat img = cvLoadImage(image_file.c_str(), 1);
		Mat img = imread(image_file);

		vector<Rect> objects;

		ccl.detectMultiScale(img, objects);		

		drawDetections(objects, Scalar(255, 0, 0), img);

		waitKey(0);

    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.

		VideoCapture cap(video_file); // open the default camera
		if(!cap.isOpened())  // check if we succeeded
			return -1;

		while(1)
		{
			Mat frame;
			cap >> frame; // get a new frame from camera

			while(frame.empty())
			{
				cap >> frame;
			}

			vector<Rect> objects;

			ccl.detectMultiScale(frame, objects);		

			drawDetections(objects, Scalar(255, 0, 0), frame);

			//imshow("BoxFace", frame);
			//if(waitKey(30) >= 0) break;
			
		}

    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.

		VideoCapture cap(0); // open the default camera
		if(!cap.isOpened())  // check if we succeeded
			return -1;

		while(1)
		{
			Mat frame;
			cap >> frame; // get a new frame from camera

			while(frame.empty())
			{
				cap >> frame;
			}

			vector<Rect> objects;

			ccl.detectMultiScale(frame, objects);		

			drawDetections(objects, Scalar(255, 0, 0), frame);

			//imshow("BoxFace", frame);
			//if(waitKey(30) >= 0) break;
			
		}

    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



