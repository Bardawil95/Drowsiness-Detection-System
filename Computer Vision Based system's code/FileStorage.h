#include <string>
#include <unistd.h>
#include <sys/stat.h>

string dirname;
VideoCapture capture;
cv::String face_cascade_name = "lbpcascade_frontalface.xml";
cv::CascadeClassifier face_cascade;
cv::String window_name = "Capture - Face detection";
stringstream openR, openL, closeR, closeL, dirnames[12], file;
int eye = 0;
string s = "/home/bardawil/Documents/Drowsiness Detection and Alerting System/";
void store() {
	stringstream f1, f2;
	printf("Enter a directory path and name to be created (C:/name):");
	cin >> dirname;
	mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	f1 << dirname.c_str() << "/Opened Eyes";
	f2 << dirname.c_str() << "/Closed Eyes";
	
	mkdir(f1.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) ; 
	mkdir(f2.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

}
void dir() {
	
	dirnames[0] << dirname << "/Opened Eyes/EyeR-1.jpg";
	dirnames[1] << dirname << "/Opened Eyes/EyeL-1.jpg";
	dirnames[2] << dirname << "/Closed Eyes/EyeR-1.jpg";
	dirnames[3] << dirname<< "/Closed Eyes/EyeL-1.jpg";
	dirnames[4] << dirname << "/Opened Eyes/EyeR-2.jpg";
	dirnames[5] << dirname << "/Opened Eyes/EyeL-2.jpg";
	dirnames[6] << dirname << "/Closed Eyes/EyeR-2.jpg";
	dirnames[7] << dirname<< "/Closed Eyes/EyeL-2.jpg";
	dirnames[8] << dirname << "/Opened Eyes/EyeR-3.jpg";
	dirnames[9] << dirname << "/Opened Eyes/EyeL-3.jpg";
	dirnames[10] << dirname << "/Closed Eyes/EyeR-3.jpg";
	dirnames[11] << dirname<< "/Closed Eyes/EyeL-3.jpg";
}
void record(cv::Mat &frame, int n) {
	stringstream file1, file2;
	const string openE = "/Opened Eyes/", closeE = "/Closed Eyes/";
	Mat *gray_image1, *gray_image2;

	std::vector<cv::Rect> faces;
	
	cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frame, frame);

	//-- Detect faces

	face_cascade.detectMultiScale(frame, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(350, 350));


	Mat face;
	if(faces.size() != 0){
		Mat ROI1(frame, Rect(faces[0].x + faces[0].width / 5.2, faces[0].y + faces[0].height / 3, faces[0].width / 3.9, faces[0].height / 6));
		Mat ROI2(frame, Rect(faces[0].x + faces[0].width / 1.8, faces[0].y + faces[0].height / 3, faces[0].width / 3.68, faces[0].height / 6));

		gray_image1 = &ROI1;
		gray_image2 = &ROI2;
		
		if (n == 1) {
			
			file1 << dirname.c_str() << openE << "EyeL-" << eye + 1 << ".jpg";
			file2 << dirname.c_str() << openE << "EyeR-" << eye + 1 << ".jpg";
			imwrite(file2.str(), *gray_image2);  // Opened Right Eye
			imwrite(file1.str(), *gray_image1);  // Opened Left Eye
		}
		if (n == 2) {
			
			file1 << dirname.c_str() << closeE << "EyeL-" << eye + 1 << ".jpg";
			file2 << dirname.c_str() << closeE << "EyeR-" << eye + 1 << ".jpg";
			imwrite(file2.str(), *gray_image2);  // Opened Left Eye
			imwrite(file1.str(), *gray_image1);  // Opened Left Eye
		}
		rectangle(frame, faces[0].tl(), faces[0].br(), cvScalar(125, 50, 255), 2, 8, 0);
	}
	cv::imshow(window_name, frame);
}
void cap(int n) {
	Mat frame;

	cv::VideoCapture capture;

	capture.open(0);

	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return; }

	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	int i = 1;
	
	while (i <= 3){
		capture.read(frame);
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		record(frame, n);
		waitKey(1000);
		eye++;
		i++;
	}
	eye = 0;
	capture.release();
}
