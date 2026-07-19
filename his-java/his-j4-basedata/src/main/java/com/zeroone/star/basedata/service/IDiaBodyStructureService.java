package com.zeroone.star.basedata.service;

import com.zeroone.star.project.dto.j4.diagnosis.BodyStructureDTO;
import com.zeroone.star.project.vo.j4.diagnosis.BodyStructureTreeNodeVO;

import java.util.List;

/**
 * <p>
 * 描述：身体结构目录服务接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
public interface IDiaBodyStructureService {

    /**
     * 构建身体结构树
     * @return 树节点列表
     */
    List<BodyStructureTreeNodeVO> buildTree();

    /**
     * 新增身体结构节点
     * @param dto 节点数据
     * @return 新增节点 ID
     */
    String saveNode(BodyStructureDTO dto);

    /**
     * 编辑身体结构节点
     * @param dto 节点数据
     */
    void modifyNode(BodyStructureDTO dto);

    /**
     * 删除身体结构节点
     * @param id 节点 ID
     */
    void deleteNode(String id);
}
