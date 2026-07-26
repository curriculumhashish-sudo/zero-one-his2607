package com.zeroone.star.project.dto.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;

import java.time.LocalDate;
import java.util.List;

public class UserUpdateDTO {

    @NotBlank(message = "用户ID不能为空")
    @Schema(description = "用户ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "323022664197214208")
    private String id;

    @NotBlank(message = "用户名不能为空")
    @Schema(description = "用户名", requiredMode = Schema.RequiredMode.REQUIRED, example = "zhangsan")
    private String username;

    @NotEmpty(message = "角色不能为空")
    @Schema(description = "角色ID列表", requiredMode = Schema.RequiredMode.REQUIRED,example = "1,2,3")
    private List<String> roleIds;

    @Schema(description = "默认登录科室ID")
    private String defaultOrgId;

    @NotBlank(message = "名字不能为空")
    @Schema(description = "名")
    private String name;

    @Schema(description = "姓", example = "梁")
    private String surname;

    @Schema(description = "手机号码")
    private String phoneNumber;

    @Schema(description = "邮箱")
    private String email;

    @NotNull(message = "性别不能为空")
    @Schema(description = "性别 (2:男, 1:女, 0:未知)", requiredMode = Schema.RequiredMode.REQUIRED, example = "2")
    private Integer genderEnum;

    @Schema(description = "出生日期 (yyyy-MM-dd)")
    private LocalDate birthDate;

    @Schema(description = "地址")
    private String address;

    @Schema(description = "医保码")
    private String ybNo;

    @Schema(description = "签名图片")
    private String signature;

    @Schema(description = "职称编码")
    private String drProfttlCode;

    @Schema(description = "职业资格证编号")
    private String pharPracCertNo;

    @NotNull(message = "状态不能为空")
    @Schema(description = "状态 (1:启用, 0:停用)", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Integer isActive;

    @Schema(description = "备注信息")
    private String remark;

    @Schema(description="出诊科室名称列表",example="[\"内科\"，\"外科\"]")
    private List<String> consultDeptNames;

    @Schema(description="可接诊号源名称列表",example="[\"专家门诊\"，\"普通门诊\"]")
    private List<String>receptionSourceNames;

    @Schema(description="库房名称列表",example="[\"西药库\"，\"中药库\"]")
    private List<String>warehouseNames;

    @Schema(description="药房名称列表",example="[\"门诊药房\"]")
    private List<String>pharmacyNames;

    @Schema(description="病区名称列表",example="[\"内科一病区\"，\"外科二病区\"]")
    private List<String> wardNames;
}
