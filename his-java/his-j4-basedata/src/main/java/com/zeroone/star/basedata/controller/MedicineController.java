package com.zeroone.star.basedata.controller;


import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineUpdateDTO;
import com.zeroone.star.project.j4.basedata.MedicineApis;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineChangeStatusVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDeleteVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.tags.Tag;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.List;

/**
 * <p>
 * 描述：药品管理控制器
 * </p>
 * <p>版权：&copy;01星球</p>
 * <p>地址：01星球总部</p>
 * @author j4组
 * @version 1.0.0
 */
@RestController
@RequestMapping("/medicine")
@Tag(name = "药品管理", description = "药品管理")
@Validated
public class MedicineController implements MedicineApis {

    @PutMapping("/update-medicine")
    @Operation(summary = "药品更新")
    public JsonVO<String> updateMedicine(@Validated @RequestBody MedicineUpdateDTO dto){
        return JsonVO.success("更新成功");
    }


    @DeleteMapping("/delete-medicine")
    @Operation(summary = "药品删除（批量）")
    public JsonVO<MedicineDeleteVO> deleteMedicine(@Validated @RequestBody MedicineDeleteDTO dto){
        List<String> list = new ArrayList<>();
        list.add("1");
        return JsonVO.success(new MedicineDeleteVO(list));
    }

    @PutMapping("/enable-status")
    @Operation(summary = "启用药品(批量)")
    public JsonVO<MedicineChangeStatusVO> updateEnableMedicineStatus(@Validated @RequestBody MedicineChangeStatusDTO dto){
        //判断状态进行启用或者提示已启用，无需重复启用
        List<String> list = new ArrayList<>();
        list.add("1");
        return JsonVO.success(new MedicineChangeStatusVO(list));
    }

    @PutMapping("/disable-status")
    @Operation(summary = "停用药品(批量)")
    public JsonVO<MedicineChangeStatusVO> updateDisableMedicineStatus(@Validated @RequestBody MedicineChangeStatusDTO dto){
        //判断状态进行停用或者提示已停用，无需重复停用
        List<String> list = new ArrayList<>();
        list.add("1");
        return JsonVO.success(new MedicineChangeStatusVO(list));
    }
}
