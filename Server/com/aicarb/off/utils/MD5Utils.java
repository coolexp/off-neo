package com.aicarb.off.utils;

import com.aicarb.off.comparator.ParamComparator;
import com.google.common.hash.Hasher;
import com.google.common.hash.Hashing;
import org.apache.commons.lang3.StringUtils;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.servlet.http.HttpServletRequest;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.nio.charset.Charset;
import java.util.*;

public class MD5Utils {
    private static String NEO_PAY_KEY="YOU_PAY_KEY";
    private static final Logger logger = LogManager.getLogger(MD5Utils.class);
    public static String md5(String value){
        Hasher hasher = Hashing.md5().newHasher();
        hasher.putString(value, Charset.forName("UTF-8"));
        return hasher.hash().toString();
    }
    public static String sign(TreeMap<String, String> params) {
        StringBuilder buf = new StringBuilder();
        List<Map.Entry<String, String>> infoIds = new ArrayList<Map.Entry<String, String>>(params.entrySet());
        Collections.sort(infoIds,new ParamComparator());
        for (Map.Entry<String, String> item : infoIds){
            if (StringUtils.isNotBlank(item.getKey())){
                String val = item.getValue();
                try{
                    val = URLEncoder.encode(val, "utf-8");
                }catch (Exception e){
                    logger.info(e.toString());
                }
                buf.append(item.getKey()+"="+val+"&");
            }
        }
        buf.append("key="+NEO_PAY_KEY);
        logger.info("md5:"+buf.toString());
        String md5Value = md5(buf.toString());
        logger.info("md5Value:"+md5Value);
        return md5Value;
    }
    public static boolean verifySign(HttpServletRequest request) throws Exception {
        TreeMap<String, String> map = new TreeMap<String, String>();

        String signStr = request.getParameter("sign");
        if(StringUtils.isBlank(signStr)){
            throw new RuntimeException("There is no signature field in the request parameter!");
        }
        Enumeration<String> enu = request.getParameterNames();
        while (enu.hasMoreElements()) {
            String paramName = enu.nextElement().trim();
            if (!paramName.equals("sign")) {
                map.put(paramName, URLDecoder.decode(request.getParameter(paramName), "UTF-8"));
            }
        }

        if (!sign(map).equals(signStr)) {
            return false;
        }
        return true;
    }

    public static void main(String[] args) {
        TreeMap<String, String> map = new TreeMap<String, String>();
        String count = "0.001";
        String from = "AMgPM4VyYxoaoRcLXQLtyo2mRmrb3Us9NU";
        String g_id = "12";
        String tm = "1533690901";
        String txid = "0x311cbc36e17ffad000050b3a4754f21147dd2e0adab53826e8a440bda97f3244";
        String net_type = "2";
        String params="{\"count\":\"0.001\",\"extString\":\"{\\\"TargetId\\\":99900004，\\\"ChargeId\\\":3}\",\"nnc\":\"0x2761020e5e6dfcd8d37fdd50ff98fa0f93bccf54\",\"sbParamJson\":[\"(address)AMgPM4VyYxoaoRcLXQLtyo2mRmrb3Us9NU\",\"(address)AafE7Vk3q8cRrgRGisxdKrzUk1ckapFZCg\",\"(integer)100000\"],\"sbPushString\":\"transfer\"}";
        map.put("count",count);
        map.put("from",from);
        map.put("g_id",g_id);
        map.put("tm",tm);
        map.put("txid",txid);
        map.put("net_type",net_type);
        map.put("params",params);

        String signStr = "66bc3bf769b7710e29e3813496e6f797";
        if (!sign(map).equals(signStr)) {
           logger.info("not maatch");
           return;
        }
        logger.info("match");
    }
}
