#include "spin_component.h"

#include "core/math/math.h"
#include "core/ncsb/node.h"

void Spin::Setup(float degrees_per_second) {
  radians_per_second_ = 3.14159265f * degrees_per_second / 180.0f;
}

void Spin::Update(const imp::FrameTime& frame_time) {
  float delta_radians = radians_per_second_ * frame_time.GetDeltaSeconds();
  imp::quatf delta_rotation =
      imp::quatf::fromAxisAngle(imp::float3{0.0f, 1.0f, 0.0f}, delta_radians);
  GetNode()->SetLocalRotation(GetNode()->GetLocalRotation() * delta_rotation);
}
