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

    // Load detector.
    CascadeClassifier unnLogoCascade;
    CascadeClassifier unnOldLogoCascade;
    CascadeClassifier openCVLogoCascade;
    CascadeClassifier itseezFullCascade;
    CascadeClassifier itseezEyeCascade;

    // TODO: change paths!!!
    if (!unnLogoCascade.load(detector_file))
    {
        cout << "UNN not ready!";
        return -1;
    }
    /*if (!unnOldLogoCascade.load(pathToCascade))
    {
        cout << "UNN old not ready!";
        return -1;
    }
    if (!openCVLogoCascade.load(pathToCascade))
    {
        cout << "openCV not ready!";
        return -1;
    }
    if (!itseezFullCascade.load(pathToCascade))
    {
        cout << "itseez full not ready!";
        return -1;
    }
    if (!itseezEyeCascade.load(pathToCascade))
    {
        cout << "itseez eye not ready!";
        return -1;
    }*/




    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
        vector<Rect> unnLogos;
        Mat image = imread(image_file);

        unnLogoCascade.detectMultiScale(image, unnLogos);
        cout << "Num of images found: " << unnLogos.size() << endl;
    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.

    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.
        Mat frame;
        VideoCapture capture;
        capture.open(0);
        capture.grab();

        while (true)
        {
            capture >> frame;

            vector<Rect> unnLogos;
            unnLogoCascade.detectMultiScale(frame, unnLogos);
            cout << unnLogos.size() << endl;

        }
    }

    return 0;
}



