package com.zeroone.star.project.dto.j4.diagnosis;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.Size;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Schema(description = "创建诊断请求DTO")
@Data
@AllArgsConstructor
@NoArgsConstructor
public class CreateDiagnosisDTO {

    @Schema(description = "病案记录标识", example = "case001", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "病案记录标识不能为空")
    private String caseRecordId;

    @Schema(description = "诊断类型编码", example = "MAIN", requiredMode = Schema.RequiredMode.REQUIRED)
    @NotBlank(message = "诊断类型编码不能为空")
    @Size(max = 32, message = "诊断类型编码长度不能超过32个字符")
    private String diagTypeCode;

    @Schema(description = "排序号", example = "1")
    private Integer sortNo;

    @Schema(description = "诊断编码", example = "D001")
    @Size(max = 32, message = "诊断编码长度不能超过32个字符")
    private String diagCode;

    @Schema(description = "诊断名称", example = "急性上呼吸道感染")
    @Size(max = 200, message = "诊断名称长度不能超过200个字符")
    private String diagName;

    @Schema(description = "入院条件编码", example = "ACUTE")
    @Size(max = 16, message = "入院条件编码长度不能超过16个字符")
    private String admCondCode;

    @Schema(description = "主诊断标志位", example = "Y")
    @Size(max = 3, message = "主诊断标志位长度不能超过3个字符")
    private String maindiagFlag;

    @Schema(description = "有效标志位", example = "1")
    @Size(max = 3, message = "有效标志位长度不能超过3个字符")
    private String valiFlag;
}