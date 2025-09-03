/**
 *   @file: ScriptEng.cc
 * @author: Nicholas Adkins
 *   @date: Aug 30 2025
 *  @brief: 
 */

#include <angelscript.h>

#include "Engine.hh"
#include "ScriptEng.hh"

#define SE_GlobalFunc(text, func, callc) r = \
    s_engine->RegisterGlobalFunction(text, func, callc); \
    assert(r >= 0);

#define SE_Funcdef(text) r = \
    s_engine->RegisterFuncdef("void VoidCallback()"); \
    assert(r >= 0);

asIScriptFunction ptr callback = nullptr; //TEMP test

/* #define SE_GlobalFunc(text, func) assert ( \
    s_engine->RegisterGlobalFunction(text, asFUNCTION(func), asCALL_CDECL) >= 0);

#define SE_Funcdef(text) assert ( \
    s_engine->RegisterFuncdef("void VoidCallback()") >= 0); */

// Implement a simple message callback function
void message_cb(const asSMessageInfo *msg, void *param)
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

void option_draw_lines()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void option_draw_polygons()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void global_func()
{
    print("Hello from global_func!\n");
}

void print_num(int num)
{
    print("print_num: {}\n", num);
}

void set_callback(asIScriptFunction *cb)
{
    if (callback)
        callback->Release();
    callback = cb;
}

void kbd_on_press(int token, asIScriptFunction *cb)
{
    /* engine->keyboard[token].on_press = [&cb](){
        asIScriptContext ptr ctx = s_engine->CreateContext();
        int r = 0;
        print("start...\n");

        r = ctx->Prepare(cb);
        print("done\n");
        assert(r >= 0);
        r = ctx->Execute();
        assert(r >= 0);

        ctx->Release();
    }; */
}

ScriptEng::ScriptEng()
{
    //Script Init

    //TODO: auto-generate as.predefined?

    s_engine = asCreateScriptEngine();
    s_engine->SetMessageCallback(asFUNCTION(message_cb), 0, asCALL_CDECL);
    s_module = s_engine->GetModule("main", asGM_CREATE_IF_NOT_EXISTS);

    SE_GlobalFunc("void global_func()",
        asFUNCTION(global_func), asCALL_CDECL);
    SE_GlobalFunc("void print_num(int num)",
        asFUNCTION(print_num), asCALL_CDECL);
    
    SE_GlobalFunc("void option_draw_lines()",
        asFUNCTION(option_draw_lines), asCALL_CDECL);
    SE_GlobalFunc("void option_draw_polygons()",
        asFUNCTION(option_draw_polygons), asCALL_CDECL);

    SE_Funcdef("void VoidCallback()");

    SE_GlobalFunc("void set_callback(VoidCallback @cb)",
        asFUNCTION(set_callback), asCALL_CDECL);
    /* SE_GlobalFunc("void kbd_on_press(int token, VoidCallback @cb)",
        asFUNCTION(kbd_on_press), asCALL_CDECL); */

    /* r = s_engine->RegisterGlobalFunction(
        "void global_func()", asFUNCTION(global_func), asCALL_CDECL
    );
    assert(r >= 0);

    r = s_engine->RegisterGlobalFunction(
        "void print_num(int)", asFUNCTION(print_num), asCALL_CDECL
    );
    assert(r >= 0);

    r = s_engine->RegisterFuncdef("void VoidCallback()");
    assert(r >= 0);

    r = s_engine->RegisterGlobalFunction(
        "void set_callback(VoidCallback @cb)", asFUNCTION(set_callback), asCALL_CDECL
    );
    assert(r >= 0);

    r = s_engine->RegisterGlobalFunction(
        "void kbd_on_press(int token, VoidCallback @cb)",
        asFUNCTION(kbd_on_press), asCALL_CDECL
    );
    assert(r >= 0);

    r = s_engine->RegisterGlobalFunction(
        "void option_draw_lines()", asFUNCTION(option_draw_lines), asCALL_CDECL
    );
    assert(r >= 0);
    r = s_engine->RegisterGlobalFunction(
        "void option_draw_polygons()", asFUNCTION(option_draw_polygons), asCALL_CDECL
    );
    assert(r >= 0); */

}

ScriptEng::~ScriptEng()
{
    s_engine->ShutDownAndRelease();
}

void ScriptEng::run()
{
    string script_code = *get_file_contents("Scripts/Main.as");
    
    r = s_module->AddScriptSection("Main.as", script_code.c_str());
    assert(r >= 0);
    r = s_module->Build();
    assert(r >= 0);

    asIScriptContext ptr s_ctx = s_engine->CreateContext();
    
    r = s_ctx->Prepare(s_module->GetFunctionByDecl("void main()"));
    assert(r >= 0);
    r = s_ctx->Execute();
    assert(r >= 0);

    r = s_ctx->Prepare(callback);
    assert(r >= 0);
    r = s_ctx->Execute();
    assert(r >= 0);

    s_ctx->Release();
}
