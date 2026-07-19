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

@RequestMapping("/pharmacy/inbound-receipt")
public interface PharmacyApis {

    @GetMapping("/list")
    JsonVO<List<InboundReceiptListVO>> queryAllInboundReceipt(@Valid InboundReceiptQuery query);

    @GetMapping("/detail")
    JsonVO<InboundReceiptDetailVO> getInboundReceiptDetail(@Valid InboundReceiptDetailQuery query);

    @DeleteMapping("/remove")
    JsonVO<String> removeInboundReceipt(@Valid @RequestBody InboundReceiptDeleteDTO dto);
}