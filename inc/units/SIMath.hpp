#pragma once

namespace si{
  template <typename T1>
  class SIMath{
    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    constexpr auto sqrt(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
    {
      return a.sqrt();
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    constexpr auto abs(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
    {
      return a > units<M1,KG1,S1,K1,MOL1,A1,CD1,T1>(0)? a : a.multiply(-1.0);
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    constexpr auto min(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
    {
      return a < b? a : b;
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    constexpr auto max(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> b)
    {
      return a > b? a : b;
    }

    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    constexpr auto clamp(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> lo, const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> hi)
    {
      return a < lo? lo : a > hi? hi : a;
    }

  }
}