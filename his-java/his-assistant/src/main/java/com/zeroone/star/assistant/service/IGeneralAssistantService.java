package com.zeroone.star.assistant.service;

import com.zeroone.star.project.dto.assistant.GeneralAssistantChatDTO;
import com.zeroone.star.project.vo.assistant.AssistantCapabilityVO;
import com.zeroone.star.project.vo.assistant.GeneralAssistantChatVO;

import java.util.List;

/**
 * 通用助手业务服务。
 */
public interface IGeneralAssistantService {

    GeneralAssistantChatVO chat(GeneralAssistantChatDTO dto);

    List<AssistantCapabilityVO> listCapabilities();

    void clearConversation(String conversationId);
}
