#pragma once

#include "cocos2d.h"
#include "sdk/SDKCom.h"
#include "NeoSDKView.h"
class SDK_NEO :public SDKCom {

public:
	static SDK_NEO* getInstance();

	virtual void init(const std::string& gameId, const std::string& gameName, const std::string& appScheme);
	virtual void login(const std::string& zoneId, const std::string& zoneName, bool isAutoLogin);
	virtual void switchAccount();
	virtual void pay(const ChargeData& chargeInfo);
	virtual void logout();
	virtual void showUserCenter();
	virtual bool isLogin();
	virtual std::string getUDID();
	virtual bool hasSwitchAccount();
	virtual bool hasUserCenter();
	/**
	* @brief 进入游戏时，提交一些数据给SDK
	*/
	virtual void enterGame(const std::string& json);
	/**
	* @brief 退出游戏时，提交一些数据给SDK
	*/
	virtual void exitGame(const std::string& json);

protected:
	static SDK_NEO* instance;
};