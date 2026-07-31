package com.zeroone.star.pharmacy.controller;

import com.baomidou.mybatisplus.core.metadata.IPage;
import com.baomidou.mybatisplus.extension.plugins.pagination.Page;
import com.zeroone.star.pharmacy.service.IWkfInventoryItemService;
import com.zeroone.star.pharmacy.service.InventoryDetailService;
import com.zeroone.star.project.dto.j6.pharmacy.inventory.InventoryStatusDTO;
import com.zeroone.star.project.j6.pharmacy.WarehouseInventoryDetailApis;
import com.zeroone.star.project.query.j6.pharmacy.inventory.InventoryDetailQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.ExcelExportVO;
import com.zeroone.star.project.vo.j6.pharmacy.inventory.InventoryDetailVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Content;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponse;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.servlet.http.HttpServletResponse;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import java.io.IOException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

@Controller
@RequestMapping("/inventory")
@RequiredArgsConstructor
@Tag(name = "库存明细", description = "药库管理相关接口")
public class WarehouseInventoryDetailController implements WarehouseInventoryDetailApis {

    @Autowired
    InventoryDetailService inventoryDetailService;

    @Autowired
    IWkfInventoryItemService iWkfInventoryItemService;

    /**
     * 获取库存明细列表(分页+条件)
     */
    @PostMapping("/pageInventoryDetailCurrent")
    @ResponseBody
    @Operation(
            summary = "获取库存明细列表（条件+分页）",
            description = "携带筛选条件分页查询库存明细数据",
            responses = @ApiResponse(responseCode = "200", description = "查询成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<IPage<InventoryDetailVO>> pageInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
        Page<InventoryDetailVO> inventoryDetailVOPage = inventoryDetailService.pageInventoryDetailCurrent(inventoryDetailQueryDTO);
        return JsonVO.success(inventoryDetailVOPage);
    }

    /**
     * 停供/取消停供
     */
    @PostMapping("/resumeInventoryItems")
    @ResponseBody
    @Operation(
            summary = "停供/取消停供",
            description = "批量修改商品停供状态，返回受影响行数",
            responses = @ApiResponse(responseCode = "200", description = "操作成功",
                    content = @Content(schema = @Schema(implementation = JsonVO.class)))
    )
    public JsonVO<Integer> updateStatus(@RequestBody InventoryStatusDTO inventoryStatusDTO) {
        int row = iWkfInventoryItemService.resumeInventoryItems(inventoryStatusDTO);
        return JsonVO.success(row);
    }



    @PostMapping("/exportInventoryDetailCurrent")
    @Operation(
            summary = "导出数据",
            description = "根据筛选条件生成Excel文件，返回base64文件流",
            responses = @ApiResponse(responseCode = "200", description = "导出成功")
    )
    @ResponseBody
    public void exportInventoryDetailCurrent(@RequestBody InventoryDetailQuery query,
                                             HttpServletResponse response) throws IOException {
        ExcelExportVO vo = inventoryDetailService.exportToExcel(query);
        byte[] bytes = Base64.getDecoder().decode(vo.getBody());

        response.setContentType(vo.getContentType());
        response.setHeader("Content-Disposition",
                "attachment;filename=" + URLEncoder.encode(vo.getFileName(), StandardCharsets.UTF_8));
        response.setContentLength(bytes.length);
        response.getOutputStream().write(bytes);
        response.getOutputStream().flush();
    }


    // 药库管理模块中库存明细业务产品原型修改, 以下方案废除
    /*
      导出库存明细Excel
     */
    /*
     *
     *     @PostMapping("/exportInventoryDetailCurrent")
     *     @Operation(
     *             summary = "导出数据",
     *             description = "根据筛选条件生成Excel文件，返回base64文件信息",
     *             responses = @ApiResponse(responseCode = "200", description = "导出成功",
     *                     content = @Content(schema = @Schema(implementation = JsonVO.class)))
     *     )
     *     public JsonVO<ExcelExportVO> exportInventoryDetailCurrent(@RequestBody InventoryDetailQuery inventoryDetailQueryDTO) {
     *         ExcelExportVO excelExportVO = inventoryDetailService.exportToExcel(inventoryDetailQueryDTO);
     *         return JsonVO.success(excelExportVO);
     *     }
     */
}
