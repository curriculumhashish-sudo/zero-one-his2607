# 门诊收费

地址：`/outpatient/charge/charge`。完成患者队列、收费项目选择、医保/自费重新计算、预结算、支付渠道、扫码结果确认/恢复和收费票据补打流程。

扫码支付记录会绑定本次勾选的收费项目，结果确认时不会误结算其他项目；存在处理中支付时禁止重复预结算。收费成功后返回自动/计划打印状态，电子票据状态不一致时页面显示明确警告。补打票据使用公共 `PrintActionPanel` 展示票据信息与收费项目，并调用浏览器 `window.print()`；只读取成功且已开票的收费记录。

页面只从 `src/apis/outpatient/charge/charge/index.ts` 调用接口；模拟支付与收费状态保存在同目录 `mock.ts`。

患者队列复用公共 `PaginationBar`，提供总记录数、每页条数、页码和跳页。页码变化后重新请求队列，修改每页条数时回到第 1 页；分页刷新会清空旧患者及其收费明细，避免跨页误操作。

## 模块与字段

| 模块 | 原型/后端字段 |
| --- | --- |
| 队列查询 | `organizationId`、`queueTabCode`、`keyword`、`startDate`、`endDate`、`pageNo`、`pageSize` |
| 患者队列 | `encounterId`、`patientId`、`patientName`、`encounterBusNo`、`genderCode/genderEnumText`、`ageText`、`organizationName`、`doctorName`、`startTime`、`totalAmount`、`paidAmount`、`paymentId`、`queueLabelCode` |
| 收费项目 | `chargeItemId`、`busNo`、`cashierName`、`chargeCategoryCode/chargeCategoryDisplay`、`itemName`、`medicalInsuranceCode`、`feeNatureDisplay`、`quantityDisplay/quantityValue/quantityUnit`、`unitPrice`、`totalPrice`、`statusCode/statusText` |
| 预结算 | `chargeItemIds`、`paymentDraftId`、`displayAmount`、`previews`、`insuranceGroups`、`selfPayRollup`、`ybFundPaySumamt`、`ybAcctPay`、`ybPsnCashPay` |
| 支付提交 | `billDate`、`chargeItemIds`、`encounterId`、`outcomeEnum`、`paymentDraftId`、`paymentDetails[].amount/payChannelCode`、`clientScanResult`、`ybChrgBchno/ybChrgBchnos` |
| 支付返回 | `invoiceId`、`invoiceStatus`、`paymentId`、`successMessageCode`、`ybSetlId` |
| 票据补打请求 | `encounterId`、`paymentId` |

页面已增加就诊日期范围和扫码付款码输入；付款码只用于在页面/API 层形成原型 `clientScanResult`，正式请求不发送自定义 `scanCode`、`channel`、`itemIds` 或 `settlementId`。adapter 优先读取 `patientName`、`encounterBusNo`、`genderEnumText`、`startTime` 等原型字段，旧字段只用于当前公共患者组件的显示兼容。
