 #include<iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include<fstream>


using namespace cv;
using namespace std;


/// Function headers
//void applyfilters();
void parametersMOG2(Ptr<BackgroundSubtractorMOG2>);
void drawpoly();
void CallBackFunc(int, int , int , int , void*);
void Drawpolygons();
void thresh_callback(int, void*);


///for contouring
int thresh = 100;
int max_thresh = 255;
//RNG rng(12345);
Mat src;
Mat sourcenew;


Point polypoints[10][4];
int number_of_polygons,vertice_count=0;
Mat frame;Mat mask;

int main()
{








Ptr<BackgroundSubtractorMOG2> pmog2;
pmog2=createBackgroundSubtractorMOG2(50,16,true);//creating background subtractor with shadow detection on



VideoCapture Cap("12.mp4");



 if(!Cap.isOpened())
 {
     return 10;
 }



parametersMOG2(pmog2);


///reading frame to draw polygon

if(Cap.read(frame))
{

    cout<<frame.cols<<" "<<frame.rows;
   drawpoly();
}
else
{

cout<<"Failed to draw polygon"<<endl;
 return -1;

}







///starting pmog2
if(waitKey(0)=='y')
{
namedWindow("original",WINDOW_NORMAL);
namedWindow("detecting",WINDOW_NORMAL);


while(1)
{



if(!Cap.read(frame))
{
    cout<<"failed"<<endl;
}
//cout<<frame.size;




Drawpolygons();

resize(frame,frame,Size(),0.3,0.3,CV_INTER_LINEAR);

//cvtColor(frame,frame,CV_BGR2GRAY);

bilateralFilter(frame,sourcenew,3,3,3,BORDER_DEFAULT);
for(int z=0;z<10;z++)
{

    blur(sourcenew,sourcenew,Size(3,3),Point(-1,-1));
}
//cvtColor(frame,frame,CV_BGR2GRAY);


    pmog2->apply(sourcenew,mask,-1);


dilate(mask,mask,Mat(),Point(-1,-1),3);



blur(src,src,Size(3,3),Point(-1,-1));

  //createTrackbar( " Canny thresh:","Contours", &thresh, max_thresh, thresh_callback );

 // cout<<"*";
  thresh_callback( 0, 0);
//cout<<"#";
    imshow("original",frame);
    imshow("detecting",mask);

  if(waitKey(5)==27)
        return -1;

}





return 0;
}
}











/*
void blobdetect()
{
SimpleBlobDetector detector;

r// Detect blobs.
vector<KeyPoint> keypoints;
detector.detect( frame,keypoints);

// Draw detected blobs as red circles.
// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
Mat im_with_keypoints;
drawKeypoints( mask, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

// Show blobs
imshow("keypoints", im_with_keypoints );

}*/






///Detect Mouse Click And Remove The noise By Making Polygons

void drawpoly()
{


namedWindow("select background",WINDOW_NORMAL);
imshow("select background",frame);


setMouseCallback("select background", CallBackFunc, NULL);

}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{


         if  ( event == EVENT_LBUTTONDOWN )
        {

          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
          polypoints[number_of_polygons][vertice_count].x=x;
          polypoints[number_of_polygons][vertice_count].y=y;
          cout<<polypoints[number_of_polygons][vertice_count].x<<"  "<<polypoints[number_of_polygons][vertice_count].y<<endl;
          vertice_count++;
        }


if(vertice_count==4)
{
vertice_count=0;
number_of_polygons++;

cout<<"number_of_polygons="<<number_of_polygons<<endl;


int lineType=8;
const Point* ppt[1] = { polypoints[number_of_polygons-1] };
int npt[] = { 4 };

  fillPoly(frame,ppt,npt,1,Scalar(0,0,0),lineType );

imshow("select background",frame);

}

}



void Drawpolygons()
{


int lineType=8;
  int npt[] = { 4 };

for(int i=0;i<number_of_polygons;i++)
{

    const Point* ppt[1] = { polypoints[i] };
fillPoly(frame,ppt,npt,1,Scalar(0,0,0),lineType );

}


}



