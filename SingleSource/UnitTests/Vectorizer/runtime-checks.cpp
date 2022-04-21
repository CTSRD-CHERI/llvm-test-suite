#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <stdint.h>

// Tests for memory runtime checks generated by the vectorizer. Runs scalar and
// vectorized versions of a loop requiring runtime checks on the same inputs
// with pointers to the same buffer using various offsets between reads and
// writes. Fails if they do not produce the same results.

static std::mt19937 rng;

// Initialize arrays A with random numbers.
template <typename Ty>
static void init_data(const std::unique_ptr<Ty[]> &A, unsigned N) {
  std::uniform_int_distribution<uint64_t> distrib(std::numeric_limits<Ty>::min(),
                                            std::numeric_limits<Ty>::max());
  for (unsigned i = 0; i < N; i++)
    A[i] = distrib(rng);
}

template <typename Ty>
static void check(const std::unique_ptr<Ty[]> &Reference,
                  const std::unique_ptr<Ty[]> &Tmp, unsigned NumElements,
                  int Offset) {
  if (!std::equal(&Reference[0], &Reference[0] + NumElements, &Tmp[0])) {
    std::cerr << "Miscompare with offset " << Offset << "\n";
    exit(1);
  }
}

#define DEFINE_SCALAR_AND_VECTOR_FN2(Loop)                                     \
  auto ScalarFn = [](auto *A, auto *B, unsigned TC) {                          \
    _Pragma("clang loop vectorize(disable)") Loop                              \
  };                                                                           \
  auto VectorFn = [](auto *A, auto *B, unsigned TC) {                          \
    _Pragma("clang loop vectorize(enable)") Loop                               \
  };

#define DEFINE_SCALAR_AND_VECTOR_FN3(Loop)                                     \
  auto ScalarFn = [](auto *A, auto *B, auto *C, unsigned TC) {                 \
    _Pragma("clang loop vectorize(disable)") Loop                              \
  };                                                                           \
  auto VectorFn = [](auto *A, auto *B, auto *C, unsigned TC) {                 \
    _Pragma("clang loop vectorize(enable)") Loop                               \
  };

// Helper to call \p f with \p args and acts as optimization barrier for \p f.
template <typename F, typename... Args>
__attribute__((optnone)) static void callThroughOptnone(F &&f, Args &&...args) {
  f(std::forward<Args>(args)...);
}

template <typename Ty> using Fn2Ty = std::function<void(Ty *, Ty *, unsigned)>;

// Run both \p ScalarFn and \p VectorFn on the same inputs with pointers to the
// same buffer. Fail if they do not produce the same results.
template <typename Ty>
static void checkOverlappingMemoryOneRuntimeCheck(Fn2Ty<Ty> ScalarFn,
                                                  Fn2Ty<Ty> VectorFn,
                                                  const char *Name) {
  std::cout << "Checking " << Name << "\n";

  unsigned N = 100;
  // Make sure we have enough extra elements so we can be liberal with offsets.
  unsigned NumArrayElements = N * 8;
  std::unique_ptr<Ty[]> Input1(new Ty[NumArrayElements]);
  std::unique_ptr<Ty[]> Reference(new Ty[NumArrayElements]);
  std::unique_ptr<Ty[]> ToCheck(new Ty[NumArrayElements]);

  auto CheckWithOffset = [&](int Offset) {
    init_data(Input1, NumArrayElements);
    for (unsigned i = 0; i < NumArrayElements; i++) {
      Reference[i] = Input1[i];
      ToCheck[i] = Input1[i];
    }

    // Run scalar function to generate reference output.
    Ty *ReferenceStart = &Reference[NumArrayElements / 2];
    ScalarFn(ReferenceStart + Offset, ReferenceStart, N);

    // Run vector function to generate output to check.
    Ty *StartPtr = &ToCheck[NumArrayElements / 2];
    callThroughOptnone(VectorFn, StartPtr + Offset, StartPtr, N);

    // Compare scalar and vector output.
    check(Reference, ToCheck, NumArrayElements, Offset);
  };

  for (int i = -100; i <= 100; i++)
    CheckWithOffset(i);
}

