# 工程简介

j7 组药房相关业务模块（临时命名，后续按组长规范对齐）。

当前阶段用于住院退药查询等接口文档自测，暂不接入数据库。

主启动类：`com.zeroone.star.j7.pharmacy.J7PharmacyApplication`

# 配置说明

端口与服务名从 Nacos `system.yaml` 读取（namespace: `his-dev`）：

```yaml
sn:
  j7-pharmacy: J7-PHARMACY
sp:
  j7-pharmacy: 10500
```

# Knife4j 文档

服务启动后访问：`http://localhost:10500/j7-pharmacy/doc.html`
# his-j7-pharmacy

J7 药房管理模块，基于 Spring Cloud 微服务架构的 business 类型模块。

## 模块信息

- **模块名称**：his-j7-pharmacy
- **基础包名**：com.zeroone.star.j7.pharmacy
- **主启动类**：com.zeroone.star.j7.pharmacy.J7PharmacyApplication
- **上下文路径**：/j7-pharmacy
- **服务端口**：10100

## 技术栈

- Spring Boot 3
- Spring Cloud Alibaba（Nacos 注册中心 & 配置中心）
- MyBatis-Plus
- Druid 连接池
- MySQL
- MapStruct
- Docker
