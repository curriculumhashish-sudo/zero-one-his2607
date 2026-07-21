# 供应商管理模块 (his-j3-supplier)

**模块名称：** 供应商管理
**负责人员：** 业之、只在此山中
**主启动类：** `com.zeroone.star.j3.supplier.SupplierApplication`
**端口变量：** `sp.supplier`
**服务名变量：** `sn.supplier`
**上下文路径：** `/supplier`

## 业务说明

供应商是采购入库和结算业务的基础数据。重点关注供应商资质、统一社会信用代码、启停状态和采购业务引用。

## 接口清单

| 序号 | 接口功能 | 请求方式 | 建议路径 |
|------|----------|----------|----------|
| 1 | 获取供应商名称 | GET | `/supplier/options` |
| 2 | 获取供应商列表（条件+分页） | GET | `/supplier/page` |
| 3 | 获取供应商详情 | GET | `/supplier/{id}` |
| 4 | 新增供应商 | POST | `/supplier` |
| 5 | 修改供应商 | PUT | `/supplier/{id}` |
| 6 | 删除供应商 | DELETE | `/supplier/{id}` |
| 7 | 启/停用供应商（支持批量） | PUT | `/supplier/batch-status` |


http://localhost:10206/j3-basedata/doc.html#/home
