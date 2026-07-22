package com.zeroone.star.project.dto.j1.login;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 选择医院请求DTO
 */
@Data
@Schema(description = "选择医院请求")
public class SelectHospitalDTO {

    @Schema(description = "医院ID", requiredMode = Schema.RequiredMode.REQUIRED, example = "1")
    private Long hospitalId;

    @Schema(description = "医院编码", requiredMode = Schema.RequiredMode.REQUIRED, example = "H001")
    private String hospitalCode;
}