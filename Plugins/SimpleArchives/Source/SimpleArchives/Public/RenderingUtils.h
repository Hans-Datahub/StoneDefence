//#pragma once
//
//#include "CoreMinimal.h"
//#include "IImageWrapperModule.h"
//#include "ImageUtils.h"
//#include "Engine/GameViewportClient.h"
//#include "Misc/FileHelper.h"
//#include "Engine/Texture2D.h"
//#include "Landscape.h"
//#include "Engine/SceneCapture2D.h"
//
//#include "IImageWrapper.h"
//#include "IImageWrapperModule.h"
//#include "Misc/FileHelper.h"
//
//namespace RenderingUtils
//{
//    // 截图完成委托定义
//    DECLARE_DELEGATE_OneParam(FOnScreenshotComplete, UTexture2D* /*Texture*/);
//
//    struct FScreenShot
//    {
//        FScreenShot(
//            int32 InWidth,
//            int32 InHeight,
//            UTexture2D*& InTexture,
//            UObject* InOuter,
//            int32 InImageQuality = 80,
//            bool bInShowUI = false,
//            bool bAddFilenameSuffix = true,
//            FOnScreenshotComplete InOnComplete = FOnScreenshotComplete()
//        ) : Texture(InTexture)
//            , ScaledWidth(InWidth)
//            , ScaledHeight(InHeight)
//            , ImageQuality(InImageQuality)
//            , Outer(InOuter)
//            , OnComplete(InOnComplete)
//            , ScreenShotDelegateHandle()
//            , Filename()
//        {
//            if (!UGameViewportClient::OnScreenshotCaptured().IsBoundToObject(this))
//            {
//                Filename = FPaths::ProjectSavedDir() / TEXT("SaveGames") / FGuid::NewGuid().ToString();
//                ScreenShotDelegateHandle = UGameViewportClient::OnScreenshotCaptured().AddRaw(
//                    this,
//                    &RenderingUtils::FScreenShot::OnScreenshotCapturedInternal
//                );
//                FScreenshotRequest::RequestScreenshot(Filename, bInShowUI, bAddFilenameSuffix);
//                Filename += TEXT(".jpg");
//            }
//        }
//
//        ~FScreenShot()
//        {
//            if (ScreenShotDelegateHandle.IsValid())
//            {
//                UGameViewportClient::OnScreenshotCaptured().Remove(ScreenShotDelegateHandle);
//            }
//        }
//
//        FString GetFilename() const { return Filename; }
//
//    private:
//        void OnScreenshotCapturedInternal(
//            int32 SrcWidth,
//            int32 SrcHeight,
//            const TArray<FColor>& OrigBitmap
//        );
//
//        UTexture2D*& Texture;
//        int32 ScaledWidth;
//        int32 ScaledHeight;
//        int32 ImageQuality;
//        UObject* Outer;
//        FOnScreenshotComplete OnComplete;
//        FDelegateHandle ScreenShotDelegateHandle;
//        FString Filename;
//    };
//
//    struct FMapSize
//    {
//        FVector2D BigMapRealSize;
//    };
//
//    ASceneCapture2D* SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FMapSize& MapSize, float Life);
//}