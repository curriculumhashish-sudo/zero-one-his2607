# 患者管理模块 (his-j3-patient)

**模块名称：** 患者管理
**负责人员：** 照夜清、波小夏
**主启动类：** `com.zeroone.star.j3.patient.PatientApplication`
**端口变量：** `sp.j3-patient`（默认 `10202`）
**服务名变量：** `sn.j3-patient`（默认 `his-j3-patient`）
**上下文路径：** `/j3-basedata`

**接口测试：** 参见 [TESTING.md](TESTING.md)

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取选择患者列表（条件+分页） | GET | `/j3-basedata/patients/options` |
| 2 | 获取患者列表（条件+分页） | GET | `/j3-basedata/patients/page` |
| 3 | 获取患者详情 | GET | `/j3-basedata/patients/{id}` |
| 4 | 新增患者 | POST | `/j3-basedata/patients` |
| 5 | 修改患者 | PUT | `/j3-basedata/patients/{id}` |
| 6 | 删除患者 | DELETE | `/j3-basedata/patients/{id}` |
