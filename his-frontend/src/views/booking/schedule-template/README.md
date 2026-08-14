# 排班设置

地址：`/booking/schedule-template`。本页维护放号时段、日排班、周排班、放号策略和节假日例外，不是门诊排班日历。

页面结构参考 `openhis-pro-ui/apps/web-ele/src/views/booking/schedule-template`：

- 放号时段按多个起止区间维护，每个区间带号源容量。
- 日排班组合排班模式、服务类型、出诊人员、地点、放号时段和放号策略，并记录生效日期与优先级。
- 周排班是一个模板，模板内可配置多条“星期 + 日排班”明细，同时维护科室默认、开放预约和启用状态。
- 放号策略支持条件分页、新增、编辑和删除，维护提前开放天数、时段粒度、单段容量和渠道配额；已被日排班引用的策略不能删除。
- 放号策略保存按原型显式提交 `policyCode/policyName/advanceDays/slotMinutes/capacityPerSlot/channelQuotaJson`；渠道配额必须是“渠道编码: 数字”的 JSON 对象，编辑时由返回的 `channelQuotas` 正确序列化回显。
- 节假日例外支持单日或日期范围、停诊/替换/减量/加诊、替代日排班和“参与生成”开关。
- 节假日例外的说明按原型为可选字段，结束日为空表示单日；结束日不能早于起始日，起始日后移时页面会清除已经失效的结束日。
- 放号时段、日排班、周排班、放号策略和节假日例外均支持分页加载；节假日例外还支持按机构、例外类型、日期范围和关键词查询。各页签的查询、翻页、保存和刷新互相独立。

参考工程只用于字段关系和操作引导；本页面仍使用当前项目的 `ActionToolbar`、`DataTablePanel`、`SearchPanel`、`StatusDisplay` 和本地 API 模拟层。

页面只调用 `src/apis/booking/schedule-template/index.ts`。模拟状态、引用限制和增删改逻辑位于同目录 `mock.ts`；接入后端时保持接口函数名不变。

关键限制：当前科室未加载时禁止保存；时段至少包含一个合法起止区间；被日排班引用的时段、被周排班引用的日模板不能直接删除；需要替代安排的节假日例外必须指定日排班模板。

## 原型字段对照

| 页签 | 原型/后端字段 | 当前页面兼容字段 |
| --- | --- | --- |
| 放号时段 | `organizationId`、`templateCode`、`templateName`、`slotTemplates[].startTime/endTime/capacity`、`enabled` | `name`、`segments[].start/end/capacity`、`status` |
| 日排班 | `templateName`、`scheduleMode`、`healthcareServiceId`、`practitionerId`、`locationId`、`slotProvisionTemplateId`、`defaultReleasePolicyId`、`effectiveStart/effectiveEnd`、`priority`、`enabled` | `serviceType`、`doctorId`、`roomId`、`periodIds`、`releasePolicyId` |
| 周排班 | `templateName`、`fallbackBookable`、`isDefault`、`enabled`、`items[].weekday/dayScheduleTemplateId/sortNo/enabled` | `appointmentOpen`、`status`、`dailyTemplateId` |
| 放号策略查询 | `organizationId`、`keyword`、`pageNo`、`pageSize` | 条件分页，刷新后清理失效选择 |
| 放号策略保存 | `policyCode`、`policyName`、`advanceDays`、`slotMinutes`、`capacityPerSlot`、`channelQuotaJson` | `name`、`openDays` 仅用于现有 UI 显示兼容 |
| 节假日例外查询 | `organizationId`、`exceptionType`、`rangeStart`、`rangeEnd`、`keyword`、`pageNo`、`pageSize` | 支持条件分页 |
| 节假日例外保存 | `exceptionDate`、`rangeEnd`、`exceptionType`、`overrideScheduleTemplateId`、`reason`、`autoApplyOnGenerate` | `date`、`type`、`replacementDailyTemplateId` 仅用于 UI 兼容 |

模拟接口会同步生成两套字段，原型字段用于后端交接，兼容字段维持现有五页签 UI。接后端时保持 `index.ts` 的函数名不变。

放号策略由 `pageReleasePolicies`、`saveReleasePolicy`、`removeReleasePolicy` 完成查询、新增/编辑和删除；节假日例外由 `pageHolidayExceptions`、`saveHolidayException`、`removeHolidayException` 完成条件分页和维护。
