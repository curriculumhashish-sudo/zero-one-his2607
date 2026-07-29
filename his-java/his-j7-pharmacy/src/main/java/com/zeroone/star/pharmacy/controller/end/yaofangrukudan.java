package com.zeroone.star.pharmacy.controller.end;

import com.zeroone.star.pharmacy.service.InStockOrderService;
import com.zeroone.star.project.dto.j7.pharmacy.InStockOrderDTO;
import com.zeroone.star.project.j7.PharmacyApis;
import com.zeroone.star.project.vo.JsonVO;
import io.swagger.v3.oas.annotations.Operation;
import jakarta.annotation.Resource;
import org.springframework.web.bind.annotation.*;


@RestController
public class yaofangrukudan implements PharmacyApis {


    @Resource
    private InStockOrderService service;
    // 新增入库单据
    @Operation(summary = "新增入库单据", description = "创建一张新的入库单，状态为草稿。返回入库单主表ID。")
    @PostMapping("/addInStockOrder")
    public JsonVO<Long> addInOrder(@RequestBody InStockOrderDTO dto) {
        Long orderId = service.create(dto);
        return JsonVO.success(orderId);
    }
    // 保存入库单据（编辑）
    @Operation(summary = "保存入库单据", description = "修改草稿态的入库单，支持对明细的全量增删改。")
    @PutMapping("/{orderId}/saveInStockOrder")
    public JsonVO<String> updateInOrder(@PathVariable Long orderId, @RequestBody InStockOrderDTO dto) {
        service.update(orderId, dto);
        return JsonVO.success("保存入库单据成功");
    }
    // 保存并提交审批
    @Operation(summary = "保存并提交审批", description = "保存草稿并立即提交审批，状态变更为“待审批”。")
    @PutMapping("/{orderId}/submitInStockOrder")
    public JsonVO<String> submitInOrder(@PathVariable Long orderId, @RequestBody InStockOrderDTO dto) {
        service.submit(orderId, dto);
        return JsonVO.success("保存并提交审批成功");
    }
}
}
