// Copyright 2024 Podyachikh Mikhail
#pragma once

#include <functional>
#include <string>
#include <vector>

#include "core/task/include/task.hpp"
#include "random"

namespace PodyachikhOMP {

class HoareSort : public ppc::core::Task {
 public:
  using obj_ty = int64_t;
  using compare_t = std::function<bool(const obj_ty&, const obj_ty&)>;
  using vec_t = std::vector<obj_ty>;

  explicit HoareSort(std::shared_ptr<ppc::core::TaskData> taskData_) : Task(std::move(taskData_)) {}
  bool pre_processing() override;
  bool validation() override;
  bool run() override;
  bool post_processing() override;

 private:
  vec_t _data;
  compare_t _comp;
  /* TODO: Allow to use std::less_equal and same functions, which returns true if items are equal.
   * Now it creates infinity recursion. */

  int max_segment_length = 1;
  void omp_hoare_sort(int left, int right);
  void seq_hoare_sort(int left, int right);
};

template <typename T = HoareSort::obj_ty>
std::vector<T> generate_random_vector(size_t size, T min_value = -1e9, T max_value = 1e9) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<T> dis(min_value, max_value);

  std::vector<T> result(size);
  for (size_t i = 0; i < size; ++i) {
    result[i] = dis(gen);
  }
  return result;
}
}  // namespace PodyachikhOMP
