### 结构：

历史进货单检索 + 单据队列 + 新增

#### 1. 历史进货单检索

复用现有组件 `SearchPanel` （路径：`src\components\common\SearchPanel.vue`）

布局：

- 关键词查询框（独占一行，无左侧字段，框内靠左为放大镜图标，placeholder="单据号检索"）；
- 日期检索器（独占一行）；
- 下拉菜单1（placeholder由父组件传入）和重置按钮（二者共占一行）；
- 下拉菜单2（由父组件控制是否显示，独占一行，placeholder由父组件传入）



#### 2. 单据队列

分为空状态和卡片队列（默认为空状态）

##### 1.空状态：

复用现有组件 `EmptyState` (路径：`src\components\common\EmptyState.vue`) + 新增单据按钮（二者上下分布）



##### 2.卡片队列：

复用现有组件 `DocumentQueueList` (路径：`src\components\warehouse\DocumentQueueList.vue`)，并采用模拟数据进行渲染，要求数据采用调用“模拟接口”的方式，而不是直接引用 mock 数据，例如：

```ts
import { pageUsers } from '@/apis/system/user'

async function loadUsers() {
  loading.value = true
  try {
    const result = await pageUsers(query.value)
    rows.value = result.rows
    total.value = result.total
  } finally {
    loading.value = false
  }
}
```

模拟数据实现在接口目录下进行：

```text
src/apis/booking/outpatient-schedule/
├─ index.ts       页面调用的接口函数
├─ type.ts        请求参数和返回数据类型
└─ mock.ts        后端未完成时的模拟实现
```

模拟数据放在接口目录下的 `mock.ts`，页面始终从 `index.ts` 导入函数，但页面不能直接从 `mock.ts` 导入

推荐的接口函数名：

```text
loadPurchaseInboundInit() 入库加载初始化
pagePurchaseInboundOrders(query) 入库页面订单
getPurchaseInboundDetail(id) 获取采购入库详情
createPurchaseInbound(data) 创建采购入库单
updatePurchaseInbound(id, data) 更新入库采购
submitPurchaseInbound(id) 提交采购入库
```



#### 3. 新增

新增按钮（独占一行，按钮名由父组件传入，与单据队列空状态时的新增单据按钮逻辑一致）
