
#pragma once

#include "../Globals.hh"

namespace MGE {

    class Obj {
    public:
        Obj();
        ~Obj();

    //protected:
        Vector<Triangle<3>> mesh {};

    };

}