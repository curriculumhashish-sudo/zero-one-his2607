# 门诊挂号页面

- 路由：`/outpatient/charge/register`
- 页面入口：`Index.vue`
- API 出口：`src/apis/outpatient/charge/register/index.ts`

本目录实现患者选择、无档案患者录入、科室医生联动、当日号源、预约转挂号、医保待遇预览、费用试算、确认缴费、扫码补结算、就诊记录分页、退号退款和挂号小票。成功后同步刷新当日统计、记录和号源余量。

患者选择使用 `SelectTable` 远程分页展示病历号、姓名、性别、年龄、电话和身份证号。表单包含原型的年龄字段；18 位居民身份证输入完成后自动识别出生日期、年龄与性别，年龄和病历号只属于页面模型，不加入 `PreRegisterSettlementParams`。

## 接口契约

`src/apis/outpatient/charge/register/type.ts` 按原型 `outpatient-registration.ts` 定义初始化、患者、医生、待遇、号源、预约、预结算、支付、退号和票据 DTO。`ybRegisterFee`、`ybUseAcctPay`、`LoadRegisterBenefitPreviewResult`、`PayRegisterFeeParams` 和扫码结果字段均与原型一致；金额和主键保留原型联合类型。

页面使用 `getRegisterAppointment(appointmentId)`、`validateRegisterCancellation(encounterId)` 和 `getRegisterTicket(encounterId)` 等任务书稳定接口。医保凭证过期或身份信息不足由接口错误提示；扫码结算异常时复用同一个 `paymentDraftId` 和 `clientScanResult` 补结算。
