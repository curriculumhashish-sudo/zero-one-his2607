package com.zeroone.star.project.dto.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.*;
import lombok.Data;

import java.time.LocalDate;
import java.util.List;

@Data
@Schema(description = "修改用户请求参数")
public class UserUpdateDTO {
    @NotBlank(message = "用户id不能为空")
    @Schema(description = "主键id", example = "336396155648217088")
    private String id;

    @NotBlank(message = "用户名不能为空")
    @Schema(description = "用户名", requiredMode = Schema.RequiredMode.REQUIRED, example = "zhangsan")
    private String username;

    @NotEmpty(message = "角色不能为空")
    @Schema(description = "角色ID列表", requiredMode = Schema.RequiredMode.REQUIRED, example = "[\"roleId1\", \"roleId2\"]")
    private List<String> roleIds;

    @Schema(description = "默认登录科室ID")
    private String defaultOrgId;

    @NotBlank(message = "用户昵称不能为空")
    @Schema(description = "名")
    private String name;


    @Pattern(regexp = "^1[3-9]\\d{9}$", message = "手机号必须为11位数字")
    @Schema(description = "手机号码")
    private String phoneNumber;

    @Schema(description = "邮箱")
    private String email;

    @NotNull(message = "性别不能为空")
    @Schema(description = "性别 (2:男, 1:女, 0:未知)", requiredMode = Schema.RequiredMode.REQUIRED, example = "2")
    private GenderEnum genderEnum;

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
    @Min(value = 0, message = "状态只能为0或1")
    @Max(value = 1, message = "状态只能为0或1")
    @Schema(description = "状态 (1:启用, 0:停用)", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Integer isActive;

    @Schema(description = "备注信息")
    private String remark;

    @Schema(description = "出诊科室名称列表id", example = "[\"10\", \"20\"]")
    private List<String> consultDeptIds;

    @Schema(description = "可接诊号源名称列表id", example = "[\"10\", \"20\"]")
    private List<String> receptionSourceIds;

    // ========== 位置信息 ==========
    @Schema(description = "库房名称列表id", example = "[\"10\", \"20\"]")
    private List<String> warehouseIds;

    @Schema(description = "药房名称列表id", example = "[\"10\", \"20\"]")
    private List<String> pharmacyIds;

    @Schema(description = "病区名称列表id", example = "[\"10\", \"20\"]")
    private List<String> wardIds;

}
