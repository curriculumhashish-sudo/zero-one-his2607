package com.zeroone.star.project.dto.j3.fee;

import com.zeroone.star.project.enums.j3.fee.FeeCategoryEnum;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

/**
 * <p>
 * 描述：费用性质数据传输对象
 * @author 白文儿
 * @version 1.0.0
 */
@Data
@Schema(description = "费用性质数据对象")
public class FeeNatureDTO implements Serializable {

    @Schema(description = "费用性质名称", example = "自费")
    private String name;

    @Schema(description = "费用性质编码（业务编号）", example = "1001")
    private String busNo;

    @Schema(description = "显示顺序", example = "1")
    private Integer displayOrder;

    @Schema(description = "费用性质分类(1自费、2公费、3省医保、4市医保)", example = "1")
    private FeeCategoryEnum categoryEnum;
}
