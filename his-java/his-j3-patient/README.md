# 患者管理模块 (his-j3-patient)

**模块名称：** 患者管理
**负责人员：** 夜照清、波小夏
**主启动类：** `com.zeroone.star.j3.patient.PatientApplication`
**端口变量：** `sp.patient`
**服务名变量：** `sn.patient`
**上下文路径：** `/patient`

## 业务说明

患者档案是门诊挂号、住院登记的前置基础。重点关注患者建档、患者去重、证件号校验、敏感字段处理。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取选择患者列表（条件+分页） | GET | `/patient/options` |
| 2 | 获取患者列表（条件+分页） | GET | `/patient/page` |
| 3 | 获取患者详情 | GET | `/patient/{id}` |
| 4 | 新增患者 | POST | `/patient` |
| 5 | 修改患者 | PUT | `/patient/{id}` |
| 6 | 删除患者 | DELETE | `/patient/{id}` |
