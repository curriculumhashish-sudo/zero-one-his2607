package com.zeroone.star.project.vo.j3.department;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：科室名称树VO
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 胡傲宇
 * @version 1.0.0
 */
@Data
@Builder
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "科室名称树VO")
public class DepartmentTreeVO {
    @Schema(description = "主键ID", example = "322663105141149696")
    private Long id;

    @Schema(description = "业务编号", example = "OR0007.OR0008")
    private String busNo;

    @Schema(description = "机构/科室名称", example = "门诊内科")
    private String name;

    @Schema(description = "类型", example = "ACTIVE")
    private String statusEnum;

    @Schema(description = "状态显示文本", example = "已启用")
    private String statusEnumDictText;

    @Schema(description = "类型", example = "1")
    private String typeEnum;

    @Schema(description = "类型显示文本", example = "科室")
    private String typeEnumDictText;

    @Schema(description = "类别", example = "CLINIC")
    private String classEnum;

    @Schema(description = "分类显示文本", example = "门诊")
    private String classEnumDictText;

    @Schema(description = "拼音码(py_str)", example = "xnxnk")
    private String pyStr;

    @Schema(description = "五笔码(wb_str)", example = "tntk")
    private String wbStr;

    @Schema(description = "默认医生ID(def_doctor_id)", example = "308791351298756608")
    private Long defDoctorId;

    @Schema(description = "医保编号(yb_no)", example = "YB-3301-001")
    private String ybNo;

    @Schema(description = "医保名称(yb_name)", example = "心血管内科(医保)")
    private String ybName;

    @Schema(description = "医疗机构标识(medins_id)", example = "MEDINS-2026-001")
    private String medinsId;

    @Schema(description = "医疗机构医保区划(medins_admdvs)", example = "330100")
    private String medinsAdmdvs;

    @Schema(description = "医疗机构类型")
    private String medinsType;

    @Schema(description = "医疗机构等级")
    private String medinsLv;

    @Schema(description = "科别(caty)", example = "内科")
    private String caty;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "父级机构/科室ID", example = "322663105141149696")
    private Long parentId;

    @Schema(description = "组织单元ID", example = "OU20260720001")
    private String ouId;

    @Schema(description = "电子发票部门编码", example = "ELE-INV-DEPT-001")
    private String eleInvoiceDeptCode;

    @Schema(description = "子节点列表")
    private List<DepartmentTreeVO> children = new ArrayList<>();
}
