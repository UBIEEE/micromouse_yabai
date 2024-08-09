#include "DriveTools/trapezoid_profile.hpp"
#include <gtest/gtest.h>

using namespace drive;

TEST(TrapezoidProfile, SymmetricProfile) {
  TrapezoidProfile::Sample sample;
  TrapezoidProfile profile;

  profile.start(1000.f, 1000.f, 0.f, 1000.f);

  // Duration.
  ASSERT_FLOAT_EQ(2.f, profile.duration_s());

  // Start.
  sample = profile.sample(0.f);
  ASSERT_FLOAT_EQ(0.f, sample.distance);
  ASSERT_FLOAT_EQ(0.f, sample.velocity);

  // Middle.
  sample = profile.sample(1.f);
  ASSERT_FLOAT_EQ(500.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // End.
  sample = profile.sample(profile.duration_s());
  ASSERT_FLOAT_EQ(1000.f, sample.distance);
  ASSERT_FLOAT_EQ(0.f, sample.velocity);
}

TEST(TrapezoidProfile, MultiplePartsProfile) {
  TrapezoidProfile::Sample sample;
  TrapezoidProfile profile;

  // Accelerate part.

  profile.start(1000.f, 1000.f, 1000.f, 1000.f);

  // Duration.
  ASSERT_FLOAT_EQ(1.5f, profile.duration_s());

  // End of rise.
  sample = profile.sample(1.f);
  ASSERT_FLOAT_EQ(500.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // End
  sample = profile.sample(profile.duration_s());
  ASSERT_FLOAT_EQ(1000.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // Cruise part.

  profile.start(1000.f, 1000.f, 1000.f, 1000.f);

  // Duration.
  ASSERT_FLOAT_EQ(1.f, profile.duration_s());

  // Start
  sample = profile.sample(0.0f);
  ASSERT_FLOAT_EQ(0.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // Middle
  sample = profile.sample(0.5f);
  ASSERT_FLOAT_EQ(500.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // Somewhere else.
  sample = profile.sample(0.234f);
  ASSERT_FLOAT_EQ(234.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // End
  sample = profile.sample(profile.duration_s());
  ASSERT_FLOAT_EQ(1000.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // Decelerate part.

  profile.start(1000.f, 1000.f, 0.f, 1000.f);

  // Duration.
  ASSERT_FLOAT_EQ(1.5f, profile.duration_s());

  // Start of fall.
  sample = profile.sample(0.5f);
  ASSERT_FLOAT_EQ(500.f, sample.distance);
  ASSERT_FLOAT_EQ(1000.f, sample.velocity);

  // End
  sample = profile.sample(profile.duration_s());
  ASSERT_FLOAT_EQ(1000.f, sample.distance);
  ASSERT_FLOAT_EQ(0.f, sample.velocity);
}
