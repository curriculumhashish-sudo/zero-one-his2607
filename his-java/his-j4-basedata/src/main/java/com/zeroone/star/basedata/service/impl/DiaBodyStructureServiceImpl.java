package com.zeroone.star.basedata.service.impl;

import com.zeroone.star.basedata.service.IDiaBodyStructureService;
import com.zeroone.star.project.dto.j4.diagnosis.BodyStructureDTO;
import com.zeroone.star.project.vo.j4.diagnosis.BodyStructureTreeNodeVO;
import org.springframework.stereotype.Service;

import java.util.List;

/**
 * @Description:
 * @Created: with IntelliJ IDEA.
 * @author: yc
 * @createTime: 2026-07-23 19:32
 **/
@Service
public class DiaBodyStructureServiceImpl implements IDiaBodyStructureService {
    @Override
    public List<BodyStructureTreeNodeVO> buildTree() {
        return List.of();
    }

    @Override
    public String saveNode(BodyStructureDTO dto) {
        return "";
    }

    @Override
    public void modifyNode(BodyStructureDTO dto) {

    }

    @Override
    public void deleteNode(String id) {

    }
}
