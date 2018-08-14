#include "SDK_NEOAndroid.h"


SDK_NEO* SDK_NEO::instance = nullptr;

SDK_NEO* SDK_NEO::getInstance() {
	if (instance == nullptr) {
		instance = new SDK_NEOAndroid();
	}
	return instance;
}