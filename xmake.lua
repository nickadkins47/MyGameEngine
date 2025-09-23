
set_project("MyGameEngine")
set_version("0.1.0")
set_xmakever("2.9.9")

set_policy("run.autobuild", true) -- xmake run -> will build & run program

add_requires("angelscript 2.37.0")
add_requires("assimp v5.4.3")
add_requires("glad v0.1.36")
add_requires("glfw 3.4")
add_requires("glm 1.0.1")
add_requires("imgui 1.91.8", {configs = {glfw_opengl3=true}})
add_requires("stb 2025.03.14")

target("main")
    set_kind("binary")
    set_basename("MyGameEngine")
    set_languages("c++23")
    set_extension(".exe")
    add_files("Src/**.cc")
    add_packages("angelscript", "assimp", "glad", "glfw", "glm", "imgui", "stb")

    set_warnings("all")
        -- all
        -- allextra
        -- everything

    set_targetdir("./Build")
    set_objectdir("./.xmake/.objs")
    set_dependir("./.xmake/.deps")
    set_autogendir("./.xmake/.gens")

    after_build(function (target)
        os.rm("Build/*.exp")
        os.rm("Build/*.lib")
    end)

target("clear-logs")
    set_kind("phony")
    on_run(function () 
        os.rm("Build/Logs/**.log")
    end)