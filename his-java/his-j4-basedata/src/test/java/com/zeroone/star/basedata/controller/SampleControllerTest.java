package com.zeroone.star.basedata.controller;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.zeroone.star.project.query.j4.sample.SpecimenDefinitionQuery;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.http.MediaType;
import org.springframework.test.web.servlet.MockMvc;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.jsonPath;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

/**
 * 样本管理控制器测试类
 */
@WebMvcTest(SampleController.class)
class SampleControllerTest {

    @Autowired
    private MockMvc mockMvc;

    @Autowired
    private ObjectMapper objectMapper;

    /**
     * 测试获取样本名称接口
     */
    @Test
    void testGetSampleName() throws Exception {
        mockMvc.perform(get("/basedata/sample/get-sample-name")
                        .param("id", "328090253175296000"))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data.name").value("血"))
                .andExpect(jsonPath("$.data.busNo").value("SPD0001"))
                .andExpect(jsonPath("$.data.typeEnum").value("BLOOD"))
                .andExpect(jsonPath("$.data.statusEnum").value("ACTIVE"));
    }

    /**
     * 测试获取样本列表（条件+分页）接口
     */
    @Test
    void testQuerySampleList() throws Exception {
        SpecimenDefinitionQuery query = new SpecimenDefinitionQuery();
        query.setName("血");
        query.setPageIndex(1L);
        query.setPageSize(10L);

        mockMvc.perform(post("/basedata/sample/query-sample-list")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(objectMapper.writeValueAsString(query)))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data.rows[0].name").value("血"))
                .andExpect(jsonPath("$.data.pageIndex").value(1))
                .andExpect(jsonPath("$.data.pageSize").value(10))
                .andExpect(jsonPath("$.data.total").value(1));
    }

    /**
     * 测试新增样本定义接口
     */
    @Test
    void testAddSample() throws Exception {
        String json = "{"
                + "\"busNo\":\"SPD0002\","
                + "\"name\":\"尿\","
                + "\"pyStr\":\"N\","
                + "\"wbStr\":\"N\","
                + "\"typeEnum\":\"URINE\","
                + "\"statusEnum\":\"ACTIVE\""
                + "}";

        mockMvc.perform(post("/basedata/sample/add-sample")
                        .contentType(MediaType.APPLICATION_JSON)
                        .content(json))
                .andExpect(status().isOk())
                .andExpect(jsonPath("$.code").value(10000))
                .andExpect(jsonPath("$.data").value("新增样本定义成功"));
    }
}
