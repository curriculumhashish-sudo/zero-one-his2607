package com.zeroone.star.project.components.user;

import lombok.Builder;
import lombok.Data;

import java.util.List;
import java.util.Map;

/**
 * <p>
 * 描述：用户数据
 * FIXME 此类需要根据实际业务需求来添加字段
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Builder
public class UserDTO {
    /**
     * 用户编号
     */
    private String id;
    /**
     * 用户名称(登录账号)
     */
    private String username;
    /**
     * 是否启用
     */
    private Byte isEnabled;
    /**
     * 用户拥有角色列表
     */
    private List<String> roles;
    /**
     * 用户名称（真实姓名）
     */
    private String name;
    /**
     * 组织单元信息
     * 每个元素：key为组织单元编号，value为组织单元名称
     */
    private List<Map<String, String>> organizationUnits;
    /**
     * 对应的医务人员编号
     */
    private String practitionerId;
    /**
     * 对应的医务人员名称
     */
    private String practitionerName;
    /**
     * 职称名称，如：主任医师、护士、药师、技师等
     */
    private String titleName;
    /**
     * 职称编码，职称名称对应的编码值
     */
    private String titleCode;
}
