# 挂号类型管理模块 (his-j3-register)

**模块名称：** 挂号类型管理
**负责人员：** 九洲、陈皮
**主启动类：** `com.zeroone.star.j3.register.RegisterApplication`
**端口变量：** `sp.register`
**服务名变量：** `sn.register`
**上下文路径：** `/register`

## 业务说明

挂号类型用于门诊挂号页的号源类型配置，如普通号、专家号等。整体是基础配置类模块，但要关注是否影响收费标准和号源配置。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取挂号类型名称 | GET | `/register/options` |
| 2 | 获取挂号类型列表（条件+分页） | GET | `/register/page` |
| 3 | 添加挂号类型 | POST | `/register` |
| 4 | 修改挂号类型 | PUT | `/register/{id}` |
| 5 | 删除挂号类型 | DELETE | `/register/{id}` |
