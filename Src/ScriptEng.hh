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

    void run(); //Run ScriptEng (after main Engine is done initializing)

    //protected:

    Engine ptr engine = nullptr; //pointer to main engine
    asIScriptEngine ptr s_engine = nullptr;

    vector<asIScriptFunction ptr> s_funcs;
};
