
--includes("config.lua")

set_project("MyGameEngine")
set_version("0.0.0")
set_xmakever("3.0.0")

add_requires("libsdl3 3.2.10")

target("main")
    set_kind("binary")
    set_basename("MyGameEngine")
    add_files("src/*.cc")
    set_languages("c++23")
    add_packages("libsdl3")
    --add_options("use_SDL3")
    --add_links("$(builddir)/.packages/s/sdl3/latest/**/lib")
    --add_rpathdirs("$(builddir)/.packages/s/sdl3/latest/**/bin")
    --add_headerfiles("$(builddir)/.packages/s/sdl3/latest/**/include/SDL3/SDL.h")
    --add_options("use_SDL3")
    --on_run(function (target) print("Target: main") end)

--[[target("test")
    set_kind("static")
    add_files("src/test.c")
    --on_run(function (target) print("Target: test") end) ]]

--target("gl_headers")
    --set_kind("headeronly")
    --add_headerfiles(string.format("%s/GL.h %s/GLU.h", gl_include_dir, gl_include_dir))
    --on_run(function (target) print("Target: gl_headers") end)
