# zero-one-his 项目理解与接口设计备忘

## 项目概览

HIS（医院信息系统），面向民营及公立一二级医院。372 项标准化功能，覆盖门诊/住院全流程、药房药库、财务医保、报表等。系统架构基于 Spring Cloud Alibaba 微服务 + Vue3 前端 + C++ 高性能服务。

## 技术栈

| 层 | 组件 |
|---|---|
| 微服务基座 | Spring Boot 3.5.0 / Spring Cloud 2025.0.0 / Spring Cloud Alibaba 2025.0.0.0 |
| ORM | MyBatis 3.5.19 + MyBatis Plus 3.5.16 |
| 数据库 | MySQL 8.4.9 |
| 服务治理 | Nacos 3.1.1 / Sentinel 1.8.9 / Seata 2.5.0 |
| 认证 | Spring Authorization Server 1.5.6 + Nimbus JOSE JWT |
| 工具库 | Hutool 5.8.16 / Lombok 1.18.38 / MapStruct 1.6.3 |
| API 文档 | Knife4j 4.5.0 |
| 前端 | Vue 3 + Element Plus + Pinia |

## 数据库核心约定

**每张表都有这些字段（通用基类）：**
- `id` VARCHAR(64) — 分布式 ID（雪花算法或类似方案）
- `tenant_id` VARCHAR(64) — 多租户标识
- `created_at` / `updated_at` DATETIME(6)
- `created_by` / `updated_by` VARCHAR(64)
- `is_deleted` TINYINT(1) DEFAULT 0 — **软删标记，所有删除都是逻辑删除**
- `deleted_at` / `deleted_by` — 删除审计
- `version` INT DEFAULT 0 — **乐观锁，所有更新必须带 version 校验**

**关键约定：**
1. **没有物理删除**，项目全域使用软删除（`is_deleted`），MyBatis Plus 配置 `logicDeleteField`
2. **所有写操作走乐观锁**，updateById 时 version 自动 +1，冲突则重试或抛异常
3. ID 策略为 `ASSIGN_ID`（雪花算法），非自增
4. 枚举值以 `_enum` 后缀列存储，值是枚举的 `getCode()` 字符串/整数，不是 ordinal

## 领域模块与核心表

### 管理域 (adm_*)
患者/机构/科室/就诊/收费项目定义等基础主数据。

### 药品域 (med_*)
- `med_medication_definition` — 药品目录定义（名称、规格、单位、医保编码、价格、厂商）
- `med_medication_request` — **药品医嘱/处方**，医生开立后签发进入发药队列
- `med_medication_dispense` — **发药记录**，配药+发药的实际执行记录

### 工作流域 (wkf_*)
- `wkf_supply_request` — **供应单据**（订货单、入库单、出库单、盘点单、请领单），同一 `bus_no` 聚合多行明细
- `wkf_supply_delivery` — 供应发放记录（关联 supply_request）
- `wkf_inventory_item` — **库存项**，按 库房×批号×药品 粒度管理
- `wkf_device_request` — 耗材请求（耗材类医嘱）
- `wkf_device_dispense` — 耗材发放记录
- `wkf_service_request` — 非药品诊疗服务申请
- `inventory_lock` — **库存锁定记录**，发药/出库时先锁库存再扣减

### 财务域 (fin_*)
- `fin_payment_reconciliation` — 支付流水
- `fin_payment_rec_detail` / `fin_payment_rec_static` — 支付明细与分类汇总

### 框架层 (whale_*)
- `whale_users` / `whale_roles` / `whale_menus` / `whale_permissions` — RBAC 权限
- `whale_tenants` — 多租户
- `whale_qrtz_*` — Quartz 定时任务

## 代码架构推断（基于 SQL 注释中的引用路径）

SQL 注释中频繁出现 Java 包路径，还原出以下分层：

