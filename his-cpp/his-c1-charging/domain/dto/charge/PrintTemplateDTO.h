#pragma once

#ifndef _PRINT_TEMPLATE_DTO_
#define _PRINT_TEMPLATE_DTO_

#include "../../GlobalInclude.h"

#include OATPP_CODEGEN_BEGIN(DTO)
/*
   元素类型元信息（printElementType 字段类型）
   仅 table 类型元素包含后续的可编辑性标志位
*/
class PrintElementTypeDTO : public oatpp::DTO
{
    DTO_INIT(PrintElementTypeDTO, DTO);

    // 元素类型（text / table / hline / vline）
    API_DTO_FIELD(String, type, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.type.description"), true, "text");

    // 元素标题（中文展示名）
    API_DTO_FIELD(String, title, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.title.description"), true, "文本");

    // 是否可编辑
    API_DTO_FIELD(Boolean, editable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.editable.description"), false, false);

    // 是否可调整列宽
    API_DTO_FIELD(Boolean, columnResizable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.columnResizable.description"), false, false);

    // 是否启用删除行
    API_DTO_FIELD(Boolean, isEnableDeleteRow, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableDeleteRow.description"), false, false);

    // 是否启用编辑字段
    API_DTO_FIELD(Boolean, isEnableEditField, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableEditField.description"), false, false);

    // 是否启用插入行
    API_DTO_FIELD(Boolean, isEnableInsertRow, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableInsertRow.description"), false, false);

    // 是否启用合并单元格
    API_DTO_FIELD(Boolean, isEnableMergeCell, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableMergeCell.description"), false, false);

    // 是否启用右键菜单
    API_DTO_FIELD(Boolean, isEnableContextMenu, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableContextMenu.description"), false, false);

    // 是否启用删除列
    API_DTO_FIELD(Boolean, isEnableDeleteColumn, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableDeleteColumn.description"), false, false);

    // 是否启用插入列
    API_DTO_FIELD(Boolean, isEnableInsertColumn, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.isEnableInsertColumn.description"), false, false);

    // 列对齐是否可编辑
    API_DTO_FIELD(Boolean, columnAlignEditable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.columnAlignEditable.description"), false, false);

    // 列标题是否可编辑
    API_DTO_FIELD(Boolean, columnTitleEditable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.columnTitleEditable.description"), false, false);

    // 列显示是否可编辑
    API_DTO_FIELD(Boolean, columnDisplayEditable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.columnDisplayEditable.description"), false, false);

    // 列显示顺序是否可编辑
    API_DTO_FIELD(Boolean, columnDisplayIndexEditable, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementTypeDTO.columnDisplayIndexEditable.description"), false, false);
};

/*
   水印选项（watermarkOptions 字段类型）
*/
class WatermarkOptionsDTO : public oatpp::DTO
{
    DTO_INIT(WatermarkOptionsDTO, DTO);

    // 水印宽度
    API_DTO_FIELD(Float64, width, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.width.description"), false, 200.0);

    // 时间戳格式
    API_DTO_FIELD(String, format, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.format.description"), false, "YYYY-MM-DD HH:mm");

    // 水印高度
    API_DTO_FIELD(Float64, height, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.height.description"), false, 200.0);

    // 旋转角度
    API_DTO_FIELD(Int32, rotate, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.rotate.description"), false, 25);

    // 水印内容
    API_DTO_FIELD(String, content, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.content.description"), false, "");

    // 字体大小
    API_DTO_FIELD(String, fontSize, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.fontSize.description"), false, "14px");

    // 填充样式（rgba）
    API_DTO_FIELD(String, fillStyle, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.fillStyle.description"), false, "rgba(184, 184, 184, 0.3)");

    // 是否显示时间戳
    API_DTO_FIELD(Boolean, timestamp, ZH_WORDS_GETTER("lhfc.charging.charge.WatermarkOptionsDTO.timestamp.description"), false, false);
};

/*
   面板布局选项（panelLayoutOptions 字段类型）
*/
class PanelLayoutOptionsDTO : public oatpp::DTO
{
    DTO_INIT(PanelLayoutOptionsDTO, DTO);

    // 布局类型（如 column）
    API_DTO_FIELD(String, layoutType, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.layoutType.description"), false, "column");

    // 纸张样式
    API_DTO_FIELD(String, paperStyle, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.paperStyle.description"), false, "");

    // 纸张宽度
    API_DTO_FIELD(Float64, paperWidth, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.paperWidth.description"), false, 0.0);

    // 纸张高度
    API_DTO_FIELD(Float64, paperHeight, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.paperHeight.description"), false, 0.0);

    // 行间距
    API_DTO_FIELD(Float64, layoutRowGap, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.layoutRowGap.description"), false, 0.0);

    // 列间距
    API_DTO_FIELD(Float64, layoutColumnGap, ZH_WORDS_GETTER("lhfc.charging.charge.PanelLayoutOptionsDTO.layoutColumnGap.description"), false, 0.0);
};

/*
   打印元素（printElements 子对象）
   options 随 type 变化较大（text/table/hline 各不同），保留 Any 类型
*/
class PrintElementDTO : public oatpp::DTO
{
    DTO_INIT(PrintElementDTO, DTO);

    // 元素选项（text: field/title/fontSize/textAlign；table: field/columns/tableBorder；hline: borderWidth）
    API_DTO_FIELD(Any, options, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementDTO.options.description"), true, {});

    // 元素类型元信息
    API_DTO_FIELD(Object<PrintElementTypeDTO>, printElementType, ZH_WORDS_GETTER("lhfc.charging.charge.PrintElementDTO.printElementType.description"), true, {});
};

/*
   打印面板（panels 子对象）
   每个面板对应一页纸张
*/
class PrintPanelDTO : public oatpp::DTO
{
    DTO_INIT(PrintPanelDTO, DTO);

    // 面板名称（如 "1"）
    API_DTO_FIELD(String, name, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.name.description"), true, "1");

    // 面板序号
    API_DTO_FIELD(Int32, index, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.index.description"), true, 0);

    // 纸张宽度（mm）
    API_DTO_FIELD(Float64, width, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.width.description"), true, 80.0);

    // 纸张高度（mm）
    API_DTO_FIELD(Float64, height, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.height.description"), true, 200.0);

    // 纸张类型（如 roll-80mm 热敏卷纸）
    API_DTO_FIELD(String, paperType, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperType.description"), true, "roll-80mm");

    // 纸张底部边距
    API_DTO_FIELD(Float64, paperFooter, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperFooter.description"), true, 566.93);

    // 纸张顶部边距
    API_DTO_FIELD(Float64, paperHeader, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperHeader.description"), true, 34.0);

    // 页码顶部位置
    API_DTO_FIELD(Float64, paperNumberTop, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperNumberTop.description"), false, 545.0);

    // 页码左侧位置
    API_DTO_FIELD(Float64, paperNumberLeft, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperNumberLeft.description"), false, 200.0);

    // 页码是否连续
    API_DTO_FIELD(Boolean, paperNumberContinue, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperNumberContinue.description"), false, true);

    // 页码是否禁用
    API_DTO_FIELD(Boolean, paperNumberDisabled, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.paperNumberDisabled.description"), false, true);

    // 水印选项
    API_DTO_FIELD(Object<WatermarkOptionsDTO>, watermarkOptions, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.watermarkOptions.description"), false, {});

    // 面板布局选项
    API_DTO_FIELD(Object<PanelLayoutOptionsDTO>, panelLayoutOptions, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.panelLayoutOptions.description"), false, {});

    // 打印元素列表
    API_DTO_FIELD(Vector<Object<PrintElementDTO>>, printElements, ZH_WORDS_GETTER("lhfc.charging.charge.PrintPanelDTO.printElements.description"), true, {});
};

/*
   模板结构（PreparePrintResponseDTO.PrintJson 字段类型）
*/
class PrintJsonDTO : public oatpp::DTO
{
    DTO_INIT(PrintJsonDTO, DTO);

    // 面板列表，每个面板对应一页
    API_DTO_FIELD(Vector<Object<PrintPanelDTO>>, panels, ZH_WORDS_GETTER("lhfc.charging.charge.PrintJsonDTO.panels.description"), true, {Object<PrintPanelDTO>(PrintPanelDTO::createShared())});
};

#include OATPP_CODEGEN_END(DTO)
#endif // !_PRINT_TEMPLATE_DTO_
