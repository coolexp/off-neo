(function(){
	 var base64EncodeChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
     var cppCallFunction=null;
	 function utf16to8(str) {
		 var out, i, len, c;
		 out = "";
		 len = str.length;
		 for(i = 0; i < len; i++) {
			 c = str.charCodeAt(i);
			 if ((c >= 0x0001) && (c <= 0x007F)) {
				 out += str.charAt(i);
			 }else if (c > 0x07FF) {
				 out += String.fromCharCode(0xE0 | ((c >> 12) & 0x0F));
				 out += String.fromCharCode(0x80 | ((c >>  6) & 0x3F));
				 out += String.fromCharCode(0x80 | ((c >>  0) & 0x3F));
			 }else {
				 out += String.fromCharCode(0xC0 | ((c >>  6) & 0x1F));
				 out += String.fromCharCode(0x80 | ((c >>  0) & 0x3F));
			 }
		 }
		 return out;
	 }
	 function base64encode(str) {
		 var out, i, len;
		 var c1, c2, c3;
		 len = str.length;
		 i = 0;
		 out = "";
		 while(i < len) {
			 c1 = str.charCodeAt(i++) & 0xff;
			 if(i == len){
				 out += base64EncodeChars.charAt(c1 >> 2);
				 out += base64EncodeChars.charAt((c1 & 0x3) << 4);
				 out += "==";
				 break;
			 }
			 c2 = str.charCodeAt(i++);
			 if(i == len){
				 out += base64EncodeChars.charAt(c1 >> 2);
				 out += base64EncodeChars.charAt(((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4));
				 out += base64EncodeChars.charAt((c2 & 0xF) << 2);
				 out += "=";
				 break;
			 }
			 c3 = str.charCodeAt(i++);
			 out += base64EncodeChars.charAt(c1 >> 2);
			 out += base64EncodeChars.charAt(((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4));
			 out += base64EncodeChars.charAt(((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6));
			 out += base64EncodeChars.charAt(c3 & 0x3F);
		 }
		 return out;
	 }
	function baseEncodeURI(value,isSafe){
		var out = base64encode(utf16to8(value));
		if(!isSafe){
			return out;
		}
		return out.replace(/[+\/]/g, function(m0) {
				return m0 == '+' ? '-' : '_';
		}).replace(/=/g, '');
	}
	function sendToCppJson(json){
		var bs64Value = baseEncodeURI(JSON.stringify(json),true);
		var link = document.createElement("a");
		link.setAttribute("href","aicarb:"+bs64Value);
		link.click();
	}
	function sendToCppJsonString(json){
		var bs64Value = baseEncodeURI(json,true);
		var link = document.createElement("a");
		link.setAttribute("href","aicarb:"+bs64Value);
		link.click();
	}
    function sendToJs(value){
        if(cppCallFunction==null){
            return;
        }
        try{
            cppCallFunction(value);
        }catch(e){}
    }
    function registerHandler(callback){
        cppCallFunction = callback;
    }
	window.NeoLib={
		encode: baseEncodeURI,
		sendToCppJson:sendToCppJson,
		sendToCpp:sendToCppJsonString,
        sendToJs:sendToJs,
        registerCppCall:registerHandler
	};
})();