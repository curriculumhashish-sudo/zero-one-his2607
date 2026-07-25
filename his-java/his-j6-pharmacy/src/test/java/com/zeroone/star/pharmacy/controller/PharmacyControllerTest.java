package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeAgreeApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeRejectApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeSubmitApprovalDTO;
import com.zeroone.star.project.dto.j6.pharmacy.stocktake.StocktakeWithdrawApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.http.MediaType;
import org.springframework.test.util.ReflectionTestUtils;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.validation.beanvalidation.LocalValidatorFactoryBean;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

class PharmacyControllerTest {
    private MockMvc mockMvc;
    private IStocktakeApprovalService stocktakeApprovalService;

    @BeforeEach
    void setUp() {
        WarehouseStocktakeController controller = new WarehouseStocktakeController();
        stocktakeApprovalService = mock(IStocktakeApprovalService.class);
        ReflectionTestUtils.setField(controller, "stocktakeApprovalService", stocktakeApprovalService);
        LocalValidatorFactoryBean validator = new LocalValidatorFactoryBean();
        validator.afterPropertiesSet();
        mockMvc = MockMvcBuilders.standaloneSetup(controller)
                .setValidator(validator)
                .build();
    }

    @Test
    void submitApprovalEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.submit(any(StocktakeSubmitApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/stocktake/submit-approval")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void withdrawApprovalEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.withdraw(any(StocktakeWithdrawApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/stocktake/withdraw-approval")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void approveEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.agree(any(StocktakeAgreeApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/stocktake/approve")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\",\"status\":\"AGREE\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void rejectEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.reject(any(StocktakeRejectApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/stocktake/reject")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\",\"changeReason\":\"inventory difference\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void submitApprovalEndpointRejectsBlankBusNo() throws Exception {
        mockMvc.perform(post("/stocktake/submit-approval")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"\"}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void approveEndpointRejectsMissingStatus() throws Exception {
        mockMvc.perform(post("/stocktake/approve")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isBadRequest());
    }

    @Test
    void rejectEndpointRejectsMissingReason() throws Exception {
        mockMvc.perform(post("/stocktake/reject")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isBadRequest());
    }
}
