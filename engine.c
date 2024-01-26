// Include
#include "engine.h"

// Main function
int main()
{
  // Initialize
  Init();

  // Run debug tests
#ifdef DEBUG
  ConversionTest();
#endif

  // Return
  return 0;
}
