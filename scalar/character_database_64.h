#pragma once

// Somehow type deduction is not correct on coat. Must convert to Vector before returning

// Not operations are grouped because ~B modifies B in coat
// In some cases, one must make a copy

//TODO probably would be better to habe a DS that groups all 8 bit streams in one
template<typename Vector>
inline Vector match_0(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B2;
  auto A4 = B1 | B0;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_1(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B0_copy = B0;
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B2;
  auto A4 = B1 | ~B0_copy;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_2(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B1_copy = B1;
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B2;
  auto A4 = ~B1_copy | B0;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_3(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B2;
  auto A4 = ~(B1 & B0);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_4(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B2_copy = B2;
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | ~B2_copy;
  auto A4 = B1 | B0;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_5(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B1;
  auto A4 = ~(B2 & B0);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_6(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | B0;
  auto A4 = ~(B2 & B1);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_7(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B2_copy = B2;
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B3 | ~B2_copy;
  auto A4 = ~(B1 & B0);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_8(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B3_copy = B3;
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = ~B3_copy | B2;
  auto A4 = B1 | B0;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

template<typename Vector>
inline Vector match_9(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B7 | B6;
  auto A2 = B5 | B4;
  auto A3 = B2 | B1;
  auto A4 = ~(B3 & B0);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}

// TODO can it be made more efficient with NOR?
template<typename Vector>
inline Vector match_0_9(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = ~(B7 | B6);
  auto A2 = ~(B5 | B4);
  auto A3 = ~(B3 & B2);
  auto A4 = ~(B3 & B1);

  auto A5 = A1 & A2;
  auto A6 = A3 & A4;

  Vector ret = A5 & A6;
  return ret;
}


template<typename Vector>
inline Vector match_130(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  auto A1 = B6 | B0;
  auto A2 = B5 | B4;
  auto A3 = B3 | B2;
  auto A4 = ~(B7 & B1);

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}


template<typename Vector>
inline Vector match_140(const Vector &B7, const Vector &B6, const Vector &B5,
    const Vector &B4, const Vector &B3, const Vector &B2, const Vector &B1, const Vector &B0) {
  Vector B7_copy = B7;
  auto A1 = ~B7_copy | B6;
  auto A2 = B5 | B4;
  auto A3 = ~(B3 & B2);
  auto A4 = B1 | B0;

  auto A5 = A1 | A2;
  auto A6 = A3 | A4;

  Vector ret = ~(A5 | A6);
  return ret;
}