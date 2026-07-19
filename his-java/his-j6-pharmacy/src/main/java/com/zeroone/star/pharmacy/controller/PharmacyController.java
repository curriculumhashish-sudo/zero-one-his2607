package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptDeleteDTO;
import com.zeroone.star.project.j6.pharmacy.PharmacyApis;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptListVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * <p>
 * 描述：药库相关接口实现
 * </p>
 * @author BMXB
 * @version 1.0.0
 */
@RestController
@RequestMapping("/pharmacy/inbound-receipt")
@Tag(name = "药库管理-进货单", description = "进货单列表查询、详情、删除")
public class PharmacyController implements PharmacyApis {

    @Override
    @Operation(summary = "获取进货单列表")
    public JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query) {
        // TODO 调用 Service 查询数据库，返回列表
        return null;
    }

    @Override
    @Operation(summary = "获取进货单详情")
    public JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query) {
        // TODO 调用 Service 查详情，返回头+明细行
        return null;
    }

    @Override
    @Operation(summary = "删除进货单")
    public JsonVO<String> removeInboundReceipt(@Valid InboundReceiptDeleteDTO dto) {
        // TODO 调用 Service 执行删除
        return null;
    }
}