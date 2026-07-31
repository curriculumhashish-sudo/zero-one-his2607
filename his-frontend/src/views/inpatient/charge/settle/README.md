# 住院结算页面

- 路由：`/inpatient/charge/settle`
- 页面入口：`Index.vue`
- API 出口：`src/apis/inpatient/charge/settle/index.ts`

本目录实现待结算/已结算患者列表、费用合计、费用大类、费用明细、历史结算、预结算、正式结算和反结。正式结算支持现金、银行卡和扫码支付；扫码扣款成功但本地结算异常时保留同一个支付草稿和扫码结果，通过“补结算”恢复，不重新发起扣款。

左侧队列用于快速浏览，顶部“选择住院患者”使用 `SelectTable` 按姓名或住院号远程分页查询，并展示性别、年龄、科室、病区、床号、就诊状态和账户状态。切换队列、筛选条件或患者时会清空旧费用、勾选、试算和历史记录。

## 接口契约

`src/apis/inpatient/charge/settle/type.ts` 与原型 `inpatient-settle.types.ts` 对齐：主键使用字符串，金额兼容 `number | string`，扫码结果严格使用 `merTradeNo/tradeNo/payType/respCode/respMsg/txnAmtFen`。页面使用任务书规定的稳定函数参数，`index.ts` 内部组装原型请求对象。

正式结算前必须先取得 `PreSettleInpatientResult`。医保患者的支付金额使用原型 `displayAmount`，原价、医保基金、个人账户和个人现金分别读取原型返回字段，不在页面自行估算。
