#include "AnimatedSprite.h"
#include "Graphics.h"

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath,
                               int srcX, int srcY, int width, int height,
                               float posX, float posY, float timeToUpdate) :
        Sprite(graphics, filePath, srcX, srcY, width, height, posX, posY),
        _frameIndex(0),
        _timeElapsed(0),
        _timeToUpdate(timeToUpdate),
        _visible(true),
        _currentAnimationOnce(false),
        _currentAnimation("") {

}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    _currentAnimationOnce = once;
    if(_currentAnimation != animation) {
        _currentAnimation = animation;
        _frameIndex = 0;
    }
}

void AnimatedSprite::update(float deltaTime) {
    Sprite::update();

    _timeElapsed += deltaTime;
    if (_timeElapsed > _timeToUpdate) {
        _timeElapsed -= _timeToUpdate;
        if (_frameIndex < _animations[_currentAnimation].size() - 1) {
            _frameIndex++;
        }
        else {
            if (_currentAnimationOnce){
                setVisible(false);
            }
            _frameIndex = 0;
            animationDone(_currentAnimation);
        }
    }
}

void AnimatedSprite::draw(Graphics &graphics, int x, int y) {
    if (_visible) {
        SDL_Rect dstRect;
        dstRect.x = x + _offsets[_currentAnimation].x;
        dstRect.y = y + _offsets[_currentAnimation].y;
        dstRect.w = (int) (srcRect.w * globals::SPRITE_SCALE);
        dstRect.h = (int) (srcRect.h * globals::SPRITE_SCALE);

        SDL_Rect sourceRect = _animations[_currentAnimation][_frameIndex];
        graphics.blitSurface(spriteSheet, &sourceRect, &dstRect);
    }
}

void AnimatedSprite::setupAnimations() {
    addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2::zero());
}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
    std::vector<SDL_Rect> rects;

    for (int i = 0; i < frames; i++) {
        SDL_Rect r = {(i + x) * width, y , width, height};
        rects.push_back(r);
    }

    _animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rects));
    _offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
    _animations.clear();
    _offsets.clear();
}

void AnimatedSprite::stopAnimation() {
    _frameIndex = 0;
    animationDone(_currentAnimation);
}

void AnimatedSprite::setVisible(bool visible) {
    _visible = visible;
}

void AnimatedSprite::animationDone(std::string currentAnimation) {

}
