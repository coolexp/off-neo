#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

// SDKView采用webview嵌入的方式
class NeoSDKView :public cocos2d::Node {
public:
	NeoSDKView();
	virtual bool init();
	CREATE_FUNC(NeoSDKView);
	void executeJSValue(const std::string& value);
	void setActionType(int type, const std::string& jsValue);
	void initView();
private:
	std::string mJsValue;
	int mType;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS||CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cocos2d::experimental::ui::WebView *_webView;
	void onWebViewCallback(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	bool onWebViewShouldStartLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFinishLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
	void onWebViewDidFailLoading(cocos2d::experimental::ui::WebView *sender, const std::string &url);
#endif
};