package com.zeroone.star.project.dto.j7.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.Valid;
import jakarta.validation.constraints.NotBlank;
import jakarta.validation.constraints.NotEmpty;
import jakarta.validation.constraints.NotNull;
import jakarta.validation.constraints.Size;
import lombok.Getter;
import lombok.Setter;

/**
 * <p>
 * 描述：药房入库单数据传输对象
 * </p>
 * <p>
 * 功能说明：用于新增、编辑和提交审批入库单。继承自 InStockOrderCreateDTO，
 * 字段与父类完全一致。建议新代码直接使用 InStockOrderCreateDTO 或 InStockOrderSaveDTO。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 * @deprecated 建议使用 {@link InStockOrderCreateDTO}（新增）或 {@link InStockOrderSaveDTO}（保存/返回）
 */
@Getter
@Setter
@Schema(description = "入库单DTO")
@Deprecated
public class InStockOrderDTO extends InStockOrderCreateDTO {
}