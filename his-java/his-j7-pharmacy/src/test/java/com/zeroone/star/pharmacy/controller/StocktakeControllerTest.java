package com.zeroone.star.pharmacy.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.datatype.jsr310.JavaTimeModule;
import com.baomidou.mybatisplus.extension.service.impl.ServiceImpl;
import com.zeroone.star.pharmacy.entity.WkfSupplyRequest;
import com.zeroone.star.pharmacy.mapper.WkfSupplyRequestMapper;
import com.zeroone.star.pharmacy.service.IStocktakeService;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeItemSaveDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSaveDTO;
import com.zeroone.star.project.dto.stocktake.StocktakeSubmitDTO;
import com.zeroone.star.project.query.stocktake.StocktakeQuery;
import com.zeroone.star.project.vo.stocktake.StocktakeDetailVO;
import com.zeroone.star.project.vo.stocktake.StocktakeListVO;
import com.zeroone.star.project.vo.stocktake.StocktakeSaveVO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.http.converter.json.MappingJackson2HttpMessageConverter;
import org.springframework.http.MediaType;
import org.springframework.test.util.ReflectionTestUtils;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

import java.math.BigDecimal;
import java.time.LocalDateTime;
import java.util.List;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

class StocktakeControllerTest {

    private MockMvc mockMvc;

    private ObjectMapper objectMapper;

    private FakeStocktakeService stocktakeService;

    @BeforeEach
    void setUp() {
        objectMapper = new ObjectMapper();
        objectMapper.registerModule(new JavaTimeModule());
        stocktakeService = new FakeStocktakeService();

        StocktakeController controller = new StocktakeController();
        ReflectionTestUtils.setField(controller, "stocktakeService", stocktakeService);
        mockMvc = MockMvcBuilders.standaloneSetup(controller)
                .setMessageConverters(new MappingJackson2HttpMessageConverter(objectMapper))
                .build();
    }

    @Test
    void addStocktakeShouldReturnDraft() throws Exception {
        stocktakeService.stocktakeSaveVO = buildSaveVO("PST202608010001", "DRAFT", 0, BigDecimal.ZERO);

        mockMvc.perform(get("/api/inventory-check/new"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data.busNo").value("PST202608010001"))
                .andExpect(jsonPath("$.data.statusEnum").value("DRAFT"))
                .andExpect(jsonPath("$.data.itemCount").value(0));

        assertEquals("addStocktake", stocktakeService.calledMethod);
    }

    @Test
    void saveStocktakeShouldReturnDraft() throws Exception {
        stocktakeService.stocktakeSaveVO = buildSaveVO("PST202608010002", "DRAFT", 1, new BigDecimal("1225.00"));

        mockMvc.perform(post("/api/inventory-check/save")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(buildSaveDTO("PST202608010002"))))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data.busNo").value("PST202608010002"))
                .andExpect(jsonPath("$.data.statusEnum").value("DRAFT"))
                .andExpect(jsonPath("$.data.itemCount").value(1))
                .andExpect(jsonPath("$.data.totalPrice").value(1225.00));

        assertEquals("saveStocktake", stocktakeService.calledMethod);
    }

    @Test
    void saveAndSubmitStocktakeShouldReturnPendingApproval() throws Exception {
        stocktakeService.stocktakeSaveVO = buildSaveVO("PST202608010003", "PENDING_APPROVAL", 1, new BigDecimal("1225.00"));

        mockMvc.perform(post("/api/inventory-check/save-submit")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(buildSaveDTO("PST202608010003"))))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data.busNo").value("PST202608010003"))
                .andExpect(jsonPath("$.data.statusEnum").value("PENDING_APPROVAL"))
                .andExpect(jsonPath("$.data.itemCount").value(1))
                .andExpect(jsonPath("$.data.totalPrice").value(1225.00));

        assertEquals("saveAndSubmitStocktake", stocktakeService.calledMethod);
    }

    private StocktakeSaveDTO buildSaveDTO(String busNo) {
        StocktakeItemSaveDTO item = new StocktakeItemSaveDTO();
        item.setItemId("322998385762111488");
        item.setBookQuantity(new BigDecimal("100.00"));
        item.setTotalQuantity(new BigDecimal("98.00"));
        item.setItemQuantity(new BigDecimal("-2.00"));
        item.setUnitCode("box");
        item.setPrice(new BigDecimal("12.50"));
        item.setTotalPrice(new BigDecimal("1225.00"));
        item.setProfitLossPrice(new BigDecimal("-25.00"));

        StocktakeSaveDTO dto = new StocktakeSaveDTO();
        dto.setBusNo(busNo);
        dto.setWarehouseId("322954949549363200");
        dto.setRemark("月度盘点");
        dto.setItems(List.of(item));
        return dto;
    }

    private StocktakeSaveVO buildSaveVO(String busNo, String status, int itemCount, BigDecimal totalPrice) {
        StocktakeSaveVO vo = new StocktakeSaveVO();
        vo.setBusNo(busNo);
        vo.setStatusEnum(status);
        vo.setItemCount(itemCount);
        vo.setTotalPrice(totalPrice);
        vo.setOperateTime(LocalDateTime.of(2026, 8, 1, 10, 0));
        return vo;
    }

    private static class FakeStocktakeService
            extends ServiceImpl<WkfSupplyRequestMapper, WkfSupplyRequest>
            implements IStocktakeService {

        private StocktakeSaveVO stocktakeSaveVO;
        private String calledMethod;

        @Override
        public StocktakeSaveVO addStocktake() {
            calledMethod = "addStocktake";
            return stocktakeSaveVO;
        }

        @Override
        public PageDTO<StocktakeListVO> queryStocktakeList(StocktakeQuery query) {
            throw new UnsupportedOperationException("not used in this test");
        }

        @Override
        public StocktakeDetailVO queryStocktakeDetail(String id) {
            throw new UnsupportedOperationException("not used in this test");
        }

        @Override
        public StocktakeSaveVO saveStocktake(StocktakeSaveDTO dto) {
            calledMethod = "saveStocktake";
            return stocktakeSaveVO;
        }

        @Override
        public StocktakeSaveVO saveAndSubmitStocktake(StocktakeSaveDTO dto) {
            calledMethod = "saveAndSubmitStocktake";
            return stocktakeSaveVO;
        }

        @Override
        public String submitStocktake(StocktakeSubmitDTO dto) {
            throw new UnsupportedOperationException("not used in this test");
        }
    }
}
