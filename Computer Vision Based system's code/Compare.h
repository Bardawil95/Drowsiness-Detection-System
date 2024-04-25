
using namespace std;
using namespace cv;

struct img{
    Point p1, p2;
    Mat frame;
};

void *alarm(void *a){
system("canberra-gtk-play -f Warning.wav");
}

void *detect (void *eye){
    struct img *args;
    args = (struct img *) eye;
    Mat frame;
    Point p1, p2;
    frame = args->frame;
    p1 = args->p1;
    p2 = args->p2;
    rectangle(frame, p1, p2, cvScalar(0, 0, 255), 2, 8, 0);
}

struct comp
{
	Mat img, tmpo1, tmpo2, tmpo3, tmpc1, tmpc2, tmpc3;
};

struct comp argl, argr;
void *l, *r;
void *crossCorrelate(void *arr){                           //Cross correlation function
	struct comp *a;
	a = (struct comp *) arr;
	Mat result1, result2,result3, result4,result5, result6; //mat objects (frames) for results
	resize(a->img, a->img, a->tmpc1.size());                //resize the real time frames to match the size of the templates for comparison
	matchTemplate(a->img, a->tmpo1, result1, 3);            //Use matchTemplate() function to find the highest matching points between the frame and template
															//Using method 3 which based on cross-correlation mathematics
	matchTemplate(a->img, a->tmpc1, result2, 3);            // and place them in the result matrices
	matchTemplate(a->img, a->tmpo2, result3, 3);
	matchTemplate(a->img, a->tmpc2, result4, 3);
	matchTemplate(a->img, a->tmpo3, result5, 3);
	matchTemplate(a->img, a->tmpc3, result6, 3);

	double maxVal1, maxVal2,maxVal3, maxVal4,maxVal5, maxVal6; 
	minMaxLoc(result1, NULL, &maxVal1, NULL, NULL, Mat());   //find the maximum peak values of matching in the result matrices
	minMaxLoc(result2, NULL, &maxVal2, NULL, NULL, Mat());
	minMaxLoc(result3, NULL, &maxVal3, NULL, NULL, Mat());
	minMaxLoc(result4, NULL, &maxVal4, NULL, NULL, Mat());
	minMaxLoc(result5, NULL, &maxVal5, NULL, NULL, Mat());
	minMaxLoc(result6, NULL, &maxVal6, NULL, NULL, Mat());
	return (void *) ((maxVal1 > maxVal2) || (maxVal3 > maxVal4) || (maxVal5 > maxVal6)); //compare the highest matching points between opened and closed eyes result 
                                                                                         //matrices
}
