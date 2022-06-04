#pragma once

namespace generic{
  /*
  `SIText` is a category class that serves as a wrapper for basic text functionality within `std`.
  Code that uses the functionality can reference the functionality using an interface or template, 
  so the same code can be repurposed to work with other data types by writing an analogous *Math class for that data type.

  This is opposed to using a namespace to store functionality, where the code that references the functionality 
  must do so without specifying the namespace to allow reuse for other data types, 
  which in turn causes other referencing issues.
  */
  template <typename T1>
  class SIText{
    template <int M1, int KG1, int S1, int K1, int MOL1, int A1, int CD1>
    std::string to_string(const units<M1,KG1,S1,K1,MOL1,A1,CD1,T1> a)
    {
      return a.to_string();
    }
  }
}