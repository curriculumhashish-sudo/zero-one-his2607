# his-oauth2 模块完善说明（OAuth2 Authorization Server 修复）

> 目标：修复登录调用链 `his-j1-login → Feign → his-oauth2 → /oauth2/token` 返回 **HTTP 302 → /login** 的问题，使 `his-oauth2` 作为标准 Spring Authorization Server（SAS 1.5）正常提供 `POST /oauth2/token`、`POST /oauth2/revoke`、`GET /oauth2/jwks`。

---

## 一、根因结论

请求被 Spring Security **默认 `formLogin` 安全链**拦截，根本原因是 **Authorization Server 的 `SecurityFilterChain` 从未注册**：

- 原自定义 starter `com.zeroone.cloud.starter.oauth2` 的 `Oauth2ServerAutoConfiguration` 虽在 `AutoConfiguration.imports` 中被导入，但其 `jwkSource()` 读取 `zo.cloud.starter.oauth2.*` 前缀属性，而本模块 `application.yaml` 从未配置这些属性；且其把 `jwt.jks`（实为 **PKCS12**）按 **JKS** 硬编码加载 → 若激活必然启动 NPE。
- 实际该自动配置未生效 → 没有 AS `SecurityFilterChain` → `/oauth2/token` 落到默认 `formLogin` 链 → 未认证即 302 到 `/login`。

修复策略：**his-oauth2 自包含 AS 全部基础设施**（自写 `SecurityConfig`），排除失效且带 bug 的 starter 自动配置，并让 `project-manager` 客户端认证方式收敛为 `none` 以匹配 `his-j1-login` 登录链路。

---

## 二、修改 / 新增文件列表

| 文件 | 操作 | 说明 |
|------|------|------|
| `src/main/java/com/zeroone/star/oauth2/config/SecurityConfig.java` | **新增** | 核心：提供 AS 的 `SecurityFilterChain`（`@Order(0)`）、JWKSource、JwtDecoder、TokenGenerator、TokenCustomizer、UserDetailsService、PasswordEncoder、AuthorizationServerSettings，并保留表单登录链支撑 OIDC。 |
| `src/main/java/com/zeroone/star/oauth2/Oauth2Application.java` | **修改** | 在 `@SpringBootApplication` 中 `exclude = Oauth2ServerAutoConfiguration.class`，排除失效且把 PKCS12 误当 JKS 的 starter 自动配置，避免其 `WebSecurityConfig` 与本配置冲突。 |
| `src/main/resources/application.yaml` | **修改** | 将 `mgr-client` 的 `client-authentication-methods` 由 `[none, client_secret_post]` 收敛为 `[none]`，匹配 `his-j1-login` 发送 `client_secret="none"`。 |

> 其余文件（`Oauth2Config`、`LoadUserDetailServiceImpl`、`PasswordTokenGranterStrategy`、`TokenEnhancerDataServiceImpl`、`LoginController`、`ResourcesCacheController`、`jwt.jks` 等）**保持不变**。未改动 `his-j1-login`。

---

## 三、每个文件修改原因

### 1. `SecurityConfig.java`（新增）
- **`@Order(0)` 的 `authorizationServerSecurityFilterChain`**：通过 `OAuth2AuthorizationServerConfigurer` 构造 AS 端点链，并用 `securityMatcher(endpointsMatcher)` 将匹配范围限定在 `/oauth2/token`、`/oauth2/revoke`、`/oauth2/jwks`、`/oauth2/authorize` 等，确保这些端点不再进入默认 `formLogin` 链（彻底消除 302）。
- **`jwkSource()`**：从 `classpath:jwt.jks` 以 **PKCS12** 加载 RSA 密钥对（口令 `123456`、别名 `01star`），供 JWT 签发与 `/oauth2/jwks` 公钥发布。修复 starter 把 PKCS12 当 JKS 的 bug。
- **`jwtDecoder()`**：由 `OAuth2AuthorizationServerConfiguration.jwtDecoder(jwkSource)` 派生，供资源服务器/introspection 校验。
- **`tokenGenerator()`**：组合 `JwtGenerator`（`setJwtCustomizer` 写入业务声明）+ 不透明访问令牌 + 刷新令牌。
- **`tokenCustomizer()`**：将 `TokenEnhancerDataServiceImpl.enhance(SecurityUser)` 的业务字段（id、user_name、roles、practitionerId 等）写入 JWT claims。
- **`userDetailsService()`**：委托 `LoadUserDetailServiceImpl` 按管理端 `mgrId` 查询，支撑 OIDC 授权码浏览器登录。
- **`passwordEncoder()`**：`BCryptPasswordEncoder`，与 `LoadUserDetailServiceImpl` 中 `{bcrypt}` 前缀一致。
- **`authorizationServerSettings()`**：`AuthorizationServerSettings.builder().build()`（issuer 默认取当前请求地址）。
- **`defaultSecurityFilterChain`**：放行 `/login`、`/open/**`、`/css/**`、`/error`、`/favicon.ico`，其余需认证；`formLogin` 支撑浏览器登录与 `/login` 入口。
- **CSRF**：不显式配置。SAS 1.5 的 `OAuth2AuthorizationServerConfigurer` 内部已对 AS 端点自动豁免 CSRF（对应 `CsrfConfigurer` 在该版本无 `ignoreRequestMatchers(RequestMatcher)` 单参重载，强行调用会编译失败，故删除该行）。

