package com.zeroone.star.mongo.service.impl;

import cn.hutool.core.bean.BeanUtil;
import com.zeroone.star.mongo.entity.MemberReadHistory;
import com.zeroone.star.mongo.mapper.MemberReadHistoryMapper;
import com.zeroone.star.mongo.service.MemberReadHistoryService;
import com.zeroone.star.project.dto.mongo.MemberReadHistoryDTO;
import com.zeroone.star.project.vo.mongo.MemberReadHistoryVO;
import jakarta.annotation.Resource;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * <p>
 * 描述：会员浏览历史记录服务实现类
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author 阿伟学长
 * @version 1.0.0
 */
@Service
public class MemberReadHistoryServiceImpl implements MemberReadHistoryService {
    @Resource
    MemberReadHistoryMapper mapper;

    @Override
    public int save(MemberReadHistoryDTO history) {
        MemberReadHistory hdo = BeanUtil.toBean(history, MemberReadHistory.class);
        hdo.setCreateTime(new Date());
        mapper.save(hdo);
        return 1;
    }

    @Override
    public int remove(List<String> ids) {
        List<MemberReadHistory> delIds = new ArrayList<>(ids.size());
        ids.forEach(id -> {
            MemberReadHistory history = new MemberReadHistory();
            history.setId(id);
            delIds.add(history);
        });
        mapper.deleteAll(delIds);
        return delIds.size();
    }

    @Override
    public List<MemberReadHistoryVO> list(Long memberId) {
        List<MemberReadHistory> res = mapper.findByMemberIdOrderByCreateTimeDesc(memberId);
        return BeanUtil.copyToList(res, MemberReadHistoryVO.class);
    }
}
