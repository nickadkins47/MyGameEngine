
set_project("MyGameEngine")
set_version("0.0.0")
set_xmakever("2.9.9")

add_requires("libsdl3 3.2.10")
add_requires("glm 1.0.1")
add_requires("glbinding 3.5.0")
--add_requires("vulkansdk", {system = true})

target("main")
    set_kind("binary")
    set_basename("MyGameEngine")
    add_files("src/**.cc")
    set_languages("c++23")
    add_packages("libsdl3", "glm", "glbinding")

    --add_files("shaders/*.vert", "shaders/*.frag")
    --add_rules("utils.glsl2spv", {outputdir = "build/shaders"})
    --on_clean(function (target) os.rm("build/shaders/*") end)
    --add_packages("vulkansdk")
