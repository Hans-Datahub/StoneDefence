//#include "RenderingUtils.h"
//#include "EngineUtils.h"  // TActorIterator需要
//#include "Landscape.h"  // ALandscape需要
//
//void RenderingUtils::FScreenShot::OnScreenshotCapturedInternal(
//    int32 SrcWidth,
//    int32 SrcHeight,
//    const TArray<FColor>& OrigBitmap
//)
//{
//    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
//    check(OrigBitmap.Num() == SrcWidth * SrcHeight);
//
//    // 调整图像大小
//    TArray<FColor> ScaledBitmap;
//    FImageUtils::ImageResize(SrcWidth, SrcHeight, OrigBitmap, ScaledWidth, ScaledHeight, ScaledBitmap, true);
//
//    // 压缩为JPEG并保存
//    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
//    ImageWrapper->SetRaw(ScaledBitmap.GetData(), ScaledBitmap.GetAllocatedSize(), ScaledWidth, ScaledHeight, ERGBFormat::BGRA, 8);
//    TArray64<uint8> JPEGData = ImageWrapper->GetCompressed(ImageQuality);
//    FFileHelper::SaveArrayToFile(JPEGData, *Filename);
//
//    // 创建纹理
//    FCreateTexture2DParameters Params;
//    Params.bDeferCompression = true;
//    Texture = FImageUtils::CreateTexture2D(ScaledWidth, ScaledHeight, ScaledBitmap, Outer, FGuid::NewGuid().ToString(), RF_NoFlags, Params);
//
//    // 执行回调通知
//    if (OnComplete.IsBound())
//    {
//        OnComplete.Execute(Texture);
//    }
//
//    // 清理
//    ImageWrapper.Reset();
//    delete this;
//}
//
//ASceneCapture2D* RenderingUtils::SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FMapSize& MapSize, float Life)
//{
//    if (SceneCaptureClass && World)
//    {
//        for (TActorIterator<ALandscape> It(World, ALandscape::StaticClass()); It; ++It)
//        {
//            if (ALandscape* BigMap = *It)
//            {
//                FVector BigMapSize = BigMap->GetActorScale3D();
//                MapSize.BigMapRealSize = FVector2D(BigMapSize.X * 7, BigMapSize.Y * 7);
//                FVector CenterPoint = FVector(MapSize.BigMapRealSize.X / 2, MapSize.BigMapRealSize.Y / 2, 0.0f);
//
//                if (ASceneCapture2D* NewCarma = World->SpawnActor<ASceneCapture2D>(SceneCaptureClass, CenterPoint, FRotator(-90.f, 0.f, 0.f)))
//                {
//                    if (Life != 0.f)
//                    {
//                        NewCarma->SetLifeSpan(Life);
//                    }
//                    return NewCarma;
//                }
//                break;
//            }
//        }
//    }
//    return nullptr;
//}