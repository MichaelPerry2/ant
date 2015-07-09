#ifndef TDATARECORD_H
#define TDATARECORD_H

#include "base/printable.h"

#include "Rtypes.h"

#ifdef __CINT__
// simulate minimal cstdint for ROOTCINT
namespace std {
typedef UChar_t    uint8_t;
typedef UInt_t     uint32_t;
typedef ULong64_t  uint64_t;
typedef Long64_t   int64_t;
}
#else
#include <cstdint>
#endif // __CINT__

#include <string>
#include <iomanip>
#include <ctime>


// if we want to change something
// of the data format defined by the ant::T* classes
#define ANT_UNPACKER_ROOT_VERSION 1

namespace ant {

struct TDataRecord : printable_traits
{
  struct ID_t  : printable_traits {
    // you may append flags, but never remove or change order!
    enum class Flags_t : unsigned {
      MC
    };

    ID_t() : Value(0), Flags(0) {}

    ID_t(
        UInt_t upper,
        UInt_t lower,
        bool isMC = false
        ) {
      Flags = 0;
      Flags |= static_cast<decltype(Flags)>(isMC) << static_cast<unsigned>(Flags_t::MC);
      Value = lower;
      Value |= static_cast<decltype(Value)>(upper) << sizeof(std::uint32_t)*8;
    }

    std::uint64_t Value;
    std::uint32_t Flags;

    virtual std::ostream& Print( std::ostream& s) const override {
      return s << std::hex << "(flags=0x" << Flags << ",0x"
               << std::setw(sizeof(decltype(Value))*2) << std::setfill('0')
               << Value
               << ")" << std::dec;
    }


  }; // ID_t


  TDataRecord() {}
  TDataRecord(const ID_t& id) : ID(id) {}
  virtual ~TDataRecord() {}

  ID_t ID;

  virtual std::ostream& Print( std::ostream& s) const override {
    return s << "TDataRecord ID=" << ID;
  }

  ClassDef(TDataRecord, ANT_UNPACKER_ROOT_VERSION)

};


} // namespace ant


#endif // TDATARECORD_H
