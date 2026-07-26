package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartStatusDTO;

/**
 * 部位管理业务服务。
 */
public interface BodyPartService {

    void modifyBodyPart(BodyPartModifyDTO dto);

    void removeBodyPart(String id);

    void modifyBodyPartStatus(BodyPartStatusDTO dto);
}
