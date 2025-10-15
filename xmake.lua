
set_project("MyGameEngine")
set_version("0.1.0")
set_xmakever("2.9.9")

add_rules(
    "mode.debug",      --All symbols,   No optimization
    "mode.releasedbg", --All symbols, Full optimization
    "mode.release"     -- No symbols, Full optimization
)

add_requires("angelscript 2.37.0")
add_requires("assimp v5.4.3")
add_requires("glbinding v3.5.0")
add_requires("glfw 3.4")
add_requires("glm 1.0.1")
add_requires("imgui 1.91.8", {configs = {glfw_opengl3=true}})
add_requires("stb 2025.03.14")

target("main")
    set_kind("binary")
    set_basename("MyGameEngine")
    set_extension(".exe")
    set_languages("c++23")
    add_files("Src/**.cc")

    after_clean(function (target)
        os.rm("Build/Bin/*.pdb")
        os.rm("Build/Bin/*.exp")
        os.rm("Build/Bin/*.lib")
    end)

    set_targetdir("./Build/Bin")
    set_objectdir("./.xmake/.objs")
    set_dependir("./.xmake/.deps")
    set_autogendir("./.xmake/.gens")

    add_packages("angelscript", "assimp", "glbinding", "glfw", "glm", "imgui", "stb")

    set_warnings("all")
        -- all
        -- allextra
        -- everything

target("clear-logs")
    set_kind("phony")
    on_run(function () 
        os.rm("Build/Logs/**.log")
    end)