package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.InventoryStatusDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.query.j6.pharmacy.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.ExcelExportVO;
import com.zeroone.star.project.vo.j6.pharmacy.InventoryDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class PharmacyController implements PharmacyApis {
    /**
     * 获取库存明细列表(分页+条件)
     */
    @PostMapping("/pageInventoryDetailCurrent")
    @Operation(
            summary = "分页条件查询库存明细",
            description = "携带筛选条件分页查询库存明细数据",
            responses = @ApiResponse(responseCode = "200", description = "查询成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<InventoryDetailVO> pageInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
        return JsonVO.fail("接口未完成");
    }

    /**
     * 停供/取消停供
     */
    @PostMapping("/resumeInventoryItems")
    @Operation(
            summary = "批量停供/取消停供商品",
            description = "批量修改商品停供状态，返回受影响行数",
            responses = @ApiResponse(responseCode = "200", description = "操作成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<Integer> updateStatus(@RequestBody InventoryStatusDTO inventoryStatusDTO) {
        return JsonVO.fail("接口未完成");
    }

    /**
     * 导出库存明细Excel
     */
    @PostMapping("/exportInventoryDetailCurrent")
    @Operation(
            summary = "按条件导出库存明细Excel",
            description = "根据筛选条件生成Excel文件，返回base64文件信息",
            responses = @ApiResponse(responseCode = "200", description = "导出成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<ExcelExportVO> exportInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
        return JsonVO.fail("接口未完成");
    }

    @GetMapping("/test")
    String test() {
        return "test";
    }
}
