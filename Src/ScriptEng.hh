/**
 *   @file: ScriptEng.hh
 * @author: Nicholas Adkins
 *   @date: Aug 30 2025
 *  @brief: 
 */

#pragma once

#include "Core.hh"

class Engine;
class asIScriptEngine;
class asIScriptModule;
class asIScriptFunction;

class ScriptEng
{
    public:

    ScriptEng(); //Initialize ScriptEng
    ~ScriptEng();

    deleteOtherOps(ScriptEng)

    //Run ScriptEng with the given Script File (Defaults to "Main.as")
    //Must be done after main Engine is done initializing
    void run(string cref file_name = "Main.as");

    //protected:

    asIScriptEngine ptr s_engine = nullptr;

    vector<asIScriptFunction ptr> s_funcs;
};
