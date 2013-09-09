#include <QCoreApplication>
#include <QDir>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <stdio.h>
#include <iostream>
#if CV_MAJOR_VERSION >= 2 && CV_MINOR_VERSION >= 4
  #include <opencv2/nonfree/features2d.hpp>
#endif
//#include <opencv2/nonfree/features2d.hpp>
#include "opencv2/nonfree/nonfree.hpp"
using namespace std;
using namespace cv;
#define NO_IMAGES 10

/**
 * @brief rotateImage
 * @param source
 * @param angle
 *
 * Rotate image by angle(degrees) in anticlockwise direction
 * @return rotated image
 */
Mat rotateImage(const Mat& source, double angle)
{
    Point2f src_center(source.cols/2.0F, source.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
    Mat dst;
    warpAffine(source, dst, rot_mat, source.size());
    return dst;
}

/**
 * @brief scaleImage
 * @param source
 * @param scaleX
 * @param scaleY
 * @return scaled image
 */
Mat scaleImage(const Mat& source, double scaleX, double scaleY)
{
    Mat dst;
    resize(source, dst, Size(0,0)/*new dimensions*/,scaleX, scaleY, INTER_CUBIC/* interpolation method*/);
    return dst;
}

Mat affineTransform(Mat src, int xSrc[], int ySrc[], float xDest[], float yDest[])
{
    char* source_window = "Source image";
    char* warp_window = "Warp";
    //char* warp_rotate_window = "Warp + Rotate";
    Point2f srcTri[3];
    Point2f dstTri[3];

      // Mat rot_mat( 2, 3, CV_32FC1 );
       Mat warp_mat( 2, 3, CV_32FC1 );
       Mat  warp_dst;
       //Mat warp_rotate_dst;

       /// Load the image
       //src = imread( argv[1], 1 );
//        src = imread("E://lena.jpg",1);
       /// Set the dst image the same type and size as src
       warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

       /// Set your 3 points to calculate the  Affine Transform
       srcTri[0] = Point2f( xSrc[0],ySrc[0] );
       //srcTri[1] = Point2f( src.cols - 1, 0 );
       //srcTri[2] = Point2f( 0, src.rows - 1 );
       srcTri[1] = Point2f( xSrc[1], ySrc[1] );
       srcTri[2] = Point2f( xSrc[2], ySrc[2] );


//       dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
//       dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
//       dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );
       dstTri[0] = Point2f( src.cols*xDest[0], src.rows*yDest[0] );
       dstTri[1] = Point2f( src.cols*xDest[1], src.rows*yDest[1] );
       dstTri[2] = Point2f( src.cols*xDest[2], src.rows*yDest[2] );

//       dstTri[0] = Point2f( src.cols*0.2, src.rows*0.0 );
//       dstTri[1] = Point2f( src.cols*1.2, src.rows-1 );
//       dstTri[2] = Point2f( src.cols*0.2, src.rows*0.0 );

       /// Get the Affine Transform
       warp_mat = getAffineTransform( srcTri, dstTri );


       /// Apply the Affine Transform just found to the src image
       warpAffine( src, warp_dst, warp_mat, warp_dst.size() );

       return warp_dst;
       /** Rotating the image after Warp */

       /// Compute a rotation matrix with respect to the center of the image
      /* Point center = Point( warp_dst.cols/2, warp_dst.rows/2 );
       double angle = -50.0;
       double scale = 0.6;

       /// Get the rotation matrix with the specifications above
       rot_mat = getRotationMatrix2D( center, angle, scale );

       /// Rotate the warped image
       warpAffine( warp_dst, warp_rotate_dst, rot_mat, warp_dst.size() );
*/
       /// Show what you got
/*       namedWindow( source_window, CV_WINDOW_AUTOSIZE );
       imshow( source_window, src );

       namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
       imshow( warp_window, warp_dst );
       imwrite("affine.jpg", warp_dst);*/

  //     namedWindow( warp_rotate_window, CV_WINDOW_AUTOSIZE );
    //   imshow( warp_rotate_window, warp_rotate_dst );
    //return src;
         // trans_mat = getTranslationMatrix2d(2, 2)
}

Mat perspectiveTransform(Mat src, float xDest[], float yDest[])
{
    char* source_window = "Source image";
    char* warp_window = "Warp";

       Point2f srcTri[4];
       Point2f dstTri[4];

       Mat warp_mat( 3, 3, CV_32FC1 );
       Mat  warp_dst;

       /// Load the image
       //src = imread( argv[1], 1 );
//        src = imread("E://lena.jpg",1);
       /// Set the dst image the same type and size as src
       warp_dst = Mat::zeros( src.rows, src.cols, src.type() );

       /// Set your 3 points to calculate the  Affine Transform
//       srcTri[0] = Point2f( 0,0 );
//       srcTri[1] = Point2f( src.cols - 1, 0 );
//       srcTri[2] = Point2f( 0, src.rows - 1 );
//       srcTri[3] = Point2f( src.cols -1 , src.rows - 1 );

       srcTri[0] = Point2f( 0,0 );
       srcTri[1] = Point2f( (src.cols - 1)/2, 0 );
       srcTri[2] = Point2f( src.cols-1,0 );
       srcTri[3] = Point2f( src.cols -1 , src.rows - 1 );

//       dstTri[0] = Point2f( src.cols*0.0, src.rows*0.33 );
//       dstTri[1] = Point2f( src.cols*0.85, src.rows*0.25 );
//       dstTri[2] = Point2f( src.cols*0.15, src.rows*0.7 );
//       dstTri[3] = Point2f( src.cols*0.85, src.rows*0.7 );


       dstTri[0] = Point2f( src.cols*xDest[0], src.rows*yDest[0] );
       dstTri[1] = Point2f( src.cols*xDest[1], src.rows*yDest[1] );
       dstTri[2] = Point2f( src.cols*xDest[2], src.rows*yDest[2] );
       dstTri[3] = Point2f( src.cols*xDest[3], src.rows*yDest[3] );

//       dstTri[0] = Point2f( src.cols*0.2, src.rows*0.0 );
//       dstTri[1] = Point2f( src.cols*1.2, src.rows-1 );
//       dstTri[2] = Point2f( src.cols*0.2, src.rows*0.0 );
//       dstTri[3] = Point2f( src.cols*1.2, src.rows -1 );

       /// Get the Affine Transform
       warp_mat = getPerspectiveTransform( srcTri, dstTri );

       /// Apply the Affine Transform just found to the src image
       warpPerspective( src, warp_dst, warp_mat, warp_dst.size() );
       return warp_dst;
       /// Show what you got
      // namedWindow( source_window, CV_WINDOW_AUTOSIZE );
      // imshow( source_window, src );

      // namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
      // imshow( warp_window, warp_dst );
     //  imwrite("perspective.jpg", warp_dst);
}

void generateRotatedImages()
{
    for(int i =1; i <= NO_IMAGES; i++)
    {
        QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference\\%1.jpg").arg(i);
        Mat m = imread(str.toStdString());
        for(int j = 10; j <=50; j = j+10)
        {
            Mat r = rotateImage(m,j);
            QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\rotated\\%1_%2.jpg").arg(i).arg(j/10);
            imwrite(str1.toStdString(),r);
        }

        Mat r1 = rotateImage(m,90);
        QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\rotated\\%1_%2.jpg").arg(i).arg(6);
        imwrite(str1.toStdString(),r1);

        Mat r2 = rotateImage(m,180);
        QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\rotated\\%1_%2.jpg").arg(i).arg(7);
        imwrite(str2.toStdString(),r2);

    }
    cout << "Finished generating rotated images"<<endl;
}

void generateAffineImages()
{
     int xSrc[3], ySrc[3]; float xDest[3], yDest[3];

    for(int i = 1; i <= NO_IMAGES; i++)
    {
        QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference\\%1.jpg").arg(i);
        Mat m = imread(str.toStdString());

        xSrc[0] = 0; xSrc[1] = m.cols-1; xSrc[2] = 0;
        ySrc[0] = 0; ySrc[1] = 0; ySrc[2] = m.rows-1;

        xDest[0] = 0; xDest[1] = 0.9; xDest[2] = 0;
        yDest[0] = 0.1; yDest[1] = 0.1; yDest[2] = 0.9;
        Mat r1 = affineTransform(m,xSrc,ySrc,xDest,yDest);
        QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine\\%1_1.jpg").arg(i);
        imwrite(str1.toStdString(), r1);

        xDest[0] = 0.3; xDest[1] = 0.7; xDest[2] = 0.3;
        yDest[0] = 0.3; yDest[1] = 0.3; yDest[2] = 0.7;
        Mat r2 = affineTransform(m,xSrc,ySrc,xDest,yDest);
        QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine\\%1_2.jpg").arg(i);
        imwrite(str2.toStdString(), r2);


        xDest[0] = 0.0; xDest[1] = 1.0; xDest[2] = 0.0;
        yDest[0] = 0.2; yDest[1] = 0.2; yDest[2] = 0.9;
        Mat r3 = affineTransform(m,xSrc,ySrc,xDest,yDest);
        QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine\\%1_3.jpg").arg(i);
        imwrite(str3.toStdString(), r3);

        xDest[0] = 0.5; xDest[1] = 1.0; xDest[2] = 0.0;
        yDest[0] = 0.0; yDest[1] = 0.5; yDest[2] = 0.5;
        Mat r4 = affineTransform(m,xSrc,ySrc,xDest,yDest);
        QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine\\%1_4.jpg").arg(i);
        imwrite(str4.toStdString(), r4);

        xDest[0] = 0.0; xDest[1] = 0.8; xDest[2] = 0.2;
        yDest[0] = 0.4; yDest[1] = 0.2; yDest[2] = 0.7;
        Mat r5 = affineTransform(m,xSrc,ySrc,xDest,yDest);
        QString str5 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine\\%1_5.jpg").arg(i);
        imwrite(str5.toStdString(), r5);

    }
    cout << "Finished generating affine images"<<endl;
}

void generatePerspectiveImages()
{
    float xDest[4], yDest[4];

    for(int i = 1; i <= NO_IMAGES; i++)
    {
        QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference\\%1.jpg").arg(i);
        Mat m = imread(str.toStdString());

        xDest[0] = 0; xDest[1] = 0.9; xDest[2] = 0; xDest[3] = 0.9;
        yDest[0] = 0.1; yDest[1] = 0.1; yDest[2] = 0.9; yDest[3] = 0.9;

        Mat r1 = perspectiveTransform(m,xDest,yDest);
        QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective\\%1_1.jpg").arg(i);
        imwrite(str1.toStdString(), r1);

        xDest[0] = 0.3; xDest[1] = 0.7; xDest[2] = 0.3; xDest[3] = 0.7;
        yDest[0] = 0.3; yDest[1] = 0.3; yDest[2] = 0.7; yDest[3] = 0.7;

        Mat r2 = perspectiveTransform(m,xDest,yDest);
        QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective\\%1_2.jpg").arg(i);
        imwrite(str2.toStdString(), r2);

        xDest[0] = 0.0; xDest[1] = 1.0; xDest[2] = 0.0; xDest[3] = 1.0;
        yDest[0] = 0.2; yDest[1] = 0.2; yDest[2] = 0.9; xDest[3] = 0.9;

        Mat r3 = perspectiveTransform(m,xDest,yDest);
        QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective\\%1_3.jpg").arg(i);
        imwrite(str3.toStdString(), r3);

        xDest[0] = 0.5; xDest[1] = 1.0; xDest[2] = 0.0; xDest[3] = 0.5;
        yDest[0] = 0.0; yDest[1] = 0.5; yDest[2] = 0.5; yDest[3] = 1.0;

        Mat r4 = perspectiveTransform(m,xDest,yDest);
        QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective\\%1_4.jpg").arg(i);
        imwrite(str4.toStdString(), r4);

        xDest[0] = 0.0; xDest[1] = 0.8; xDest[2] = 0.2; xDest[3] = 0.7;
        yDest[0] = 0.4; yDest[1] = 0.2; yDest[2] = 0.7; yDest[3] = 0.6;
        Mat r5 = perspectiveTransform(m,xDest,yDest);
        QString str5 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective\\%1_5.jpg").arg(i);
        imwrite(str5.toStdString(), r5);
    }
    cout << "Finished generating perspective images"<<endl;
}

void generateSimilarityImages()
{
    for(int i = 1; i <= NO_IMAGES; i++)
    {
        for(int j = 1; j <=7; j++)
        {
            QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\rotated\\%1_%2.jpg").arg(i).arg(j);
            Mat m = imread(str.toStdString());

            QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\similarity\\%1_%2_%3.jpg").arg(i).arg(j).arg(1);
            imwrite(str1.toStdString(),scaleImage(m,0.25,0.25));

            QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\similarity\\%1_%2_%3.jpg").arg(i).arg(j).arg(2);
            imwrite(str2.toStdString(),scaleImage(m,0.5,0.5));

            QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\similarity\\%1_%2_%3.jpg").arg(i).arg(j).arg(3);
            imwrite(str3.toStdString(),scaleImage(m,2,2));

            QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\similarity\\%1_%2_%3.jpg").arg(i).arg(j).arg(4);
            imwrite(str4.toStdString(),scaleImage(m,4,4));

        }
    }
    cout << "Finished generating similarity images"<<endl;
}

void generateScaledImages()
{
    for(int i = 1; i <= NO_IMAGES; i++)
    {
        QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference\\%1.jpg").arg(i);
        Mat m = imread(str.toStdString());

        QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(1);
        imwrite(str1.toStdString(), scaleImage(m,0.125,0.125));


        QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(2);
        imwrite(str2.toStdString(), scaleImage(m,0.25,0.25));


        QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(3);
        imwrite(str3.toStdString(), scaleImage(m,0.5,0.5));


        QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(4);
        imwrite(str4.toStdString(), scaleImage(m,0.75,0.75));


        QString str5 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(5);
        imwrite(str5.toStdString(), scaleImage(m,2,2));

        QString str6 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\scaled\\%1_%2.jpg").arg(i).arg(6);
        imwrite(str6.toStdString(), scaleImage(m,4,4));

    }
    cout << "Finished generating scaled images"<<endl;
}

void rescaleReferenceImages()
{
    for(int i = 1; i <= NO_IMAGES; i++)
    {
        QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference1\\%1.jpg").arg(i);
        Mat m = imread(str.toStdString());
        Mat r = scaleImage(m,0.5,0.5);
        QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\reference\\%1.jpg").arg(i);
        imwrite(str1.toStdString(), r);
    }
    cout << "Finished generation of reference images"<<endl;
}

void generateDemonstrationImage()
{
     int xSrc[3], ySrc[3]; float xDest[3], yDest[3];
    // affine
    QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\a.jpg");
    Mat m = imread(str.toStdString());

    xSrc[0] = 0; xSrc[1] = m.cols-1; xSrc[2] = 0;
    ySrc[0] = 0; ySrc[1] = 0; ySrc[2] = m.rows-1;

    xDest[0] = 0; xDest[1] = 0.9; xDest[2] = 0;
    yDest[0] = 0.1; yDest[1] = 0.1; yDest[2] = 0.9;
    Mat r1 = affineTransform(m,xSrc,ySrc,xDest,yDest);
    QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine_1.jpg");
    imwrite(str1.toStdString(), r1);

    xDest[0] = 0.3; xDest[1] = 0.7; xDest[2] = 0.3;
    yDest[0] = 0.3; yDest[1] = 0.3; yDest[2] = 0.7;
    Mat r2 = affineTransform(m,xSrc,ySrc,xDest,yDest);
    QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine_2.jpg");
    imwrite(str2.toStdString(), r2);


    xDest[0] = 0.0; xDest[1] = 1.0; xDest[2] = 0.0;
    yDest[0] = 0.2; yDest[1] = 0.2; yDest[2] = 0.9;
    Mat r3 = affineTransform(m,xSrc,ySrc,xDest,yDest);
    QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine_3.jpg");
    imwrite(str3.toStdString(), r3);

    xDest[0] = 0.5; xDest[1] = 1.0; xDest[2] = 0.0;
    yDest[0] = 0.0; yDest[1] = 0.5; yDest[2] = 0.5;
    Mat r4 = affineTransform(m,xSrc,ySrc,xDest,yDest);
    QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine_4.jpg");
    imwrite(str4.toStdString(), r4);

    xDest[0] = 0.0; xDest[1] = 0.8; xDest[2] = 0.2;
    yDest[0] = 0.4; yDest[1] = 0.2; yDest[2] = 0.7;
    Mat r5 = affineTransform(m,xSrc,ySrc,xDest,yDest);
    QString str5 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\affine_5.jpg");
    imwrite(str5.toStdString(), r5);
}

void generateDemonstrationImagePerspective()
{
    float xDest[4], yDest[4];
    QString str = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\a.jpg");
    Mat m = imread(str.toStdString());

    xDest[0] = 0; xDest[1] = 0; xDest[2] = 0.9; xDest[3] = 0.9;
    yDest[0] = 0.1; yDest[1] = 0.4; yDest[2] = 0.1; yDest[3] = 0.9;

    Mat r1 = perspectiveTransform(m,xDest,yDest);
    QString str1 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective_1.jpg");
    imwrite(str1.toStdString(), r1);
    cout << r1;

    xDest[0] = 0.3; xDest[1] = 0.7; xDest[2] = 0.3; xDest[3] = 0.7;
    yDest[0] = 0.3; yDest[1] = 0.3; yDest[2] = 0.7; yDest[3] = 0.7;

    Mat r2 = perspectiveTransform(m,xDest,yDest);
    QString str2 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective_2.jpg");
    imwrite(str2.toStdString(), r2);

    xDest[0] = 0.0; xDest[1] = 1.0; xDest[2] = 0.0; xDest[3] = 1.0;
    yDest[0] = 0.2; yDest[1] = 0.2; yDest[2] = 0.9; xDest[3] = 0.9;

    Mat r3 = perspectiveTransform(m,xDest,yDest);
    QString str3 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective_3.jpg");
    imwrite(str3.toStdString(), r3);

    xDest[0] = 0.5; xDest[1] = 1.0; xDest[2] = 0.0; xDest[3] = 0.5;
    yDest[0] = 0.0; yDest[1] = 0.5; yDest[2] = 0.5; yDest[3] = 1.0;

    Mat r4 = perspectiveTransform(m,xDest,yDest);
    QString str4 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective1_4.jpg");
    imwrite(str4.toStdString(), r4);

    xDest[0] = 0.0; xDest[1] = 0.8; xDest[2] = 0.2; xDest[3] = 0.7;
    yDest[0] = 0.4; yDest[1] = 0.2; yDest[2] = 0.7; yDest[3] = 0.6;
    Mat r5 = perspectiveTransform(m,xDest,yDest);
    QString str5 = QString("J:\\iit\\3rd sem\\eel806_vision\\project\\dataset\\perspective_5.jpg");
    imwrite(str5.toStdString(), r5);
}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    Mat a = imread("E://lena.jpg",1);
    Mat d = rotateImage(a, 20);
    Mat s;
    //s = scaleImage(d, 1/2, 1/2);
    //QDir dir("J:\iit\3rd sem\eel806_vision\project\dataset\reference");
    rescaleReferenceImages();
    generateRotatedImages();
    generateAffineImages();
    generatePerspectiveImages();
    generateScaledImages();
    generateSimilarityImages();

   // cv::namedWindow("Output");
    //cv::imshow("Output", s);
    //imwrite("rotatedImage.jpg", d);
    //imwrite("rigidbodyimage.jpg",s);
    //affineTransform(a);
    //perspectiveTransform(a);
   // affineTransform(a);
    cvWaitKey(0);

   return 0;
    //return a.exec();
}
