package com.zeroone.star.cpp.factory;

import com.zeroone.star.cpp.service.impl.SampleServiceImpl;
import org.springframework.cloud.openfeign.FallbackFactory;
import org.springframework.stereotype.Component;

@Component
public class SampleServiceFallBack implements FallbackFactory<SampleServiceImpl> {
    @Override
    public SampleServiceImpl create(Throwable cause) {
        return new SampleServiceImpl(cause);
    }
}
