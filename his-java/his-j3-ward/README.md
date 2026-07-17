# 病区病房管理模块 (his-j3-ward)

**模块名称：** 病区病房管理
**负责人员：** 小苗莜、zhu
**主启动类：** `com.zeroone.star.j3.ward.WardApplication`
**端口变量：** `sp.ward`
**服务名变量：** `sn.ward`
**上下文路径：** `/ward`

## 业务说明

病区、病房、病床是三级结构，和住院登记、护士分床、入出转管理强关联。删除和启停要考虑是否已有在院患者。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取病床名称（根据病房，含状态） | GET | `/ward/beds/options` |
| 2 | 获取病房名称（根据病区） | GET | `/ward/rooms/options` |
| 3 | 获取病区名称 | GET | `/ward/areas/options` |
| 4 | 获取病区/房/床列表（条件） | GET | `/ward/page` |
| 5 | 添加病区/房/床 | POST | `/ward` |
| 6 | 修改病区/房/床 | PUT | `/ward/{id}` |
| 7 | 删除病区/房/床 | DELETE | `/ward/{id}` |
| 8 | 停/启用病区/房/床 | PUT | `/ward/status` |
