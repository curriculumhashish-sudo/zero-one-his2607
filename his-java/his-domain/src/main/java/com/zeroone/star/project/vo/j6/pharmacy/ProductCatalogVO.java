package com.zeroone.star.project.vo.j6.pharmacy;

import io.swagger.v3.oas.annotations.media.Schema;
import lombok.Data;
import java.math.BigDecimal;
import java.util.List;

/**
 * <p>
 * 描述：药品品类信息VO
 * </p>
 * @author 01星球
 */
@Data
@Schema(description = "药品品类信息")
public class ProductCatalogVO {
    @Schema(description = "多单位包装列表")
    private List<UnitBandVO> unitBands;

    @Schema(description = "采购价")
    private BigDecimal purchasePrice;

    @Schema(description = "基准价格")
    private BigDecimal productBasePrice;

    @Schema(description = "最小单位编码")
    private String minUnitCode;

    @Schema(description = "默认采购单位编码")
    private String defaultPurchaseUnitCode;

    @Schema(description = "采购单位换算比例")
    private BigDecimal purchaseUnitRatioToSplit;

    @Schema(description = "商品名称", example = "黄芪")
    private String merchandiseName;

    @Schema(description = "分类编码")
    private String categoryCode;

    @Schema(description = "分类名称", example = "中草药")
    private String categoryCodeDictText;

    @Schema(description = "药品业务编号")
    private String productBusNo;

    @Schema(description = "体积/剂型描述")
    private String totalVolume;
}
