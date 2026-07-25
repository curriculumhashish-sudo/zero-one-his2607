package com.zeroone.star.sysmgr.service.login;

import com.zeroone.star.sysmgr.fallback.login.OauthServiceFallbackFactory;
import org.springframework.cloud.openfeign.FeignClient;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;

import java.util.Map;

@FeignClient(value = "${sn.auth}", fallbackFactory = OauthServiceFallbackFactory.class)
public interface OauthService {
    String ERROR_KEY = "error";

    @PostMapping(value = "/oauth2/token", consumes = "application/x-www-form-urlencoded")
    Map<String, Object> postAccessToken(@RequestBody Map<String, String> parameters);

    @PostMapping(value = "/oauth2/revoke", consumes = "application/x-www-form-urlencoded")
    Map<String, Object> revokeToken(@RequestBody Map<String, String> parameters);
}
