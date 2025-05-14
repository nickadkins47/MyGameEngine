
#include "Keyboard.hh"

namespace MGE {

    Keyboard::Keyboard() {}
    Keyboard::~Keyboard() {}

    Keyboard::Key const& Keyboard::at(SDL_Keycode const& keycode) {
        return keys[keycode];
    }

    Keyboard::Key& Keyboard::operator[](SDL_Keycode const& keycode) {
        return keys[keycode];
    }

    void Keyboard::keyDown(SDL_Keycode const& keycode) {
        Key& thisKey = keys[keycode];
        thisKey.onKeyDown();
        thisKey.isKeyDown = true;
    }

    void Keyboard::keyUp(SDL_Keycode const& keycode) {
        Key& thisKey = keys[keycode];
        thisKey.onKeyUp();
        thisKey.isKeyDown = false;
    }

    bool Keyboard::isKeyDown(SDL_Keycode const& keycode) {
        return keys[keycode].isKeyDown;
    }

    bool Keyboard::isKeyUp(SDL_Keycode const& keycode) {
        return !keys[keycode].isKeyDown;
    }

}