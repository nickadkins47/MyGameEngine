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

void global_func();
void print_num(int num);

void option_draw_lines();
void option_draw_polygons();

void set_callback(asIScriptFunction *cb);
void kbd_on_press(int token, asIScriptFunction *cb);

class ScriptEng
{
    public:

    ScriptEng(); //Initialize ScriptEng
    ~ScriptEng();

    void run(); //Run ScriptEng (after main Engine is done initializing)

    deleteOtherOps(ScriptEng)

    //protected:

    Engine ptr engine = nullptr; //pointer to main engine
    asIScriptEngine ptr s_engine = nullptr;
    asIScriptModule ptr s_module = nullptr;
    int r = 0; //return value TEMP
};
