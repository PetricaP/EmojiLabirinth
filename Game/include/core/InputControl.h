#pragma once

#include "MathUtils.h"

class InputControl {
  private:
	float amount = 0;

  public:
	InputControl() = default;

	void AddAmount(float amountToAdd) { amount = math::clamp(amount + amountToAdd, -1.0f, 1.0f); }

	float GetAmount() const { return amount; }
};

