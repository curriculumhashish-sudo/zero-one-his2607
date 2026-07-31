# B组：号源查看

## 页面信息

- 菜单位置：排班预约 -> 号源查看
- 页面地址：`/booking/slot-calendar`
- 页面入口：`Index.vue`
- 对应路由：`src/router/main/booking/index.ts`
- 对应接口：`src/apis/booking/slot-calendar/`

## 页面用途

用于按日期、科室、医生和关键词查看门诊号源。页面按上午、下午、晚间分组展示各时段的总号数、已用号数、剩余号数和状态，并支持批量封锁、批量解封和标记超售。第一版不办理患者预约登记。

## 目录职责

```text
slot-calendar/
├─ Index.vue                         页面查询、汇总和号源详情流程
├─ type.ts                           页面查询类型
├─ adapter.ts                        接口号源转换为公共号源网格模型
├─ README.md                         本说明
└─ components/
   └─ SlotDetailDialog.vue           只读号源详情弹窗
```

## 复用组件

- `SearchPanel`：组织日期、科室、医生和关键词查询。
- `RemoteSelectField`：远程加载科室和医生选项。
- `TimeSlotGrid`：按 `group` 展示上午、下午、晚间号源。
- `DescriptionList`：显示号源详细信息。
- `StatusDisplay`：显示可约、已满、停诊和过期状态。
- `MyDialog`：作为号源详情弹窗外壳。

页面不得自行重新实现号源网格，也不得把业务接口写入公共组件。

## 接口

页面只从 `src/apis/booking/slot-calendar/index.ts` 导入：

```text
loadSlotInit()
pageSlotsByDate(query)
getSlotDetail(id)
loadDepartmentOptions(keyword)
loadDoctorOptions(keyword, departmentId)
batchBlockSlots(data)
batchUnblockSlots(data)
markSlotOverbooked(data)
```

当前 `index.ts` 使用同目录 `mock.ts` 提供模拟实现。号源模拟数据会读取排班模拟状态，排班停诊后对应号源显示为停诊。

## 关键行为

- 查询和重置后按所选日期重新加载号源。
- 号源通过 `group-by="group"` 按上午、下午和晚间分组。
- 可约号源可以选中并打开详情。
- 已满、停诊和过期号源不可选择，点击时显示不可用原因。
- 详情按原型 `SlotDetailDto` 展示科室、医师、号源类型、状态、时段、容量、已约、剩余、超售、备注、号源 ID 和日程 ID。
- 重新查询前清空旧号源选择并关闭旧详情弹窗，避免详情与当前结果不一致。
- 列表和详情均使用请求取消与请求序号保护，快速查询或切换号源时旧结果不能覆盖新结果。
- 初始化错误和号源列表错误分别显示，并提供对应重试操作。
- 详情底部保留禁用的预约登记操作入口，但当前不办理患者预约登记。

## 数据约定

- 日期：本地日历字符串 `YYYY-MM-DD`。
- 时间：24 小时制 `HH:mm`。
- 分组：`上午`、`下午`、`晚间`。
- 号源状态：`available`、`full`、`stopped`、`expired`。
- 接口模型必须通过 `adapter.ts` 转换后再传给 `TimeSlotGrid`。
- 页面和组件不能直接导入 `mock.ts`。

## 原型字段对照

| 模块 | 原型/后端字段 | 页面兼容字段或说明 |
| --- | --- | --- |
| 查询 | `organizationId`、`date`、`practitionerId`、`scheduleId`、`statusCode`、`pageNo`、`pageSize` | 页面当前提供日期、科室、医生和关键词查询 |
| 标识与关联 | `slotId`、`scheduleId`、`practitionerId`、`healthcareServiceId` | 页面兼容 `id`、`doctorId`、`departmentId` |
| 时段 | `startTime`、`endTime` | `group` 由 adapter/mock 按上午、下午、晚间计算 |
| 数量 | `capacity`、`bookedCount`、`remainingCount` | 页面兼容 `used`、`remaining` |
| 状态 | `statusCode`、`displayStatus`、`overbookedFlag`、`commentText` | 页面状态为 `available/full/stopped/expired` |

`adapter.ts` 优先读取 `slotId`、`bookedCount`、`remainingCount` 和 `statusCode`。页面不办理患者预约登记；思维导图后续要求的批量封锁、批量解封和超售标记由独立状态维护区完成，并复用同一号源查询结果。`organizationId` 始终是机构标识，科室筛选在页面取得正式结果后按 `healthcareServiceId` 处理。

## 验证

```powershell
pnpm type-check
pnpm exec vitest src/views/booking/booking-pages.spec.ts src/views/booking/booking-pages.interaction.spec.ts --run
pnpm build
```
