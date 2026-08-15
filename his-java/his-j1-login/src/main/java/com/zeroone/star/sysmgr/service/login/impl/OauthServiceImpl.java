package com.zeroone.star.sysmgr.service.login.impl;

import com.zeroone.star.sysmgr.service.login.OauthService;
import lombok.AllArgsConstructor;

import java.util.Map;

@AllArgsConstructor
public class OauthServiceImpl implements OauthService {
    private Throwable throwable;

    private Map<String, Object> commonError() {
        if (throwable.getMessage() != null) {
            return Map.of(OauthService.ERROR_KEY, throwable.getMessage());
        } else {
            return Map.of(OauthService.ERROR_KEY, throwable.getClass().toGenericString());
        }
    }

    @Override
    public Map<String, Object> postAccessToken(Map<String, String> parameters) {
        return commonError();
    }

    @Override
    public Map<String, Object> revokeToken(Map<String, String> parameters) {
        return commonError();
    }
}
