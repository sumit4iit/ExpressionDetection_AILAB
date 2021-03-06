 #include "StdAfx.h"
 #include <opencv/cv.h>  
 #include <opencv/cvaux.h>  
 #include <opencv/highgui.h>  
 #include <stdio.h>  
 #include <string>  
 #include <fstream>  
 #include <sstream>  
 #include <dirent.h>  
 using namespace std;  
 IplImage* image=0;  
 IplImage* image2=0;  
 //int start_roi=0;
 int lp = 1443;
// int pp = 1;
 int roi_x0=0;  
 int roi_y0=0;  
 int roi_x1=0;  
 int roi_y1=0;  
 int numOfRec=0;  
 int startDraw = 0;  
 char* window_name="<SPACE>add <ENTER>save and load next <ESC>exit";  
 string IntToString(int num)  
 {  
      ostringstream myStream; //creates an ostringstream object  
      myStream << num << flush;  
      /*  
      * outputs the number into the string stream and then flushes  
      * the buffer (makes sure the output is put into the stream)  
      */  
      return(myStream.str()); //returns the string form of the stringstream object  
 };  
 void on_mouse(int event,int x,int y,int flag, void *param)  
 {  
   if(event==CV_EVENT_LBUTTONDOWN)  
   {  
           if(!startDraw)  
           {  
                roi_x0=x;  
                roi_y0=y;  
                startDraw = 1;  
           } else {  
                roi_x1=x;  
                roi_y1=y;  
                startDraw = 0;  
           }  
   }  
   if(event==CV_EVENT_MOUSEMOVE && startDraw)  
      {  
           //redraw ROI selection  
           image2=cvCloneImage(image);  
           cvRectangle(image2,cvPoint(roi_x0,roi_y0),cvPoint(x,y),CV_RGB(255,0,255),1);  
           cvShowImage(window_name,image2);  
           cvReleaseImage(&image2);  
   }  
 }  
  string argv[2];
 int main()  
 {  
	 cin >> argv[0] >> argv[1];
      int iKey=0;  
      string strPrefix;  
      string strPostfix;  
      string input_directory;  
      string output_file;  
      input_directory = argv[1];  
      output_file = argv[0];  
      /* Get a file listing of all files with in the input directory */  
      DIR     *dir_p = opendir (input_directory.c_str());  
      struct dirent *dir_entry_p;  
      if(dir_p == NULL) {  
           fprintf(stderr, "Failed to open directory %s\n", input_directory.c_str());  
           return -1;  
      }  
      fprintf(stderr, "Object Marker: Input Directory: %s Output File: %s\n", input_directory.c_str(), output_file.c_str());  
      //     init highgui  
      cvAddSearchPath(input_directory);  
      cvNamedWindow(window_name,1);  
      cvSetMouseCallback(window_name,on_mouse, NULL);  
      fprintf(stderr, "Opening directory...");  
      //     init output of rectangles to the info file  
      ofstream output(output_file.c_str());  
      fprintf(stderr, "done.\n");  
      while((dir_entry_p = readdir(dir_p)) != NULL)  
      {  
           numOfRec=0;  
           if(strcmp(dir_entry_p->d_name, ""))  
           fprintf(stderr, "Examining file %s\n", dir_entry_p->d_name);  
           strPostfix="";  
           strPrefix=input_directory;  
           strPrefix+=dir_entry_p->d_name;  
           //strPrefix+=bmp_file.name;  
           fprintf(stderr, "Loading image %s\n", strPrefix.c_str());  
           if((image=cvLoadImage(strPrefix.c_str(),1)) != 0)  
           {  

			   
                //     work on current image  
                do  
                {  
                     cvShowImage(window_name,image);  
                     // used cvWaitKey returns:  
                     //     <Enter>=13          save added rectangles and show next image  
                     //     <ESC>=27          exit program  
                     //     <Space>=32          add rectangle to current image  
                     // any other key clears rectangle drawing only  
                     iKey=cvWaitKey(0);
					 
                     switch(iKey)  
                     {  
                     case 27:  
                               cvReleaseImage(&image);  
                               cvDestroyWindow(window_name);  
                               return 0;  
                     case 32:  
                               numOfRec++;  
                               if(roi_x0<roi_x1 && roi_y0<roi_y1)  
                               {  

									 printf(" Pictures%d \t%d\t %d\t%d\t%d\t%d\n",lp,numOfRec,roi_x0,roi_y0,roi_x1-roi_x0,roi_y1-roi_y0);  
                                    // append rectangle coord to previous line content  
                                    strPostfix+=" "+IntToString(roi_x0)+" "+IntToString(roi_y0)+" "+IntToString(roi_x1-roi_x0)+" "+IntToString(roi_y1-roi_y0);  
									lp++;
                               }  
                               if(roi_x0>roi_x1 && roi_y0>roi_y1)  
                               {  
								   printf(" Pictures%d \t%d\t %d\t%d\t%d\t%d\n",lp,numOfRec,roi_x1,roi_y1,roi_x0-roi_x1,roi_y0-roi_y1);  
                                    // append rectangle coord to previous line content  
                                    strPostfix+=" "+IntToString(roi_x1)+" "+IntToString(roi_y1)+" "+IntToString(roi_x0-roi_x1)+" "+IntToString(roi_y0-roi_y1);  
									lp++;
									
                               }  
                               break;  
                     }  
                }  
                while(iKey!=13);  
                // save to info file as later used for HaarTraining:  
                //     <rel_path>\bmp_file.name numOfRec x0 y0 width0 height0 x1 y1 width1 height1...  
                if(numOfRec>0 && iKey==13)  
                {  
                     //append line  
                     output << strPrefix << " "<< numOfRec << strPostfix <<"\n";  
                }  
                cvReleaseImage(&image);  
           } else {  
                fprintf(stderr, "Failed to load image, %s\n", strPrefix.c_str());  
           }  
      }  
      output.close();  
      cvDestroyWindow(window_name);  
      closedir(dir_p);  
      return 0;  
 }  
