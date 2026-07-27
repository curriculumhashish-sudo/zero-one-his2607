package com.zeroone.star.project.vo.j1.user;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Data
@Schema(description = "用户详情对象")
public class UserDetailVO {

    // ========== 基础信息 ==========
    @Schema(description = "主键id", example = "336396155648217088")
    private String id;

    @Schema(description = "用户名", example = "lianglaoshi")
    private String username;

    @Schema(description = "角色", example = "13800138000")
    private String roleName;

    @Schema(description = "默认科室", example = "[\"内科\", \"外科\"]")
    private String defaultDeptNames;

    @Schema(description = "名")
    private String name;

    @Schema(description = "手机号码", example = "13800138000")
    private String phone;

    @Schema(description = "电子邮箱", example = "lianglaoshi@example.com")
    private String email;

    @Schema(description = "性别 (2:男, 1:女, 0:未知)", example = "2")
    private Integer genderEnum;

    @Schema(description = "出生日期 (yyyy-MM-dd)", example = "1990-01-01")
    private LocalDate birthDate;

    @Schema(description = "地址", example = "人民东路100号")
    private String address;

    // ========== 职业信息 ==========
    @Schema(description = "医保码", example = "YB20240001")
    private String ybNo;

    @Schema(description = "签名图片", example = "/upload/signatures/sign_322663.png")
    private String signature;

    @Schema(description = "职称编码", example = "PROF_001")
    private String drProfttlCode;

    @Schema(description = "职业资格证编号", example = "PHAR20240001")
    private String pharPracCertNo;

    @Schema(description = "备注信息", example = "表现优异")
    private String remark;

    // ========== 状态控制 ==========
    @Schema(description = "启用状态 (1:启用, 0:停用)", example = "1")
    private Integer isActive;

    @Schema(description = "锁定状态 (1:已锁定, 0:未锁定)", example = "0")
    private Integer isLockedOut;

    @Schema(description = "登录失败次数", example = "0")
    private Integer accessFailedCount;

    @Schema(description = "锁定结束时间")
    private LocalDateTime lockoutEnd;

    // ========== 审计信息 ==========
    @Schema(description = "创建时间")
    private LocalDateTime createdAt;

    @Schema(description = "更新时间")
    private LocalDateTime updatedAt;

    // ========== 关联列表 ==========
    @Schema(description = "角色ID列表", example = "[\"10\", \"20\"]")
    private List<String> roleIds;
    @Schema(description = "角色名称列表",example = "[\"管理员\", \"医生\"]")
    private List<String> roleNames;


    @Schema(description = "出诊科室名称列表名称", example = "[\"内科\", \"外科\"]")
    private List<String> consultDeptNames;
    @Schema(description = "出诊科室名称列表id", example = "[\"10\", \"20\"]")
    private List<String> consultDeptIds;

    @Schema(description = "可接诊号源名称列表名称", example = "[\"专家门诊\", \"普通门诊\"]")
    private List<String> receptionSourceNames;
    @Schema(description = "可接诊号源名称列表id", example = "[\"10\", \"20\"]")
    private List<String> receptionSourceIds;

    // ========== 位置信息 ==========
    @Schema(description = "库房名称列表名称", example = "[\"西药库\", \"中药库\"]")
    private List<String> warehouseNames;
    @Schema(description = "库房名称列表id", example = "[\"10\", \"20\"]")
    private List<String> warehouseIds;

    @Schema(description = "药房名称列表名称", example = "[\"门诊药房\"]")
    private List<String> pharmacyNames;
    @Schema(description = "药房名称列表id", example = "[\"10\"]")
    private List<String> pharmacyIds;

    @Schema(description = "病区名称列表名称", example = "[\"内科一病区\", \"外科二病区\"]")
    private List<String> wardNames;
    @Schema(description = "病区名称列表id", example = "[\"10\", \"20\"]")
    private List<String> wardIds;

}