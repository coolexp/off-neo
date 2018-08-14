#include "SDK_NEOWin.h"


SDK_NEO* SDK_NEO::instance = nullptr;

SDK_NEO* SDK_NEO::getInstance() {
	if (instance == nullptr) {
		instance = new SDK_NEOWin();
	}
	return instance;
}