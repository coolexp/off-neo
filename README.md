# off-neo

cocos2d-x项目上链neo相关代码

##### client 
1. res里的js目录属于cocos2d-x项目里的res目录或者resources目录
2. html文件请部署至服务端，可以以外网地址访问譬如：sg.aicarb.com
3. cpp文件属于以webview的方式加载html


#### server
主要验证充值部分。接收neo-sdk服务器的回调，给玩家充值