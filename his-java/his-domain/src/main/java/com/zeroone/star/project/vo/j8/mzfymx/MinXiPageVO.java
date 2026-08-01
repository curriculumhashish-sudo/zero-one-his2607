package com.zeroone.star.project.vo.j8.mzfymx;

import com.zeroone.star.project.dto.j8.mzfymx.MinXiDTO;
import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：门诊费用明细-分页查询接口返回 VO（方案 B 纯平铺）
 *      = rows（MinXiDTO 多行列表，1行=1条收费项）
 *      + totalRowCount（明细总行数，全量 SUM，不带 LIMIT）
 *      + pageNo / pageSize（前端分页栏高亮用，返回入参原值）
 *      + 8 个 *Sum 金额合计（全量 SUM，不带 LIMIT，对应原型图顶部红色数字）
 *      对应 MinXiApis#getMinXiList 接口返回值
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Data
@Schema(description = "门诊费用明细-分页查询返回结构（纯平铺）")
public class MinXiPageVO {

    @Schema(description = "分页明细行列表（每行 = 1 条收费项，同一次就诊的就诊字段每行重复）")
    private List<MinXiDTO> rows = new ArrayList<>();

    @Schema(description = "满足条件的明细总行数（全量，不含 LIMIT，用于前端分页栏显示总条数）")
    private Long totalRowCount = 0L;

    // ============================================
    // 分页栏高亮用（你要求新增返回，= 入参 pageIndex / pageSize 原值，不动 PageQuery 父类）
    // ============================================
    @Schema(description = "当前页码（= 入参 pageIndex 原值，前端分页栏高亮当前页用）")
    private Long pageNo;

    @Schema(description = "每页大小（= 入参 pageSize 原值，前端分页栏下拉高亮用）")
    private Long pageSize;

    // ============================================
    //  全量合计（原型图顶部红色 8 个数字，SUM 全量数据，不带 LIMIT）
    // ============================================
    @Schema(description = "原价合计（原型顶部：原价合计 0）")
    private BigDecimal baseAmountSum = BigDecimal.ZERO;

    @Schema(description = "优惠合计（原型顶部：优惠合计 0）")
    private BigDecimal discountAmountSum = BigDecimal.ZERO;

    @Schema(description = "应收合计（原型顶部：应收合计 0，= baseAmountSum - discountAmountSum 内存计算）")
    private BigDecimal receivableAmountSum = BigDecimal.ZERO;

    @Schema(description = "折后合计（原型顶部：折后合计 0）")
    private BigDecimal discountedAmountSum = BigDecimal.ZERO;

    @Schema(description = "行金额合计（原型顶部：行金额合计 0）")
    private BigDecimal lineAmountSum = BigDecimal.ZERO;

    @Schema(description = "实收合计（原型顶部：实收合计 0）")
    private BigDecimal actualReceivedAmountSum = BigDecimal.ZERO;

    @Schema(description = "医保合计（原型顶部：医保合计 0）")
    private BigDecimal insuranceAmountSum = BigDecimal.ZERO;

    @Schema(description = "自费合计（原型顶部：自费合计 0，= lineAmountSum - insuranceAmountSum 内存计算）")
    private BigDecimal selfPayAmountSum = BigDecimal.ZERO;
}