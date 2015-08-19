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
     "{   | camera   | false | whether to detect on video stream from camera }"
	 "{   | other_detector  | false | use other detector                     }"
	 "{   | detector1 |       | other detector                               }";


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

void image_detection(string detector_file,
	string image_file,string detector_file_other,
	bool use_other_detector )
{
	CascadeClassifier face_cascade;
	
	face_cascade.load(detector_file);


	vector<Rect> rectangle;
	vector<int> rejectLevels;
	vector<double> levelW;

	Mat img;

	if (use_other_detector)
		{
			CascadeClassifier other_detector(detector_file_other);

			img = imread(image_file);

			face_cascade.detectMultiScale(img,rectangle,rejectLevels,levelW);	
			
			drawDetections(rectangle,red,img);

			other_detector.detectMultiScale(img,rectangle,rejectLevels,levelW);	
			
			drawDetections(rectangle,green,img);

			imshow("img",img);
			waitKey(0);
		}
		else
		{
		
			img = imread(image_file);
			face_cascade.detectMultiScale(img,rectangle,rejectLevels,levelW);	
			drawDetections(rectangle,red,img);
			imshow("img",img);
			waitKey(0);
		}
}
void video_detection(string video_file,VideoCapture cap,string detector_file )
{
	CascadeClassifier face_cascade(detector_file);


	vector<Rect> rectangle;
	vector<int> rejectLevels;
	vector<double> levelW;

	cap.open(video_file);

		cout<<video_file<<endl;

		if (!cap.isOpened())
			return ;	

		Mat frame;
		namedWindow("img",1);
		for (;;)
		{
			cap>>frame;
			if (frame.empty() )
			{
				cout << "frame don't open" << endl;
				break;
			}
			else
			{
				face_cascade.detectMultiScale(frame,rectangle,rejectLevels,levelW);
				drawDetections(rectangle,red,frame);
				imshow("img",frame);
				if(waitKey(27) >= 0)
					break;
			}
		}
}



void camera_detection(VideoCapture cap, string detector_file,bool use_other_detector,string detector_file_other)
{
	CascadeClassifier face_cascade(detector_file);
	CascadeClassifier face_cascade_other(detector_file_other);
	vector<Rect> rectangle;
	vector<int> rejectLevels;
	vector<double> levelW;

	cap.open(0);

	if (!cap.isOpened())
	{
		cout << "camera don't open" << endl;
		return ;
	}

	Mat frame;
	while(true)
	{
		cap.read(frame);
		if (!frame.empty())
		{
			
			if (use_other_detector)
			{
					face_cascade.detectMultiScale(frame,rectangle,rejectLevels,levelW);	
					drawDetections(rectangle,red,frame);
					face_cascade_other.detectMultiScale(frame,rectangle,rejectLevels,levelW);		
					drawDetections(rectangle,green,frame);
					imshow("img",frame);
					if(waitKey(27) >= 0)
							break;
					
			}
			else
			{
					face_cascade.detectMultiScale(frame,rectangle,rejectLevels,levelW);	
					drawDetections(rectangle,red,frame);
					imshow("frame",frame);
					if(waitKey(27) >= 0)
							break;
					
			}
		}
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
	string detector_file_other = parser.get<string>("detector1");
	
	

	VideoCapture cap;


    CV_Assert(!detector_file.empty());
    string image_file = parser.get<string>("image");
    string video_file = parser.get<string>("video");
    bool use_camera = parser.get<bool>("camera");


	bool use_other_detector = parser.get<bool>("other_detector");


    // TODO: Load detector.

    if (!image_file.empty())
    {
		
		image_detection(detector_file,image_file,detector_file_other,use_other_detector );
        

    }
    else if (!video_file.empty())
    {
		video_detection(video_file,cap,detector_file );
		
        // TODO: Detect objects on every frame of a video.

    }
    else if (use_camera)
    {
		camera_detection(cap,detector_file ,use_other_detector,detector_file_other );
		
        // TODO: Detect objects on a live video stream from camera.

    }
    else
    {
        cout << "Declare a source of images to detect on." << endl;
    }

	waitKey(0);

    return 0;
}



