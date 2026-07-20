package com.zeroone.star.project.vo.j3.register;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;
import java.math.BigDecimal;

/**
 * <p>
 * 描述：挂号类型列表显示数据对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author j3小组
 * @version 1.0.0
 */
@Data
@Schema(description = "挂号类型显示对象")
public class RegisterVO implements Serializable {

    /**
     * 挂号类型唯一标识（序号）
     */
    @Schema(description = "挂号类型唯一标识", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Long id;

    /**
     * 挂号类型名称
     */
    @Schema(description = "挂号类型名称", example = "普通号", requiredMode = Schema.RequiredMode.REQUIRED)
    private String name;

    /**
     * 医保编码
     */
    @Schema(description = "医保编码", example = "001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String mdtrtCertNo;

    /**
     * 业务编码
     */
    @Schema(description = "业务编码", example = "HS0001", requiredMode = Schema.RequiredMode.REQUIRED)
    private String businessCode;

    /**
     * 金额
     */
    @Schema(description = "金额", example = "5.00", requiredMode = Schema.RequiredMode.REQUIRED)
    private BigDecimal amount;

    /**
     * 状态：1 启用 0 禁用
     */
    @Schema(description = "状态 1启用 0禁用", example = "1", requiredMode = Schema.RequiredMode.REQUIRED)
    private Integer status;
}
