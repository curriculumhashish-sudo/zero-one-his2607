用途：单据工作站左侧待处理、历史或审核队列

1.接收任意单据数组和稳定 rowKey。
2.支持选中 key 受控、点击选择和键盘操作。
3.通过字段解析函数或配置展示单据号、日期、经办人、往来单位和状态。
4.字段不存在时不显示多余标签和分隔符。
5.支持搜索/状态过滤区域 slot、刷新、新增事件
6.支持 loading、空列表、失败、重试和长列表滚动。
7.状态显示可直接使用已有组件StatusDisplay(路径：src/components/common/StatusDisplay.vue)，状态映射由外部传入。

Props: orders、rowkey、selectedkey、fields、statusMap、 1oading、erroremptyText、showCreate
Emits: update:selectedkey、select、create、refresh、retry、action
Slots:filters、item、itemExtra、empty、footer
