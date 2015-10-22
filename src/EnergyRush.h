#pragma comment(lib, "Diesel2D.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxerr.lib")
#pragma warning(disable : 4995)

#pragma once
#include "base\BaseApp.h"
#include "dxstdafx.h"
#include <renderer\render_types.h>
#include "GameSettings.h"
#include "GameContext.h"

class EnergyRush : public ds::BaseApp {


public:	
	EnergyRush();
	virtual ~EnergyRush();
	bool loadContent();
	const char* getTitle() {
		return "EnergyRush";
	}
	void init();
	void update(float dt);
	void draw();
	void onGUIButton(ds::DialogID dlgID, int button);
private:
	GameSettings* _gameSettings;
	GameContext* _context;
};