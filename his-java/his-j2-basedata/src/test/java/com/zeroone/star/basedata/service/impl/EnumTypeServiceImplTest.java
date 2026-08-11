package com.zeroone.star.basedata.service.impl;

import com.nimbusds.jose.JOSEException;
import com.zeroone.star.project.components.jwt.JwtComponent;
import jakarta.annotation.Resource;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit.jupiter.SpringExtension;


import static org.junit.jupiter.api.Assertions.*;
@SpringBootTest
@ExtendWith(SpringExtension.class)
class EnumTypeServiceImplTest {

    @Resource
    private JwtComponent jwtComponent;

    @Test
    public void getTestJwtToken() throws JOSEException {
        // 生成 JWT Token
        String token = jwtComponent.generateTokenByHmac(
                jwtComponent.getDefaultPayloadDto("test", "test", null, 86400),
                "RS256"
        );

        // 输出 Token
        System.out.println("生成的 JWT Token: \n\t" + token + "\n");

        // 验证 Token 不为空
        assertNotNull(token, "Token 不应为空");
        assertTrue(token.length() > 0, "Token 长度应大于0");

        // 可选：验证 Token 格式（JWT 通常由三段组成，用 . 分隔）
        String[] parts = token.split("\\.");
        assertEquals(3, parts.length, "JWT Token 应由三部分组成");
    }
}