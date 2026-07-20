package com.zeroone.star.project.dto.j3.supplier;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.NotBlank;
import lombok.Data;
import java.io.Serializable;

/**
 * <p>
 * 描述：供应商数据传输对象
 * </p>
 * @author 业之
 * @version 1.0.0
 */
@Data
@Schema(description = "供应商数据对象")
public class SupplierDTO implements Serializable {

    @Schema(description = "供应商ID", example = "1", requiredMode = Schema.RequiredMode.NOT_REQUIRED)
    private Long id;

    @NotBlank(message = "供应商编号不能为空")
    @Schema(description = "供应商编号", example = "SUPP002", requiredMode = Schema.RequiredMode.REQUIRED)
    private String supplierCode;

    @NotBlank(message = "供应商名称不能为空")
    @Schema(description = "供应商名称", example = "华东医疗器械制造有限公司", requiredMode = Schema.RequiredMode.REQUIRED)
    private String supplierName;

    @NotBlank(message = "供应商类型不能为空")
    @Schema(description = "供应商类型", example = "生产厂家", requiredMode = Schema.RequiredMode.REQUIRED)
    private String supplierType;

    @Schema(description = "信用代码", example = "913100001234567890")
    private String creditCode;

    @Schema(description = "联系人", example = "张三")
    private String contactPerson;

    @Schema(description = "联系电话", example = "13800138000")
    private String contactPhone;

    @Schema(description = "地址", example = "上海市浦东新区")
    private String address;

    @Schema(description = "资质信息", example = "医疗器械经营许可证")
    private String qualificationInfo;
}
