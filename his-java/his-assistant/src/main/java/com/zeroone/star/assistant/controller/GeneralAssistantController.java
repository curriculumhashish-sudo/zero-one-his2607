package com.zeroone.star.assistant.controller;

import com.zeroone.star.assistant.service.IGeneralAssistantService;
import com.zeroone.star.project.assistant.GeneralAssistantApis;
import com.zeroone.star.project.dto.assistant.GeneralAssistantChatDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.assistant.AssistantCapabilityVO;
import com.zeroone.star.project.vo.assistant.GeneralAssistantChatVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Pattern;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * 通用助手控制器。
 */
@RestController
@Validated
@RequestMapping("/general-assistant")
@Tag(name = "通用助手", description = "HIS 智能总控、意图路由与多助手组合调度")
public class GeneralAssistantController implements GeneralAssistantApis {

    private final IGeneralAssistantService assistantService;

    public GeneralAssistantController(IGeneralAssistantService assistantService) {
        this.assistantService = assistantService;
    }

    @Override
    @PostMapping("/chat")
    @Operation(summary = "与通用助手对话", description = "识别业务意图，路由或组合调度垂直助手，并返回执行轨迹")
    public JsonVO<GeneralAssistantChatVO> chat(@Valid @RequestBody GeneralAssistantChatDTO dto) {
        return JsonVO.success(assistantService.chat(dto));
    }

    @Override
    @GetMapping("/list-capabilities")
    @Operation(summary = "获取助手能力", description = "获取通用助手当前可路由的全部垂直助手能力")
    public JsonVO<List<AssistantCapabilityVO>> listCapabilities() {
        return JsonVO.success(assistantService.listCapabilities());
    }

    @Override
    @DeleteMapping("/clear-conversation/{conversationId}")
    @Operation(summary = "清除会话", description = "清除通用助手及各垂直助手保存的会话上下文")
    public JsonVO<Boolean> clearConversation(
            @PathVariable
            @Pattern(regexp = "^[A-Za-z0-9_-]{1,64}$", message = "会话ID格式不正确") String conversationId) {
        assistantService.clearConversation(conversationId);
        return JsonVO.success(true);
    }
}
