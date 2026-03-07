/**
 *   @file: RenderInit.cc
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#include <angelscript.h>
#include "../Ext/ScriptBuilder.hh"

#include "../Camera.hh"
#include "../Engine.hh"
#include "../Model.hh"
#include "../Shader.hh"

// This callback will be called for each #include directive encountered by the
// builder. The callback should call the AddSectionFromFile or AddSectionFromMemory
// to add the included section to the script. If the include cannot be resolved
// then the function should return a negative value to abort the compilation.
int script_include_cb(
    char cptr include, char cptr from, CScriptBuilder ptr builder, void ptr userParam
) {
    optional<string> s_code = get_file_contents(string("Scripts/") + include);
    if (s_code == nullopt) return -1;

    int r = builder->AddSectionFromMemory(include, s_code.value().data(), cast<uint>(s_code.value().length()));
    if (r < 0) return -2;
    return 0;
}

void Engine::render_init()
{
    Log log("asdf TEMP");

    is_running = true;

    //Load Script Entrypoint

    int r = 0;
    CScriptBuilder s_builder;
    s_builder.SetIncludeCallback(script_include_cb, 0);
    r = s_builder.StartNewModule(engine->script_engine, 0);
    if (r < 0)
    {
        log.fail("Cannot start new module");
        return;
    }

    string_view const s_file = engine->script_entrypoint;
    string const s_code = get_file_contents(s_file).value();

    r = s_builder.AddSectionFromMemory(s_file.data(), s_code.data(), cast<uint>(s_code.length()));
    //r = s_builder.AddSectionFromFile(script_path.data());
    if (r < 0)
    {
        log.fail("Cannot add section \"{}\"", s_file);
        return;
    }

    r = s_builder.BuildModule();
    if (r < 0)
    {
        log.fail("Cannot build module");
        return;
    }

    asIScriptFunction ptr s_main = 
        engine->script_engine->GetModule(0)->GetFunctionByDecl("void main()");

    if (!run_function_as(s_main)) return;

    //Init Lights
    for (auto cref [_, shader] : Shader::get_map())
    {
        shader.use();
        for (size_t i = 0; i < lights.size(); i++)
            update_light(i, &shader);
    }

    //Init Instanced Mesh data
    for (auto ref [_, model] : Model::get_map())
    {
        for (auto ref mesh : model.meshes)
        {
            const_cast<Mesh ref>(mesh).update_instance_m_mats();
        }
    }

    //Init Camera values
    camera->update_angle(0.0f, 0.0f);

}

bool Engine::run_function_as(asIScriptFunction ptr cb)
{
    string_view name = cb->GetName();
    Log log("Running script function \"{}\"", name);

    asIScriptContext ptr s_ctx = engine->script_engine->CreateContext();
    int r = 0;

    r = s_ctx->Prepare(cb);
    if (r < 0)
    {
        log.fail("Cannot get function", name);
        return false;
    }

    r = s_ctx->Execute();
    if (r < 0)
    {
        log.fail("Cannot execute", name);
        return false;
    }

    r = s_ctx->Release();
    if (r < 0)
    {
        log.fail("Cannot release context");
        return false;
    }

    return true;
}