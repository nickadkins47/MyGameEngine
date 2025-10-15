/**
 *   @file: Manager.cc
 * @author: Nicholas Adkins
 *   @date: Sep 29 2025
 *  @brief: 
 */

#pragma once

#include "Manager.hh"

template<typename T>
Manager<T>::Manager() {}

template<typename T>
T ptr Manager<T>::new_val(string cref val_name)
{
    return &get_map_base()[val_name];
}

template<typename T>
optional<T ptr> Manager<T>::get_opt(string cref val_name)
{
    unordered_map<string, T> ref val_map = get_map_base();
    auto iter = val_map.find(val_name);
    if (iter == val_map.end())
    {
        return nullopt;
    }
    else return &iter->second;
}

template<typename T>
T ptr Manager<T>::get(string cref val_name)
{
    return &get_map_base().at(val_name);
}

template<typename T>
void Manager<T>::remove(string cref val_name)
{
    get_map_base().erase(val_name);
}

template<typename T>
bool Manager<T>::exists(string cref val_name)
{
    return get_map_base().contains(val_name);
}

template<typename T>
unordered_map<string, T> cref Manager<T>::get_map()
{
    return get_map_base();
}

template<typename T>
unordered_map<string, T> ref Manager<T>::get_map_base()
{
    unordered_map<string, T> static val_map;
    return val_map;
}