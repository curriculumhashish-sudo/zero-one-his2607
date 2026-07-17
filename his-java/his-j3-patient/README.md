# 患者管理模块 (his-j3-patient)

**模块名称：** 患者管理
**负责人员：** 夜照清、波小夏
**主启动类：** `com.zeroone.star.j3.patient.PatientApplication`
**端口变量：** `sp.patient`
**服务名变量：** `sn.patient`
**上下文路径：** `/database`

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取选择患者列表（条件+分页） | GET | `/database/patients/options` |
| 2 | 获取患者列表（条件+分页） | GET | `/database/patients/page` |
| 3 | 获取患者详情 | GET | `/database/patients/{id}` |
| 4 | 新增患者 | POST | `/database/patients` |
| 5 | 修改患者 | PUT | `/database/patients/{id}` |
| 6 | 删除患者 | DELETE | `/database/patients/{id}` |
