#pragma once
#include <cstdint>
#include <memory>

#include "core/view/view_host.h"

// Bridges the Rust application with the C++ Impress framework.
class RustBridge {
 public:
  RustBridge() = default;
  ~RustBridge();

  // Initializes the Impress engine and creates the rendering swap chain.
  bool Init(uint8_t* native_window);
  // Renders the next frame in the scene.
  void RenderFrame();

 private:
  std::unique_ptr<imp::ViewHost> view_host_;
};

std::unique_ptr<RustBridge> CreateRustBridge();
