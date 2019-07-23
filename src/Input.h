#pragma once

#include <SDL.h>
#include <map>

/// Keeps track of keyboard state
class Input {
public:
    /// This function gets called at the beginning of each new frame
    /// to reset the keys that are no longer relevant
    void beginNewFrame();

    /// This gets called when key has been pressed
    void keyUpEvent(const SDL_Event &event);

    /// This gets called when a key has pressed
    void keyDownEvent(const SDL_Event &event);

    /// Check if a certain key was pressed during the current frame
    bool wasKeyPressed(SDL_Scancode key);

    /// Check if a certain key was released during the current frame
    bool wasKeyReleased(SDL_Scancode key);

    /// Check if a certain key is currently being held
    bool isKeyHeld(SDL_Scancode key);

private:
    std::map<SDL_Scancode, bool> heldKeys;
    std::map<SDL_Scancode, bool> pressedKeys;
    std::map<SDL_Scancode, bool> releasedKeys;
};
