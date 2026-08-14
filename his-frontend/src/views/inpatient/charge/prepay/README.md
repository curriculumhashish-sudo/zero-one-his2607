# 住院预交金管理

地址：`/inpatient/charge/prepay`。页面提供在院患者选择、账户余额与累计金额、快捷收款、原记录退款、扫码支付结果确认和中断恢复。切换患者会清空上一患者的金额、弹窗和处理中状态。

扫码收款和退款会自动轮询结果，也可手动查询、恢复或取消；处理中禁止重复交易。扫码退款只有在支付结果确认成功后才扣减原收款记录和账户余额，取消交易不会改变余额。

页面只从 `src/apis/inpatient/charge/prepay/index.ts` 调用接口，模拟状态位于同目录 `mock.ts`。

在院患者队列复用公共 `PaginationBar`，提供总记录数、每页条数、页码和跳页。页码变化后重新请求队列，修改每页条数时回到第 1 页；分页刷新会停止旧交易轮询并清空旧患者、余额和流水选择。

## 模块与字段

| 模块 | 原型/后端字段 |
| --- | --- |
| 队列查询 | `admissionDate`、`keyword`、`pageIndex`、`pageSize` |
| 在院患者 | `encounterId`、`patientId`、`patientName`、`encounterBusNo`、`genderEnum/genderEnumText`、`ageText`、`organizationName`、`wardName`、`bedName`、`startTime`、`statusEnum/statusEnumText`、`contractDisplay` |
| 余额 | `accountBalance`、`availableBalance`、`prepayNetBalance`、`pendingSettlementTotal`、`lowBalanceWarning` |
| 流水 | `paymentId`、`tradeTime`、`bizTypeText`、`amount`、`payMethodText`、`paymentEnum`、`operatorName`、`ticketNo`、`statusEnum/statusEnumText` |
| 收款 | `encounterId`、`patientId`、`amount`、`paymentDetails[].accountId/amount/payChannelCode`、`paymentCode`、`paymentDraftId`、`clientScanResult`、`operatorId`、`remark` |
| 退款 | `encounterId`、`patientId`、`originalPaymentId`、`refundAmount`、`paymentDetails`、`clientRefundResult`、`operatorId`、`remark` |

页面使用患者队列自带的日期范围筛选入院日期；单日查询同时映射原型字段 `admissionDate`。页面的扫码码值只转换为原型 `clientScanResult/clientRefundResult`，正式请求不发送自定义 `scanCode`、`channel` 或 `recordId`。通用原型接口使用 `patientId`；C1 适配层按其接口文档改用 `inpatientNo`，不要求 C1 患者队列返回 `patientId`。响应只依赖原型 `accountBalance/paymentId`。`operatorId` 已在接口类型中预留，接入登录用户上下文后由接口层补充，页面不伪造操作员。
