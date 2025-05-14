
#include "../Globals.hh"
#include "Obj.hh"

namespace MGE {
    
    enum class QuadOrientation {
        XY, XZ, YZ
    };

    class ObjQuad : public Obj {
    public:
        ObjQuad(glm::vec3 const& position, QuadOrientation const& orientation);
        ~ObjQuad();

        ObjQuad() = delete;
    };

}