package com.zeroone.star.oauth2.config;

import com.nimbusds.jose.jwk.JWKSet;
import com.nimbusds.jose.jwk.RSAKey;
import com.nimbusds.jose.jwk.source.ImmutableJWKSet;
import com.nimbusds.jose.jwk.source.JWKSource;
import com.nimbusds.jose.proc.SecurityContext;
import com.zeroone.cloud.oauth2.entity.SecurityUser;
import com.zeroone.cloud.starter.oauth2.service.TokenGranterStrategy;
import com.zeroone.star.oauth2.config.Oauth2Properties;
import com.zeroone.star.oauth2.service.impl.oauth2.LoadUserDetailServiceImpl;
import com.zeroone.star.oauth2.service.impl.oauth2.TokenEnhancerDataServiceImpl;
import jakarta.annotation.Resource;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.annotation.Order;
import org.springframework.core.io.ClassPathResource;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.oauth2.jwt.JwtDecoder;
import org.springframework.security.oauth2.jwt.NimbusJwtEncoder;
import org.springframework.security.oauth2.server.authorization.OAuth2AuthorizationService;
import org.springframework.security.oauth2.server.authorization.config.annotation.web.configuration.OAuth2AuthorizationServerConfiguration;
import org.springframework.security.oauth2.server.authorization.config.annotation.web.configurers.OAuth2AuthorizationServerConfigurer;
import org.springframework.security.oauth2.server.authorization.settings.AuthorizationServerSettings;
import org.springframework.security.oauth2.server.authorization.token.DelegatingOAuth2TokenGenerator;
import org.springframework.security.oauth2.server.authorization.token.JwtEncodingContext;
import org.springframework.security.oauth2.server.authorization.token.JwtGenerator;
import org.springframework.security.oauth2.server.authorization.token.OAuth2AccessTokenGenerator;
import org.springframework.security.oauth2.server.authorization.token.OAuth2RefreshTokenGenerator;
import org.springframework.security.oauth2.server.authorization.token.OAuth2TokenCustomizer;
import org.springframework.security.oauth2.server.authorization.token.OAuth2TokenGenerator;
import org.springframework.security.web.SecurityFilterChain;
import org.springframework.security.web.access.AccessDeniedHandlerImpl;
import org.springframework.security.web.authentication.LoginUrlAuthenticationEntryPoint;

import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.Certificate;
import java.security.interfaces.RSAPublicKey;
import java.util.List;
import java.util.Map;

/**
 * <p>
 * 描述：OAuth2 Authorization Server 安全配置（Spring Authorization Server 1.5 标准写法）
 * </p>
 * <p>职责：</p>
 * <ol>
 *   <li>提供 Authorization Server 的 SecurityFilterChain（最高优先级），暴露
 *       /oauth2/token、/oauth2/revoke、/oauth2/jwks 等端点，避免被默认 formLogin 拦截返回 302；</li>
 *   <li>加载 jwt.jks（PKCS12）生成 JWKSource，使 JWT 签发与公钥获取可用；</li>
 *   <li>注册自定义授权模式（password 密码模式）及其他基础设施 Bean；</li>
 *   <li>提供表单登录链，支撑 OIDC 授权码浏览器登录流程。</li>
 * </ol>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长（完善）
 * @version 2.0.0
 */
@Configuration
@EnableWebSecurity
public class SecurityConfig {

    /**
     * jwt.jks 密钥库口令（与密钥口令一致）。
     * 注意：原自定义 starter 的 jwkSource 写死 KeyStore.getInstance("JKS")，
     * 但 jwt.jks 实际为 PKCS12 格式，故此处使用 PKCS12 直接加载。
     */
    private static final String KEYSTORE_PASSWORD = "123456";
    private static final String KEYSTORE_ALIAS = "01star";
    private static final String KEYSTORE_RESOURCE = "jwt.jks";

    @Resource
    Oauth2Properties oauth2Properties;

    /**
     * JWK 密钥源：从 classpath 下的 jwt.jks（PKCS12）加载 RSA 密钥对，
     * 供 JWT 签发（JwtGenerator）与 /oauth2/jwks 公钥发布使用。
     */
    @Bean
    public JWKSource<SecurityContext> jwkSource() throws Exception {
        ClassPathResource resource = new ClassPathResource(KEYSTORE_RESOURCE);
        KeyStore keyStore = KeyStore.getInstance("PKCS12");
        char[] password = KEYSTORE_PASSWORD.toCharArray();
        keyStore.load(resource.getInputStream(), password);

        PrivateKey privateKey = (PrivateKey) keyStore.getKey(KEYSTORE_ALIAS, password);
        Certificate certificate = keyStore.getCertificate(KEYSTORE_ALIAS);
        RSAPublicKey publicKey = (RSAPublicKey) certificate.getPublicKey();

        RSAKey rsaKey = new RSAKey.Builder(publicKey)
                .privateKey(privateKey)
                .keyID(KEYSTORE_ALIAS)
                .build();
        return new ImmutableJWKSet(new JWKSet(rsaKey));
    }

    /**
     * JWT 解码器（用于资源服务器 / introspection 校验）。
     */
    @Bean
    public JwtDecoder jwtDecoder(JWKSource<SecurityContext> jwkSource) {
        return OAuth2AuthorizationServerConfiguration.jwtDecoder(jwkSource);
    }

