#ifndef GLOBAL
#define GLOBAL

#include "types/xpainter_toolitem_id.h"

/* GUI */  
extern GtkWidget *window;
extern GtkWidget *canvas;
extern GtkWidget *fill_pattern_spin_button_widget;
extern GtkWidget *fill_pattern_spin_button_widget;
extern GdkPixbuf *fill_patterns[10];
extern GtkWidget *text_input_widget;

/* Types */
struct history{
  cairo_surface_t *surfaces[100];
  int current_index;
  int count;
};

typedef struct{
  guint16 red;
  guint16 green;
  guint16 blue;
  guint16 alpha;
} XPainterColor;

/* Functionality */
extern XPainterToolItemType current_tool;
extern int line_width;
extern XPainterColor color1, color2;
extern guint16 color1_alpha, color2_alpha;

extern cairo_surface_t *current_surface;
extern GdkPixbuf *current_surface_pixbuf;
extern guchar *pixbuf_pixels;

extern GdkPixbuf *clipboard_pixbuf;
extern double paste_x1,paste_y1,paste_x2,paste_y2;

extern char* filename;

/* Tools */
extern gboolean selection_is_on;
extern double selection_x1, selection_y1, selection_x2, selection_y2;
extern GdkPixbuf *selection_pixbuf;
extern double move_x1, move_y1;
extern gboolean figure_is_filled;
extern gboolean fill_with_pattern;

/* Dirty variables */
extern gboolean canvas_drawn;

extern struct history canvas_history;

void assign_current_tool(GtkWidget *widget, gpointer data);
void undo(GtkWidget *widget, gpointer data);
void redo(GtkWidget *widget, gpointer data);

void paint_current_surface_on_canvas(cairo_t *canvas_context);
void save_current_surface(cairo_surface_t *surface);
void save_current_surface_in_history();
void set_new_canvas(GtkWidget *widget, gpointer data);
void open_file(GtkWidget *widget, gpointer data);
void save_file(GtkWidget *widget, gpointer data);
void save_file_as(GtkWidget *widget, gpointer data);
void save_current_selection(GtkWidget *widget, gpointer data);
void show_error_message(char *message);
void set_pattern_from_selection(GtkWidget *widget, gpointer data);
void set_pattern_from_file(GtkWidget *widget, gpointer data);
#endif
