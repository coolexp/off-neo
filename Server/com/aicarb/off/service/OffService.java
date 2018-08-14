package com.aicarb.off.service;

import com.aicarb.off.utils.MediaUtils;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.lang3.StringUtils;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;

import java.net.URI;
import java.util.HashMap;
import java.util.Map;

@Service
public class OffService implements IOffService {
    private static final Logger logger = LogManager.getLogger(OffService.class);
    private static Integer SERVER_ID=999;
    private static String ORDER_PREFIX="NEO_";
    private static String TRANC_PREFIX="1024_";
    private static String CHARGE_URL="http://api.api.aicarb.com/do_charge";
    @Override
    public boolean pay(String count,String userId,String extString) {
        JSONObject extObj = JSON.parseObject(extString);
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders requestHeaders = new HttpHeaders();
        String order_id = ORDER_PREFIX+System.currentTimeMillis();
        String transaction_id = TRANC_PREFIX+System.currentTimeMillis();
        HttpEntity<String> requestEntity = new HttpEntity<String>(null, requestHeaders);
        Map<String, Object> params = new HashMap<>();
        params.put("serverid",SERVER_ID);
        params.put("open_id",userId);
        params.put("amount",-1);
        params.put("product_id",extObj.getString("productId"));
        params.put("order_id",order_id);
        params.put("transaction_id",transaction_id);
        Map<String,Object> extMap = new HashMap<String,Object>();
        extMap.put("TargetId",extObj.getInteger("TargetId"));
        extMap.put("ChargeId",extObj.getInteger("ChargeId"));
        try{
            params.put("extra",JSON.toJSONString(extMap));
        }catch (Exception e){
            logger.info("extMap:"+e.toString());
        }
        String paramsValue = MediaUtils.generateParams(params);
        String url = CHARGE_URL+paramsValue;
        logger.info("request url:"+url);
        URI uri = UriComponentsBuilder.fromUriString(url).build().encode().toUri();
        String value = restTemplate.getForObject(uri,String.class);
        if(StringUtils.isEmpty(value)){
            return false;
        }
        logger.info("rss.getBody():"+value);
        Integer result = Integer.parseInt(value);
        if(result==1){
            return true;
        }
        return false;
    }

    public static void main(String[] args) {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders requestHeaders = new HttpHeaders();
        HttpEntity<String> requestEntity = new HttpEntity<String>(null, requestHeaders);
        Map<String,Object> extMap = new HashMap<String,Object>();
        extMap.put("TargetId",99900004);
        extMap.put("ChargeId",3);
        String ext = "";
        try{
            ext = JSON.toJSONString(extMap);
//            URLCodec codec = new URLCodec();
//            ext = codec.encode(ext);
        }catch (Exception e){
            logger.info(e.toString());
        }
        String url="http://api.api.aicarb.com/do_charge?transaction_id=1024_15336973215539&amount=-1&open_id=AMgPM4VyYxoaoRcLXQLtyo2mRmrb3Us9NU&product_id=null&extra=%s&serverid=999&order_id=NEO_1533697321559";
        url = String.format(url,ext);
        URI uri = UriComponentsBuilder.fromUriString(url).build().encode().toUri();
        //ResponseEntity<String> rss = restTemplate.exchange(url, HttpMethod.GET, requestEntity, String.class);
        System.out.println("rss.getBody():"+restTemplate.getForObject(uri,String.class));
        System.out.println("end");
    }
}
