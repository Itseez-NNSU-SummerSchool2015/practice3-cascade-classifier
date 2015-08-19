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

    CascadeClassifier detector;
    detector.load(detector_file);

    if (!image_file.empty())
    {
		cout << "reading image from file '" << image_file << "'" << endl;
        Mat image = imread(image_file);
        CV_Assert(!image.empty());
        vector<Rect> detections;
        detector.detectMultiScale(image, detections);
        drawDetections(detections, colors[0], image);
        imshow("detections", image);
        waitKey(0);
    }
    else if (!video_file.empty())
    {
		cout << "reading video from file '" << video_file << "'" << endl;
        char key = 0;
        VideoCapture video(video_file);
        CV_Assert(video.isOpened());
        Mat frame;
        video >> frame;
        while (!frame.empty() && key != 27)
        {
            vector<Rect> detections;
            detector.detectMultiScale(frame, detections);
            drawDetections(detections, colors[0], frame);
            imshow("detections", frame);
            key = waitKey(1);
            video >> frame;
        }
    }
    else if (use_camera)
    {
		cout << "reading live video stream from camera..." << endl;
        char key = 0;
        VideoCapture video(0);
        CV_Assert(video.isOpened());
        Mat frame;
        video >> frame;
		// Dirty hack to use camera on Windows.
		while (frame.empty())
		{
			video >> frame;
		}
        while (!frame.empty() && key != 27)
        {
            vector<Rect> detections;
            detector.detectMultiScale(frame, detections);
            drawDetections(detections, colors[0], frame);
            imshow("detections", frame);
            key = waitKey(1);
            video >> frame;
        }
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



