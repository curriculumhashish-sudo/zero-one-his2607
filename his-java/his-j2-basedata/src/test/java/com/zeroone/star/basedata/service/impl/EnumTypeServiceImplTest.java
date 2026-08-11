package com.zeroone.star.basedata.service.impl;

import com.nimbusds.jose.JOSEException;
import com.zeroone.star.project.components.jwt.JwtComponent;
import jakarta.annotation.Resource;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;
class EnumTypeServiceImplTest {

    @Resource
    private JwtComponent jwtComponent;

    @Test
    public void getTestJwtToken() throws JOSEException {
        String token = jwtComponent.generateTokenByHmac(
                jwtComponent.getDefaultPayloadDto("test", "test", null, 86400),
                "test"
        );

        System.out.println(token);
    }
}