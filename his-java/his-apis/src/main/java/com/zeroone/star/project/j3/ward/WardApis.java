package com.zeroone.star.project.j3.ward;

import com.zeroone.star.project.dto.j3.ward.BedAddDTO;
import com.zeroone.star.project.dto.j3.ward.BedDTO;
import com.zeroone.star.project.dto.j3.ward.RoomAddDTO;
import com.zeroone.star.project.dto.j3.ward.RoomDTO;
import com.zeroone.star.project.dto.j3.ward.WardAddDTO;
import com.zeroone.star.project.dto.j3.ward.WardDTO;
import com.zeroone.star.project.query.j3.ward.WardTreeQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j3.ward.BedOptionVO;
import com.zeroone.star.project.vo.j3.ward.OptionVO;
import com.zeroone.star.project.vo.j3.ward.WardTreeVO;

import java.util.List;

/**
 * <p>
 * 描述：病区病房接口
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author zhujian
 * @version 1.0.0
 */
public interface WardApis {

    /** ==================== 树形结构 ==================== */

    /**
     * 获取病区-病房-床位树形列表
     *
     * @param query 树形查询条件（按机构名称筛选）
     * @return 树形结构数据
     */
    JsonVO<List<WardTreeVO>> getWardTree(WardTreeQuery query);

    /** ==================== 病区管理 ==================== */

    /**
     * 获取病区列表（用于下拉框）
     *
     * @return 病区下拉选项列表
     */
    JsonVO<List<OptionVO>> getWardList();

    /**
     * 添加病区
     *
     * @param wardAddDTO 病区添加数据
     * @return 添加结果
     */
    JsonVO<String> addWard(WardAddDTO wardAddDTO);

    /**
     * 修改病区
     *
     * @param id      病区ID
     * @param wardDTO 病区数据
     * @return 修改结果
     */
    JsonVO<String> updateWard(String id, WardDTO wardDTO);

    /**
     * 删除病区
     *
     * @param id 病区ID
     * @return 删除结果
     */
    JsonVO<String> deleteWard(String id);

    /** ==================== 病房管理 ==================== */

    /**
     * 根据病区获取病房列表（用于下拉框）
     *
     * @param wardId 病区ID
     * @return 病房下拉选项列表
     */
    JsonVO<List<OptionVO>> getRoomListByWard(String wardId);

    /**
     * 添加病房
     *
     * @param roomAddDTO 病房添加数据
     * @return 添加结果
     */
    JsonVO<String> addRoom(RoomAddDTO roomAddDTO);

    /**
     * 修改病房
     *
     * @param id      病房ID
     * @param roomDTO 病房数据
     * @return 修改结果
     */
    JsonVO<String> updateRoom(String id, RoomDTO roomDTO);

    /**
     * 删除病房
     *
     * @param id 病房ID
     * @return 删除结果
     */
    JsonVO<String> deleteRoom(String id);

    /** ==================== 床位管理 ==================== */

    /**
     * 根据病房获取病床列表
     *
     * @param roomId 病房ID
     * @return 病床列表（含名称、唯一标识、状态）
     */
    JsonVO<List<BedOptionVO>> getBedListByRoom(String roomId);

    /**
     * 添加床位
     *
     * @param bedAddDTO 床位添加数据
     * @return 添加结果
     */
    JsonVO<String> addBed(BedAddDTO bedAddDTO);

    /**
     * 修改床位
     *
     * @param id     床位ID
     * @param bedDTO 床位数据
     * @return 修改结果
     */
    JsonVO<String> updateBed(String id, BedDTO bedDTO);

    /**
     * 删除床位
     *
     * @param id 床位ID
     * @return 删除结果
     */
    JsonVO<String> deleteBed(String id);
}
