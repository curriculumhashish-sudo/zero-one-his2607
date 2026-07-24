package com.zeroone.star.mongo.mapper;

import com.zeroone.star.mongo.entity.MemberReadHistory;
import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.List;

/**
 * <p>
 * 描述：会员浏览历史记录Mapper
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
public interface MemberReadHistoryMapper extends MongoRepository<MemberReadHistory, String> {
    /**
     * 根据会员ID查询会员浏览历史记录
     * @param memberId 会员ID
     * @return 会员浏览历史记录列表
     */
    List<MemberReadHistory> findByMemberIdOrderByCreateTimeDesc(Long memberId);
}
