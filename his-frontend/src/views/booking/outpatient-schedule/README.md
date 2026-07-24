# B组：门诊排班管理

## 页面信息

- 菜单位置：排班预约 -> 门诊排班
- 页面地址：`/booking/outpatient-schedule`
- 页面入口：`Index.vue`
- 对应路由：`src/router/main/booking/index.ts`
- 对应接口：`src/apis/booking/outpatient-schedule/`

## 页面用途

用于按日期范围管理门诊医生排班。支持按科室、医生、诊室和关键词查询，在周视图与月视图之间切换，并完成排班新增、编辑、发布和停诊。

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
```

当前 `index.ts` 使用同目录 `mock.ts` 提供模拟实现。接入真实后端时保持以上函数名称和页面调用方式不变，只替换接口层实现。

## 关键行为

- `CalendarSchedule` 的 `range-change` 触发当前可见日期范围查询。
- 点击空日期的日期数字或单元格空白区域均可新增排班。
- 点击已有排班打开详情面板。
- 发布和停诊必须二次确认，停诊必须填写原因。
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

## 验证

```powershell
pnpm type-check
pnpm exec vitest src/views/booking/booking-pages.spec.ts src/views/booking/booking-pages.interaction.spec.ts --run
pnpm build
```

