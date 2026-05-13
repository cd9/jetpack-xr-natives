#pragma once
#include <cstdint>

#include "core/view/framework/view.h"
#include "core/view/utils/frame_time.h"

// SampleView is the main view implementation for this demo. It overrides
// the Setup lifecycle method to load the 3D model and set up the scene.
class SampleView : public imp::View {
 public:
  SampleView();

 protected:
  void Setup() override;
};
