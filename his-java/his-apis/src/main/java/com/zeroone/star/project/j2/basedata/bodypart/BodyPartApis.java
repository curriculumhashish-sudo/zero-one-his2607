package com.zeroone.star.project.j2.basedata.bodypart;

import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartAddDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartModifyDTO;
import com.zeroone.star.project.dto.j2.basedata.bodypartdto.BodyPartStatusDTO;
import com.zeroone.star.project.query.j2.basedata.BodyPartPageQuery;
import com.zeroone.star.project.vo.JsonVO;

import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartListVO;
import com.zeroone.star.project.vo.j2.basedata.bodypart.BodyPartTreeVO;


import java.util.List;

/**
 * <p>
 * 描述：部位管理接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 *
 * @author 吴志洲
 * @version 1.0.0
 */
public interface BodyPartApis {

    /**
     * 获取部位名称树。
     *
     * @return 部位名称树
     */
    JsonVO<List<BodyPartTreeVO>> getTree();

    /**
     * 条件分页获取部位列表。
     *
     * @param query 查询条件
     * @return 部位分页列表
     */
    JsonVO<PageDTO<BodyPartListVO>> getList(BodyPartPageQuery query);

    /**
     * 添加部位。
     *
     * @param dto 添加部位数据
     * @return 新增部位ID
     */
    JsonVO<String> add(BodyPartAddDTO dto);

    /**
     * 修改部位。
     *
     * @param dto 修改部位数据
     * @return 修改结果
     */
    JsonVO<Void> modifyBodyPart(BodyPartModifyDTO dto);

    /**
     * 删除部位。
     *
     * @param id 部位ID
     * @return 删除结果
     */
    JsonVO<Void> removeBodyPart(String id);

    /**
     * 启用或停用部位。
     *
     * @param dto 启停用部位数据
     * @return 修改结果
     */
    JsonVO<Void> modifyBodyPartStatus(BodyPartStatusDTO dto);
}
