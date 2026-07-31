# 住院登记

- 路由：`/inpatient/charge/register`
- 页面入口：`Index.vue`
- 接口出口：`src/apis/inpatient/charge/register/index.ts`
- 对应思维导图：收费工作站 → 住院登记（4）

页面覆盖住院登记列表、记录详情、住院登记、普通取消、医保退入院、医保待遇预览、合同匹配/推荐及登记时预交金。切换患者或关闭已修改表单时会确认是否放弃；提交和取消期间禁止重复操作。

登记弹窗按原型展示患者、证件号码、出生日期、年龄、性别、联系方式、住院号、住院次数、入院科室、入院病区、入科床位、门诊医生、主治医生、入院诊断、患者病情、入院类型、入院方式、费用性质和入院日期。年龄、住院次数和床位等仅用于展示，不会被擅自加入提交 DTO；输入 18 位居民身份证后自动回填出生日期、年龄和性别。

模拟接口会在登记、取消和医保撤销成功后更新内存状态。正式接入后端时只替换 API 层实现，页面函数名和返回语义保持不变。

## 接口契约

`src/apis/inpatient/charge/register/type.ts` 按原型 `inpatient-registration.ts` 保留请求和响应 DTO。页面使用任务书规定的稳定函数参数，例如 `getInpatientRegistrationContext(encounterId)`、`listRegistrationWards(departmentId)`、`getWardCapacity(wardId)` 和 `cancelInpatientRegistration(id, reason)`；`index.ts` 内部再转换为原型请求对象。

医保待遇复用原型 `outpatient-registration.ts` 的 `LoadRegisterBenefitPreviewResult`，提交字段使用原型的 `psnNo`、`insuplcAdmdvs`、`insutype`、`medType`、`ybCardInfo` 和 `ybMediumType`。`operatorId` 来自登录用户，不在页面或组件中固定写死。

患者通过 `SelectTable` 调用 `pageRegistrationPatientOptions` 远程分页选择；科室变化会立即清空旧病区和医生并重新加载病区，病区变化后再加载床位余量与主治医生。

任务书要求登记列表支持日期筛选，但原型 `PageRegistrationQueueParams` 暂无日期字段。当前将 `dateFrom/dateTo` 隔离为 Mock 演示筛选，不加入正式请求 DTO；接入真实后端前需由后端确认日期查询字段。
