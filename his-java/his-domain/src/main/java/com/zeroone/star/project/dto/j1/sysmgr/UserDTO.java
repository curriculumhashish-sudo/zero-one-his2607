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
@Schema(description = "新增用户请求参数")
public class UserDTO {
    @Schema(description = "用户名", requiredMode = Schema.RequiredMode.REQUIRED, example = "zhangsan")
    private String username;

    @Schema(description = "密码", requiredMode = Schema.RequiredMode.REQUIRED,example = "12345678")
    private String password;

    @Schema(description = "角色 ID列表", requiredMode = Schema.RequiredMode.REQUIRED,example = "1,2,3")
    private List<String> roleIds;

    @Schema(description = "默认登录科室ID")
    private String defaultOrgId;

    @Schema(description = "用户昵称", requiredMode = Schema.RequiredMode.REQUIRED, example = "张三")
    private String nickname;

    @Schema(description = "手机号码")
    private String phoneNumber;

    @Schema(description = "邮箱")
    private String email;

    @Schema(description = "性别 (2:男, 1:女, 0:未知)", requiredMode = Schema.RequiredMode.REQUIRED, example = "2")
    private Integer gender;

    @Schema(description = "出生日期 (yyyy-MM-dd)")
    private LocalDate birthDate;

    @Schema(description = "地址")
    private String address;

    @Schema(description = "医保码")
    private String ybNo;

    @Schema(description = "签名图片")
    private String signatureImage;

    @Schema(description = "职称编码")
    private String drProfiterole;

    @Schema(description = "职业资格证编号")
    private String pharPracCertNo;

    @Schema(description = "状态 (1:启用, 0:停用)", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Integer status;

    @Schema(description = "备注信息")
    private String remark;

//    @Schema(description = "出诊科室ID列表 ")
//    private List<String> consultDeptIds;
//
//    @Schema(description = "可接诊号源ID列表")
//    private List<String> receptionSourceIds;
}