package com.zeroone.star.pharmacy.controller;

import com.zeroone.star.pharmacy.controller.end.yaofangrukudan;
import com.zeroone.star.project.dto.j7.pharmacy.InboundApprovalDTO;
import com.zeroone.star.project.vo.j7.pharmacy.InboundApprovalVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.media.Schema;
import io.swagger.v3.oas.annotations.responses.ApiResponses;
import org.junit.jupiter.api.Test;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;

import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Set;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertTrue;

/**
 * G7 入库单审批接口与 Swagger 文档契约测试。
 *
 * @author lvv
 * @version 1.0.0
 */
class InboundApprovalContractTest {

    @Test
    void controllerExposesExpectedBasePath() {
        RequestMapping mapping = yaofangrukudan.class.getAnnotation(RequestMapping.class);
        assertNotNull(mapping);
        assertEquals("/j7/inbound-approval", mapping.value()[0]);
    }

    @Test
    void allOperationsExposeDocumentedRoutes() throws NoSuchMethodException {
        assertOperation("withdrawApproval", "/withdraw", "撤回审批");
        assertOperation("approveApproval", "/approve", "审批通过");
        assertOperation("rejectApproval", "/reject", "驳回审批");
    }

    @Test
    void responseStatusDocumentsOfficialSupplyStatusValues() throws NoSuchFieldException {
        Schema schema = InboundApprovalVO.class.getDeclaredField("status").getAnnotation(Schema.class);

        assertNotNull(schema, "status 缺少 Swagger Schema");
        assertEquals(
                Set.of("WITHDRAWN", "APPROVED", "REJECTED"),
                Set.copyOf(Arrays.asList(schema.allowableValues()))
        );
    }

    private void assertOperation(String methodName, String path, String summary)
            throws NoSuchMethodException {
        Method method = yaofangrukudan.class.getMethod(methodName, InboundApprovalDTO.class);
        PostMapping mapping = method.getAnnotation(PostMapping.class);
        Operation operation = method.getAnnotation(Operation.class);
        ApiResponses responses = method.getAnnotation(ApiResponses.class);

        assertNotNull(mapping, methodName + " 缺少 PostMapping");
        assertNotNull(operation, methodName + " 缺少 Swagger Operation");
        assertNotNull(responses, methodName + " 缺少 Swagger ApiResponses");
        assertEquals(path, mapping.value()[0]);
        assertEquals(summary, operation.summary());
        assertTrue(operation.description().contains("PENDING_APPROVAL"));
        assertTrue(Arrays.stream(responses.value())
                .anyMatch(response -> "200".equals(response.responseCode())
                        && response.description().contains("10000")
                        && response.description().contains("9999")
                        && response.description().contains("9995")));
    }
}
