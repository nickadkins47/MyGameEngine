/**
 *   @file: Manager.hh
 * @author: Nicholas Adkins
 *   @date: Sep 29 2025
 *  @brief: 
 */

//#pragma once
#ifndef MGE_MANAGER_HH
#define MGE_MANAGER_HH

#include "Core.hh"

//TODO figure out how to make constructors protected while also letting manager use them

#define manager_funcs_hh(class_name, param_name) \
    friend class Manager<class_name>; \
    static optional<class_name ptr> get(string cref param_name); \
    static void remove(string cref param_name); \
    static bool exists(string cref param_name); \
    static unordered_map<string, class_name> cref get_map();

#define manager_funcs_cc(class_name, param_name) \
    optional<class_name ptr> class_name::get(string cref param_name) \
    { \
        return manager.get(param_name); \
    } \
    void class_name::remove(string cref param_name) \
    { \
        manager.remove(param_name); \
    } \
    bool class_name::exists(string cref param_name) \
    { \
        return manager.exists(param_name); \
    } \
    unordered_map<string, class_name> cref class_name::get_map() \
    { \
        return manager.get_map(); \
    }

template<typename T>
class Manager
{
    public:

    Manager();

    delete_other_ops(Manager)

    T ptr get_new(string cref name);

    optional<T ptr> get(string cref val_name);

    void remove(string cref val_name);

    bool exists(string cref val_name) const;

    unordered_map<string, T> cref get_map() const;

    protected:

    unordered_map<string, T> val_map;

};

#include "Manager.cc"

#endif