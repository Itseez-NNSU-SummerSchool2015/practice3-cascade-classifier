#include <iostream>
#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace std;
using namespace cv;

const char* params =
     "{ h | help     | false | print usage                                   }"
     "{   | detector |       | XML file with a cascade detector              }"
	 "{   | d2       |       | Additional detector                           }"
	 "{   | d3       |       | Additional detector                           }"
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

void detect(const Mat& src, Mat& dst, vector<CascadeClassifier>& detector) {
	//const Size kSize(5, 5);
	//GaussianBlur(src, dst, kSize, 3); 
	dst = src.clone();

	for (auto it = detector.begin(), iend = detector.end(); it != iend; ++it) {
		vector<Rect> objects;
		(*it).detectMultiScale(dst, objects);
		drawDetections(objects, colors[it - detector.cbegin()], dst);
	}	
}

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
	string d2_file = parser.get<string>("d2");
	string d3_file = parser.get<string>("d3");
    CV_Assert(!detector_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    vector<CascadeClassifier> detector;
	detector.emplace_back(detector_file);
	if (d2_file.empty() == false) {
		detector.emplace_back(d2_file);
	}
	if (d3_file.empty() == false) {
		detector.emplace_back(d3_file);
	}

    if (!image_file.empty())
    {
		Mat img = imread(image_file);
		Mat out;
		detect(img, out, detector);
		imshow("Detected frames", out);
		waitKey();
    }
    else if (!video_file.empty())
    {        
		VideoCapture video(video_file);
		CV_Assert(video.isOpened());
		
		Mat img, out;
		vector<Rect> objects;

		video >> img;
		while (img.empty() == false) {
			if (waitKey(30) >= 0) { break; }
			detect(img, out, detector);
			imshow("Detected frames", out);
			video >> img;
		}

		video.release();
    }
    else if (use_camera)
    {
        VideoCapture video(0);
		CV_Assert(video.isOpened());
		
		Mat img, out;
		vector<Rect> objects;

		video >> img;
		video >> img; //bugfix on Win
		while (img.empty() == false) {
			if (waitKey(30) >= 0) { break; }
			detect(img, out, detector);
			imshow("Detected frames", out);
			video >> img;
		}

		video.release();
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

    return 0;
}



