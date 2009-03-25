/*
 * gtk.c: GTK front end for my puzzle collection.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <edrawable.h>
#include "puzzles.h"
#include "frontend.h"

/* ----------------------------------------------------------------------
 * GTK front end to puzzles.
 */


struct font {
    int type;
    int size;
};

void e_start_draw(void *handle)
{
    struct frontend * fe = (struct frontend *) handle;
    Ewl_Drawable *d = fe->area;

    printf("e_start_draw()\n");
    ewl_drawable_draw_rectangle_fill(d, 0, 0, fe->w, fe->h);
    fe->bbox_l = fe->w;
    fe->bbox_r = 0;
    fe->bbox_u = fe->h;
    fe->bbox_d = 0;
}

void e_clip(void *handle, int x, int y, int w, int h)
{
    frontend *fe = (frontend *)handle;

    printf("e_clip(%d,%d,%d,%d)\n", x, y, w, h);
    ewl_drawable_set_clip(fe->area, x, y, w, h);
}

void e_unclip(void *handle)
{
    frontend *fe = (frontend *)handle;
    printf("e_unclup()\n");
    ewl_drawable_reset_clip(fe->area);
}

void e_draw_text(void *handle, int x, int y, int fonttype, int fontsize,
		   int align, int color, char *text)
{
    frontend *fe = (frontend *)handle;
    int i;
    printf("e_draw_text(...,%d, %d, \"%s\")\n", x, y, text);
	const char * fontname = (fonttype == FONT_FIXED ? "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf" : "/usr/share/fonts/truetype/liberation//LiberationSans-Regular.ttf");
    gui_apply_color(fe, color);
    ewl_drawable_draw_text(fe->area, x, y, fontname, fontsize, text);
}

void e_draw_rect(void *handle, int x, int y, int w, int h, int color)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_rect()\n");
    gui_apply_color(fe, color);
    ewl_drawable_draw_rectangle(fe->area, x, y, w, h);
}

void e_draw_line(void *handle, int x1, int y1, int x2, int y2, int color)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_line()\n");
    gui_apply_color(fe, color);
    ewl_drawable_draw_line(fe->area, x1, y1, x2, y2);
}

void e_draw_poly(void *handle, int *coords, int npoints,
		   int fillcolor, int outlinecolor)
{
    frontend *fe = (frontend *)handle;
    EDrawablePolygon *p;
    int i;

    printf("e_draw_poly()\n");

    p = ewl_drawable_polygon_new();
    for (i = 0; i < npoints; i++) {
          ewl_drawable_polygon_add(p, coords[i*2], coords[i*2+1]);
//        points[i].x = coords[i*2];
//        points[i].y = coords[i*2+1];
    }

    if (fillcolor >= 0) {
        gui_apply_color(fe, fillcolor);
        ewl_drawable_draw_polygon_fill(fe->area, p);
    }
    assert(outlinecolor >= 0);
    gui_apply_color(fe, outlinecolor);
    ewl_drawable_draw_polygon(fe->area, p);
    ewl_drawable_polygon_delete(p);

    /*
     * In principle we ought to be able to use gdk_draw_polygon for
     * the outline as well. In fact, it turns out to interact badly
     * with a clipping region, for no terribly obvious reason, so I
     * draw the outline as a sequence of lines instead.
     */
#if 0
    for (i = 0; i < npoints; i++)
	gdk_draw_line(fe->pixmap, fe->gc,
		      points[i].x, points[i].y,
		      points[(i+1)%npoints].x, points[(i+1)%npoints].y);

    sfree(points);
#endif
}

void e_draw_circle(void *handle, int cx, int cy, int radius,
		     int fillcolor, int outlinecolor)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_circle()\n");
    if (fillcolor >= 0) {
        gui_apply_color(fe, fillcolor);
        ewl_drawable_draw_ellipse_filled(fe, cx, cy, radius, radius);
    }

    assert(outlinecolor >= 0);
    gui_apply_color(fe, outlinecolor);
    ewl_drawable_draw_ellipse(fe, cx, cy, radius, radius);
}

