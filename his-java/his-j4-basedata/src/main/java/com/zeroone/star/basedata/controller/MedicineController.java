package com.zeroone.star.basedata.controller;


import com.zeroone.star.basedata.service.IMedicineService;
import com.zeroone.star.basedata.service.impl.MsMedicineMapper;
import com.zeroone.star.project.dto.PageDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineAddDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineChangeStatusDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDeleteDTO;
import com.zeroone.star.project.dto.j4.medicine.MedicineDetailDTO;
import com.zeroone.star.project.j4.basedata.MedicineApis;
import com.zeroone.star.project.query.j4.medicine.MedicinePageQuery;
import com.zeroone.star.project.query.j4.medicine.MedicineSelectPageQuery;
import com.zeroone.star.project.vo.JsonVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineAddVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineDetailVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineListVO;
import com.zeroone.star.project.vo.j4.medicine.MedicineSelectVO;
import io.swagger.v3.oas.annotations.Operation;
import io.swagger.v3.oas.annotations.Parameter;
import io.swagger.v3.oas.annotations.tags.Tag;
import jakarta.annotation.Resource;
import jakarta.validation.Valid;
import lombok.extern.slf4j.Slf4j;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

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
@Tag(name = "药品管理", description = "药品管理接口")
@Validated
@Slf4j
public class MedicineController implements MedicineApis {

    @Resource
    IMedicineService medicineService;

    @Resource
    MsMedicineMapper msMedicineMapper;

    @Override
    @GetMapping("/query-medicine-list")
    @Operation(summary = "获取药品列表（条件+分页）")
    public JsonVO<PageDTO<MedicineListVO>> queryMedicineList(MedicinePageQuery query) {
        return JsonVO.success(medicineService.queryMedicinePage(query));
    }

    @Override
    @GetMapping("/query-medicine-select-list")
    @Operation(summary = "获取选择药品列表（条件+分页）")
    public JsonVO<PageDTO<MedicineSelectVO>> queryMedicineSelectList(
            MedicineSelectPageQuery query) {

        PageDTO<MedicineSelectVO> pageResult =
                medicineService.queryMedicineSelectPage(query);

        return JsonVO.success(pageResult);
    }

    @Override
    @GetMapping("/query-medicine")
    @Operation(summary = "获取药品详情")
    @Parameter(name = "id",example = "1")
    public JsonVO<MedicineDetailVO> queryMedicineDetail(String id) {
        if(id.isBlank()){
            return JsonVO.fail("id不能为空");
        }
        MedicineDetailDTO detailDTO = medicineService.queryMedicineDetailById(id);
        MedicineDetailVO detailVO = msMedicineMapper.medicineDetailDtoToVo(detailDTO);
        if(detailDTO!=null){
            return JsonVO.success(detailVO);
        }
        return JsonVO.fail("获取失败");
    }


    @PutMapping("/update-medicine")
    @Operation(summary = "修改药品")
    public JsonVO<String> updateMedicine(@Validated @RequestBody MedicineDetailDTO dto){

        int i =  medicineService.updateMedicine(dto);
        if(i != 0) return JsonVO.success("修改成功");
        return JsonVO.fail("修改失败") ;
    }


    @DeleteMapping("/delete-medicine")
    @Operation(summary = "删除药品（支持批量）")
    public JsonVO<String> deleteMedicine(@Validated @RequestBody MedicineDeleteDTO dto){

        int i = medicineService.deleteMedicine(dto);
        // 让删除失败的在service层抛异常
        if(i != 0) return JsonVO.success("删除成功"+ i + "个药品");
        return JsonVO.fail("删除失败");
    }

    @PutMapping("/change-status/{status}")
    @Operation(summary = "启/停用药品(支持批量)",description = "status为1表示启用，为0表示停用")
    @Parameter(name = "status",example = "1")
    public JsonVO<Void> updateChangeMedicineStatus(
            @PathVariable Integer status,
            @Validated @RequestBody MedicineChangeStatusDTO dto
    ){
        if(status != 1 && status !=0 ){
            return JsonVO.fail("status状态值输入不符合规范");
        }
        //在service里处理复杂逻辑
        int i = medicineService.updateChangeMedicineStatus(status,dto);

        if (i == dto.getSize()){
            return JsonVO.success(null);
        }
        return JsonVO.fail("状态修改存在异常");
    }

    @Override
    @PostMapping("/add-medicine")
    @Operation(summary = "新增药品", description = "新增药品主体及价格信息")
    public JsonVO<MedicineAddVO> addMedicine(@Valid @RequestBody MedicineAddDTO dto) {
        MedicineAddVO vo = medicineService.addMedicine(dto);
        return JsonVO.success(vo);
    }
}
