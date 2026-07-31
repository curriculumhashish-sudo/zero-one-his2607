package com.zeroone.star.mongo.service;

import com.zeroone.star.project.dto.mongo.MemberReadHistoryDTO;
import com.zeroone.star.project.vo.mongo.MemberReadHistoryVO;

import java.util.List;

/**
 * <p>
 * 描述：会员浏览历史记录服务
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface MemberReadHistoryService {
    /**
     * 保存会员浏览历史记录
     * @param history 会员浏览历史记录DTO
     * @return 受影响的行数
     */
    int save(MemberReadHistoryDTO history);

    /**
     * 批量删除会员浏览历史记录
     * @param ids 会员ID列表
     * @return 受影响的行数
     */
    int remove(List<String> ids);

    /**
     * 根据会员ID查询会员浏览历史记录
     * @param memberId 会员ID
     * @return 会员浏览历史记录列表
     */
    List<MemberReadHistoryVO> list(Long memberId);
}
