
#pragma once

#include "Globals.hh"
#include "Keyboard.hh"

namespace MGE {
    
    class Camera {
    public:
        Camera();
        ~Camera();

        deleteOtherOps(Camera)

        void update();
        void updateCamAngle(Float xrel, Float yrel);

        Mat<4,4> getVPMat() const;

        void setProjMat(Float fov, Float window_width, Float window_height, Float near_z, Float far_z);
        
        Keyboard* kbdPtr = nullptr;

    //protected:
        Mat<4,4> projMat {0};

        Vec<3> pos {0,0,0}; //camera's position in x/y/z
        Vec<3> angle {0,0,0}; //camera's angle in pitch/yaw/roll

        Vec<3> lookDirF {0};
        Vec<3> lookDirL {0};
        Vec<3> lookDirU {0};

    };

}