```
com.openhis.health
├── domain
│   ├── administration    (adm) — 管理域
│   │   ├── 发票.service.InvoiceDomainService
│   │   └── ...
│   ├── medication        (med) — 药品域
│   ├── workflow          (wkf) — 工作流域
│   ├── finance           (fin) — 财务域
│   └── share
│       └── enums         — 跨域共享枚举
│           ├── AccountBillingStatus
│           ├── PublicationStatus
│           ├── SupplyType
│           ├── SupplyStatus
│           ├── SupplyCategory
│           └── DispenseStatus
└── ...
```

项目大概率采用 **DDD 轻量分层**：controller → application service → domain service → repository(mapper)。

## 单据删除接口设计要点

### 涉及表
- **主表**: `wkf_supply_request` — 供应单据（订货/入库/出库/盘点/请领）
- **关联表**: `wkf_supply_delivery` — 发放记录（如果单据已产生发放）
- **关联表**: `inventory_lock` — 库存锁定（通过 `bus_no` 关联）

### 业务规则
1. **状态约束**：只有 `status_enum = 'PENDING_APPROVAL'`（待审核/草稿）状态的单据可删除。已审批（APPROVED）、已驳回（REJECTED）、已撤回（WITHDRAWN）等状态不可删。
2. **级联范围**：
   - 同一 `bus_no` 下所有 `wkf_supply_request` 明细行一起删（一个单据号是多行聚合）
   - 关联的 `wkf_supply_delivery` 需要检查，已有发放记录的单据不能删，或者一并标记删除
   - 关联的 `inventory_lock` 需要解锁或标记删除
3. **删除策略**：软删——设置 `is_deleted=1`, `deleted_at=now()`, `deleted_by=当前用户ID`
4. **版本控制**：删除操作也需要乐观锁校验，version 不匹配则拒绝
5. **权限**：需要校验当前用户是否有删除该类型单据的权限（角色+数据权限）

### 接口签名（推测）
```
DELETE /inventory/{module}/supply-request/{busNo}
# 或
POST /inventory/{module}/supply-request/{busNo}/delete
```
其中 `{module}` 可能是 pharmacy（药房）/ warehouse（药库）/ department（科室）

### 实现注意事项
- 同一个 `bus_no` 下多行记录用 `LambdaUpdateWrapper` 批量软删，不用逐行删
- 删除前需要 `SELECT ... FOR UPDATE` 或者先查 version 再更新，防止并发
- 需要检查是否有未决的 `inventory_lock`（lock_flag=1），如果有则先解锁
- 打印日志记录删除原因/来源（如果前端传入 reason）

---

## 批量发药接口设计要点

### 涉及表
- **读取**: `med_medication_request` — 查询已签发待发药的处方
- **写入**: `med_medication_dispense` — 创建/更新发药记录（配药+发药）
- **写入**: `wkf_inventory_item` — 扣减库存（quantity 减少）
- **写入**: `inventory_lock` — 发药前锁定库存，发药后释放
- **读取/关联**: `adm_charge_item` — 费用记录确认（已收费才能发药）

### 业务规则
1. **前置条件**：
   - 处方状态必须是已签发（`status_enum = 'ACTIVE'` 或类似已签发状态）
   - 费用必须已结算（对应 `adm_charge_item` 的 claim_state 确认）
   - 未被其他发药操作锁定
2. **必填项**：
   - `preparer_id`（调配药师）— 必须选择，否则拒绝发药
   - `location_id`（发放库房）— 必须指定从哪个药房出库
   - 如果多批号库存，需指定 `lot_number`
3. **库存扣减逻辑**：
   - 先锁定库存（`inventory_lock` 写入 lock_flag=1）
   - 计算扣减量 = 处方 quantity × 拆零单位换算比
   - 按批号FIFO/指定批号扣 `wkf_inventory_item.quantity`
   - 库存不足时事务回滚，返回明确的库存不足提示（哪个药、差多少）
4. **发药记录创建**：
   - `med_medication_dispense.status_enum` = `'DISPENSED'`
   - `dispense_time` = 当前时间
   - `preparer_id` = 调配药师 / `practitioner_id` = 发药药师
   - 中药和西药可能分属不同的 dispense record，但同一次 batch 操作
