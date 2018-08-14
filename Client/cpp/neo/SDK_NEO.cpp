#include "SDK_NEO.h"
#include "GameSystem.h"
#include "mod/sdk/SDK.h"


using namespace cocos2d;


void SDK_NEO::init(const std::string& gameId, const std::string& gameName, const std::string& appScheme) {
	SDK::getInstance()->onInit(0);
}
void SDK_NEO::login(const std::string& zoneId, const std::string& zoneName, bool isAutoLogin) {
	NeoSDKView* mView = NeoSDKView::create();
	mView->setActionType(1, "window.NeoJS.login()");
	mView->initView();
	GameSystem::getUIManager()->initNeoSDK(mView);
}
void SDK_NEO::switchAccount() {

}
void SDK_NEO::pay(const ChargeData& chargeInfo) {
	std::string moneyValue = chargeInfo.moneyStringValue;
	std::string targetId = chargeInfo.gameAccountId;
	std::string chargeId = chargeInfo.chargeId;
	std::string productId = chargeInfo.productId;
	std::string jsValue = StringUtils::format("window.NeoJS.charge('%s','%s','%s','%s')", moneyValue.c_str(), targetId.c_str(), chargeId.c_str(), productId.c_str());
	NeoSDKView* mView = NeoSDKView::create();
	mView->setActionType(2, jsValue);
	mView->initView();
	GameSystem::getUIManager()->initNeoSDK(mView);
}
void SDK_NEO::logout() {

}
void SDK_NEO::showUserCenter() {

}
bool SDK_NEO::isLogin() {
	return false;
}
std::string SDK_NEO::getUDID() {
	return "";
}
bool SDK_NEO::hasSwitchAccount() {
	return true;
}
bool SDK_NEO::hasUserCenter() {
	return true;
}
/**
* @brief 进入游戏时，提交一些数据给SDK
*/
void SDK_NEO::enterGame(const std::string& json) {

}
/**
* @brief 退出游戏时，提交一些数据给SDK
*/
void SDK_NEO::exitGame(const std::string& json) {

}