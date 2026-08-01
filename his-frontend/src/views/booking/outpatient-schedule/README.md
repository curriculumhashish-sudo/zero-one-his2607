# B组：门诊排班管理

## 页面信息

- 菜单位置：排班预约 -> 门诊排班
- 页面地址：`/booking/outpatient-schedule`
- 页面入口：`Index.vue`
- 对应路由：`src/router/main/booking/index.ts`
- 对应接口：`src/apis/booking/outpatient-schedule/`

## 页面用途

用于按日期范围管理门诊医生排班。支持按科室、医生、诊室和关键词查询，在周视图与月视图之间切换，并覆盖去重后的 14 项功能：日程列表与详情、新建、生成正式排班、设置/撤销替诊、部分停诊、解锁停诊、整日停诊、恢复开诊、作废日程、已放号源列表、批量放号和删除号源。

## 目录职责

```text
outpatient-schedule/
├─ Index.vue                         页面状态、查询和业务流程
├─ type.ts                           页面内部类型
├─ adapter.ts                        接口排班转换为日历事件和医生资源
├─ constants.ts                      排班状态展示配置
├─ README.md                         本说明
└─ components/
   ├─ ScheduleEditDialog.vue         新增、编辑排班表单
   ├─ ScheduleGenerateDialog.vue     选择日/周模板并生成正式排班
   └─ ScheduleDetailPanel.vue        排班详情及编辑、发布、停诊入口
```

## 复用组件

- `SearchPanel`：组织查询条件和查询、重置操作。
- `RemoteSelectField`：远程加载科室和医生选项。
- `CalendarSchedule`：展示周/月排班日历。
- `ActionToolbar`：提供新增排班和刷新操作。
- `StatusDisplay`：显示草稿、已发布和停诊状态。
- `DescriptionList`：显示排班详情。
- `MyDialog`：作为排班编辑弹窗外壳。

页面不得在上述公共组件中写入排班业务接口或业务字段。

## 接口

页面只从 `src/apis/booking/outpatient-schedule/index.ts` 导入：

```text
loadScheduleInit()
pageSchedulesByRange(query)
getScheduleDetail(id)
createSchedule(data)
updateSchedule(id, data)
publishSchedule(id)
stopSchedule(id, reason)
loadDepartmentOptions(keyword)
loadDoctorOptions(keyword, departmentId)
loadScheduleGenerationOptions(organizationId)
generateFormalSchedules(data)
setScheduleSubstitution(id, data)
clearScheduleSubstitution(id)
previewScheduleImpact(id, data)
partialStopSchedule(id, data)
unlockPartialStop(id, data)
resumeSchedule(id)
voidSchedule(id, reason)
listReleasedSlots({ scheduleId, PageIndex, PageSize })
provisionScheduleSlots(id, data)
deleteReleasedSlots(id, data)
```

当前 `index.ts` 使用同目录 `mock.ts` 提供模拟实现。接入真实后端时保持以上函数名称和页面调用方式不变，只替换接口层实现。

## 关键行为

- `CalendarSchedule` 的 `range-change` 触发当前可见日期范围查询。
- 点击空日期的日期数字或单元格空白区域均可新增排班。
- 点击已有排班打开详情面板。
- 发布和停诊必须二次确认，停诊必须填写原因。
- 替诊支持设置替诊医生与撤销替诊；部分停诊支持预约转移、取消预约和人工后续处理三种处置方式。
- 预约转移使用 `dispositionMode: 'TRANSFER'`，并按预约提交 `transferItems[].appointmentId/targetSlotId`；取消预约使用 `CANCEL`，人工处理使用 `MANUAL` 和 `deferManualHandling`。
- 已发布排班支持部分停诊、解锁停诊、整日停诊、恢复开诊和作废；已放号源支持查询、批量放号和删除。
- 保存、发布和停诊期间禁止重复操作。
- 快速切换范围或查询时会取消旧请求，并通过请求序号避免旧结果覆盖新结果。
- 初始化错误和排班列表错误分别显示，并提供对应重试操作。
- 刷新后如果当前详情已不在查询结果中，会清空旧选择并关闭详情。

## 数据约定

- 日期：本地日历字符串 `YYYY-MM-DD`。
- 时间：24 小时制 `HH:mm`。
- 排班状态：`draft`、`published`、`stopped`。
- 接口模型必须通过 `adapter.ts` 转换后再传给 `CalendarSchedule`。
- 页面和组件不能直接导入 `mock.ts`。

## 原型字段对照

| 模块 | 原型/后端字段 | 页面兼容字段或说明 |
| --- | --- | --- |
| 范围查询 | `organizationId`、`rangeStart`、`rangeEnd`、`practitionerId`、`locationId`、`healthcareServiceId` | 当前查询 UI 中科室、医生、诊室分别兼容 `departmentId`、`doctorId`、`roomId` |
| 排班标识 | `scheduleId`、`name`、`statusEnum` | 页面兼容 `id`、`status` |
| 时间窗口 | `planningHorizonStart`、`planningHorizonEnd` | adapter 转换为 `date`、`startTime`、`endTime` |
| 人员地点 | `actors`、`practitionerId`、`locationId`、`healthcareServiceId` | 详情显示医生、诊室、科室/服务名称 |
| 放号 | `slotProvisionTemplateId`、`bookable` | `capacity` 通过原型放号接口的 `slotTemplates[].capacity` 提交；发布通过独立发布接口完成 |
| 替诊 | `originalPractitionerId`、`substitutePractitionerId`、`substituteDisplay` | 已实现设置替诊与撤销替诊 |
| 部分停诊 | `scheduleId`、`startTime`、`endTime`、`reason`、`dispositionMode`、`transferItems[].appointmentId/targetSlotId`、`targetPractitionerId`、`deferManualHandling` | 已实现转移、取消、人工处理三种预约处置方式 |
| 整日停诊/恢复/作废 | `scheduleId`、`stopReason`、`reason` | 操作前二次确认，处理中禁止重复提交 |

`mock.ts` 会同时返回原型字段和当前页面兼容字段；`adapter.ts` 优先读取原型字段。接真实后端时，在 API 层完成兼容字段归一化，不修改公共日历组件。

`organizationId` 固定表示当前医疗机构；科室/服务类型使用 `healthcareServiceId` 或 `actors` 中 `actorType: 30` 表示，二者不得互相替代。正式生成排班只提交机构和模板标识，排班所属科室由被选模板决定。

“生成正式排班”遵循参考原型的模板生成流程：先从排班设置加载已启用的日排班和周排班模板，用户选择模板类型、具体模板和 `rangeStart/rangeEnd`，并设置 `autoPublish`、`autoProvision`。页面不要求手工输入模板 ID；`DAY` 请求只发送 `scheduleTemplateId`，`WEEK` 请求只发送 `weekScheduleTemplateId`。自动放号依赖自动发布，关闭自动发布时自动放号会被重置并禁用。生成后清空筛选、切换到生成起始日的周视图并刷新日历。

门诊排班不录入、不保存也不展示挂号费用。挂号页面根据初始化接口返回的 `serviceTypeFeeById` 显示所选号源类型的参考价格，实际应付费用以后端挂号预结算返回的 `chargeItems` 和 `totalAmount` 为准。

## 验证

```powershell
pnpm type-check
pnpm exec vitest src/views/booking/booking-pages.spec.ts src/views/booking/booking-pages.interaction.spec.ts --run
pnpm build
```
