package com.zeroone.star.sysmgr.fallback.login;

import com.zeroone.star.sysmgr.service.login.impl.OauthServiceImpl;
import org.springframework.cloud.openfeign.FallbackFactory;
import org.springframework.stereotype.Component;

@Component
public class OauthServiceFallbackFactory implements FallbackFactory<OauthServiceImpl> {
    @Override
    public OauthServiceImpl create(Throwable throwable) {
        return new OauthServiceImpl(throwable);
    }
}