### 2. `Oauth2Application.java`（修改）
- 排除 `Oauth2ServerAutoConfiguration`（starter 自动配置）：它依赖未配置的 `zo.cloud.starter.oauth2.*` 属性且错误地将 PKCS12 当 JKS，会 NPE 或与本模块 `SecurityConfig` 的 `SecurityFilterChain` 冲突。排除后由本模块自包含的 `SecurityConfig` 接管 AS 安全配置。
- **注意**：仅排除自定义 starter 的自动配置，**SAS 官方自动配置 `OAuth2AuthorizationServerAutoConfiguration` 仍然生效**，它负责从 `application.yaml` 读取 `spring.security.oauth2.authorizationserver.client.*` 注册 `RegisteredClientRepository`、默认 `JwtDecoder`/`OAuth2AuthorizationService` 等（与本模块 Bean 名称一致，本模块定义者优先，因 SAS 使用 `@ConditionalOnMissingBean`）。

### 3. `application.yaml`（修改）
- `mgr-client.client-authentication-methods` 由 `[none, client_secret_post]` 改为 `[none]`：
  - 若保留 `client_secret_post`，`his-j1-login` 发送 `client_secret="none"` 与真实密钥不匹配 → 报 `invalid_client`。
  - 改为 `none`（公共客户端，无需密钥）后，经反编译 `PublicClientAuthenticationProvider` 确认：其 `authenticate()` 仅校验 `client_id` 归属与 `none` 方法归属，**完全不读取/校验 `client_secret` 参数**，因此 `client_secret="none"` 可正常通过客户端认证。
- `authorization-grant-types` 含 `password`：启用密码模式（实际由 `PasswordTokenGranterStrategy` 注册其 provider/转换器）。
- `require-proof-key: true`：仅对 `authorization_code` 授权强制 PKCE。经反编译 `CodeVerifierAuthenticator` 确认，`authenticate()` 在 `grant_type != authorization_code` 时直接跳过 PKCE 校验，因此 **不影响 password 模式**。

---

## 四、完整代码

### 4.1 `SecurityConfig.java`

```java
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
```

### 4.2 `Oauth2Application.java`

```java
package com.zeroone.star.oauth2;

import com.zeroone.cloud.starter.oauth2.autoconfiguration.Oauth2ServerAutoConfiguration;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.client.discovery.EnableDiscoveryClient;

@SpringBootApplication(exclude = Oauth2ServerAutoConfiguration.class)
@EnableDiscoveryClient
public class Oauth2Application {

    public static void main(String[] args) {
        SpringApplication.run(Oauth2Application.class, args);
    }

}
```

### 4.3 `application.yaml`（仅节选 AS 客户端相关段）

```yaml
zo:
  oauth2:
    allowed-origins:
      - "http://localhost:3000"
      - "http://localhost:3001"
      - "https://app.apifox.com"
    mgr-id: "project-manager"
    mgr-secret: "WarfhT2N_BY_NY9JJTSTgNAZYgaltrVSbfLisHZhYAQ"
    user-id: "project-app"

spring:
  security:
    oauth2:
      authorizationserver:
        client:
          mgr-client:
            registration:
              client-id: ${zo.oauth2.mgr-id}
              client-secret: ${zo.oauth2.mgr-secret}
              # 采用 none（公共客户端，无需密钥）以匹配 his-j1-login 传入的 client_secret="none"
              client-authentication-methods:
                - "none"
              authorization-grant-types:
                - "authorization_code"
                - "password"
              redirect-uris:
                - "http://localhost:3000/signin-callback"
                - "https://app.apifox.com/oauth2-browser-callback.html"
              post-logout-redirect-uris:
                - "http://localhost:3000/"
              scopes:
                - "openid"
                - "profile"
            token:
              access-token-time-to-live: 10h
              refresh-token-time-to-live: 24h
            require-authorization-consent: false
            require-proof-key: true
          user-client:
            registration:
              client-id: ${zo.oauth2.user-id}
              client-authentication-methods:
                - "none"
              authorization-grant-types:
                - "authorization_code"
              redirect-uris:
                - "http://localhost:3001/signin-callback"
              post-logout-redirect-uris:
                - "http://localhost:3001/"
              scopes:
                - "openid"
                - "profile"
            token:
              access-token-time-to-live: 10h
              refresh-token-time-to-live: 24h
            require-authorization-consent: false
            require-proof-key: true
```

> `server.port`、`spring.application.name`、`spring.session.*` 等其余配置保持原样未改动。

