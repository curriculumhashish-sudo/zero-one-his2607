package com.zeroone.star.project.vo.j1.login;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

/**
 * 医院信息VO
 */
@Data
@Schema(description = "医院信息")
public class HospitalVO {

    @Schema(description = "医院ID", example = "1")
    private Long hospitalId;

    @Schema(description = "医院编码", example = "H001")
    private String hospitalCode;

    @Schema(description = "医院名称", example = "XX市人民医院")
    private String hospitalName;

    @Schema(description = "医院等级", example = "三级甲等")
    private String level;

    @Schema(description = "医院地址")
    private String address;
}