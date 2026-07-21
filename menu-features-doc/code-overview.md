# his-j1-menu-manager 代码结构说明

> 菜单权限与当前用户菜单两个功能的实现文件详解

## 文件总览

本次实现共涉及 14 个文件（3 个修改 + 11 个新建），按分层架构组织。

| 层 | 文件 | 操作 | 核心职责 |
|---|---|---|---|
| 配置 | `ComponentInit.java` | 修改 | Spring 包扫描 |
| 配置 | `RestTemplateConfig.java` | 新建 | 跨服务调用的 HTTP 客户端 |
| 配置 | `application.yml` | 修改 | MyBatis XML 路径 |
| Controller | `MenuController.java` | 修改 | HTTP 入口与响应封装 |
| Entity | `WhaleMenu.java` | 新建 | whale_menus 表实体 |
| Entity | `WhalePermissionGrant.java` | 新建 | whale_permission_grants 表实体 |
| Entity | `WhaleRole.java` | 新建 | whale_roles 表实体 |
| Mapper | `MenuMapper.java` | 新建 | 菜单查询接口 |
| Mapper | `PermissionGrantMapper.java` | 新建 | 权限授权增删改接口 |
| Mapper | `RoleMapper.java` | 新建 | 角色查询接口 |
| Mapper XML | `MenuMapper.xml` | 新建 | 五表 JOIN 的 SQL |
| Mapper XML | `PermissionGrantMapper.xml` | 新建 | 删/查/插的 SQL |
| Service | `IMenuService.java` | 新建 | 业务接口定义 |
| Service | `MenuServiceImpl.java` | 新建 | 核心业务逻辑实现 |

---

## 配置层（3 个文件）

### ComponentInit.java — Spring 包扫描修复

原始只扫描了 swagger 包，但 `getTreeByLoginUser` 需要用 `UserHolder`（在 `components.user` 包），JWT 也在 `components.jwt` 包。修复后加上这两个包扫描，否则 `UserHolder` 无法注入，`getCurrentUser()` 会抛 NPE。

### RestTemplateConfig.java — 跨服务调用客户端

创建一个 `@LoadBalanced` 的 `RestTemplate` Bean，用于通过 Nacos 服务名调用 `his-oauth2` 的缓存刷新接口。配置了连接超时 3 秒、读取超时 5 秒，防止 oauth2 不可用时长时间阻塞。

### application.yml — MyBatis 配置

加了一行 `mybatis-plus.mapper-locations: classpath*:/mapper/**/*.xml`，让 MyBatis-Plus 能扫描到 `resources/mapper/` 目录下的 XML 文件。没有这行，Mapper 接口找不到对应的 SQL 定义。

---

## Controller 层（1 个文件）

### MenuController.java — HTTP 入口

实现了 `MenuApis` 接口的两个方法，原先全部返回 null：

- `getTreeByLoginUser`：调用 service 获取菜单树列表，处理三种情况——空列表返回 null、单棵树直接返回、多棵树创建虚拟根节点合并
- `assignPermissions`：调用 service 分配权限，返回 `JsonVO.success(布尔值)`

Controller 本身不含业务逻辑，只做调用转发和响应封装。

---

## Entity 层（3 个文件）

这三个实体类对应 whale 数据库的三张表，字段类型与表结构一致，id 都是 `String`（对应 VARCHAR(64)）。

### WhaleMenu.java — 菜单实体

对应 `whale_menus` 表，包含 id、code、name、displayName、icon、path、component、permissionName、parentId、sortOrder、isEnabled、isHidden 等字段。`getTreeByLoginUser` 查询返回的就是这个实体的列表。

### WhalePermissionGrant.java — 权限授权实体

对应 `whale_permission_grants` 表，包含 id、permissionName、providerName、providerKey、tenantId、createdAt、createdBy、isDeleted。这是权限模型的枢纽表，`assignPermissions` 的删和插操作的就是这张表。

