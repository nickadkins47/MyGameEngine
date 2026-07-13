package MyGameEngine

import "base:intrinsics"

/*
Anything in the 3D environment is an Object.
IE it has a position & direction
*/
Object :: struct
{
    pos: [3]f32, //position
    dir: [3]f32, //direction (roll/x, pitch/y, yaw/z)
}

//TODO: use subtype polymorphism, Example:
/* foo :: proc(bar: $T) where intrinsics.type_is_subtype_of(T, Object)
{

} */