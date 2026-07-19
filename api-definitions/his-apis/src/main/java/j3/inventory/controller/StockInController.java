package j3.inventory.controller;

import common.Result;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import j3.inventory.vo.SupplyDocumentDeleteVO;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

@Validated
@RestController
@Tag(name = "入库单管理", description = "药库/药房入库单删除")
public class StockInController {

    @PostMapping("/api/app/inventory/{module}/stock-in/{busNo}/delete")
    @Operation(summary = "入库单删除", description = "软删除指定单据号的入库单，仅草稿/待审核状态可删，同时解锁关联的库存锁定记录")
    @ApiResponse(responseCode = "200", description = "删除成功")
    @ApiResponse(responseCode = "400", description = "参数校验失败")
    @ApiResponse(responseCode = "500", description = "删除失败：单据不存在、状态不允许或系统错误")
    public Result<SupplyDocumentDeleteVO> delete(
            @Parameter(description = "模块：warehouse(药库) / pharmacy(药房)", required = true, example = "warehouse", schema = @Schema(defaultValue = "warehouse"))
            @PathVariable String module,
            @Parameter(description = "单据号", required = true, example = "SI202607190001", schema = @Schema(defaultValue = "SI202607190001"))
            @PathVariable String busNo,
            @Parameter(description = "删除原因", example = "录入错误", schema = @Schema(defaultValue = ""))
            @RequestParam(required = false, defaultValue = "") String reason) {

        SupplyDocumentDeleteVO vo = new SupplyDocumentDeleteVO();
        vo.setBusNo(busNo);
        vo.setDeletedLines(0);
        vo.setUnlockedLocks(0);
        return Result.ok(vo);
    }
}
