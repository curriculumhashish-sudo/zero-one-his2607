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