struct blitter {
    Evas *evas;
    int w, h, x, y;
};

blitter *e_blitter_new(void *handle, int w, int h)
{
    printf("e_blitter_new()\n");
#if 0
    /*
     * We can't create the pixmap right now, because fe->window
     * might not yet exist. So we just cache w and h and create it
     * during the firs call to blitter_save.
     */
    blitter *bl = snew(blitter);
    bl->pixmap = NULL;
    bl->w = w;
    bl->h = h;
    return bl;
#endif
}

void e_blitter_free(void *handle, blitter *bl)
{
    printf("e_blitter_free()\n");
#if 0
    if (bl->pixmap)
        gdk_pixmap_unref(bl->pixmap);
    sfree(bl);
#endif
}

void e_blitter_save(void *handle, blitter *bl, int x, int y)
{
    printf("e_blitter_save()\n");
#if 0
    frontend *fe = (frontend *)handle;
    if (!bl->pixmap)
        bl->pixmap = gdk_pixmap_new(fe->area->window, bl->w, bl->h, -1);
    bl->x = x;
    bl->y = y;
    gdk_draw_pixmap(bl->pixmap,
                    fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
                    fe->pixmap,
                    x, y, 0, 0, bl->w, bl->h);
#endif
}

void e_blitter_load(void *handle, blitter *bl, int x, int y)
{
    printf("e_blitted_load()\n");
#if 0
    frontend *fe = (frontend *)handle;
    assert(bl->pixmap);
    if (x == BLITTER_FROMSAVED && y == BLITTER_FROMSAVED) {
        x = bl->x;
        y = bl->y;
    }
    gdk_draw_pixmap(fe->pixmap,
                    fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
                    bl->pixmap,
                    0, 0, x, y, bl->w, bl->h);
#endif
}

void e_draw_update(void *handle, int x, int y, int w, int h)
{
    frontend *fe = (frontend *)handle;
    printf("e_draw_update(%d,%d,%d,%d)\n",x,y,w,h);
    if (fe->bbox_l > x  ) fe->bbox_l = x  ;
    if (fe->bbox_r < x+w) fe->bbox_r = x+w;
    if (fe->bbox_u > y  ) fe->bbox_u = y  ;
    if (fe->bbox_d < y+h) fe->bbox_d = y+h;
}

void e_end_draw(void *handle)
{
    frontend *fe = (frontend *)handle;
    printf("e_end_draw()\n");
    ewl_drawable_commit(fe->area);
#if 0
    gdk_gc_unref(fe->gc);
    fe->gc = NULL;

    if (fe->bbox_l < fe->bbox_r && fe->bbox_u < fe->bbox_d) {
	gdk_draw_pixmap(fe->area->window,
			fe->area->style->fg_gc[GTK_WIDGET_STATE(fe->area)],
			fe->pixmap,
                        fe->bbox_l, fe->bbox_u,
                        fe->ox + fe->bbox_l, fe->oy + fe->bbox_u,
                        fe->bbox_r - fe->bbox_l, fe->bbox_d - fe->bbox_u);
    }
#endif
}

void
e_status_bar(void *handler, char *text) {
    struct frontend *fe = (struct frontend *) handler;
    ewl_statusbar_pop(fe->statusbar);
    ewl_statusbar_push(fe->statusbar, text);
}

const struct drawing_api e_drawing_api = {
    e_draw_text,
    e_draw_rect,
    e_draw_line,
    e_draw_poly,
    e_draw_circle,
    e_draw_update,
    e_clip,
    e_unclip,
    e_start_draw,
    e_end_draw,
    e_status_bar,
    e_blitter_new,
    e_blitter_free,
    e_blitter_save,
    e_blitter_load,
    NULL, NULL, NULL, NULL, NULL, NULL, /* {begin,end}_{doc,page,puzzle} */
    NULL,			       /* line_width */
};