5. **追溯码**：如果药品要求追溯码（`trace_no`），发药时必填

### 接口签名（推测）
```
POST /pharmacy/dispense/batch
Body: {
  encounterIds: [...],          // 就诊ID列表（可以一次发多个患者的药）
  medicationDispenseIds: [...], // 待发药的发药记录ID
  preparerId: "...",           // 调配药师
  locationId: "...",           // 发放库房
  lotSelections: [{            // 批号选择
    dispenseId: "...",
    lotNumber: "...",
    inventoryId: "..."
  }],
  traceCodes: [...]            // 追溯码
}
```

### 实现注意事项
- **必须在一个事务里完成**：锁定库存 → 创建发药记录 → 扣库存 → 释放锁。任何一步失败全部回滚。
- **并发控制**：多个药师同时对同一库存发药时通过 `inventory_lock` 行锁或乐观锁防止超卖。发药前 SELECT inventory FOR UPDATE 是必要的。
- **库存不足处理**：不是直接回滚整个批次。好的做法是：能发多少发多少，发不了的行单独标记失败原因。但如果业务要求必须全发，则全部回滚。
- **中药特殊处理**：中药可能按剂付数（dose × 付数 = 总量），需注意 `chinese_herbs_dose_quantity` 字段。
- **住院 vs 门诊差异**：
  - 门诊发药：可以选库房批号
  - 住院发药：库房批号由后端决定（汇总领药场景）
- **发药后触发的副作用**：
  - 更新 `adm_charge_item.dispense_id` 关联发药记录
  - 如果有打印需求，触发处方打印队列
  - 通知相关护士站/医生站状态更新

### 流程伪代码
```java
@Transactional(rollbackFor = Exception.class)
public BatchDispenseResult batchDispense(BatchDispenseCommand cmd) {
    // 1. 校验调配药师必填
    // 2. 加载待发药的 medication_request 列表，校验状态
    // 3. 加载对应的 medication_dispense（可能已有配药草稿）
    // 4. 逐行锁定库存（inventory_lock + SELECT FOR UPDATE on wkf_inventory_item）
    // 5. 校验库存充足（quantity >= 需求拆零量）
    // 6. 创建/更新 medication_dispense（status → DISPENSED, dispense_time = now）
    // 7. 扣减 wkf_inventory_item.quantity
    // 8. 释放/标记 inventory_lock
    // 9. 回写 adm_charge_item.dispense_id
    // 10. 返回结果（成功行 + 失败行及原因）
}
```

---

## 开发约定

### 分层结构（按 DDD 轻量）
```
his-java/
└── src/main/java/com/openhis/health/
    ├── domain/
    │   ├── medication/         # 药品域
    │   │   ├── entity/         # 实体（对应 DB 表）
    │   │   ├── repository/     # Mapper 接口
    │   │   ├── service/        # 域服务（核心业务逻辑）
    │   │   └── dto/            # 域内 DTO
    │   ├── workflow/           # 工作流/库房域
    │   └── share/enums/        # 跨域枚举
    ├── application/            # 应用服务层（编排、事务边界）
    ├── interfaces/             # REST Controller
    └── infrastructure/         # 基础设施（配置、拦截器、工具）
```

### 实体基类
所有实体继承通用 BaseEntity，包含 id/tenantId/createdAt/updatedAt/createdBy/updatedBy/isDeleted/deletedAt/deletedBy/version 字段。
MyBatis Plus 配置：
- `logicDeleteField: is_deleted`
- `versionField: version`
- `idType: ASSIGN_ID`

### 枚举规范
枚举值存储在 DB 中是 `getCode()` 的返回值（字符串），不是 `ordinal()` 或 `name()`。
所有 `_enum` 后缀的列对应一个枚举类，如 `status_enum` → `SupplyStatus`。

### 接口规范
- RESTful 风格，URL 按模块/子模块/动作
- 统一返回格式：`Result<T>`（code/message/data）
- 参数校验用 `@Valid` + Bean Validation
- API 文档用 Knife4j 自动生成
