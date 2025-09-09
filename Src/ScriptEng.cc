/**
 *   @file: ScriptEng.cc
 * @author: Nicholas Adkins
 *   @date: Aug 30 2025
 *  @brief: 
 */

#include <fstream>

#include <angelscript.h>

#include "Ext/ScriptBuilder.hh"
#include "Engine.hh"
#include "ScriptEng.hh"

//Helper macro; stringifies the input (s -> "s")
#define m_str(s) #s

//Same as global_func, but with seperate parameter lists for C++ & AS
#define global_func_sp(ret, name, cc_params, as_params, cap, body) \
    typedef ret (*_t_##name)##cc_params; \
    _t_##name static _f_##name = cap##cc_params##body; \
    r = s_engine->RegisterGlobalFunction(m_str(ret name##as_params), \
        asFUNCTION(_f_##name), asCALL_CDECL); \
    assert(r >= 0); \
    predefs << m_str(ret name##as_params) << ";\n";

//Macro for defining a function in the Scripts that interfaces with the C++ code
#define global_func(ret, name, params, cap, body) \
    global_func_sp(ret, name, params, params, cap, body)

//Macro for creating function definitions
#define funcdef(ret, name, params) \
    r = s_engine->RegisterFuncdef(m_str(ret name##params)); \
    assert(r >= 0); \
    predefs << "funcdef " << m_str(ret name##params) << ";\n";

//Info/Warn/Err Message Callback for Script Engine
void message_cb(asSMessageInfo cptr msg, void ptr param)
{
    string type = (msg->type == asMSGTYPE_WARNING) ?
        "WARN"
    : ( msg->type == asMSGTYPE_INFORMATION ) ?
        "INFO"
    : /*Else:*/ 
        "ERR"
    ;
    print("{} ({}, {}) : {} : {}\n",
        msg->section, msg->row, msg->col, type, msg->message
    );
}

// This callback will be called for each #include directive encountered by the
// builder. The callback should call the AddSectionFromFile or AddSectionFromMemory
// to add the included section to the script. If the include cannot be resolved
// then the function should return a negative value to abort the compilation.
int include_cb(
    const char *include, const char *from, CScriptBuilder *builder, void *userParam
) {
    //print("({}) from ({})\n", include, from);
    optional<string> s_code = get_file_contents(format("Scripts/{}", include));
    if (s_code == nullopt) return -1;

    int r = builder->AddSectionFromMemory(include, (*s_code).c_str(), (*s_code).length());
    if (r < 0) return -2;
    return 0;
}

ScriptEng::ScriptEng()
{
    std::ofstream predefs(get_file_path("as.predefined"));
    if (predefs.fail())
    {
        print("ERROR: couldn't open as.predefined\n");
        return;
    }

    s_engine = asCreateScriptEngine();
    int r = s_engine->SetMessageCallback(asFUNCTION(message_cb), 0, asCALL_CDECL);
    assert(r >= 0);

    //Core Functionality

    funcdef(void, VoidCallback, ());

    global_func_sp(
        void, kbd_on_press, (int token, asIScriptFunction ptr cb),
        (int token, VoidCallback @cb), [],
    {
        engine->script_engine.s_funcs.push_back(cb);
        engine->keyboard[token].on_press = [cb](){
            asIScriptContext ptr ctx = engine->script_engine.s_engine->CreateContext();
            int r = 0;

            r = ctx->Prepare(cb);
            assert(r >= 0);
            r = ctx->Execute();
            assert(r >= 0);

            r = ctx->Release();
            assert(r >= 0);
        };
    });

    global_func(void, camera_proj_mat, (float fov_degrees, float near_z, float far_z), [], {
        engine->camera.set_proj_mat(fov_degrees, near_z, far_z);
    });

    global_func(void, camera_pos, (float x, float y, float z), [], {
        engine->camera.pos = glm::vec3(x,y,z);
    });

    //General Functions

    global_func(void, exit, (), [], {
        glfwSetWindowShouldClose(engine->window, true);
    });

    global_func(void, option_draw_lines, (), [], {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    });

    global_func(void, option_draw_fill, (), [], {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    });
    
    predefs.close();
}

ScriptEng::~ScriptEng()
{
    for (asIScriptFunction ptr func : s_funcs)
        if (func != nullptr) assert(func->Release() >= 0);
    assert(s_engine->ShutDownAndRelease() >= 0);
}

void ScriptEng::run(string cref file_name)
{
    int r = 0;
    CScriptBuilder s_builder;
    s_builder.SetIncludeCallback(include_cb, 0);
    r = s_builder.StartNewModule(s_engine, 0);
    assert(r >= 0);

    string const s_code = get_file_contents(format("Scripts/{}", file_name)).value();

    r = s_builder.AddSectionFromMemory(file_name.c_str(), s_code.c_str(), s_code.length());
    assert(r >= 0);
    r = s_builder.BuildModule();
    assert(r >= 0);

    asIScriptContext ptr s_ctx = s_engine->CreateContext();

    r = s_ctx->Prepare(s_engine->GetModule(0)->GetFunctionByDecl("void main()"));
    assert(r >= 0);
    r = s_ctx->Execute();
    assert(r >= 0);
    r = s_ctx->Release();
    assert(r >= 0);
}
