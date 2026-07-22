package com.zeroone.star.project.dto.j7.outpharm;

import com.zeroone.star.project.dto.PageDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 获取患者队列 前端请求入参DTO
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者队列查询请求参数")
public class PatientQueueDTO extends PageDTO {

    @Schema(description = "租户ID", example = "t001")
    private String tenantId;

    @Schema(description = "患者业务编号", example = "PAT20260101")
    private String busNo;

    @Schema(description = "患者姓名", example = "张三")
    private String name;

    @Schema(description = "性别编码(gender_enum)：0-未知 1-男 2-女 9-未说明", example = "1")
    private String genderEnum;

    @Schema(description = "身份证号", example = "110101199001011234")
    private String idCard;

    @Schema(description = "联系电话", example = "13800000000")
    private String phone;

    @Schema(description = "患者状态编码(status_enum)：1-候诊 2-叫号中 3-已完成 4-弃号", example = "1")
    private String statusEnum;

    @Schema(description = "有效标识(0-无效 1-有效)", example = "1")
    private Integer activeFlag;

    @Schema(description = "所属机构ID", example = "org_001")
    private String organizationId;
}