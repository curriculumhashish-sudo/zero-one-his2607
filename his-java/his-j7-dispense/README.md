# his-j7-dispense

J7 药房住院发退药记录服务。

- 启动类：com.zeroone.star.dispense.DispenseApplication
- 默认端口：10510
- 上下文路径：/j7-dispense
- 服务名：J7-DISPENSE
- Knife4j 文档：http://localhost:10510/j7-dispense/doc.html
- Swagger UI：http://localhost:10510/j7-dispense/swagger-ui.html
- OpenAPI JSON：http://localhost:10510/j7-dispense/v3/api-docs
- 分页接口：POST /pharmacy/dispense-record/inpatient/page
- 导出接口：GET /pharmacy/dispense-record/inpatient/export

运行时从 Nacos 加载系统配置和数据源配置，并注册到 Nacos 服务发现。
