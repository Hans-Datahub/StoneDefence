// ArchivesScreenshotTypes.h
// 插件内部使用的截图相关类型定义
// 这样插件就不需要依赖主项目的 RenderingUtils

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"

/**
 * 插件内部使用的截图完成委托
 * 当截图完成并生成纹理后，会调用此委托
 *
 * @param Texture - 生成的缩略图纹理，可能为nullptr（如果截图失败）
 */
DECLARE_DELEGATE_OneParam(FArchivesScreenshotComplete, UTexture2D* /*Texture*/);