package com.zeroone.star.project.query.j7.dispense;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

/**
 * <p>
 * 描述：住院发药驳回查询对象
 * </p>
 * <p>
 * 继承 DispensingSummaryQuery，新增驳回原因字段，
 * 用于住院发药驳回操作。
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 01star
 * @version 1.0.0
 */
@Getter
@Setter
@ToString(callSuper = true)
@Schema(description = "住院发药驳回查询对象")
public class DispenseRejectQuery extends DispensingSummaryQuery {

    @Schema(description = "驳回原因")
    private String rejectReason;
}