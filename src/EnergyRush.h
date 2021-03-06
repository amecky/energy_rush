#pragma comment(lib, "D11.lib")
#pragma comment(lib, "DieselCore.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include <renderer\render_types.h>
#include "GameContext.h"

class EnergyRush : public ds::BaseApp {


public:	
	EnergyRush();
	virtual ~EnergyRush();
	bool initialize() {
		return true;
	}
	bool loadContent();
	const char* getTitle() {
		return "EnergyRush";
	}
	void init();
	void update(float dt);
	void render();
	void onShutdown();
protected:
	void prepare(ds::Settings* settings);
private:
	GameContext* _context;
};