/*
///Blur REMOVE THIS FUNCTION
void applyfilters(Mat frame)
{

blur(frame,frame,Size(3,3),Point(-1,-1));


}
*/


/// Tinkering with the parameters of MOG2 class
void parametersMOG2(Ptr<BackgroundSubtractorMOG2> pmog2)
{

cout<<"getBackgroundRatio"<<pmog2->getBackgroundRatio()<<endl;

pmog2->setBackgroundRatio(0.3);//double


cout<<"getComplexityReductionThreshold"<<pmog2->getComplexityReductionThreshold ()<<endl ;
pmog2->setComplexityReductionThreshold(0.05);   //double

cout<<"getDetectShadows"<<pmog2->getDetectShadows () <<endl;
//pmog2->setDetectShadows ();    //bool

cout<<"getHistory"<<pmog2->getHistory() <<endl;
//pmog2->setHistory(); //int

cout<<"getNMixtures"<<pmog2->getNMixtures()<<endl;
//pmog2->setNMixtures();//int

cout<<"getShadowThreshold "<<pmog2->getShadowThreshold ()<<endl;
pmog2->setShadowThreshold(0.00001);     //double

cout<<"getShadowValue ()"<<pmog2->getShadowValue ()<<endl;
pmog2->setShadowValue(0); //int

cout<<"getVarInit"<<pmog2->getVarInit ()<<endl;
pmog2->setVarInit(0.1);       //double

cout<<"getVarInit"<<pmog2->getVarInit ()<<endl;

cout<<"getVarMax "<<pmog2->getVarMax ()<<endl;
pmog2->setVarMax(1);          //double

cout<<"getVarMin "<<pmog2->getVarMin () <<endl;
pmog2->setVarMin(255);            //double

cout<<"getVarThreshold "<<pmog2->getVarThreshold ()<<endl;
pmog2->setVarThreshold(50);             //double

cout<<"getVarThresholdGen"<<pmog2->getVarThresholdGen ()<<endl;
//pmog2->setVarThresholdGen();  //double



}




///Contouring and Filling
void thresh_callback(int, void* )
{
    src=mask;

  Mat canny_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  vector<Point> center(100);

   int no_of_contours_in_this_critera=0;

   Rect bounding_rect;

  /// Detect edges using canny
  Canny( src, canny_output, thresh, thresh*2, 3 );

  dilate(canny_output,canny_output,Mat(),Point(-1,-1),1);

  /// Find contours
  findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0) );

  /// Draw contours
  Mat drawing = Mat::zeros( canny_output.size(), CV_8UC1 );

  vector<Moments> mu(1000);
  for( size_t i = 0; i< contours.size(); i++ )
{


    //   Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       double a=contourArea( contours[i],false);
       if(a>1000)
       {


 drawContours( drawing,contours,i,Scalar(255, 0, 0), CV_FILLED,LINE_8,hierarchy );

 bounding_rect=boundingRect(contours[i]);
 rectangle(frame, bounding_rect,  Scalar(0,255,0),6, 8,0);




mu[no_of_contours_in_this_critera]=moments(contours[i],true);
no_of_contours_in_this_critera++;


}



}



ofstream myfile("output.txt");
if(myfile.is_open())
{

myfile<<no_of_contours_in_this_critera<<"\n";


   vector<Point> mc(no_of_contours_in_this_critera);

  for( int i = 0; i < no_of_contours_in_this_critera; i++ )
    {

   ///center
   mc[i] = Point( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    circle(frame,mc[i],4,Scalar(255,0,0),-1,LINE_8,0);


  ///saving into file
  myfile<<center[i].x<<" "<<center[i].y<<"\n";
  }

}
myfile.close();

  /// Show in a window
  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
  threshold(drawing,drawing,1,255,0);

  imshow( "Contours", drawing );

}
