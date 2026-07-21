用途：入库、出库、调拨、盘点、损益和药房单据工作站骨架

1.提供左侧 queue 区和右侧 main 区，main 内支持 header、content、footer。
2.支持左栏宽度、折叠、隐藏和中间区域自适应。
3.左队列和右主区独立滚动。
4.无选中单据时展示 empty slot;新建单据时仍可显示右侧编辑区,5.支持页面传入 loading/readonly 状态，但不自行管理单据请求。6.所有内容通过 slots 提供，不能写死采购、入库或供应商字段。

Props:queuewidth、queuecollapsed、co1lapsible、hasselection、loadingreadonly、gap
Emits:update:queueco1lapsed、co1lapsechange
Slots: queueHeader、queue、header、default、footer、empty

整个工作站外壳
分为Queue 区（左侧单据列表）和Main 区（右侧主区域）

Queue 区
<slot name="queue-header" />
<slot name="queue" />

Main 区（右侧主区域）
<slot name="main-header" />
<slot name="main-content" />
<slot name="main-footer" />