#pragma once
#include <renderer\graphics.h>

class PostProcessEffect {

public:
	PostProcessEffect() : _active(false) {}
	virtual ~PostProcessEffect() {}
	virtual void begin() = 0;
	virtual void update(float dt) {}
	virtual void end() = 0;
	void activate() {
		_active = true;
	}
	void deactivate() {
		_active = false;
	}
	const bool isActive() const {
		return _active;
	}
private:
	bool _active;

};

class FadeOutEffect : public PostProcessEffect {

public:
	FadeOutEffect();
	~FadeOutEffect() {}
	void begin();
	void update(float dt);
	void end();
private:
	int createFadeShader(int textureId);
	int _rtID;
	float _fadeTimer;
	ds::Shader* _shader;
	int _shaderID;
};