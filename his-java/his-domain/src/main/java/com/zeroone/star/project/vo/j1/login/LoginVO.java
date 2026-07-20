package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：当前用户信息视图对象（VO）
 * </p>
 * <p>
 * 说明：本类用于封装登录成功后返回给前端的当前用户信息，属于<strong>视图展示型VO</strong>。
 * 数据由 Service 层从 Token（JWT）中解析并组装，返回给 Controller，最终包装成 JsonVO 响应给前端。
 * 前端使用该对象渲染页面右上角的用户信息、进行权限控制（按钮级、菜单级）等。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author J1组
 * @version 1.0.0
 */
/*专门用于把当前登录用户的信息返回给前端显示*/
@Schema(description = "当前用户信息视图对象")
@Data
public class LoginVO {

    /**
     * 用户唯一编号
     * <p>对应数据库字段：whale_users.id（VARCHAR(64)，主键），与数据库类型一致</p>
     * <p>用途：唯一标识用户身份，用于前端展示用户编号、关联用户操作记录等</p>
     */
    @Schema(description = "用户唯一编号", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    /**
     * 用户名（账户名）
     * <p>对应数据库字段：whale_users.user_name（VARCHAR(256)），字段名与数据库列名保持一致的驼峰形式，
     * 便于与数据持久层实体通过反射自动转换</p>
     * <p>用途：显示在页面右上角、个人中心等位置，标识当前登录用户</p>
     */
    @Schema(description = "用户名", example = "admin", requiredMode = Schema.RequiredMode.REQUIRED)
    private String userName;

    /**
     * 用户是否启用
     * <p>对应数据库字段：whale_users.is_active（TINYINT(1)），Java 端使用 Boolean 与数据库类型对应</p>
     * <p>取值：false-禁用，true-启用</p>
     * <p>用途：前端判断用户状态，禁用用户（值为false）时提示无法操作或引导联系管理员</p>
     */
    @Schema(description = "是否启用 true-启用 false-禁用", example = "true", requiredMode = Schema.RequiredMode.REQUIRED)
    private Boolean isActive;

    /**
     * 用户角色列表
     * <p>对应数据库字段：whale_roles.code（VARCHAR(256)，角色编码）集合，
     * 通过 whale_user_roles 关联表按 user_id → role_id 查询得到</p>
     * <p>用途：前端权限控制，判断当前用户是否拥有某个角色（如 ROLE_ADMIN、ROLE_DOCTOR），
     * 从而决定是否显示某些按钮、菜单或执行某些操作</p>
     */
    @Schema(description = "用户角色编码列表", example = "[\"ROLE_ADMIN\"]", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> roles;
}
