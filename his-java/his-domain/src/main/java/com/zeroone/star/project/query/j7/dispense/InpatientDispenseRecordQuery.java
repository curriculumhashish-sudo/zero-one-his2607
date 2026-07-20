package com.zeroone.star.project.query.j7.dispense;

import com.zeroone.star.project.query.PageQuery;
import io.swagger.v3.oas.annotations.media.Schema;
import jakarta.validation.constraints.Size;
import lombok.Data;
import lombok.EqualsAndHashCode;
import org.springframework.format.annotation.DateTimeFormat;

import java.time.LocalDate;

/**
 * <p>
 * 描述：住院发退药记录查询条件
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhishixuebao
 * @version 1.0.0
 */
@Data
@EqualsAndHashCode(callSuper = true)
@Schema(description = "住院发退药记录查询条件；导出接口复用筛选字段并忽略分页参数")
public class InpatientDispenseRecordQuery extends PageQuery {

    @Size(max = 64, message = "病区ID长度不能超过64个字符")
    @Schema(description = "病区ID", example = "322963602326687744")
    private String wardId;

    @Size(max = 100, message = "患者关键字长度不能超过100个字符")
    @Schema(description = "患者关键字，支持患者姓名或住院号", example = "张三")
    private String patientKeyword;

    @Size(max = 64, message = "床号长度不能超过64个字符")
    @Schema(description = "床号", example = "08-12")
    private String bedNo;

    @Size(max = 64, message = "发退药类型长度不能超过64个字符")
    @Schema(description = "发退药类型枚举值；为空时查询全部类型", example = "DISPENSE")
    private String dispenseType;

    @Size(max = 64, message = "记录状态长度不能超过64个字符")
    @Schema(description = "发退药记录状态枚举值", example = "COMPLETED")
    private String status;

    @DateTimeFormat(iso = DateTimeFormat.ISO.DATE)
    @Schema(description = "发退药开始日期，格式yyyy-MM-dd", example = "2026-07-01")
    private LocalDate startDate;

    @DateTimeFormat(iso = DateTimeFormat.ISO.DATE)
    @Schema(description = "发退药结束日期，格式yyyy-MM-dd", example = "2026-07-20")
    private LocalDate endDate;

    @Size(max = 100, message = "药品关键字长度不能超过100个字符")
    @Schema(description = "药品关键字，支持药品名称或商品名称", example = "阿莫西林")
    private String medicationKeyword;
}
