package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

@Data
@Schema(description = "医院信息")
public class HospitalVO {

    @Schema(description = "医院ID", example = "1")
    private Long id;

    @Schema(description = "医院编码", example = "H001")
    private String code;

    @Schema(description = "医院名称", example = "XX市人民医院")
    private String name;

    @Schema(description = "医院显示名称", example = "XX市人民医院")
    private String displayName;
}
