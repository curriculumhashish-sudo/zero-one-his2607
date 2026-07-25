package com.zeroone.star.oauth2.service.impl.oauth2;

import com.baomidou.mybatisplus.core.conditions.query.QueryWrapper;
import com.zeroone.cloud.starter.oauth2.service.TokenGranterStrategy;
import com.zeroone.cloud.starter.oauth2.service.grant.AbstractCustomAuthenticationConverter;
import com.zeroone.cloud.starter.oauth2.service.grant.AbstractCustomAuthenticationProvider;
import com.zeroone.cloud.starter.oauth2.service.grant.CustomGrantAuthenticationToken;
import com.zeroone.star.oauth2.entity.User;
import com.zeroone.star.oauth2.service.IUserService;
import jakarta.annotation.Resource;
import org.springframework.security.authentication.AuthenticationProvider;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.authority.AuthorityUtils;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.oauth2.core.OAuth2AuthenticationException;
import org.springframework.security.oauth2.core.OAuth2Token;
import org.springframework.security.oauth2.server.authorization.OAuth2AuthorizationService;
import org.springframework.security.oauth2.server.authorization.client.RegisteredClient;
import org.springframework.security.oauth2.server.authorization.token.OAuth2TokenGenerator;
import org.springframework.security.web.authentication.AuthenticationConverter;
import org.springframework.stereotype.Component;
import org.springframework.util.StringUtils;

import java.util.Map;

/** OAuth2 密码模式授权策略，认证数据源为 whale_users。 */
@Component
public class PasswordTokenGranterStrategy implements TokenGranterStrategy {

    private static final String GRANT_TYPE = "password";

    @Resource
    private IUserService userService;

    @Override
    public String getGrantType() {
        return GRANT_TYPE;
    }

    @Override
    public AuthenticationConverter getConverter() {
        return new AbstractCustomAuthenticationConverter<CustomGrantAuthenticationToken>(GRANT_TYPE) {
            @Override
            protected CustomGrantAuthenticationToken createAuthentication(Authentication clientPrincipal,
                                                                          Map<String, Object> parameters) {
                return new CustomGrantAuthenticationToken(GRANT_TYPE, clientPrincipal, parameters);
            }
        };
    }

    @Override
    public AuthenticationProvider getProvider(OAuth2AuthorizationService authorizationService,
                                              OAuth2TokenGenerator<? extends OAuth2Token> tokenGenerator) {
        return new AbstractCustomAuthenticationProvider(authorizationService, tokenGenerator) {
            @Override
            protected Authentication authenticateUser(CustomGrantAuthenticationToken authentication,
                                                      RegisteredClient registeredClient) {
                String username = value(authentication, "username");
                String password = value(authentication, "password");
                if (!StringUtils.hasText(username) || !StringUtils.hasText(password)) {
                    throw new OAuth2AuthenticationException("invalid_grant");
                }
                User user = userService.getOne(new QueryWrapper<User>()
                        .eq("user_name", username).eq("is_active", 1)
                        .eq("is_locked_out", 0).eq("is_deleted", 0));
                if (user == null || !new BCryptPasswordEncoder().matches(password, bcryptHash(user.getPasswordHash()))) {
                    throw new OAuth2AuthenticationException("invalid_grant");
                }
                return new UsernamePasswordAuthenticationToken(user, null, AuthorityUtils.NO_AUTHORITIES);
            }
        };
    }

    private String value(CustomGrantAuthenticationToken authentication, String key) {
        Object value = authentication.getAdditionalParameters().get(key);
        return value == null ? null : value.toString();
    }

    private String bcryptHash(String passwordHash) {
        return passwordHash != null && passwordHash.startsWith("{bcrypt}")
                ? passwordHash.substring("{bcrypt}".length()) : passwordHash;
    }
}
