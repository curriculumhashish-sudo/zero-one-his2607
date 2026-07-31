# 门诊退费

地址：`/outpatient/charge/refund`。页面按原支付记录分组展示可退项目，前端与 API 双重限制跨支付记录选择；支持退前校验、半退保留项目费用属性切换、原渠道退款、扫码结果查询和恢复。

页面刷新或重新选择患者后仍会恢复处理中退款标识，处理中禁止同原支付记录重复退款；接口强制退回原支付渠道，并保留退款失败原因供页面提示。

页面只从 `src/apis/outpatient/charge/refund/index.ts` 调用接口，模拟状态位于同目录 `mock.ts`。

## 模块与字段

| 模块 | 原型/后端字段 |
| --- | --- |
| 队列查询 | `queueTabCode`、`keyword`、`startDate`、`endDate`、`pageNo`、`pageSize` |
| 患者队列 | `encounterId`、`patientId`、`patientName`、`encounterBusNo`、`genderCode/genderEnumText`、`ageText`、`organizationName`、`doctorName`、`startTime`、`totalAmount`、`paidAmount`、`queueLabelCode` |
| 原支付分组 | `paymentId`、`paymentBusNo`、`halfRefundOnPayment`、`originalPayChannelCodes`、`paymentStatusEnum`、`refundableAmount`、`refundingLines`、`preservedLines` |
| 退费行 | `chargeItemId`、`chargeItemStatusEnum`、`itemDisplayName`、`lineTotalAmount` |
| 校验与提交 | `refundingChargeItemIds`、`preservedNewChargeItemIds`、`originalPaymentId`、`refundPaymentDetails[].amount/payChannelCode`、`clientRefundResult`、`rejectReasonCode` |

页面已增加就诊日期范围和扫码退款授权码输入。一次退费仍严格限制在同一个 `originalPaymentId`，半退时保留项目会重新计算医保/自费属性。

## 已知契约缺口

思维导图包含“打印门诊退费小票”，但第二轮任务书没有定义对应 API，参考原型也没有退费小票请求或返回 DTO。原型退款响应仅包含 `success`、`halfRefundExecuted`、`insuranceRevoked`、`originalPaymentId` 和 `reversePaymentId`。为避免自行编造后端字段，当前页面没有实现退费小票打印；需产品和后端补充票据查询/打印契约后再接入。
