#include "color.hh"

TColorDefinitions ColorDefinitions;

/* **********************************************************************
   Initialize TColorDefinitions */
TColorDefinitions::TColorDefinitions() {
  Colors["white"] = TColor(1.0, 1.0, 1.0);
  Colors["red"]   = TColor(1.0, 0.0, 0.0);
  Colors["green"] = TColor(0.0, 1.0, 0.0);
  Colors["blue"]  = TColor(0.0, 0.0, 1.0);
  Colors["black"] = TColor(0.0, 0.0, 0.0);

  Colors["yellow"]  = TColor(1.0, 1.0, 0.0);
  Colors["cyan"]    = TColor(0.0, 1.0, 1.0);
  Colors["magenta"] = TColor(1.0, 0.0, 1.0);

  Colors["gray"]  = TColor(0.75, 0.75, 0.75);
}
