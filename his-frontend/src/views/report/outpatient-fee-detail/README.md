# 门诊费用明细

地址：`/report/outpatient-fee-detail`。

页面按原型 `PageOutpatientFeeDetailParams` 查询门诊费用，就诊块由 `adapter.ts` 平铺为明细行和就诊小计；金额汇总使用接口返回的 `summary`，不在页面重新估算。分页请求统一提交 `pageIndex/pageSize`，后端八个全量合计字段以 `*Sum` 平铺返回，由 API 适配层组装为页面使用的 `summary.*Total`；导出提交去除分页字段后的同一组筛选条件，因此不会只导出当前页。

API 只从 `src/apis/report/outpatient-fee-detail/index.ts` 调用，函数固定为 `pageOutpatientFeeDetails`、`exportOutpatientFeeDetails`；模拟实现位于同目录 `mock.ts`。

Mock 就诊日期和结算时间相对当前日期生成，保证默认当天查询始终有可验证数据；请求和响应仍只使用原型字段，没有为演示增加额外 DTO 字段。日期范围限制为 31 天，查询失败可重试，导出和刷新期间均禁止重复操作。
