/**
 *   @file: ScriptEng.cc
 * @author: Nicholas Adkins
 *   @date: Aug 30 2025
 *  @brief: 
 */

#include <angelscript.h>

#include "Ext/ScriptBuilder.hh"
#include "Engine.hh"
#include "ScriptEng.hh"

//TODO: get rid of assert statements in these macros, add logging

//Helper macro; stringifies the input (s -> "s")
#define m_str(s) #s

//Same as global_func, but with seperate parameter lists for C++ & AS
#define global_func_sp(ret, name, cc_params, as_params, cap, body) \
    char cptr _s_##name = m_str(ret name##as_params); \
    Log::info("Script Global Func \"{}\"...", _s_##name); \
    typedef ret (*_t_##name)##cc_params; \
    _t_##name static _f_##name = cap##cc_params##body; \
    r = s_engine->RegisterGlobalFunction(_s_##name, \
        asFUNCTION(_f_##name), asCALL_CDECL); \
    if (r < 0) \
    { \
        Log::error("Script Global Func \"{}\": Failed (Cannot register function)", _s_##name); \
        return; \
    } \
    Log::info("Script Global Func \"{}\": Success", _s_##name); \
    predefs << _s_##name << ";\n";

//Macro for defining a function in the Scripts that interfaces with the C++ code
#define global_func(ret, name, params, cap, body) \
    global_func_sp(ret, name, params, params, cap, body)

//Macro for creating function definitions
#define funcdef(ret, name, params) \
    char cptr _s_##name = m_str(ret name##params); \
    Log::info("Script Funcdef \"{}\"...", _s_##name); \
    r = s_engine->RegisterFuncdef(_s_##name); \
    if (r < 0) \
    { \
        Log::error("Script Funcdef \"{}\": Failed (Cannot register funcdef)", _s_##name); \
        return; \
    } \
    Log::info("Script Funcdef \"{}\": Success", _s_##name); \
    predefs << "funcdef " << _s_##name << ";\n";

//Info/Warn/Err Message Callback for Script Engine
void message_cb(asSMessageInfo cptr msg, void ptr param)
{
    if (msg->type == asMSGTYPE_INFORMATION)
    {
        Log::info("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    }
    else if (msg->type == asMSGTYPE_WARNING)
    {
        Log::warn("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    }
    else /*else, Error*/
    {
        Log::error("Script Engine [{}, {}, {}]: {}", 
            msg->section, msg->row, msg->col, msg->message
        );
    }
}

// This callback will be called for each #include directive encountered by the
// builder. The callback should call the AddSectionFromFile or AddSectionFromMemory
// to add the included section to the script. If the include cannot be resolved
// then the function should return a negative value to abort the compilation.
int include_cb(
    const char *include, const char *from, CScriptBuilder *builder, void *userParam
) {
    optional<string> s_code = get_file_contents(path(string("Scripts/") + include));
    if (s_code == nullopt) return -1;

    int r = builder->AddSectionFromMemory(include, s_code.value().c_str(), cast<uint>(s_code.value().length()));
    if (r < 0) return -2;
    return 0;
}

ScriptEng::ScriptEng()
{
    Log::info("Initializing Script Engine...");

    std::ofstream predefs("as.predefined");
    if (predefs.fail())
    {
        Log::warn("Initializing Script Engine: Failed (Couldn't open as.predefined)");
        return;
    }

    s_engine = asCreateScriptEngine();
    int r = s_engine->SetMessageCallback(asFUNCTION(message_cb), 0, asCALL_CDECL);
    if (r < 0)
    {
        Log::error("Initializing Script Engine: Failed (Couldn't set message callback)");
        return;
    }

    //Core Functionality

    funcdef(void, VoidCallback, ());

    global_func_sp(
        void, kbd_on_press, (int token, asIScriptFunction ptr cb),
        (int token, VoidCallback @cb), [],
    {
        engine->script_engine.s_funcs.push_back(cb);
        engine->keyboard[token].on_press = [cb](){
            engine->script_engine.run_as_function(cb);
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
    Log::info("Initializing Script Engine: Success");
}

ScriptEng::~ScriptEng()
{
    Log::info("Shutting Down Script Engine...");
    int r = 0;
    for (asIScriptFunction ptr func : s_funcs)
    {
        if (func != nullptr)
        {
            r = func->Release();
            if (r < 0)
                Log::warn("Shutting Down Script Engine: Failed (func \"{}\" failed to release)", func->GetName());
        }
    }
    r = s_engine->ShutDownAndRelease();
    if (r < 0)
        Log::warn("Shutting Down Script Engine: Failed (engine shutdown function failed)");

    Log::info("Shutting Down Script Engine: Success");
}

void ScriptEng::run(path cref script_p)
{
    Log::info("Running Script Engine...");

    int r = 0;
    CScriptBuilder s_builder;
    s_builder.SetIncludeCallback(include_cb, 0);
    r = s_builder.StartNewModule(s_engine, 0);
    if (r < 0)
    {
        Log::error("Running Script Engine: Failed (Builder cannot start new module)");
        return;
    }

    string const s_code = get_file_contents(script_p).value();

    r = s_builder.AddSectionFromMemory(script_p.string().c_str(), s_code.c_str(), cast<uint>(s_code.length()));
    //r = s_builder.AddSectionFromFile(script_p.string().c_str());
    if (r < 0)
    {
        Log::error("Running Script Engine: Failed (Cannot add section \"{}\")", script_p.string());
        return;
    }

    r = s_builder.BuildModule();
    if (r < 0)
    {
        Log::error("Running Script Engine: Failed (Cannot build module)");
        return;
    }

    asIScriptFunction ptr s_main = s_engine->GetModule(0)->GetFunctionByDecl("void main()");
    if (!run_as_function(s_main)) return;

    Log::info("Running Script Engine: Success");
}

bool ScriptEng::run_as_function(asIScriptFunction ptr cb)
{
    char cptr cref name = cb->GetName();
    Log::info("ScrEng running function \"{}\"...", name);

    asIScriptContext ptr s_ctx = s_engine->CreateContext();
    int r = 0;

    r = s_ctx->Prepare(cb);
    if (r < 0)
    {
        Log::error("ScrEng running function \"{}\": Failed (Cannot get function)", name);
        return false;
    }

    r = s_ctx->Execute();
    if (r < 0)
    {
        Log::error("ScrEng running function \"{}\": Failed (Cannot execute)", name);
        return false;
    }

    r = s_ctx->Release();
    if (r < 0)
    {
        Log::error("ScrEng running function \"{}\": Failed (Cannot release context)", name);
        return false;
    }

    Log::info("ScrEng running function \"{}\": Success", name);
    return true;
}