package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.service.IStocktakeApprovalService;
import com.zeroone.star.project.dto.j6.pharmacy.StocktakeApprovalDTO;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.http.MediaType;
import org.springframework.test.util.ReflectionTestUtils;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;

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
        PharmacyController controller = new PharmacyController();
        stocktakeApprovalService = mock(IStocktakeApprovalService.class);
        ReflectionTestUtils.setField(controller, "stocktakeApprovalService", stocktakeApprovalService);
        mockMvc = MockMvcBuilders.standaloneSetup(controller).build();
    }

    @Test
    void submitApprovalEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.submit(any(StocktakeApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/warehouse/stocktake/submit-approval")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void withdrawApprovalEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.withdraw(any(StocktakeApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/warehouse/stocktake/withdraw-approval")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void approveEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.agree(any(StocktakeApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/warehouse/stocktake/approve")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }

    @Test
    void rejectEndpointReturnsSuccess() throws Exception {
        when(stocktakeApprovalService.reject(any(StocktakeApprovalDTO.class))).thenReturn(true);

        mockMvc.perform(post("/warehouse/stocktake/reject")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content("{\"busNo\":\"CST2026-001\",\"changeReason\":\"inventory difference\"}"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value(true));
    }
}
