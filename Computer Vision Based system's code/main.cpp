#include "opencv2/objdetect.hpp"

#include "opencv2/highgui.hpp"

#include "opencv2/imgproc.hpp"

#include <iostream>
#include <iomanip> 
#include <stdlib.h>
#include <ctime>
#include <fstream>
#include "dAndC.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int menuChoice;
int timer=0;
int fd;

ofstream outfile;
Mat frame;
Mat tmps[12];
string ecgs;
stringstream gf[4];
int main(void)
{	std::ifstream infile("/home/bardawil/Desktop/integration/mac2/mac/ECG Integration.txt");
	face_cascade.load(face_cascade_name);
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };
	cout << "choose one of the following options"<<endl;
        cout << "1- Create Templates"<<endl;
        cout << "2- Load templates"<<endl;
        cout << "3- LBP Comparison"<<endl;
        cout << "4- Cross Correlation"<<endl;
        cout << "5- Exit"<<endl;
        cin >> menuChoice;
	do{
		switch(menuChoice){
			case 1:
				cout<<"you chose to create templates"<<endl;
				store();
				cout << "First, open your eyes please!" << endl;
				waitKey(1000);
				cap(1);
				cout << "Now, close your eyes please!" << endl;
				waitKey(1000);
				cap(2);
				waitKey(1000);
				dir();
				cout<<"Templates created successfully!"<<endl;
				for(int i = 0; i < 12; i++){
					tmps[i] = imread(dirnames[i].str(), CV_LOAD_IMAGE_GRAYSCALE);
				}
				destroyAllWindows();
				c.release();
				break;
			case 2:
				cout<<"you chose to load templates"<<endl;
				cout << "Set the path you wish to load your data from: " << endl;
				cin >> dirname;
				dir();
				for(int i = 0; i < 12; i++){
					tmps[i] = imread(dirnames[i].str(), CV_LOAD_IMAGE_GRAYSCALE);
				}
				c.release();
				break;
			case 3:
				cout<<"you chose LBP"<<endl; //linking error
				c.open(0);
				if(!c.isOpened()) 
					printf("--(!)Error opening video capture\n");
				c.set(CV_CAP_PROP_FRAME_WIDTH,640);
				c.set(CV_CAP_PROP_FRAME_HEIGHT,480);
			
				while (true){
					c.read(frame);
					if (frame.empty()){
						printf(" --(!) No captured frame -- Break!");
						break;
					}
					if(waitKey(10) >= 0){
						c.release();
						break;
					}
					imshow("Frame.jpg", frame);
				}
				destroyAllWindows();
				c.release();
				break;
			case 4:
				waitKey(10);
				getline(infile,ecgs);
				cout<<"you chose Cross Correlation"<<endl;
				c.open(0);
				if(!c.isOpened()) 
					printf("--(!)Error opening video capture\n");
				c.set(CV_CAP_PROP_FRAME_WIDTH,640);
				c.set(CV_CAP_PROP_FRAME_HEIGHT,480);			
				while (true){	
					c.read(frame);
					if (frame.empty()){
						printf(" --(!) No captured frame -- Break!");
						break;
					}
					if(waitKey(10) >= 0){
						c.release();
						break;
					}
					if (timer >= 240){
						getline(infile,ecgs);
						timer =0;
					}
					dAndC(tmps, face_cascade, 1, frame,ecgs);
					cout<<"  ecg "<<ecgs<<endl;
					timer++;
					//pthread_join(tha, NULL);
					imshow("Frame.jpg", frame);
				}   	
				destroyAllWindows();
				c.release();
				infile.close();
				break;
		}
		cout <<"Do you wish to continue? (1 if Yes, 0 if No)" << endl;
		cin >> menuChoice;
		if(menuChoice == 0) break;
		cout << "choose one of the following options"<<endl;
		cout << "1- Create Templates"<<endl;
		cout << "2- Load templates"<<endl;
		cout << "3- LBP Comparison"<<endl;
		cout << "4- Cross Correlation"<<endl;
		cout << "5- Exit"<<endl;
		cin >> menuChoice;
	}
	while(menuChoice != 5);
	if (!conditions.empty()){
		outfile.open("intergration test.CSV", std::ios_base::app);
        outfile << "\n"<< "\n"<<"Case" << ","<<"Alarm" << "," << "Duration" << "," << "Start time" << "," << "End time" << ","
        	    << "TP" << "," << "FP" << "," << "TN" << "," << "FN" << ",";
        for (int i = 0; i < conditions.size() - 1; i++){
			double duration1 = st1[i] - st0[i];
            if (conditions[i] == "Closed" && conditions2[i] == "yes" && duration1!=0){
                TP = 1;
                TN = 0;
                FP = 0;
                FN = 0;
                totTP++;
            }
            else if (conditions[i] == "Opened" && conditions2[i] == "yes") {
                TP = 0;
                TN = 0;
                FP = 1;
                FN = 0;
                totFP++;
            }
            else if (conditions[i] == "Closed" && conditions2[i] == "no" && duration1 > 2.5) {
                TP = 0;
                TN = 0;
                FP = 0;
                FN = 1;
                totFN++;
            }
            outfile << "\n" << conditions[i] << "," << conditions2[i] << "," << setprecision(4) << (st1[i] - st0[i]) << "," <<
             	   	setprecision(4) << st0[i] << "," << setprecision(4) << st1[i]<< "," << TP << "," << FP << "," << TN << "," << FN;
        }  
        outfile << "\n" << "TP" << "," << totTP << "\n" << "FP" << "," << totFP << "\n" << "TN" << "," << totTN <<
            	"\n" << "FN" << "," << totFN << "\n Precison" << "," << totTP / (totTP + totFP) * 100 << "% \n Recall " << "," << totTP / (totTP + totFN) * 100
            	<< "% \n Accuracy" << "," << (totTP + totTN) / (totTP + totFP + totFN + totTN) * 100 << "% \n";
        outfile.close();
 	}       
	return 0;
}
