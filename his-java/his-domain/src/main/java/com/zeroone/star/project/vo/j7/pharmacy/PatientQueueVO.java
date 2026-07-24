package com.zeroone.star.project.vo.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

/**
 * <p>
 * 描述：患者队列视图对象
 * </p>
 * <p>
 * 功能说明：用于展示退药患者队列信息，包含患者基本信息、床位号、科室等。
 * 数据来源于 return_header 表及相关联的患者信息表。
 * 在分页查询退药患者队列时，将数据库记录转换为该 VO 返回给前端。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@AllArgsConstructor
@NoArgsConstructor
@Schema(description = "患者队列VO")
public class PatientQueueVO {
    @Schema(description = "患者ID")
    private String patientId;

    @Schema(description = "患者姓名")
    private String patientName;

    @Schema(description = "病历号")
    private String medicalNo;

    @Schema(description = "床位号")
    private String bedNo;

    @Schema(description = "科室名称")
    private String deptName;

    @Schema(description = "单据状态（0-待处理 1-已处理）")
    private Integer status;
}
