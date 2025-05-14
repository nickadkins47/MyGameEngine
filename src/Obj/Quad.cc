
#include "Quad.hh"

namespace MGE {

    ObjQuad::ObjQuad(Vec<3> const& position, QuadOrientation const& orientation) {
        switch (orientation) {
            case QuadOrientation::XY: {
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{0.0f, 1.0f, 0.0f}, position + Vec<3>{1.0f, 1.0f, 0.0f}
                });
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{1.0f, 1.0f, 0.0f}, position + Vec<3>{1.0f, 0.0f, 0.0f}
                });
                break;
            }
            case QuadOrientation::XZ: {
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{0.0f, 0.0f, 1.0f}, position + Vec<3>{1.0f, 0.0f, 1.0f}
                });
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{1.0f, 0.0f, 1.0f}, position + Vec<3>{1.0f, 0.0f, 0.0f}
                });
                break;
            }
            case QuadOrientation::YZ: {
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{0.0f, 1.0f, 0.0f}, position + Vec<3>{0.0f, 1.0f, 1.0f}
                });
                mesh.push_back({
                    position + Vec<3>{0.0f, 0.0f, 0.0f}, position + Vec<3>{0.0f, 1.0f, 1.0f}, position + Vec<3>{0.0f, 0.0f, 1.0f}
                });
                break;
            }
        }
    }

    ObjQuad::~ObjQuad() {}

}