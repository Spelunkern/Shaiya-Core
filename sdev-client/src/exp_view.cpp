#include <util/util.h>
#include "include/main.h"

/// <summary>
/// The client was coded to display ten times the actual experience points. 
/// This code will make it show the actual value.
/// </summary>

void hook::exp_view()
{
    // Pass false to a function that appends '0' to exp text
    util::write_memory((void*)0x4963DE, 0x00, 1);
    util::write_memory((void*)0x496407, 0x00, 1);
    util::write_memory((void*)0x4F9852, 0x00, 1);
    util::write_memory((void*)0x529D05, 0x00, 1);
    util::write_memory((void*)0x529E19, 0x00, 1);
    util::write_memory((void*)0x594BA7, 0x00, 1);
    // Ignore exp multiplication
    // Note: this does not affect locale 0 or 1
    util::write_memory((void*)0x4FA494, 0x44, 1);
}
