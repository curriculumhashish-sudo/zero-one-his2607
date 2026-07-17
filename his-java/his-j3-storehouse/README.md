# 库房药房管理模块 (his-j3-storehouse)

**模块名称：** 库房药房管理
**负责人员：** mie、isme
**主启动类：** `com.zeroone.star.j3.storehouse.StorehouseApplication`
**端口变量：** `sp.storehouse`
**服务名变量：** `sn.storehouse`
**上下文路径：** `/storehouse`

## 业务说明

库房和药房都属于药品/物资流转基础数据，但用途不同。需要分析类型筛选，以及后续采购入库、库存管理、发药业务的关联。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取库房药房名称（支持类型筛选） | GET | `/storehouse/options` |
| 2 | 获取库房药房列表（条件+分页） | GET | `/storehouse/page` |
| 3 | 添加库房药房 | POST | `/storehouse` |
| 4 | 修改库房药房 | PUT | `/storehouse/{id}` |
| 5 | 删除库房药房 | DELETE | `/storehouse/{id}` |
| 6 | 启/停用库房药房 | PUT | `/storehouse/status` |
