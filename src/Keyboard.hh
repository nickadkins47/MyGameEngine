
#pragma once

#include <unordered_map>
#include <functional>

#include "Globals.hh"

namespace MGE {

    //Keeps track of keyboard events like onKeyUp, onKeyDown, as
    //well as handling whether or not a key is currently pressed
    class Keyboard {
    public:
        Keyboard();
        ~Keyboard();
        
        deleteOtherOps(Keyboard)

        //stores things to do upon pressing a given key
        struct Key {
            std::function<void(void)> onKeyDown = [](){return;};
            std::function<void(void)> onKeyUp   = [](){return;};
            bool isKeyDown = false;
        };

        Key const& at(SDL_Keycode const& keycode); //return const& to key 
        Key & operator[](SDL_Keycode const& keycode); //return reference to key

        void keyDown(SDL_Keycode const& keycode); //triggers when key is pressed
        void keyUp(SDL_Keycode const& keycode); //triggers when key is released

        bool isKeyDown(SDL_Keycode const& keycode);
        bool isKeyUp(SDL_Keycode const& keycode);

    protected:
        std::unordered_map<SDL_Keycode, Key> keys;

    };

}