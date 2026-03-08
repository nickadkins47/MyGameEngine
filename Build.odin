package main_build
/*
    Lazily thown-together basic "build system" for my project
    Use "odin run . --" then add 'b' flag to build, and/or
    'r' flag to run. Alternatively, use 'c' to clean the Build dir

    Didnt feel like writing seperate shell/batch scripts, plus didnt
    want to make the use seperately require python or gnu make etc

    NOTE: Changes directory to ./Build during building
*/

//===================================
// Config

target_dir  :: "Build"
target_file :: "MyGameEngine.exe"

build_flags :: `
    -keep-executable
    -thread-count:4
`

//===================================

import "core:fmt"
import "core:os"

main :: proc() {
    if !os.exists("Build") do os.mkdir("Build")
    os.chdir(target_dir)

    for arg in os.args do switch arg {
        case "b": exec("odin build ../Src -out:"+target_file+" "+build_flags)
        case "r": exec("./"+target_file)
        case "c": exec("rm "+target_file)
    }
}

exec :: proc(command: string) {
    //fmt.println("DBG: command:", command)

    p, e1 := os.process_start({
        command = {"bash", "-c", command},
        stdin = os.stdin,
        stdout = os.stdout,
        env = nil
    })
    if e1 != nil {
        fmt.println("BUILD ERROR:", e1)
    }

    _, e2 := os.process_wait(p)
    if e2 != nil {
        fmt.println("BUILD ERROR:", e2)
    }
}