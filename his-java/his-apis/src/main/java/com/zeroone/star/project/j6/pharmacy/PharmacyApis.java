package com.zeroone.star.project.j6.pharmacy;

import com.zeroone.star.project.dto.j6.pharmacy.InboundReceiptDeleteDTO;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptDetailQuery;
import com.zeroone.star.project.query.j6.pharmacy.InboundReceiptQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptDetailVO;
import com.zeroone.star.project.vo.j6.pharmacy.InboundReceiptListVO;
import jakarta.validation.Valid;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * <p>
 * 描述：药库管理 — 进货单的各种操作
 * </p>
 * @author BMXB
 * @version 1.0.0
 */
@RequestMapping("/pharmacy/inbound-receipt")
public interface PharmacyApis {


    /**
     * 展示表单全部信息
     * 展示出所有待审批或者正在审批或是已完成审批的表单信息
     * @param query 查询条件
     * @return 所有进货单列表
     */
    @GetMapping("/list")
    JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query);

    /**
     * 展示选中表单的全部信息
     * 可以查看所有表单选中之后的列表
     * @param query 表单详情
     * @return 具体的表单数值
     */
    @GetMapping("/detail")
    JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query);

    /**
     * 删除表单
     * @param dto 选中表单的字段
     * @return 删除是否成功
     */
    @DeleteMapping("/remove")
    JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto);
}