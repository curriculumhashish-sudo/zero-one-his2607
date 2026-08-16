package com.zeroone.star.project.assistant;

import com.zeroone.star.project.dto.assistant.GeneralAssistantChatDTO;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.assistant.AssistantCapabilityVO;
import com.zeroone.star.project.vo.assistant.GeneralAssistantChatVO;
import jakarta.validation.Valid;
import jakarta.validation.constraints.Pattern;

import java.util.List;

/**
 * <p>
 * 描述：通用助手 API 接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 01star
 * @version 1.0.0
 */
public interface GeneralAssistantApis {

    /**
     * 识别用户意图并路由、调度相应的垂直助手。
     *
     * @param dto 会话请求
     * @return 助手答复与路由轨迹
     */
    JsonVO<GeneralAssistantChatVO> chat(@Valid GeneralAssistantChatDTO dto);

    /**
     * 获取通用助手当前可路由的能力。
     *
     * @return 助手能力列表
     */
    JsonVO<List<AssistantCapabilityVO>> listCapabilities();

    /**
     * 清理指定会话的上下文。
     *
     * @param conversationId 会话ID
     * @return 是否清理成功
     */
    JsonVO<Boolean> clearConversation(
            @Pattern(regexp = "^[A-Za-z0-9_-]{1,64}$", message = "会话ID格式不正确") String conversationId);
}
