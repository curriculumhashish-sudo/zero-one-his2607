package com.zeroone.star.project.dto.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Data;

import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：新增用户数据传输对象
 * </p>
 */
@Data
@Schema(description = "新增用户请求参数")
public class UserAddDTO {
    /**
     * 用户名
     * 校验规则：
     * 1. 不能为空
     * 2. 长度 4~32 个字符
     * 3. 仅允许字母、数字、下划线、连字符，且必须以字母或下划线开头
     * 4. 后端需统一转小写并校验全局唯一性
     */
    @NotBlank(message = "用户名不能为空")
    @Size(min = 4, max = 32, message = "用户名长度必须在4到32个字符之间")
    @Schema(description = "用户名", requiredMode = Schema.RequiredMode.REQUIRED, example = "zhangsan")
    private String username;

    /**
     * 密码
     * 校验规则：
     * 1. 不能为空
     * 2. 长度 8~32 个字符
     * 3. 复杂度：至少包含大写、小写、数字、特殊符号中的3种
     * 4. 后端需额外校验：禁止包含用户名、连续字符或重复字符
     */
    @NotBlank(message = "密码不能为空")
    @Size(min = 8, max = 32, message = "密码长度必须在8到32个字符之间")
    @Pattern(regexp = "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*])[a-zA-Z\\d!@#$%^&*]{8,32}$",
            message = "密码必须包含大写字母、小写字母、数字和特殊符号(!@#$%^&*)")
    @Schema(description = "密码", requiredMode = Schema.RequiredMode.REQUIRED, example = "Pass@123")
    private String password;

    /**
     * 角色 ID列表
     * 校验规则：
     * 1. 不能为 null 且不能为空数组
     * 2. 后端需校验每个 ID 在系统中存在，且状态为启用(status=1)
     */
    @NotEmpty(message = "请至少为用户分配一个角色")
    @Schema(description = "角色 ID列表", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"1\", \"2\"]")
    private List<String> roleIds;

    /**
     * 默认登录科室ID
     * 校验规则：
     * 1. 若传入，后端需校验科室真实存在
     * 2. 后端需校验当前操作用户有权管辖该科室（防越权）
     */
    @Schema(description = "默认登录科室ID", example = "10086")
    private String defaultOrgId;

    /**
     * 名字
     */
    @NotBlank(message = "名字不能为空")
    @Schema(description = "名", example = "三")
    private String name;

    /**
     * 姓氏
     */
    @Schema(description = "姓", example = "梁")
    private String surname;

    /**
     * 手机号码
     * 校验规则：
     * 1. 若不为空，必须符合中国大陆手机号正则
     * 2. 后端需校验全局唯一性
     */
    @Pattern(regexp = "^1[3-9]\\d{9}$", message = "手机号码格式不正确")
    @Schema(description = "手机号码", example = "13800138000")
    private String phoneNumber;

    /**
     * 邮箱
     * 校验规则：
     * 1. 若不为空，必须符合标准 Email 格式，且长度不超过 100
     * 2. 后端需校验全局唯一性
     */
    @Email(message = "邮箱格式不正确")
    @Size(max = 100, message = "邮箱长度不能超过100个字符")
    @Schema(description = "邮箱", example = "zhangsan@example.com")
    private String email;

    /**
     * 性别
     * 校验规则：枚举类型，仅允许 0(未知), 1(女), 2(男)
     */
    @NotNull(message = "性别不能为空")
    @Min(value = 0, message = "性别取值不合法")
    @Max(value = 2, message = "性别取值不合法")
    @Schema(description = "性别 (0:未知, 1:女, 2:男)", requiredMode = Schema.RequiredMode.REQUIRED, example = "2")
    private Integer genderEnum;

    /**
     * 出生日期
     */
    @Schema(description = "出生日期 (yyyy-MM-dd)", example = "1990-01-01")
    private LocalDate birthDate;

    /**
     * 地址
     */
    @Schema(description = "地址", example = "北京市朝阳区")
    private String address;

    /**
     * 医保码
     */
    @Schema(description = "医保码", example = "YB12345678")
    private String ybNo;

    /**
     * 签名图片
     * 说明：通常存储图片的 URL 或 Base64 字符串
     */
    @Schema(description = "签名图片URL或Base64")
    private String signature;

    /**
     * 职称编码
     */
    @Schema(description = "职称编码", example = "ZC001")
    private String drProfttlCode;

    /**
     * 职业资格证编号
     */
    @Schema(description = "职业资格证编号", example = "CERT20230001")
    private String pharPracCertNo;

    /**
     * 账号状态
     * 校验规则：整型，仅允许 0(停用), 1(启用)
     */
    @NotNull(message = "账号状态不能为空")
    @Min(value = 0, message = "账号状态取值不合法")
    @Max(value = 1, message = "账号状态取值不合法")
    @Schema(description = "账号状态 (0:停用, 1:启用)", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Integer isActive;

    /**
     * 备注信息
     */
    @Schema(description = "备注信息", example = "新员工入职")
    private String remark;
}