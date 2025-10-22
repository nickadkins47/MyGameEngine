/**
 *   @file: ScriptMacros.hh
 * @author: Nicholas Adkins
 *   @date: Oct 21 2025
 *  @brief: 
 */

#pragma once

//Helper macro; stringifies the input (s -> "s")
#define m_str(s) #s

//Same as global_func, but with seperate parameter lists for C++ & AS
#define m_global_func_sp(ret, name, cap, cc_params, as_params, body) \
    char cptr _s_##name = m_str(ret name##as_params); \
    log.info("Script Global Func \"{}\"", _s_##name); \
    typedef ret (*_t_##name)##cc_params; \
    _t_##name static _f_##name = cap##cc_params##body; \
    r = engine->script_engine->RegisterGlobalFunction(_s_##name, \
        asFUNCTION(_f_##name), asCALL_CDECL); \
    if (r < 0) \
    { \
        log.fail("Cannot register function"); \
        return; \
    } \
    predefs << _s_##name << ";\n";

//Macro for defining a function in the Scripts that interfaces with the C++ code
#define m_global_func(ret, name, cap, params, body) \
    m_global_func_sp(ret, name, cap, params, params, body)

//Macro for creating function definitions
#define m_funcdef(ret, name, params) \
    char cptr _s_##name = m_str(ret name##params); \
    log.info("Script Funcdef \"{}\"", _s_##name); \
    r = engine->script_engine->RegisterFuncdef(_s_##name); \
    if (r < 0) \
    { \
        log.fail("Cannot register funcdef"); \
        return; \
    } \
    predefs << "funcdef " << _s_##name << ";\n";