template <typename Ty>
using Fn3Ty = std::function<void(Ty *, Ty *, Ty *, unsigned)>;
template <typename Ty>
static void checkOverlappingMemoryTwoRuntimeChecks(Fn3Ty<Ty> ScalarFn,
                                                   Fn3Ty<Ty> VectorFn,
                                                   const char *Name) {
  std::cout << "Checking " << Name << "\n";

  unsigned N = 100;
  // Make sure we have enough extra elements so we can be liberal with offsets.
  unsigned NumArrayElements = N * 8;
  std::unique_ptr<Ty[]> Input1(new Ty[NumArrayElements]);
  std::unique_ptr<Ty[]> Input2(new Ty[NumArrayElements]);
  std::unique_ptr<Ty[]> Reference(new Ty[NumArrayElements]);
  std::unique_ptr<Ty[]> ToCheck(new Ty[NumArrayElements]);

  auto CheckWithOffsetSecond = [&](int Offset) {
    init_data(Input1, NumArrayElements);
    init_data(Input2, NumArrayElements);
    for (unsigned i = 0; i < NumArrayElements; i++) {
      Reference[i] = Input1[i];
      ToCheck[i] = Input1[i];
    }

    // Run scalar function to generate reference output.
    Ty *ReferenceStart = &Reference[NumArrayElements / 2];
    ScalarFn(ReferenceStart + Offset, &Input2[0], ReferenceStart, N);

    // Run vector function to generate output to check.
    Ty *StartPtr = &ToCheck[NumArrayElements / 2];
    callThroughOptnone(VectorFn, StartPtr + Offset, &Input2[0], StartPtr, N);

    // Compare scalar and vector output.
    check(Reference, ToCheck, NumArrayElements, Offset);
  };

  for (int i = -100; i <= 100; i++)
    CheckWithOffsetSecond(i);
}

int main(void) {
  rng = std::mt19937(15);

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = 0; i < TC; i++)
        A[i] = B[i] + 10;
    );

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, step 1, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(ScalarFn, VectorFn,
                                               "1 read, 1 write, step 1, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn, "1 read, 1 write, step 1, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = 0; i < TC; i++)
        A[i] = B[i + 3] + 10;
    );

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset 3, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset 3, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset 3, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = 3; i < TC; i++)
        A[i] = B[i - 3] + 10;
    );

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset -3, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset -3, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn, "1 read, 1 write, offset -3, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = TC; i > 0; i--)
        A[i] = B[i] + 10;);

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = TC; i > 2; i -= 2)
        A[i] = B[i] + 10;
    );

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down 2, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down 2, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn, "1 read, 1 write, index count down 2, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      for (unsigned i = 0, j = 0; i < TC; i++) {
        A[i] = B[j] + 10;
        j += 2;
      }
    );
    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn,
        "1 read, 1 write, 2 inductions, different steps, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn,
        "1 read, 1 write, 2 inductions, different steps, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn,
        "1 read, 1 write, 2 inductions, different steps, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN2(
      _Pragma("clang loop unroll(disable)")
      for (unsigned i = 0; i < TC; i += 2)
        A[i] = B[i] + 10;
    );

    checkOverlappingMemoryOneRuntimeCheck<uint8_t>(
        ScalarFn, VectorFn, "1 read, 1 write, induction increment 2, uint8_t");
    checkOverlappingMemoryOneRuntimeCheck<uint32_t>(
        ScalarFn, VectorFn, "1 read, 1 write, induction increment 2, uint32_t");
    checkOverlappingMemoryOneRuntimeCheck<uint64_t>(
        ScalarFn, VectorFn,
        "1 read, 1 write, induction increment 2, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN3(
      for (unsigned i = 0; i < TC; i++)
        A[i] = B[i] + C[i] + 10;
    );

    checkOverlappingMemoryTwoRuntimeChecks<uint32_t>(
        ScalarFn, VectorFn, "2 reads, 1 write, simple indices, uint32_t");
    checkOverlappingMemoryTwoRuntimeChecks<uint8_t>(
        ScalarFn, VectorFn, "2 reads, 1 write, simple indices, uint8_t");
    checkOverlappingMemoryTwoRuntimeChecks<uint64_t>(
        ScalarFn, VectorFn, "2 reads, 1 write, simple indices, uint64_t");
  }

  {
    DEFINE_SCALAR_AND_VECTOR_FN3(
      for (unsigned i = 0; i < TC; i++) {
        auto X = C[i] + 10;
        A[i] = X;
        B[i] = X + 9;
      }
    );

    checkOverlappingMemoryTwoRuntimeChecks<uint8_t>(
        ScalarFn, VectorFn, "1 read, 2 writes, simple indices, uint8_t");
    checkOverlappingMemoryTwoRuntimeChecks<uint32_t>(
        ScalarFn, VectorFn, "1 read, 2 writes, simple indices, uint32_t");
    checkOverlappingMemoryTwoRuntimeChecks<uint64_t>(
        ScalarFn, VectorFn, "1 read, 2 writes, simple indices, uint64_t");
  }

  return 0;
}
