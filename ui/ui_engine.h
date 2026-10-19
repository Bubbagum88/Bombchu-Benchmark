#ifndef UI_ENGINE_H
#define UI_ENGINE_H

#include "benchmark.h"

void ui_clear_screen(void);
void ui_draw_text(int x, int y, const char *msg);
void ui_draw_menu_item(const char *label, int x, int y, int selected);
void ui_draw_menu(const char **items, int count, int selected);
void ui_draw_title(const char *title);
void ui_show_results_screen(const BenchmarkResult *result);

#endif