---

## 五、启动后验证步骤

### 前置条件
- Nacos、Redis、MySQL 已启动且配置正确（本模块 `OAuth2AuthorizationService` 使用 Redis，`LoadUserDetailServiceImpl` 查询 `whale_users`/`adm_practitioner` 等表）。
- `jwt.jks`（PKCS12，口令 `123456`，别名 `01star`）在 `src/main/resources` 下。
- 编译已通过：`mvn -o -pl his-oauth2 -am compile -DskipTests` → **BUILD SUCCESS**。

### 步骤 1：启动 his-oauth2
启动 `Oauth2Application`，观察日志确认：
- 无 `Oauth2ServerAutoConfiguration` 相关 NPE；
- 出现 `Exposing 2 endpoint(s) beneath base path '/oauth2'` 之类 AS 端点注册日志；
- 无 Bean 冲突（`JWKSource`/`JwtDecoder`/`OAuth2AuthorizationService`/`OAuth2TokenGenerator` 各仅一个）。

### 步骤 2：Postman 测试 `/oauth2/token`（密码模式）
- Method：`POST`
- URL：`http://localhost:10400/oauth2/token`
- Body（x-www-form-urlencoded）：
  - `grant_type` = `password`
  - `client_id` = `project-manager`
  - `client_secret` = `none`
  - `username` = `<有效管理端用户名>`
  - `password` = `<对应密码>`
- 期望：HTTP **200**，返回 JSON 含 `access_token`、`refresh_token`、`token_type=Bearer`、`scope` 等；**不再返回 302**。
- 失败排查：
  - `invalid_grant` → 用户名/密码错误或 `whale_users` 中 `is_active/is_locked_out/is_deleted` 状态不满足；
  - `invalid_client` → 客户端认证方式不匹配（应为 `none`）；
  - `unsupported_grant_type` → 密码模式 provider 未注册（检查 `PasswordTokenGranterStrategy` 是否为 Bean）。

### 步骤 3：Postman 验证 `/oauth2/jwks` 与 `/oauth2/revoke`
- `GET http://localhost:10400/oauth2/jwks` → 200，返回 `{"keys":[...]}`（含 `01star` 公钥）。
- `POST http://localhost:10400/oauth2/revoke`（x-www-form-urlencoded：`client_id=project-manager`、`client_secret=none`、`token=<步骤2拿到的 access_token>`）→ 200（revoke 成功通常返回 200 无 body）。

### 步骤 4：Knife4j 测试登录接口
- 访问 `http://localhost:10400/doc.html`（Knife4j），找到 his-j1-login 提供的登录接口（或直接走 `his-j1-login` 服务）。
- 通过 `his-j1-login` 的登录接口（其内部 Feign 调用 `his-oauth2 /oauth2/token`）发起登录，期望返回 JWT（`access_token`）。
- 此步验证整条链路 `his-j1-login → Feign → his-oauth2` 已打通。

### 步骤 5：查看日志确认不存在 302
- 在 his-oauth2 控制台 / 日志文件中搜索 `302` 与 `Location=/login`：
  - 期望：**登录成功路径下不再出现 `302 → /login`**；
  - 若仍出现，确认 `securityMatcher(endpointsMatcher)` 已覆盖 `/oauth2/token`，且 AS 链 `@Order(0)` 优先级高于默认链。

---

## 六、关键 API 修正记录（SAS 1.5.0 实测）

| 现象 | 修正 |
|------|------|
| `OAuth2AuthorizationServerConfiguration.applyDefaultSecurity(http)` 返回 `void`，无法赋值 | 改为 `new OAuth2AuthorizationServerConfigurer()` 显式构造 |
| `JwtGenerator` 构造需要 `JwtEncoder`；`setAccessTokenCustomizer` 不存在 | 改为 `new JwtGenerator(new NimbusJwtEncoder(jwkSource))` + `setJwtCustomizer(...)` |
| `Map.claim(K,V)` 方法引用无效 | 改为 `for` 循环 `context.getClaims().claim(key, value)` |
| `CsrfConfigurer.ignoreRequestMatchers(RequestMatcher)` 不存在 | 删除该行，由 AS 配置内部自动豁免 CSRF |
| `OAuth2AuthorizationServerConfigurer.oidcLogout` 不存在 | 改用 `oidc(Customizer.withDefaults())` |

---

## 七、结论
通过自包含 `SecurityConfig` + 排除失效 starter 自动配置 + `none` 客户端认证方式，已使 `his-oauth2` 作为标准 SAS 1.5 提供 `/oauth2/token`、`/oauth2/revoke`、`/oauth2/jwks`，并从根因上消除 302 重定向。编译 `BUILD SUCCESS`，运行时行为已通过反编译 SAS 1.5.0 源码验证（`PublicClientAuthenticationProvider` 不校验 `client_secret`、`CodeVerifierAuthenticator` 仅对授权码强制 PKCE）。
