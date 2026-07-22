package com.zeroone.star.project.vo.j1.sysmgr;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Data

@Schema(description = "用户详情对象")
public class UserDetailVO {
    @Schema(description = "主键id", example = "336396155648217088")
    private String id;

    @Schema(description = "用户名", example = "lianglaoshi")
    private String username;

    @Schema(description = "角色ID列表", example = "[\"10\", \"20\"]")
    private List<String> roleIds;

    @Schema(description = "默认登录科室ID", example = "322663218269917184")
    private String defaultOrgId;

    @Schema(description = "用户昵称", example = "梁老师")
    private String nickname;

    @Schema(description = "手机号码", example = "13800138000")
    private String phoneNumber;

    @Schema(description = "电子邮箱", example = "lianglaoshi@example.com")
    private String email;

    @Schema(description = "性别 (2:男, 1:女, 0:未知)", example = "2")
    private Integer gender;

    @Schema(description = "出生日期 (yyyy-MM-dd)", example = "1990-01-01")
    private LocalDate birthDate;

    @Schema(description = "地址", example = "人民东路100号")
    private String address;

    @Schema(description = "医保码", example = "YB20240001")
    private String ybNo;

    @Schema(description = "签名图片", example = "/upload/signatures/sign_322663.png")
    private String signatureImage;

    @Schema(description = "职称编码", example = "PROF_001")
    private String drProfiterole;

    @Schema(description = "职业资格证编号", example = "PHAR20240001")
    private String pharPracCertNo;

    @Schema(description = "状态 (1:启用, 0:停用)", example = "1")
    private Integer status;

    @Schema(description = "备注信息", example = "表现优异，可重点关注")
    private String remark;

    @Schema(description = "出诊科室ID列表", example = "[\"dept_001\", \"dept_002\"]")
    private List<String> consultDeptIds;

    @Schema(description = "可接诊号源ID列表", example = "[\"src_10\", \"src_20\"]")
    private List<String> receptionSourceIds;

}