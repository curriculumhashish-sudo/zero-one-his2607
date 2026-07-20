package com.zeroone.star.project.query.j7.outpharm;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import lombok.EqualsAndHashCode;

/**
 * 患者队列查询条件（数据库层）
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "患者队列数据库查询条件")
public class PatientQueueQuery extends PageQuery {

    @Schema(description = "租户ID", example = "t001")
    private String tenantId;

    @Schema(description = "患者业务号", example = "PAT20260101")
    private String busNo;

    @Schema(description = "患者姓名", example = "张三")
    private String name;

    @Schema(description = "性别编码(gender_enum)", example = "1")
    private String genderEnum;

    @Schema(description = "身份证号", example = "110101199001011234")
    private String idCard;

    @Schema(description = "手机号", example = "13800000000")
    private String phone;

    @Schema(description = "就诊状态(status_enum)", example = "1")
    private String statusEnum;

    @Schema(description = "有效标识(0-无效 1-有效)", example = "1")
    private Integer activeFlag;

    @Schema(description = "机构ID", example = "org_001")
    private String organizationId;
}