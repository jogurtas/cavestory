#pragma once

#include <map>
#include <string>
#include <vector>
#include "Sprite.h"
#include "Globals.h"

class Graphics;

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite();

	AnimatedSprite(Graphics &gfx, const std::string &filePath,
		int srcX, int srcY, int width, int height,
		float posX, float posY, float timeToUpdate);

    /// Plays the animation provided if it's not already playing
    void playAnimation(std::string animation, bool once = false);

    /// Updates the animation sprite (timer)
    void update(float deltaTime);

    /// Draws the sprite to the screen
    void draw(Graphics &graphics, int x, int y);

protected:
    double _timeToUpdate;
    bool _currentAnimationOnce;
    std::string _currentAnimation;

    /// Adds an animation to the map of animations for the sprite
    void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

    /// Resets all animations associated with this sprite
    void resetAnimations();

    /// Stops current animation
    void stopAnimation();

    /// Changes the visiblity of the animated sprite
    void setVisible(bool visible);

    /// Logic that happens when an animation ends
    virtual void animationDone(std::string currentAnimation) = 0;

	/// A required function that sets up all animations for a sprite
	virtual void setupAnimations() = 0;

private:
    std::map<std::string, std::vector<SDL_Rect>> _animations;
    std::map<std::string, Vector2> _offsets;

    Uint32 _frameIndex;
    double _timeElapsed;
    bool _visible;
};
