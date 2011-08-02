#include <gtk/gtk.h>
#include "mouse_handler.h"
#include "global.h"
#include "widgets_drawer.h"
#include "tools.h"

int main( int argc,
          char *argv[] )
{
  GtkWidget *window;
  GtkWidget *main_vbox;
  GtkWidget *menubar;
  GtkWidget *toolbar;

  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (gtk_main_quit), "WM destroy");
  gtk_window_set_title (GTK_WINDOW(window), "X Painter");
  gtk_widget_set_usize (GTK_WIDGET(window), 500, 700);
  
  main_vbox = gtk_vbox_new (FALSE, 1);
  gtk_container_border_width (GTK_CONTAINER (main_vbox), 1);
  gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_widget_show (main_vbox);
  
  /* top menu bar */
  get_main_menu (window, &menubar);
  gtk_box_pack_start (GTK_BOX (main_vbox), menubar, FALSE, TRUE, 0);
  
  /* toolbar */
  get_toolbar(window, &toolbar);  
  gtk_box_pack_start (GTK_BOX (main_vbox), toolbar, FALSE, TRUE, 0);
  
  /* canvas */  
  GtkWidget *canvas = gtk_drawing_area_new();
  current_surface_index = -1;
  //gtk_widget_set_app_paintable(canvas,TRUE);
  gtk_widget_set_has_window(canvas, TRUE);
  gtk_widget_set_size_request (canvas, 500, 500);      
    
  gtk_widget_add_events(canvas, GDK_ALL_EVENTS_MASK);
  g_signal_connect(canvas, "button-press-event", G_CALLBACK(handle_mouse), GINT_TO_POINTER(MOUSE_CLICK));
  g_signal_connect(canvas, "button-release-event", G_CALLBACK(handle_mouse), GINT_TO_POINTER(MOUSE_CLICK));
  g_signal_connect(canvas, "motion-notify-event",G_CALLBACK(handle_mouse), GINT_TO_POINTER(MOUSE_DRAG));
  g_signal_connect(canvas, "expose-event",G_CALLBACK(redraw_canvas), GINT_TO_POINTER(MOUSE_DRAG));
    
  gtk_box_pack_start (GTK_BOX (main_vbox), canvas, FALSE, TRUE, 0);
    
  gtk_widget_show_all (window);    
  //paint_white_canvas(cairo_get_target(gdk_cairo_create(canvas->window)),gdk_cairo_create(canvas->window));
  //save_surface_in_history(cairo_get_target(gdk_cairo_create(canvas->window)));
  gtk_main ();
      
  return(0);
}
/* example-end */
