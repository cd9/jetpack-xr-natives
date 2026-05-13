#pragma once
#include "core/ncsb/component.h"
#include "core/view/utils/frame_time.h"

// A component that makes a node spin in place over time at a given speed.
class Spin : public imp::Component {
 public:
  // Setup and Update are standard lifecycle methods for Impress components.
  // Setup is used for initialization, and Update is called automatically by 
  // the framework on every frame to update the component's state.
  void Setup(float degrees_per_second);
  void Update(const imp::FrameTime& frame_time);

 private:
  float radians_per_second_;
};
