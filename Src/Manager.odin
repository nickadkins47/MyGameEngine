package MyGameEngine

import "core:fmt"
import "base:intrinsics"

/* MngIndex :: u32 //Index type for data stored in managed maps

@(private="file") camera_map    : map[MngIndex]Camera
@(private="file") modeltype_map : map[MngIndex]ModelType
@(private="file") modelobj_map  : map[MngIndex]ModelObj
//@(private="file") shader_map    : map[MngIndex]
@(private="file") texture_map   : map[MngIndex]Texture

//Create new value in the specified map, any index
//
//Returns pointer to new value, or nil if it failed to do so
mng_add :: proc(m: ^map[MngIndex]$T) -> ^T
{
    for i in min(MngIndex) ..< max(MngIndex) {
        if i not_in m {
            return map_insert(m, i, T{})
        }
    }
    return nil
}

//proc == "delete_key(m, index)"
mng_remove :: proc(m: ^map[MngIndex]$T, index: MngIndex)
{
    delete_key(m, index)
}

//proc == "index in m"
mng_exists :: proc(m: ^map[MngIndex]$T, index: MngIndex) -> bool
{
    return index in m
}

//Returns pointer to m[index], or nil if value doesnt exist.
//
//Does not create a new value at m[index] when called
mng_get :: proc(m: ^map[MngIndex]$T, index: MngIndex) -> ^T
{
    if index in m {
        _,v,_,_ := map_entry(m, index)
        return v
    } else {
        return nil
    }
}
 */