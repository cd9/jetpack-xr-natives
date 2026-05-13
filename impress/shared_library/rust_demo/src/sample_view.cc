#include "sample_view.h"

#include "core/common/file_helpers.h"
#include "core/math/math.h"
#include "core/view/framework/assets/gltf_renderer.h"
#include "core/view/framework/camera/camera_component.h"
#include "core/view/framework/camera/camera_manager.h"
#include "core/view/framework/client_api.h"
#include "core/view/view_host.h"
#include "spin_component.h"

SampleView::SampleView() : imp::View() {}

void SampleView::Setup() {
  // Set 1:1 aspect ratio to match a square window.
  auto* filament_camera = GetCameraManager().GetCamera()->GetCamera();
  filament_camera->setProjection(/*fov=*/45.0f, /*aspect=*/1.0f, /*near=*/0.1f,
                                 /*far=*/100.0f);

  // Note: DamagedHelmet.glb is from the Khronos Group repository
  // and is licensed under the Apache License, Version 2.0.
  auto status_or_buffer = imp::LoadFile("src/DamagedHelmet.glb");
  if (!status_or_buffer.ok()) {
    return;
  }

  // Load the 3D model from the memory buffer using the View's AssetManager.
  // This returns a Future that will resolve when the model is loaded.
  GetAssetManager()
      .LoadModel(absl::Cord(status_or_buffer.value().StringView()),
                 "DamagedHelmet.glb")
      .Then([this](absl::StatusOr<imp::NodeHandle> model_or_status) {
        // Executes asynchronously when the LoadModel future completes.
        auto model = model_or_status.value();
        model->SetLocalPosition({0.0f, 0.0f, -5.0f});
        model->AddComponent<Spin>(45.0f);
      })
      .KeptBy(this);
}
