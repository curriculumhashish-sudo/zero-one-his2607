package com.zeroone.star.project.dto.j3.register;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotNull;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：挂号类型新增/修改传输数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * <p>
 * 使用说明：
 * 1. 新增时 id 不需要传入，由系统自动生成；
 * 2. 修改时 id 必须传入，用于定位待修改的记录。
 * </p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Data
@Schema(description = "挂号类型传输数据对象")
public class RegisterDTO implements Serializable {

    /**
     * 挂号类型唯一标识（修改时必传，新增时不传）
     */
    @Schema(description = "挂号类型唯一标识（修改时必填）", example = "1")
    private Long id;

    /**
     * 挂号类型名称
     */
    @NotBlank(message = "名称不能为空")
    @Schema(description = "挂号类型名称", example = "普通号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 医保编码
     */
    @NotBlank(message = "医保编码不能为空")
    @Schema(description = "医保编码", example = "001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String mdtrtCertNo;

    /**
     * 业务编码
     */
    @NotBlank(message = "业务编码不能为空")
    @Schema(description = "业务编码", example = "HS0001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String businessCode;

    /**
     * 金额
     */
    @NotNull(message = "金额不能为空")
    @DecimalMin(value = "0", message = "金额不能小于0")
    @Schema(description = "金额", example = "5.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal amount;

    /**
     * 状态：1 启用 0 禁用
     */
    @NotNull(message = "状态不能为空")
    @Schema(description = "状态 1启用 0禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;


}
