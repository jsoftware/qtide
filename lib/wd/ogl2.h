#ifndef OGL2_H
#define OGL2_H

#include "../base/base.h"

#define gl_arc_n   2101
#define gl_brush_n   2102
#define gl_brushnull_n   2103
#define gl_capture_n   2104
#define gl_caret_n   2105
#define gl_clear_n   2106
#define gl_clip_n   2107
#define gl_clipreset_n   2108
#define gl_cmds_n   2109
#define gl_cursor_n   2110
#define gl_ellipse_n   2111
#define gl_fill_n   2112
#define gl_font_n   2113
#define gl_font2_n   2114
#define gl_fontangle_n   2115
#define gl_lines_n   2116
#define gl_nodblbuf_n   2117
#define gl_paint_n   2118
#define gl_paintx_n   2119
#define gl_pen_n   2120
#define gl_pie_n   2121
#define gl_pixel_n   2122
#define gl_pixels_n   2123
#define gl_pixelsx_n   2124
#define gl_polygon_n   2125
#define gl_rect_n   2126
#define gl_rgb_n   2127
#define gl_rgba_n   2128
#define gl_sel_n   2129
#define gl_sel2_n   2130
#define gl_text_n   2131
#define gl_textcolor_n   2132
#define gl_textxy_n   2133
#define gl_waitgl_n   2134
#define gl_waitnative_n   2135
#define gl_windoworg_n   2136
#define gl_updategl_n   2137
#define gl_setlocale_n   2138
#define gl_qhandles_n   2139
#define gl_qwh_n   2140
#define gl_qpixelm_n   2141
#define gl_qpixels_n   2142
#define gl_qextent_n   2143
#define gl_qextentw_n   2144
#define gl_qtextmetrics_n   2145
#define gl_fontextent_n   2146
#define gl_qtype_n   2147

extern "C" {
  Dllexport int gl_arc (const int *p);
  Dllexport int gl_brush ();
  Dllexport int gl_brushnull ();
  Dllexport int gl_capture (int a);
  Dllexport int gl_caret (const int *p);
  Dllexport int gl_clear ();
  Dllexport int gl_clear2 (void *,int);
  Dllexport int gl_clip (const int *);
  Dllexport int gl_clipreset ();
  Dllexport int gl_cmds (const int *, const int);
  Dllexport int gl_cursor (int a);
  Dllexport int gl_ellipse (const int *p);
  Dllexport int gl_fill (const int *p);
  Dllexport int gl_font (char *face);
  Dllexport int gl_font2 (const int *p, int len);
  Dllexport int gl_fontangle (int a);
  Dllexport int gl_fontextent (char *face);
  Dllexport int gl_lines (const int *p, int len);
  Dllexport int gl_nodblbuf (int a);
  Dllexport int gl_paint ();
  Dllexport int gl_paintx ();
  Dllexport int gl_pen (const int *p);
  Dllexport int gl_pie (const int *p);
  Dllexport int gl_pixel (const int *p);
  Dllexport int gl_pixels (const int *, int);
  Dllexport int gl_pixelsx (const int *p);
  Dllexport int gl_polygon (const int *p, int len);
  Dllexport int gl_qextent (char *s,int *wh);
  Dllexport int gl_qextentw (char *s,int *w);
  Dllexport int gl_qhandles (void **p);
  Dllexport int gl_qpixels (const int *p,int *pix);
  Dllexport int gl_qpixelm (const int *p,int *shape,int *pix);
  Dllexport int gl_qtextmetrics (int *tm);
  Dllexport int gl_qwh (int *wh);
  Dllexport int gl_rect (const int *p);
  Dllexport int gl_rgb (const int *p);
  Dllexport int gl_rgba (const int *p);
  Dllexport int gl_sel (void *g);
  Dllexport int gl_sel2 (char *id);
  Dllexport int gl_setlocale (char *loc);
  Dllexport int gl_text (char *ys);
  Dllexport int gl_textcolor ();
  Dllexport int gl_textxy (const int *p);
  Dllexport int gl_windoworg (const int *p);

  Dllexport int gl_updategl(void *g);

  Dllexport int gl_query (int t, int *ptr, int ncnt);
  Dllexport int gl_query2 ( int t, int *ptr, int ncnt);
}

#endif
