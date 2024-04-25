#include "Compare.h"
#include "FileStorage.h"
pthread_t thf, thp1, thp2, thl, thr, tha;

struct img *arg1, *arg2, *arg3;

Mat eyes[4];
    bool closestate = true, openstate = true;
    vector<string> conditions, conditions2;
    vector<double> duration, st0, st1, st2, st3;
    int TP = 0, FP = 0, TN = 0, FN = 0, alarmc1 =0,faceCounter = 0;
    double y, totTP = 0, totFP = 0, totFN = 0, totTN = 0, eyec = 0, fpsv, tncounter = 0,closecounter=0;

vector<Rect> faces;
VideoCapture c;
void dAndC(Mat tmps[12], CascadeClassifier face_cascade, int method, Mat &frame,string ecgs) //detection of the eyes for the 3 methods, state 
{	
	struct img args1, args2, args3;
	cvtColor(frame, frame, COLOR_BGR2GRAY); //convert real time frame to gray scale
	equalizeHist(frame, frame);
	face_cascade.detectMultiScale(frame, faces, 1.1, 10, CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0), cvSize(350, 350));
	if (faceCounter>=30){
	pthread_create(&tha, NULL, alarm, NULL);
        cout<<"Normal alarm 1 0 case"<<endl;
	}	
	if (faces.size() == 0){
		if (ecgs =="1"){
			pthread_create(&tha, NULL, alarm, NULL);
			cout<<"Integ alarm 1 1 case"<<endl;
			faceCounter = 0;
	}
	else{
		cout << "no face detected (Full face turn)" << endl; //no face is detected
		faceCounter++;
		}
	}
	else{
		faceCounter =0;
		tncounter++;

        Mat ROI1(frame, Rect(faces[0].x + faces[0].width / 5.2, faces[0].y + faces[0].height / 3, faces[0].width / 3.9, faces[0].height / 6));
		Mat ROI2(frame, Rect(faces[0].x + faces[0].width / 1.8, faces[0].y + faces[0].height / 3, faces[0].width / 3.68, faces[0].height / 6));

      	Point face_point1(faces[0].x + faces[0].width / 2.2, faces[0].y + faces[0].height / 1.9); //draw rectangles around the eyes
        Point face_point2(faces[0].x + faces[0].width / 5.5, faces[0].y + faces[0].height / 3.2);

        Point face_point3(faces[0].x + faces[0].width / 1.2, faces[0].y + faces[0].height / 1.9);
        Point face_point4(faces[0].x + faces[0].width / 1.9, faces[0].y + faces[0].height / 3.2);
        
		args3.frame = frame;
		args3.p1 = faces[0].tl();
		args3.p2 = faces[0].br();
		
		args1.frame = frame;
		args1.p1 = face_point1;
		args1.p2 = face_point2;
		
		args2.frame = frame;
		args2.p1 = face_point3;
		args2.p2 = face_point4;
				
		argl.img = ROI1;
		argl.tmpo1 = tmps[1];
		argl.tmpc1 = tmps[3];
		argl.tmpo2 = tmps[5];
		argl.tmpc2 = tmps[7];
		argl.tmpo3 = tmps[9];
		argl.tmpc3 = tmps[11];
		
		argr.img = ROI2;
		argr.tmpo1 = tmps[0];
		argr.tmpc1 = tmps[2];
		argr.tmpo2 = tmps[4];
		argr.tmpc2 = tmps[6];
		argr.tmpo3 = tmps[8];
		argr.tmpc3 = tmps[10];
    			
		imshow("tempo1.jpg",argl.tmpc1);
		imshow("tempo1.jpg",argr.tmpc1);
		pthread_create(&thf, NULL, detect, (void *) &args3);
		pthread_create(&thp1, NULL, detect, (void *) &args1);
		pthread_create(&thp2, NULL, detect, (void *) &args2);

		if(method == 1){
			pthread_create(&thl, NULL, crossCorrelate, (void *) &argl);
			pthread_create(&thr, NULL, crossCorrelate, (void *) &argr);	
		}
		pthread_join(thf, NULL);
		pthread_join(thp1, NULL);        		
		pthread_join(thp2, NULL);
		pthread_join(thl, &l);
		pthread_join(thr, &r);
				
		if(l && r){
	        tncounter++;
			cout << "Eyes are opened!" <<tncounter<< endl;
			if (openstate){
				conditions.push_back("Opened");
				openstate = false;
				st0.push_back((c.get(CAP_PROP_POS_MSEC) / 1000));
			}
			if (!closestate){
                closestate = true;
                st1.push_back((c.get(CAP_PROP_POS_MSEC) / 1000));
                if(closecounter>=50)
                	conditions2.push_back("yes");
                else
                	conditions2.push_back("no");
                closecounter=0;
			}
			closecounter=0;
			if(tncounter>=70){
				totTN++;
				tncounter=0;
			}
		}
		else{
			if (ecgs  =="1"){
				pthread_create(&tha, NULL, alarm, NULL);
				cout<<"Integration alarm, 1 1 case"<<endl;
				closecounter = 0;
			}
     		cout << "Eyes are closed" << endl;
     		closecounter++;
     		tncounter=0;
     		if (!openstate){
				conditions2.push_back("no");
                openstate = true;
                st1.push_back((c.get(CAP_PROP_POS_MSEC) / 1000));
			}
			if (closecounter>=30){
				cout<<"normal alarm, 1 0 case"<<endl;
				pthread_create(&tha, NULL, alarm, NULL);
			}
			if (closestate){
				conditions.push_back("Closed");
				closestate = false;
				st0.push_back((c.get(CAP_PROP_POS_MSEC) / 1000));
			}
  		}
  	}       	
}
