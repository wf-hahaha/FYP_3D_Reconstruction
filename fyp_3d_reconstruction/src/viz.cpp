/*
 * viz.cpp
 *
 *  Created on: Mar 2, 2016
 *      Author: xsunaf
 */
#include "viz.h"
#include <iostream>

void r2q( cv::Mat _R[] ,Eigen::Quaterniond q[], int num ){

    Eigen::Matrix3d R_eigen;
    for(int i=0;i<num;i++){
        cv::cv2eigen(_R[i],R_eigen);
        q[i]=R_eigen;
        q[i].normalize();
//        std::cout << "Quaternion"<<i<<": "<< std::endl << "w: "<<q[i].w()<<std::endl<< std::endl<<q[i].vec() << std::endl;
    }
}

void invertpose(const cv::Mat R[], const cv::Mat t[],cv:: Mat _R[], cv::Mat _t[], int num) {
    for(int i = 0;i<num;i++){
        // Obtain inverse:
        _R[i] = R[i].inv();
        _t[i] = -1 * _R[i] * t[i];
//	    std::cout << "Rotation Matrix"<<i<< ": " << std::endl <<" "<< R[i]<<std::endl;
//	    std::cout << "__Rotation Matrix"<<i<< ": " << std::endl <<" "<< _R[i]<<std::endl;
//	    std::cout << "Translation"<<i<< ": " << std::endl<<" "<<t[i]<<std::endl;
//	    std::cout << "__Translation"<<i<< ": " << std::endl<<" "<<_t[i]<<std::endl;
    }

}


void viz(cv::Mat points3D,ros::Publisher& pub_pts,ros::Publisher& pub_cam,const cv::Mat t[], const cv::Mat R[], Eigen::Quaterniond q[],int num){

        cv::Mat _R[num],_t[num]; 			// rotation matrices, cam translations
        invertpose(R,t,_R,_t, num);
        r2q(_R,q, num);

	 	visualization_msgs::Marker pts_array;  // cam_1, cam_2;

        pts_array.header.frame_id = "/world_frame";
        /////////////////////////////////////////////////////////////////////////////
        visualization_msgs::Marker cam[num];
        for(int i = 0;i<num;i++){
        	cam[i].header.frame_id = "/world_frame";
        }

        //cam_1.header.frame_id = "/world_frame";
        //cam_2.header.frame_id = "/world_frame";
        ////////////////////////////////////////////////////////////////////////////////////
        pts_array.header.stamp = ros::Time::now();
        for(int i = 0;i<num;i++){
        	cam[i].header.stamp = ros::Time::now();
        }
        //cam_1.header.stamp = ros::Time::now();
        //cam_2.header.stamp = ros::Time::now();
        /////////////////////////////////////////////////////
        pts_array.ns = "points_and_cameras";
        for(int i = 0;i<num;i++){
        	cam[i].ns = "points_and_cameras";
        }
        //cam_1.ns = "points_and_cameras";
        //cam_2.ns = "points_and_cameras";
        ////////////////////////////////////////////////////////
        pts_array.action = visualization_msgs::Marker::ADD;
        for(int i = 0;i<num;i++){
        	cam[i].action = visualization_msgs::Marker::ADD;
        }
        //cam_1.action = visualization_msgs::Marker::ADD;
        //cam_2.action = visualization_msgs::Marker::ADD;
        ////////////////////////////////////////////////////////////////
        pts_array.id = 0;
        for(int i = 0;i<num;i++){
        	cam[i].id =i + 1;
        }
        //cam_1.id = 1;
        //cam_2.id = 2;
        ///////////////////////////////////////////////////////////////////
        pts_array.type = visualization_msgs::Marker::POINTS;
        for(int i = 0;i<num;i++){
        	cam[i].type = visualization_msgs::Marker::ARROW;
        }
        //cam_1.type = visualization_msgs::Marker::ARROW;
        //cam_2.type = visualization_msgs::Marker::ARROW;
        /////////////////////////////////////////////////////////////////
        pts_array.scale.x = 0.04;
        pts_array.scale.y = 0.04;
        pts_array.scale.z = 0.04;

        for(int i = 0;i<num;i++){
        	cam[i].scale.x = 0.2;
        	cam[i].scale.y = 0.2;
        	cam[i].scale.z = 0.2;
        }
        //cam_1.scale.x = 0.1;
        //cam_1.scale.y = 0.1;
        //cam_1.scale.z = 0.1;
        //cam_2.scale.x = 0.1;
        //cam_2.scale.y = 0.1;
        //cam_2.scale.z = 0.1;
        //////////////////////////////////////////////////////////////////
// TODO: Here all the points colors are in red. If possible change them according to their pixel value in the photos.
//
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

        pts_array.color.a = 1.0;
        pts_array.color.r = 1.0;
        pts_array.color.g = 0.0;
        pts_array.color.b = 0.0;

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
        for(int i = 0;i<num;i++){
        	cam[i].color.a = 1.0;
        	cam[i].color.r = 0.0;
        	cam[i].color.g = 0.0;
        	cam[i].color.b = 1.0;
        }
        //cam_1.color.a = 1.0;
        //cam_1.color.r = 0.0;
        //cam_1.color.g = 0.0;
        //cam_1.color.b = 1.0;
        //cam_2.color.a = 1.0;
        //cam_2.color.r = 0.0;
        //cam_2.color.g = 0.0;
        //cam_2.color.b = 1.0;

        /* cameras are initialized with Markers::pose */

        for(int i = 0;i<num;i++){
            cam[i].pose.position.x = _t[i].at<float>(0);
            cam[i].pose.position.y = _t[i].at<float>(1);
            cam[i].pose.position.z = _t[i].at<float>(2);
            cam[i].pose.orientation.x = q[i].x();
            cam[i].pose.orientation.y = q[i].y();
            cam[i].pose.orientation.z = q[i].z();
            cam[i].pose.orientation.w = q[i].w();
        }

        //cam_2.pose.position.x = _t[0].at<double>(0);
        //cam_2.pose.position.y = _t[0].at<double>(1);
        //cam_2.pose.position.z = _t[0].at<double>(2);


        //cam_2.pose.orientation.x = q[0].x();
        //cam_2.pose.orientation.y = q[0].y();
        //cam_2.pose.orientation.z = q[0].z();
        //cam_2.pose.orientation.w = q[0].w();

        /* points are initialized with geometry_msgs::Point */
/*
        for (int i=0;i<n_matches[0];i++)
        {
            geometry_msgs::Point p;
            p.x = points3D.at<float>(0,i);			//no double here
            p.y = points3D.at<float>(1,i);
            p.z = points3D.at<float>(2,i);
            pts_array.points.push_back(p);
        }
*/
        	int inlier_count = 0;
            int outlier_count = 0;
            for (int i=0;i<points3D.cols;i++)
            {
                    geometry_msgs::Point p;

                    p.x = points3D.at<float>(0,i);
                    p.y = points3D.at<float>(1,i);
                    p.z = points3D.at<float>(2,i);
                    pts_array.points.push_back(p);
                    inlier_count++;
            }

        pub_pts.publish(pts_array);

        for(int i = 0;i<num;i++){
        	pub_cam.publish(cam[i]);
        	//std::cout<<i<<std::endl;
        }
        //pub_cam.publish(cam_1);
        //pub_cam.publish(cam_2);

    }


