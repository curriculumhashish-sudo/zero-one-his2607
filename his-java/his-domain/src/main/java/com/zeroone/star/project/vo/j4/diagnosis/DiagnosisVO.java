package com.zeroone.star.project.vo.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Schema(description = "诊断响应VO")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class DiagnosisVO {

    @Schema(description = "诊断唯一编号", example = "uuid-001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String id;

    @Schema(description = "病案记录标识", example = "case001")
    private String caseRecordId;

    @Schema(description = "租户标识", example = "default")
    private String tenantId;

    @Schema(description = "诊断类型编码", example = "MAIN")
    private String diagTypeCode;

    @Schema(description = "排序号", example = "1")
    private Integer sortNo;

    @Schema(description = "诊断编码", example = "D001")
    private String diagCode;

    @Schema(description = "诊断名称", example = "急性上呼吸道感染")
    private String diagName;

    @Schema(description = "入院条件编码", example = "ACUTE")
    private String admCondCode;

    @Schema(description = "主诊断标志位", example = "Y")
    private String maindiagFlag;

    @Schema(description = "有效标志位", example = "1")
    private String valiFlag;

    @Schema(description = "创建时间", example = "2024-01-01T10:00:00")
    private LocalDateTime createdAt;

    @Schema(description = "最后更新时间", example = "2024-01-01T11:00:00")
    private LocalDateTime updatedAt;
}