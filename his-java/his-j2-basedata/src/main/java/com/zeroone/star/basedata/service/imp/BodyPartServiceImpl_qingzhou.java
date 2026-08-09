package com.zeroone.star.basedata.service.imp;

import cn.hutool.core.lang.UUID;
import com.zeroone.star.basedata.mapper.BodyPartMapper;
import com.zeroone.star.basedata.service.IBodyPartService;
import com.zeroone.star.project.doo.BodyPartDO;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;

@Service
@RequiredArgsConstructor
public class BodyPartServiceImpl_qingzhou implements IBodyPartService {

    private final BodyPartMapper mapper;

    @Override
    public List<BodyPartTreeVO> getTree() {
        return mapper.selectTree();
    }

    @Override
    public PageDTO<BodyPartListVO> getList(BodyPartPageQuery query) {
        Long total = mapper.selectListCount(query);
        if (total == 0) {
            return new PageDTO<>(
                    (long) query.getPageIndex(),
                    (long) query.getPageSize(),
                    0L, 0L,
                    new ArrayList<>()
            );
        }
        int offset = (query.getPageIndex() - 1) * query.getPageSize();
        int limit = query.getPageSize();
        List<BodyPartListVO> rows = mapper.selectList(query, offset, limit);
        long pages = (total + limit - 1) / limit;
        return new PageDTO<>(
                (long) query.getPageIndex(),
                (long) query.getPageSize(),
                total, pages, rows
        );
    }

    @Override
    public String add(BodyPartAddDTO dto) {
        BodyPartDO doObj = new BodyPartDO();
        String id = UUID.fastUUID().toString(true);
        doObj.setId(id);
        doObj.setBusNo("BS" + System.currentTimeMillis());
        doObj.setName(dto.getPartName());
        doObj.setParentId(
                dto.getParentId() == null || dto.getParentId().isEmpty()
                        ? null : dto.getParentId()
        );
        doObj.setTenantId("1");
        doObj.setStatusEnum("ACTIVE");
        doObj.setIsDeleted(0);
        doObj.setVersion(0);
        LocalDateTime now = LocalDateTime.now();
        doObj.setCreatedAt(now);
        doObj.setUpdatedAt(now);
        doObj.setCreatedBy("1");
        doObj.setUpdatedBy("1");
        mapper.insert(doObj);
        return id;
    }
}