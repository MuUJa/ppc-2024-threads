// Copyright 2024 Podyachikh Mikhail
#include <gtest/gtest.h>

#include "omp/podyachikh_m_hoare_sort_simple_merge/include/hoare_sort.hpp"

using namespace PodyachikhOMP;

class PodyachikhMTestSort : public ::testing::Test {
 public:
  HoareSort testTaskSequential;
  const uint32_t COUNT = 1367;

  PodyachikhMTestSort() : testTaskSequential{HoareSort(std::make_shared<ppc::core::TaskData>())} {}

  void set_data(HoareSort::vec_t* in_vec, HoareSort::compare_t* in_comp = nullptr) {
    std::shared_ptr<ppc::core::TaskData> taskDataSeq = std::make_shared<ppc::core::TaskData>();

    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in_vec));
    taskDataSeq->inputs_count.emplace_back(1);

    if (in_comp != nullptr) {
      taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in_comp));
      taskDataSeq->inputs_count.emplace_back(1);
    }

    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(in_vec));
    taskDataSeq->outputs_count.emplace_back(1);

    testTaskSequential.set_data(taskDataSeq);
  }

  bool run_test() {
    if (!testTaskSequential.validation()) return false;
    if (!testTaskSequential.pre_processing()) return false;
    if (!testTaskSequential.run()) return false;
    if (!testTaskSequential.post_processing()) return false;
    return true;
  }
};

TEST_F(PodyachikhMTestSort, Reverse_Vec) {
  HoareSort::vec_t in_vec(COUNT);
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSort::obj_ty>(COUNT - i);
  }
  set_data(&in_vec);

  EXPECT_TRUE(run_test());
  EXPECT_TRUE(std::is_sorted(in_vec.begin(), in_vec.end()));
}

TEST_F(PodyachikhMTestSort, Sorted_Vec) {
  HoareSort::vec_t in_vec(COUNT);
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSort::obj_ty>(i);
  }
  set_data(&in_vec);

  EXPECT_TRUE(run_test());
  EXPECT_TRUE(std::is_sorted(in_vec.begin(), in_vec.end()));
}

TEST_F(PodyachikhMTestSort, EQ_Vec) {
  HoareSort::vec_t in_vec(COUNT);
  for (uint32_t i = 0; i < COUNT; ++i) {
    in_vec[i] = static_cast<HoareSort::obj_ty>(COUNT);
  }
  set_data(&in_vec);

  EXPECT_TRUE(run_test());
  EXPECT_TRUE(std::is_sorted(in_vec.begin(), in_vec.end()));
}

TEST_F(PodyachikhMTestSort, Random_Vec) {
  HoareSort::vec_t in_vec = generate_random_vector(COUNT);
  set_data(&in_vec);

  EXPECT_TRUE(run_test());
  EXPECT_TRUE(std::is_sorted(in_vec.begin(), in_vec.end()));
}

TEST_F(PodyachikhMTestSort, With_Comp) {
  HoareSort::vec_t in_vec = generate_random_vector(COUNT);
  HoareSort::compare_t in_comp = [](const HoareSort::obj_ty& a, const HoareSort::obj_ty& b) -> bool {
    if ((a % 3) == (b % 3)) return a < b;
    return (a % 3) < (b % 3);
  };
  set_data(&in_vec, &in_comp);

  EXPECT_TRUE(run_test());
  EXPECT_TRUE(std::is_sorted(in_vec.begin(), in_vec.end(), in_comp));
}