### WhaleRole.java — 角色实体

对应 `whale_roles` 表，包含 id、name、displayName、code、description、isDefault、isStatic 等。主要用于 `assignPermissions` 时校验角色是否存在。

---

## Mapper 层（3 个接口 + 2 个 XML）

### MenuMapper.java + MenuMapper.xml — 菜单查询

接口定义了 `selectMenusByUserId(String userId)` 方法，XML 里是一条五表 JOIN 的 SQL：

```
whale_user_roles → whale_roles → whale_permission_grants → whale_permissions → whale_menus
```

按 userId 查询该用户所有可见菜单，每张表都过滤 `is_deleted=0`，菜单额外过滤 `is_enabled=1` 和 `is_hidden=0`，结果按 `sort_order` 排序。

### PermissionGrantMapper.java + PermissionGrantMapper.xml — 权限授权增删改

接口定义三个方法，XML 对应三条 SQL：

- `deleteByProviderKey`：按角色 ID 删除该角色的所有授权记录（`DELETE FROM whale_permission_grants WHERE provider_name='R' AND provider_key=#{providerKey}`）
- `selectPermissionNamesByMenuIds`：按菜单 ID 列表查对应的 permission_name（去重，过滤空值）
- `batchInsertGrants`：批量插入新的授权记录（foreach 拼接 VALUES）

### RoleMapper.java — 角色查询

只继承 `BaseMapper<WhaleRole>`，用 MyBatis-Plus 自带的 `selectById` 校验角色存在，不需要额外 XML。

---

## Service 层（2 个文件）

### IMenuService.java — 业务接口

定义两个方法：

- `listMenuTreeByLoginUser()`：返回当前用户菜单树
- `assignPermissions(RolePermissionAssignDTO)`：分配角色权限

### MenuServiceImpl.java — 核心实现

这是最核心的文件，包含全部业务逻辑，分为四个部分：

#### 1. WhaleMenuTreeNodeMapper 内部类

把 `WhaleMenu` 映射为 `MenuTreeVO`。关键点：`tnId`/`tnPid` 用原始 String 保证拼树正确（UUID 也能处理），`id`/`pid` 用 `parseIntSafe` 尝试转 Integer（失败设 null，因为 VO 字段类型是 Integer）。`text` 优先用 `displayName`，没有则用 `name`。

#### 2. listMenuTreeByLoginUser 方法（当前用户菜单）

流程：取当前用户 → 查五表 JOIN → 拼树 → 返回。用户获取失败或为空时返回空列表，不抛异常。

#### 3. assignPermissions 方法（权限分配）

流程：空值检查 → Long 转 String → 校验角色存在 → 获取操作人 ID（审计字段）→ 事务内先删后插 → 注册缓存刷新回调。角色不存在直接返回 false，所选菜单均未关联权限时记录日志并返回 true。

#### 4. 缓存刷新机制

`registerCacheRefresh` 方法判断是否有事务上下文：有则注册 `TransactionSynchronization.afterCommit` 回调（事务提交后才刷新），无则直接刷新。`refreshOauth2Cache` 方法通过 RestTemplate 调用 `his-oauth2` 的 `PUT /open/resources/cache/reload` 接口，失败只记日志不抛异常。

---

## 文件间调用关系

```
HTTP 请求
  → MenuController（入口）
    → MenuServiceImpl（业务逻辑）
      ├→ UserHolder（取当前用户）
      ├→ MenuMapper + XML（查菜单 SQL）
      ├→ PermissionGrantMapper + XML（删/插权限 SQL）
      ├→ RoleMapper（校验角色）
      ├→ TreeUtils（拼菜单树）
      └→ RestTemplate（事务后刷 oauth2 缓存）
```

每一层职责单一：Controller 管协议，Service 管逻辑，Mapper 管数据，Entity 管映射，Config 管基础设施。
