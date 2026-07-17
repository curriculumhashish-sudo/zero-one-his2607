# 科室管理模块 (his-j3-department)

**模块名称：** 科室管理  
**负责人员：** 很帅大王、胡傲宇  
**主启动类：** `com.zeroone.star.j3.department.DepartmentApplication`  
**端口变量：** `sp.department`  
**服务名变量：** `sn.department`  
**上下文路径：** `/department`

## 业务说明

科室管理是门诊挂号、住院登记、入出转管理的前置基础数据。支持科室树形结构、门诊/住院类型筛选、启停状态控制。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取科室名称树（支持门诊/住院筛选） | GET | `/department/tree` |
| 2 | 获取科室列表（条件） | GET | `/department/page` |
| 3 | 获取科室详情 | GET | `/department/{id}` |
| 4 | 新增科室 | POST | `/department` |
| 5 | 修改科室 | PUT | `/department/{id}` |
| 6 | 删除科室 | DELETE | `/department/{id}` |
| 7 | 启用/停用科室（支持批量） | PUT | `/department/batch-status` |
