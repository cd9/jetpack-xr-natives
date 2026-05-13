#include "rust_bridge.h"

#include <memory>

#include "core/view/framework/client_api.h"
#include "core/view/view_host.h"
#include "filament/Viewport.h"
#include "sample_view.h"

RustBridge::~RustBridge() {
  if (!view_host_) {
    return;
  }
  view_host_->DestroySwapChain();
  view_host_->Cleanup();
}

bool RustBridge::Init(uint8_t* native_window) {
  // Create the custom SampleView defined in sample_view.h
  auto view = imp::View::Create<SampleView>("sImple");

  view_host_ = std::make_unique<imp::ViewHost>(std::move(view));
  // Initialize Filament with OpenGL and pass the native window handle.
  view_host_->Setup(filament::Engine::Backend::OPENGL, nullptr, nullptr);
  view_host_->CreateSwapChain(reinterpret_cast<void*>(native_window));

  if (auto filament_view = view_host_->FilamentHost::GetView()) {
    filament_view->setViewport(filament::Viewport(0, 0, 1024, 1024));
  }

  return true;
}

void RustBridge::RenderFrame() {
  if (!view_host_) {
    return;
  }

  static absl::Duration time_cursor = absl::ZeroDuration();
  time_cursor += absl::Milliseconds(16);  // Simulate 60fps

  view_host_->RenderNextFrame(time_cursor,
                              time_cursor + absl::Milliseconds(16));
}

std::unique_ptr<RustBridge> CreateRustBridge() {
  return std::make_unique<RustBridge>();
}
