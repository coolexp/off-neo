package com.aicarb.off.controller;

import com.aicarb.off.service.IOffService;
import com.aicarb.off.utils.MD5Utils;
import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;

@RestController
@RequestMapping("/sg")
public class OFFController {
    private static final Logger logger = LogManager.getLogger(HelloController.class);

    @Autowired
    private IOffService offService;
    @RequestMapping(value = "/pay", method ={RequestMethod.POST,RequestMethod.GET})
    @ResponseBody
    public String sgPay(HttpServletRequest request) {
        logger.info(request.getRequestURL());
        logger.info(JSON.toJSONString(request.getParameterMap()));
        boolean isCorrect = false;
        try{
            isCorrect = MD5Utils.verifySign(request);
        }catch (Exception e){
            logger.info(e.toString());
        }
        if(!isCorrect){
            logger.info("sign  not match");
            return "failed";
        }
        logger.info("sign match");
        // 获取额外参数
        try{
            JSONObject jsonObject = JSON.parseObject(request.getParameter("params"));
            String extString = jsonObject.getString("extString");
            // 调用充值
            boolean chargeResult= offService.pay(request.getParameter("count"),request.getParameter("from"),extString);
            if(chargeResult){
                logger.info("success");
                return "success";
            }
        }catch (Exception e){
            logger.info("controller:"+e.toString());
        }
        logger.info("failed");
        return "failed";
    }
}
