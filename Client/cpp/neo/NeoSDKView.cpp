#include "NeoSDKView.h"
#include "json/document-wrapper.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

#include "string-utils.h"
#include "base64.h"
#include "GameSystem.h"
#include "sdk/SDK.h"
#include "GameSystem.h"
#include <chrono>
#include <ctime>
#include "common/DlgAlert.h"

using namespace cocos2d;
const std::string JS_SCHEME_VALUE = "aicarb";
const std::string LOGIN_CMD = "loginRes";
const std::string ON_INIT_CMD = "ON_INIT_CMD";
const std::string HIDE_WEB_VIEW = "HIDE_WEBVIEW";
const std::string CHARGE_BACK = "CHARGE_BACK";


std::time_t getTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	std::time_t timestamp = tmp.count();
	//std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
	return timestamp;
}

NeoSDKView::NeoSDKView() {
	mType = 0;
}
void NeoSDKView::executeJSValue(const std::string& value) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (_webView != nullptr) {
		_webView->evaluateJS(value);
	}
#endif
}

void NeoSDKView::setActionType(int type, const std::string& jsValue) {
	mType = type;
	mJsValue = jsValue;
}
bool NeoSDKView::init() {
	if (!Node::init())
	{
		return false;
	}
	return true;
}

void NeoSDKView::initView() {
	std::string url = StringUtils::format("http://sg.aicarb.com/index.html?type=%d", mType);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	Size winSize = Director::getInstance()->getVisibleSize();
	_webView = cocos2d::experimental::ui::WebView::create();
	_webView->setPosition(winSize / 2);
	_webView->loadURL(url);
	_webView->setContentSize(winSize);
	_webView->setScalesPageToFit(true);

	_webView->setOnShouldStartLoading(CC_CALLBACK_2(NeoSDKView::onWebViewShouldStartLoading, this));
	_webView->setOnDidFinishLoading(CC_CALLBACK_2(NeoSDKView::onWebViewDidFinishLoading, this));
	_webView->setOnDidFailLoading(CC_CALLBACK_2(NeoSDKView::onWebViewDidFailLoading, this));
	_webView->setJavascriptInterfaceScheme(JS_SCHEME_VALUE);
	_webView->setOnJSCallback(CC_CALLBACK_2(NeoSDKView::onWebViewCallback, this));
	this->addChild(_webView);
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
bool NeoSDKView::onWebViewShouldStartLoading(experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewShouldStartLoading, url is %s", url.c_str());
	//don't do any OpenGL operation here!! It's forbidden!
	return true;
}
void NeoSDKView::onWebViewCallback(experimental::ui::WebView *sender, const std::string &url) {
	// URLSafe Base64 
	std::string base64 = ReplaceAll(url, JS_SCHEME_VALUE + ":", "");
	base64 = ReplaceAll(base64, "-", "+");
	base64 = ReplaceAll(base64, "_", "/");
	std::string json = base64_decode(base64);
	//
	rapidjson::Document doc;
	doc.Parse<0>(json.c_str());
	if (doc.HasParseError()) {
		CCLOG("invalid json format value:%s", url.c_str());
		Director::getInstance()->end();
		return;
	}
	std::string code = doc["cmd"].GetString();
	if (code.compare(ON_INIT_CMD) == 0){
		if (mJsValue.empty()) {
			GameSystem::getUIManager()->toggleNeoSDK(false);
			return;
		}
		this->executeJSValue(mJsValue);
		return;
	}
	if (code.compare(LOGIN_CMD) == 0){
		std::string wallet = doc["data"]["wallet"].GetString();
		GameSystem::getUIManager()->toggleNeoSDK(false);
		SDK::getInstance()->onLogin(wallet, wallet, json);
		return;
	}
	if (code.compare(HIDE_WEB_VIEW) == 0) {
		GameSystem::getUIManager()->toggleNeoSDK(false);
		return;
	}
	if (code.compare(CHARGE_BACK) == 0) {
		GameSystem::getUIManager()->toggleNeoSDK(false);
		return;
	}
}
void NeoSDKView::onWebViewDidFinishLoading(experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewDidFinishLoading, url is %s", url.c_str());
	// 需要在webview初始化进行一些js的类库的runtime
	std::string jsLib = FileUtils::getInstance()->getStringFromFile("js/neo.lib.min.js");
	_webView->evaluateJS(jsLib);
}

void NeoSDKView::onWebViewDidFailLoading(experimental::ui::WebView *sender, const std::string &url)
{
	CCLOG("onWebViewDidFailLoading, url is %s", url.c_str());
}
#endif