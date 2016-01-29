#include "UnpackerWriter.h"

#include "TEvent.h"
#include "TUnpackerMessage.h"
#include "TSlowControl.h"


#include "base/WrapTFile.h"
#include "base/Logger.h"

#include <algorithm>

using namespace std;
using namespace ant;
using namespace ant::tree;


UnpackerWriter::UnpackerWriter(const string& outputfile)
{
    file = std_ext::make_unique<WrapTFileOutput>(outputfile,WrapTFileOutput::mode_t::recreate, true);
    Event.Init();
    UnpackerMessage.Init();
    SlowControl.Init();
}

UnpackerWriter::~UnpackerWriter() {}

void UnpackerWriter::Fill(TDataRecord* record) noexcept
{
   if(Event.TryFill(record))
       return;
   if(UnpackerMessage.TryFill(record))
       return;
   if(SlowControl.TryFill(record))
       return;
}
