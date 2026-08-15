package com.zeroone.star.project.vo.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * 科室列表及详情返回对象。
 *
 * @author 很师大王
 * @version 1.0.0
 */
@Data
@Schema(description = "科室信息")
public class DepartmentVO implements Serializable {

    @Schema(description = "科室ID", example = "322663218269917184")
    private String id;

    @Schema(description = "科室名称", example = "门诊内科")
    private String name;

    @Schema(description = "业务编码", example = "OR0007.OR0008")
    private String busNo;

    @Schema(description = "上级组织ID", example = "322663105141149696")
    private String parentId;

    @Schema(description = "类型枚举", example = "DEPARTMENT")
    private String typeEnum;

    @Schema(description = "类型名称", example = "科室")
    private String typeEnumDictText;

    @Schema(description = "类别枚举", example = "CLINIC")
    private String classEnum;

    @Schema(description = "类别名称", example = "门诊")
    private String classEnumDictText;

    @Schema(description = "状态枚举", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "状态名称", example = "已启用")
    private String statusEnumDictText;

    @Schema(description = "是否启用，1启用、0停用", example = "1")
    private Integer activeFlag;

    @Schema(description = "医保编码", example = "HIS001")
    private String ybNo;

    @Schema(description = "医保名称", example = "门诊内科")
    private String ybName;

    @Schema(description = "医疗机构标识", example = "H12345678")
    private String medinsId;

    @Schema(description = "医保区划", example = "440100")
    private String medinsAdmdvs;

    @Schema(description = "医疗机构类型", example = "HOSPITAL")
    private String medinsType;

    @Schema(description = "医疗机构等级", example = "LEVEL_3_A")
    private String medinsLv;

    @Schema(description = "拼音码", example = "MZNK")
    private String pyStr;

    @Schema(description = "五笔码", example = "UYMT")
    private String wbStr;

    @Schema(description = "默认医师ID", example = "322663105141149700")
    private String defDoctorId;

    @Schema(description = "科别", example = "01")
    private String caty;

    @Schema(description = "显示顺序", example = "1")
    private String displayOrder;

    @Schema(description = "组织单元ID")
    private String ouId;

    @Schema(description = "电子发票部门编码")
    private String eleInvoiceDeptCode;
}