    /**
     * 令牌生成器：组合 JWT 访问令牌、不透明访问令牌与刷新令牌。
     * 注入 OAuth2TokenCustomizer 以写入业务声明（角色、人员编号等）。
     */
    @Bean
    public OAuth2TokenGenerator<?> tokenGenerator(JWKSource<SecurityContext> jwkSource,
                                                   OAuth2TokenCustomizer<JwtEncodingContext> tokenCustomizer) {
        JwtGenerator jwtGenerator = new JwtGenerator(new NimbusJwtEncoder(jwkSource));
        jwtGenerator.setJwtCustomizer(tokenCustomizer);
        OAuth2AccessTokenGenerator accessTokenGenerator = new OAuth2AccessTokenGenerator();
        OAuth2RefreshTokenGenerator refreshTokenGenerator = new OAuth2RefreshTokenGenerator();
        return new DelegatingOAuth2TokenGenerator(jwtGenerator, accessTokenGenerator, refreshTokenGenerator);
    }

    /**
     * 令牌声明增强：将 TokenEnhancerDataServiceImpl 的业务字段写入 JWT claims。
     */
    @Bean
    public OAuth2TokenCustomizer<JwtEncodingContext> tokenCustomizer(TokenEnhancerDataServiceImpl enhancer) {
        return context -> {
            Object principal = context.getPrincipal().getPrincipal();
            if (principal instanceof SecurityUser securityUser) {
                Map<String, Object> extra = enhancer.enhance(securityUser);
                if (extra != null) {
                    for (Map.Entry<String, Object> entry : extra.entrySet()) {
                        context.getClaims().claim(entry.getKey(), entry.getValue());
                    }
                }
            }
        };
    }

    /**
     * 用户详情服务：支撑 OIDC 授权码浏览器登录（formLogin）。
     * 委托 LoadUserDetailServiceImpl，按管理端 clientId 查询。
     */
    @Bean
    public UserDetailsService userDetailsService(LoadUserDetailServiceImpl loadUserDetailService) {
        return username -> loadUserDetailService.loadUserDetail(username, oauth2Properties.getMgrId());
    }

    /**
     * 密码编码器：与 LoadUserDetailServiceImpl 中 {bcrypt} 前缀保持一致。
     */
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new BCryptPasswordEncoder();
    }

    /**
     * Authorization Server 基础设置（issuer 默认取当前请求地址）。
     */
    @Bean
    public AuthorizationServerSettings authorizationServerSettings() {
        return AuthorizationServerSettings.builder().build();
    }

    /**
     * Authorization Server 安全过滤链（最高优先级）。
     * 标准写法：构造 OAuth2AuthorizationServerConfigurer，注册自定义授权模式（password 密码模式），
     * 并将安全匹配器限定为 AS 端点，避免 /oauth2/token 被默认 formLogin 拦截返回 302。
     */
    @Bean
    @Order(0)
    public SecurityFilterChain authorizationServerSecurityFilterChain(
            HttpSecurity http,
            List<TokenGranterStrategy> customStrategies,
            OAuth2AuthorizationService authorizationService,
            OAuth2TokenGenerator<?> tokenGenerator) throws Exception {

        OAuth2AuthorizationServerConfigurer authorizationServerConfigurer =
                new OAuth2AuthorizationServerConfigurer();

        // 注册自定义授权模式（password 等）
        authorizationServerConfigurer.tokenEndpoint(tokenEndpoint ->
                customStrategies.forEach(strategy -> {
                    tokenEndpoint.accessTokenRequestConverter(strategy.getConverter());
                    tokenEndpoint.authenticationProvider(
                            strategy.getProvider(authorizationService, tokenGenerator));
                })
        );

        // 启用 OIDC（userinfo 端点）
        authorizationServerConfigurer.oidc(Customizer.withDefaults());

        // 将安全匹配器限定为 AS 端点（token / jwks / authorize / revoke 等）
        org.springframework.security.web.util.matcher.RequestMatcher endpointsMatcher =
                authorizationServerConfigurer.getEndpointsMatcher();

        http
                .securityMatcher(endpointsMatcher)
                .authorizeHttpRequests(authorize -> authorize.anyRequest().authenticated())
                // CSRF 由 OAuth2AuthorizationServerConfigurer 内部对 AS 端点（/oauth2/token、/oauth2/revoke 等）自动豁免
                .with(authorizationServerConfigurer, Customizer.withDefaults())
                .exceptionHandling(exceptions -> exceptions
                        .authenticationEntryPoint(new LoginUrlAuthenticationEntryPoint("/login"))
                        .accessDeniedHandler(new AccessDeniedHandlerImpl())
                );

        return http.build();
    }

    /**
     * 默认安全过滤链：处理非 AS 端点（如登录页、内部开放接口）。
     * 表单登录支撑 OIDC 授权码浏览器登录；其余请求需认证。
     */
    @Bean
    public SecurityFilterChain defaultSecurityFilterChain(HttpSecurity http) throws Exception {
        http
                .authorizeHttpRequests(authorize -> authorize
                        .requestMatchers("/login", "/open/**", "/css/**", "/error", "/favicon.ico")
                        .permitAll()
                        .anyRequest().authenticated()
                )
                .formLogin(formLogin -> formLogin.loginPage("/login").permitAll());
        return http.build();
    }
}
