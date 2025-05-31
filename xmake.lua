
set_project("MyGameEngine")
set_version("0.0.0")
set_xmakever("2.9.9")

add_requires("glad v0.1.36")
add_requires("glfw 3.4")
add_requires("glm 1.0.1")
add_requires("stb 2025.03.14")

target("main")
    set_kind("binary")
    set_basename("MyGameEngine")
    add_files("Src/**.cc")
    set_languages("c++23")
    add_packages("glad", "glfw", "glm", "stb")
