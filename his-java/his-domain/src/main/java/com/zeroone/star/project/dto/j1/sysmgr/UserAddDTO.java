package com.zeroone.star.project.dto.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.time.LocalDate;
import java.util.List;

/**
 * <p>
 * 描述：新增用户数据传输对象
 * </p>
 */
@Data
@Schema(description = "新增用户数据传输对象")
public class UserAddDTO {
    @Schema(description = "用户名", example = "zhangsan", requiredMode = Schema.RequiredMode.REQUIRED)
    private String username;

    @Schema(description = "密码(至少8位)", example = "12345678", requiredMode = Schema.RequiredMode.REQUIRED)
    private String password;

    @Schema(description = "角色名称", example = "1001", requiredMode = Schema.RequiredMode.REQUIRED)
    private List<String> roleNames;

    @Schema(description = "默认登录科室", example = "门诊内科")
    private Long deptId;

    @Schema(description = "用户昵称", example = "梁老师", requiredMode = Schema.RequiredMode.REQUIRED)
    private String nickname;

    @Schema(description = "手机号码", example = "13800138000")
    private String phone;

    @Schema(description = "邮箱", example = "test@example.com")
    private String email;

    @Schema(description = "用户性别 (1:男, 2:女, 0:未知)", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer gender;

    @Schema(description = "状态 (1:正常, 0:停用)", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;

    @Schema(description = "出生日期", example = "1990-01-01")
    private LocalDate birthday;

    @Schema(description = "地址", example = "北京市朝阳区...")
    private String address;

    @Schema(description = "医保码", example = "YB123456")
    private String medicalCode;

    @Schema(description = "签名图片URL", example = "/upload/sign.png")
    private String signImg;

    @Schema(description = "职称编码", example = "TITLE_01")
    private String titleCode;

    @Schema(description = "职业资格证编号", example = "ZG2023001")
    private String certNo;

    @Schema(description = "备注", example = "暂无备注")
    private String remark;
}
