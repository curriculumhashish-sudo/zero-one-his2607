package com.zeroone.star.project.dto.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;

import java.time.LocalDate;
import java.util.List;

public class UserUpdateDTO {
    /**
     * 用户名
     */
    @Schema(description = "用户名", example = "zhangsan", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    /**
     * 角色ID
     */
    @Schema(description = "角色ID", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<Long> roleId;

    /**
     * 科室ID(不填时默认登录科室)
     */
    @Schema(description = "科室ID(不填时默认登录科室)", example = "1001")
    private Long deptId;

    /**
     * 出诊科室ID列表
     */
    @Schema(description = "出诊科室ID列表")
    private List<Long> consultationDeptIds;

    /**
     * 可接诊号源类型列表
     */
    @Schema(description = "可接诊号源类型列表")
    private List<String> registrationSourceTypes;

    /**
     * 用户昵称
     */
    @Schema(description = "用户昵称", example = "梁老师", requiredMode = Schema.RequiredMode.REQUIRED)
    private String nickname;

    /**
     * 手机号码
     */
    @Schema(description = "手机号码", example = "13800138000")
    private String phone;

    /**
     * 邮箱
     */
    @Schema(description = "邮箱", example = "test@example.com")
    private String email;

    /**
     * 用户性别 (1:男, 2:女, 0:未知)
     */
    @Schema(description = "用户性别 (1:男, 2:女, 0:未知)", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer gender;

    /**
     * 状态 (1:正常, 0:停用)
     */
    @Schema(description = "状态 (1:正常, 0:停用)", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;

    /**
     * 出生日期
     */
    @Schema(description = "出生日期", example = "1990-01-01")
    private LocalDate birthday;

    /**
     * 地址
     */
    @Schema(description = "地址", example = "北京市朝阳区...")
    private String address;

    /**
     * 医保码
     */
    @Schema(description = "医保码", example = "YB123456")
    private String medicalCode;

    /**
     * 签名图片URL
     */
    @Schema(description = "签名图片URL", example = "/upload/sign.png")
    private String signImg;

    /**
     * 职称编码
     */
    @Schema(description = "职称编码", example = "TITLE_01")
    private String titleCode;

    /**
     * 职业资格证编号
     */
    @Schema(description = "职业资格证编号", example = "ZG2023001")
    private String certNo;

    /**
     * 备注
     */
    @Schema(description = "备注", example = "暂无备注")
    private String remark;
}
