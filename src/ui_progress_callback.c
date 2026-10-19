#include "ui_progress_callback.h"

void ui_progress_callback(float pct) {
    // pct is 0.0 → 1.0
    // Left empty for now: this runs inside the timed loop, so drawing
    // here would be counted as benchmark time.
    (void)pct;
}
