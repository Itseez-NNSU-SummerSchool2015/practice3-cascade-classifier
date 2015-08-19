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
	CascadeClassifier cc1;
	cc1.load(detector_file);

    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
		Mat img = imread(image_file);
		vector<Rect> objs;
		cc1.detectMultiScale(img, objs);
		namedWindow("newWindow", WINDOW_AUTOSIZE);
		drawDetections(objs, red, img);
		imshow("newWindow", img);
		waitKey(0);

    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.
		
    }
	else if (use_camera)
	{
		// TODO: Detect objects on a live video stream from camera.
		VideoCapture cap(0);
		Mat frame;
		vector<Rect> objs;
		cap.open(0);
		
		if (!cap.isOpened())
		{
			cout << "muy mal" <<endl;
			return -1;
		}
		namedWindow("myVideo", 1);
		while(true) {
			cap >> frame;
			if (frame.empty()) continue;
			cc1.detectMultiScale(frame, objs);
			drawDetections(objs, red, frame);
			imshow("myVideo", frame);
			if (waitKey(1) == 27) break;
		}

    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



