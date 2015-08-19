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
     "{   | detector1|       | XML file with a cascade detector1             }"
     "{   | image    |       | image to detect objects on                    }"
     "{   | video    |       | video file to detect on                       }"
     "{   | camera   | false | whether to detect on video stream from camera }"
	 "{   | detector2|       | XML file with a cascade detector2             }"
	 ;


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

    string detector_file1 = parser.get<string>("detector1");
	string detector_file2 = parser.get<string>("detector2");
    CV_Assert(!detector_file1.empty());
	CV_Assert(!detector_file2.empty());	
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");

    // TODO: Load detector.
	CascadeClassifier* cascade1=new CascadeClassifier();
	CascadeClassifier* cascade2=new CascadeClassifier();
	cascade1->load(detector_file1);
	cascade2->load(detector_file2);

    if (!image_file.empty())
    {
        // TODO: Detect objects on image.
		Mat image=imread(image_file);
		vector<Rect>image_objects1;
		vector<Rect>image_objects2;
		cascade1->detectMultiScale(image, image_objects1);
		cascade2->detectMultiScale(image, image_objects2);
		drawDetections(image_objects1, green, image);
		drawDetections(image_objects2, blue, image);
		//imshow("image_detected", image);
		imwrite("image_unn_detected.png", image);
    }
    else if (!video_file.empty())
    {
        // TODO: Detect objects on every frame of a video.
		cout << video_file << endl;
		VideoCapture video(video_file);
		CV_Assert(video.isOpened());
		Mat video_frame;
		vector<Rect> video_objects1;
		vector<Rect> video_objects2;
		video>>video_frame;
		for (;;) {
			video>>video_frame;
			cascade1->detectMultiScale(video_frame, video_objects1);
			cascade2->detectMultiScale(video_frame, video_objects2);
			drawDetections(video_objects1, blue, video_frame);
			drawDetections(video_objects2, green, video_frame);
			imshow("video_detected", video_frame);
			//imwrite("video_detected_piece.png", video_frame);
			if (waitKey(30)>=0) break;
		}
    }
    else if (use_camera)
    {
        // TODO: Detect objects on a live video stream from camera.
		VideoCapture cap(0);
		if (!cap.isOpened()) return -1;
		Mat cap_frame;
		vector<Rect>cap_objects1;
		vector<Rect>cap_objects2;
		cap>>cap_frame;
		for (;;) {
			cap>>cap_frame;
			cascade1->detectMultiScale(cap_frame, cap_objects1);
			cascade2->detectMultiScale(cap_frame, cap_objects2);
			drawDetections(cap_objects1, red, cap_frame);
			drawDetections(cap_objects2, green, cap_frame);
			imshow("on_camera_detected", cap_frame);
			if (waitKey(30)>=0) break;
		}
    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }
    return 0;
}



