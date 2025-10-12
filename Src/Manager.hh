/**
 *   @file: Manager.hh
 * @author: Nicholas Adkins
 *   @date: Sep 29 2025
 *  @brief: Class that can be inherited & creates bunch of static functions
 *          such that all instances of T are stored in a map
 */

//#pragma once
#ifndef MGE_MANAGER
#define MGE_MANAGER

#include "Core.hh"

template<typename T>
class Manager
{
    public:

    Manager();

    delete_other_ops(Manager)

    static T ptr get_new(string cref name);

    static optional<T ptr> get(string cref val_name);

    static void remove(string cref val_name);

    static bool exists(string cref val_name);

    static unordered_map<string, T> cref get_map();

    protected:

    static unordered_map<string, T> ref get_map_base();
};

#include "Manager.cc"

#endif