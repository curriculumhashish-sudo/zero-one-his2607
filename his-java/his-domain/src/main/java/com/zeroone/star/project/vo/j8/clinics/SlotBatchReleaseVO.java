package com.zeroone.star.project.vo.j8.clinics;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.util.List;

/**
 * <p>
 * 描述：批量放号响应对象
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 友人帐
 * @version 1.0.0
 */
@Data
@Schema(description = "批量放号响应数据")
public class SlotBatchReleaseVO {
    @Schema(description = "新生成的号源时段id集合")
    private List<String> slotIds;

    @Schema(description = "成功放号数量", example = "1")
    private Integer generatedCount;
}
