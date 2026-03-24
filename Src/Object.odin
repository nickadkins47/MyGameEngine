package MyGameEngine

/*
Anything in the 3D environment is an Object.
IE it has a position & direction
*/
Object :: struct
{
    pos: [3]f32, //position
    dir: [3]f32, //direction (roll/x, pitch/y, yaw/z)
}
