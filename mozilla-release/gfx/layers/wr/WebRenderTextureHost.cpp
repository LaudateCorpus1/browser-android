/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "WebRenderTextureHost.h"

#include "mozilla/layers/ImageDataSerializer.h"
#include "mozilla/layers/LayersSurfaces.h"
#include "mozilla/webrender/RenderThread.h"
#include "mozilla/webrender/WebRenderAPI.h"

#ifdef MOZ_WIDGET_ANDROID
#  include "mozilla/layers/TextureHostOGL.h"
#endif

namespace mozilla {
namespace layers {

<<<<<<< HEAD
WebRenderTextureHost::WebRenderTextureHost(
    const SurfaceDescriptor& aDesc, TextureFlags aFlags, TextureHost* aTexture,
    wr::ExternalImageId& aExternalImageId)
    : TextureHost(aFlags), mExternalImageId(aExternalImageId) {
||||||| merged common ancestors
WebRenderTextureHost::WebRenderTextureHost(const SurfaceDescriptor& aDesc,
                                           TextureFlags aFlags,
                                           TextureHost* aTexture,
                                           wr::ExternalImageId& aExternalImageId)
  : TextureHost(aFlags)
  , mExternalImageId(aExternalImageId)
{
=======
class ScheduleNofityForUse : public wr::NotificationHandler {
 public:
  explicit ScheduleNofityForUse(uint64_t aExternalImageId)
      : mExternalImageId(aExternalImageId) {}

  virtual void Notify(wr::Checkpoint aCheckpoint) override {
    if (aCheckpoint == wr::Checkpoint::FrameTexturesUpdated) {
      MOZ_ASSERT(wr::RenderThread::IsInRenderThread());
      wr::RenderThread::Get()->NofityForUse(mExternalImageId);
    } else {
      MOZ_ASSERT(aCheckpoint == wr::Checkpoint::TransactionDropped);
    }
  }

 protected:
  uint64_t mExternalImageId;
};

WebRenderTextureHost::WebRenderTextureHost(
    const SurfaceDescriptor& aDesc, TextureFlags aFlags, TextureHost* aTexture,
    wr::ExternalImageId& aExternalImageId)
    : TextureHost(aFlags), mExternalImageId(aExternalImageId) {
>>>>>>> upstream-releases
  // The wrapped textureHost will be used in WebRender, and the WebRender could
  // run at another thread. It's hard to control the life-time when gecko
  // receives PTextureParent destroy message. It's possible that textureHost is
  // still used by WebRender. So, we only accept the textureHost without
  // DEALLOCATE_CLIENT flag here. If the buffer deallocation is controlled by
  // parent, we could do something to make sure the wrapped textureHost is not
  // used by WebRender and then release it.
  MOZ_ASSERT(!(aFlags & TextureFlags::DEALLOCATE_CLIENT));

  MOZ_COUNT_CTOR(WebRenderTextureHost);
  mWrappedTextureHost = aTexture;

  CreateRenderTextureHost(aDesc, aTexture);
}

WebRenderTextureHost::~WebRenderTextureHost() {
  MOZ_COUNT_DTOR(WebRenderTextureHost);
  wr::RenderThread::Get()->UnregisterExternalImage(
      wr::AsUint64(mExternalImageId));
}

void WebRenderTextureHost::CreateRenderTextureHost(
    const layers::SurfaceDescriptor& aDesc, TextureHost* aTexture) {
  MOZ_ASSERT(aTexture);

  aTexture->CreateRenderTexture(mExternalImageId);
}

<<<<<<< HEAD
bool WebRenderTextureHost::Lock() {
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
||||||| merged common ancestors
bool
WebRenderTextureHost::Lock()
{
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
=======
bool WebRenderTextureHost::Lock() {
  MOZ_ASSERT(!mWrappedTextureHost.get() ||
             mWrappedTextureHost->AsBufferTextureHost());

  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    return mWrappedTextureHost->Lock();
  }
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
void WebRenderTextureHost::Unlock() {
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
||||||| merged common ancestors
void
WebRenderTextureHost::Unlock()
{
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
=======
void WebRenderTextureHost::Unlock() {
  MOZ_ASSERT(!mWrappedTextureHost.get() ||
             mWrappedTextureHost->AsBufferTextureHost());

  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    mWrappedTextureHost->Unlock();
  }
}

void WebRenderTextureHost::PrepareTextureSource(
    CompositableTextureSourceRef& aTexture) {
  MOZ_ASSERT(!mWrappedTextureHost.get() ||
             mWrappedTextureHost->AsBufferTextureHost());

  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    mWrappedTextureHost->PrepareTextureSource(aTexture);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool WebRenderTextureHost::BindTextureSource(
    CompositableTextureSourceRef& aTexture) {
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
||||||| merged common ancestors
bool
WebRenderTextureHost::BindTextureSource(CompositableTextureSourceRef& aTexture)
{
  MOZ_ASSERT_UNREACHABLE("unexpected to be called");
=======
bool WebRenderTextureHost::BindTextureSource(
    CompositableTextureSourceRef& aTexture) {
  MOZ_ASSERT(!mWrappedTextureHost.get() ||
             mWrappedTextureHost->AsBufferTextureHost());

  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    return mWrappedTextureHost->BindTextureSource(aTexture);
  }
>>>>>>> upstream-releases
  return false;
}

<<<<<<< HEAD
already_AddRefed<gfx::DataSourceSurface> WebRenderTextureHost::GetAsSurface() {
||||||| merged common ancestors
already_AddRefed<gfx::DataSourceSurface>
WebRenderTextureHost::GetAsSurface()
{
=======
void WebRenderTextureHost::UnbindTextureSource() {
  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    mWrappedTextureHost->UnbindTextureSource();
  }
  // Handle read unlock
  TextureHost::UnbindTextureSource();
}

void WebRenderTextureHost::SetTextureSourceProvider(
    TextureSourceProvider* aProvider) {
  // During using WebRender, only BasicCompositor could exist
  MOZ_ASSERT(!aProvider || aProvider->AsBasicCompositor());
  MOZ_ASSERT(!mWrappedTextureHost.get() ||
             mWrappedTextureHost->AsBufferTextureHost());

  if (mWrappedTextureHost && mWrappedTextureHost->AsBufferTextureHost()) {
    mWrappedTextureHost->SetTextureSourceProvider(aProvider);
  }
}

already_AddRefed<gfx::DataSourceSurface> WebRenderTextureHost::GetAsSurface() {
>>>>>>> upstream-releases
  if (!mWrappedTextureHost) {
    return nullptr;
  }
  return mWrappedTextureHost->GetAsSurface();
}

<<<<<<< HEAD
void WebRenderTextureHost::SetTextureSourceProvider(
    TextureSourceProvider* aProvider) {}

YUVColorSpace WebRenderTextureHost::GetYUVColorSpace() const {
||||||| merged common ancestors
void
WebRenderTextureHost::SetTextureSourceProvider(TextureSourceProvider* aProvider)
{
}

YUVColorSpace
WebRenderTextureHost::GetYUVColorSpace() const
{
=======
gfx::YUVColorSpace WebRenderTextureHost::GetYUVColorSpace() const {
>>>>>>> upstream-releases
  if (mWrappedTextureHost) {
    return mWrappedTextureHost->GetYUVColorSpace();
  }
  return gfx::YUVColorSpace::UNKNOWN;
}

gfx::IntSize WebRenderTextureHost::GetSize() const {
  if (!mWrappedTextureHost) {
    return gfx::IntSize();
  }
  return mWrappedTextureHost->GetSize();
}

gfx::SurfaceFormat WebRenderTextureHost::GetFormat() const {
  if (!mWrappedTextureHost) {
    return gfx::SurfaceFormat::UNKNOWN;
  }
  return mWrappedTextureHost->GetFormat();
}

<<<<<<< HEAD
gfx::SurfaceFormat WebRenderTextureHost::GetReadFormat() const {
||||||| merged common ancestors
gfx::SurfaceFormat
WebRenderTextureHost::GetReadFormat() const
{
=======
void WebRenderTextureHost::NotifyNotUsed() {
#ifdef MOZ_WIDGET_ANDROID
  if (mWrappedTextureHost && mWrappedTextureHost->AsSurfaceTextureHost()) {
    wr::RenderThread::Get()->NotifyNotUsed(wr::AsUint64(mExternalImageId));
  }
#endif
  TextureHost::NotifyNotUsed();
}

void WebRenderTextureHost::PrepareForUse() {
#ifdef MOZ_WIDGET_ANDROID
  if (mWrappedTextureHost && mWrappedTextureHost->AsSurfaceTextureHost()) {
    // Call PrepareForUse on render thread.
    // See RenderAndroidSurfaceTextureHostOGL::PrepareForUse.
    wr::RenderThread::Get()->PrepareForUse(wr::AsUint64(mExternalImageId));
  }
#endif
}

gfx::SurfaceFormat WebRenderTextureHost::GetReadFormat() const {
>>>>>>> upstream-releases
  if (!mWrappedTextureHost) {
    return gfx::SurfaceFormat::UNKNOWN;
  }
  return mWrappedTextureHost->GetReadFormat();
}

int32_t WebRenderTextureHost::GetRGBStride() {
  if (!mWrappedTextureHost) {
    return 0;
  }
  gfx::SurfaceFormat format = GetFormat();
  if (GetFormat() == gfx::SurfaceFormat::YUV) {
    // XXX this stride is used until yuv image rendering by webrender is used.
    // Software converted RGB buffers strides are aliened to 16
    return gfx::GetAlignedStride<16>(
        GetSize().width, BytesPerPixel(gfx::SurfaceFormat::B8G8R8A8));
  }
  return ImageDataSerializer::ComputeRGBStride(format, GetSize().width);
}

<<<<<<< HEAD
bool WebRenderTextureHost::HasIntermediateBuffer() const {
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->HasIntermediateBuffer();
}

uint32_t WebRenderTextureHost::NumSubTextures() const {
||||||| merged common ancestors
uint32_t
WebRenderTextureHost::NumSubTextures() const
{
=======
bool WebRenderTextureHost::HasIntermediateBuffer() const {
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->HasIntermediateBuffer();
}

uint32_t WebRenderTextureHost::NumSubTextures() {
>>>>>>> upstream-releases
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->NumSubTextures();
}

void WebRenderTextureHost::PushResourceUpdates(
    wr::TransactionBuilder& aResources, ResourceUpdateOp aOp,
    const Range<wr::ImageKey>& aImageKeys, const wr::ExternalImageId& aExtID) {
  MOZ_ASSERT(mWrappedTextureHost);
  MOZ_ASSERT(mExternalImageId == aExtID || SupportsWrNativeTexture());

  mWrappedTextureHost->PushResourceUpdates(aResources, aOp, aImageKeys, aExtID);
}

void WebRenderTextureHost::PushDisplayItems(
    wr::DisplayListBuilder& aBuilder, const wr::LayoutRect& aBounds,
    const wr::LayoutRect& aClip, wr::ImageRendering aFilter,
    const Range<wr::ImageKey>& aImageKeys) {
  MOZ_ASSERT(mWrappedTextureHost);
  MOZ_ASSERT(aImageKeys.length() > 0);

  mWrappedTextureHost->PushDisplayItems(aBuilder, aBounds, aClip, aFilter,
                                        aImageKeys);
}

bool WebRenderTextureHost::SupportsWrNativeTexture() {
  return mWrappedTextureHost->SupportsWrNativeTexture();
}

<<<<<<< HEAD
}  // namespace layers
}  // namespace mozilla
||||||| merged common ancestors
} // namespace layers
} // namespace mozilla
=======
bool WebRenderTextureHost::NeedsYFlip() const {
  bool yFlip = TextureHost::NeedsYFlip();
  if (mWrappedTextureHost->AsSurfaceTextureHost()) {
    MOZ_ASSERT(yFlip);
    // With WebRender, SurfaceTextureHost always requests y-flip.
    // But y-flip should not be handled, since
    // SurfaceTexture.getTransformMatrix() is not handled yet.
    // See Bug 1507076.
    yFlip = false;
  }
  return yFlip;
}

void WebRenderTextureHost::MaybeNofityForUse(wr::TransactionBuilder& aTxn) {
#if defined(MOZ_WIDGET_ANDROID)
  if (!mWrappedTextureHost->AsSurfaceTextureHost()) {
    return;
  }
  // SurfaceTexture of video needs NofityForUse() to detect if it is rendered
  // on WebRender.
  aTxn.Notify(wr::Checkpoint::FrameTexturesUpdated,
              MakeUnique<ScheduleNofityForUse>(wr::AsUint64(mExternalImageId)));
#endif
}

}  // namespace layers
}  // namespace mozilla
>>>>>>> upstream-releases
