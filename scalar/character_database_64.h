#pragma once

inline uint64_t match_0(uint64_t B7, uint64_t B6, uint64_t B5,
    uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | B2;
  uint64_t A4 = B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_1(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | B2;
  uint64_t A4 = B1 | ~B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_2(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | B2;
  uint64_t A4 = ~B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_3(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | B2;
  uint64_t A4 = ~(B1 & B0);

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_4(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | ~B2;
  uint64_t A4 = B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_5(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | ~B2;
  uint64_t A4 = B1 | ~B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_6(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | ~B2;
  uint64_t A4 = ~B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_7(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | ~B2;
  uint64_t A4 = ~(B1 & B0);

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_8(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = ~B3 | B2;
  uint64_t A4 = B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

inline uint64_t match_9(uint64_t B7, uint64_t B6, uint64_t B5,
                        uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = ~B3 | B2;
  uint64_t A4 = B1 | ~B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}

// TODO can it be made more efficient with NOR?
inline uint64_t match_0_9(uint64_t B7, uint64_t B6, uint64_t B5,
                          uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = ~(B7 | B6);
  uint64_t A2 = ~(B5 | B4);
  uint64_t A3 = ~(B3 & B2);
  uint64_t A4 = ~(B3 & B1);

  uint64_t A5 = A1 & A2;
  uint64_t A6 = A3 & A4;

  return A5 & A6;
}


inline uint64_t match_130(uint64_t B7, uint64_t B6, uint64_t B5,
                          uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = ~B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = B3 | B2;
  uint64_t A4 = ~B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}


inline uint64_t match_140(uint64_t B7, uint64_t B6, uint64_t B5,
                          uint64_t B4, uint64_t B3, uint64_t B2, uint64_t B1, uint64_t B0) {
  uint64_t A1 = ~B7 | B6;
  uint64_t A2 = B5 | B4;
  uint64_t A3 = ~B3 | ~B2;
  uint64_t A4 = B1 | B0;

  uint64_t A5 = A1 | A2;
  uint64_t A6 = A3 | A4;

  return ~(A5 | A6);
}