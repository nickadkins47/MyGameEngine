/**
 *   @file: ScriptEng.hh
 * @author: Nicholas Adkins
 *   @date: Aug 30 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class asIScriptEngine;
class asIScriptFunction;

class ScriptEng
{
    public:

    ScriptEng(); //Initialize ScriptEng
    ~ScriptEng();

    delete_other_ops(ScriptEng)

    //Run ScriptEng with the given Script File (Defaults to "Scripts/Main.as")
    //Must be done after main Engine is done initializing
    void run(string_view script_path = "Scripts/Main.as");

    //protected:

    asIScriptEngine ptr s_engine = nullptr;

    vector<asIScriptFunction ptr> s_funcs;

    bool run_as_function(asIScriptFunction ptr cb);

};
