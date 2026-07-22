package com.zeroone.star.project.vo.j4.medicine;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.io.Serializable;

@Data
@Schema(description = "新增药品结果")
public class MedicineAddVO implements Serializable {

    private static final long serialVersionUID = 1L;

    @Schema(description = "药品ID")
    private String id;

    @Schema(description = "药品业务编号")
    private String busNo;

    @Schema(description = "通用名称")
    private String name;

    @Schema(description = "商品名称")
    private String merchandiseName;

    @Schema(description = "药品状态")
    private String statusEnum;

    @Schema(description = "是否有效")
    private Boolean activeFlag;
}