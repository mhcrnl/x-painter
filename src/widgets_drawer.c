#include <gtk/gtk.h>
#include <strings.h>
#include "types/xpainter_toolitem.h"
#include "global.h"
#include "mouse_handler.h"
#include "tools.h"
#include <string.h> /* memset */
//#define _GNU_SOURCE
#include <stdio.h>

/* This is the GtkItemFactoryEntry structure used to generate new menus.
   Item 1: The menu path. The letter after the underscore indicates an
           accelerator key once the menu is open.
   Item 2: The accelerator key for the entry
   Item 3: The callback function.
   Item 4: The callback action.  This changes the parameters with
           which the function is called.  The default is 0.
   Item 5: The item type, used to define what kind of an item it is.
           Here are the possible values:
*/

struct history canvas_history;
extern GtkLabel *coordinates_label;

static GtkItemFactoryEntry menu_items[] = {
  { "/Archivo", NULL, NULL, 0, "<Branch>" },
  { "/Archivo/Nuevo", "<control>N", NULL, 0, NULL},
  { "/Archivo/Abrir", "<control>O", NULL, 0, NULL },
  { "/Archivo/Guardar", "<control>S", NULL, 0, NULL },
  { "/Archivo/Guardar como", NULL, NULL, 0, NULL },
  { "/Archivo/separador", NULL, NULL, 0, "<Separator>" },
  { "/Archivo/Salir", "<control>Q", gtk_main_quit, 0, NULL },
  { "/Ayuda", NULL, NULL, 0, "<LastBranch>" },
  { "/Ayuda/Acerca de", NULL, NULL, 0, NULL },
};

void get_main_menu( GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.*/

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", accel_group);

  /* This function generates the menu items. Pass the item factory,
     the number of items in the array, the array itself, and any
     callback data for the the menu items. */
  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

  /* Attach the new accelerator group to the window. */
  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

  if (menubar)
    /* Finally, return the actual menu bar created by the item factory. */ 
    *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
}

//GTK_STOCK_COLOR_PICKER
//GTK_STOCK_CUT
//GTK_STOCK_ZOOM_IN
//GTK_STOCK_ZOOM_OUT
static XPainterToolItem toolbar_item_icons[] = {
  { "../icons/icon_select.png", "Seleccionar", XPainter_SELECT_TOOL },
  { "../icons/icon_move.png", "Mover", XPainter_MOVE_TOOL },
  { "../icons/icon_undo.png", "Deshacer", XPainter_UNDO_TOOL},
  { "../icons/icon_redo.png", "Rehacer", XPainter_REDO_TOOL},
  { "../icons/icon_line.png", "Línea", XPainter_LINE_TOOL },
  { "../icons/icon_circle.png", "Círculo", XPainter_CIRCLE_TOOL },
  { "../icons/icon_ellipse.png", "Elipse", XPainter_ELLIPSE_TOOL },
  { "../icons/icon_rectangle.png", "Rectangulo", XPainter_RECTANGLE_TOOL },
  { "../icons/icon_polygon.png", "Polígono", XPainter_POLYGON_TOOL },
  { "../icons/icon_flood.png", "Rellenar", XPainter_FLOOD_TOOL },
  { "../icons/icon_text.png", "Texto", XPainter_TEXT_TOOL },
  { "../icons/icon_eraser.png", "Borrador", XPainter_ERASER_TOOL },
  { "../icons/icon_spray.png", "Spray", XPainter_SPRAY_TOOL },
  { "../icons/icon_brush.png", "Brush", XPainter_BRUSH_TOOL},
  { "../icons/icon_pencil.png", "Lapicero", XPainter_PEN_TOOL},
  { "../icons/icon_zoom.png", "Zoom", XPainter_ZOOM_TOOL}
};

void get_toolbar(GtkWidget *window, GtkWidget **toolbar){
  GtkToolItem *new_tool_item;
  GtkWidget *new_tool_item_icon;

  *toolbar = gtk_toolbar_new();
  //gtk_toolbar_set_style(GTK_TOOLBAR(*toolbar), GTK_TOOLBAR_BOTH);
  gtk_toolbar_set_style(GTK_TOOLBAR(*toolbar), GTK_TOOLBAR_ICONS);
  
  int i;
  int nicons = sizeof toolbar_item_icons / sizeof (*toolbar_item_icons);
  for (i = 0; i < nicons; i++){  
    new_tool_item_icon = gtk_image_new_from_file(toolbar_item_icons[i].path_to_image);    
    new_tool_item = gtk_tool_button_new(new_tool_item_icon,toolbar_item_icons[i].name);
    
    XPainterToolItemType tool_type = toolbar_item_icons[i].type;
    switch(tool_type){
    case XPainter_UNDO_TOOL:
      g_signal_connect(new_tool_item, "clicked", G_CALLBACK(undo), NULL);   
      break;
    case XPainter_REDO_TOOL:
      g_signal_connect(new_tool_item, "clicked", G_CALLBACK(redo), NULL);   
      break;
    default: 
      g_signal_connect(new_tool_item, "clicked", G_CALLBACK(assign_current_tool), (gpointer) tool_type);
      break;
    }
    gtk_toolbar_insert(GTK_TOOLBAR(*toolbar), new_tool_item, -1);
    gtk_widget_set_can_focus ((GtkWidget*) new_tool_item, TRUE);
  }
}

gboolean redraw_canvas(GtkWidget *widget, gpointer userdata){
  //is the drawing area initialized?
  if (!canvas_drawn){
    canvas_drawn = TRUE;
    canvas_history.current_index = -1;
    return FALSE;
  }
  else{
    //it has been initialized, now we are either in a redrawing or a new canvas escenario
    cairo_t *cr = gdk_cairo_create(widget->window);

    //if (current_surface_index < 0){ //new canvas escenario, paint it white
    if (canvas_history.current_index < 0){ //new canvas escenario, paint it white
      line_width = 1;
      cairo_set_source_rgb(cr, 255, 255, 255);
      cairo_paint(cr);
      save_current_surface(cairo_get_target(cr));
      save_current_surface_in_history();
    }else{      
      //redraw
      paint_current_surface_on_canvas(cr);
    }
    return TRUE;
  }  
}

gboolean update_coordinates_label(GtkWidget *widget, void *e, gpointer *t){  
  GdkEventMotion *event = (GdkEventMotion*) e;
  char *s = NULL;
  asprintf (&s, "x,y = %f,%f", event->x,event->y);
  gtk_label_set_text(coordinates_label,s);    
  return FALSE; //propagate next events
}

void adjust_line_width(GtkRange *range,gpointer user_data){  
  line_width = (int) gtk_range_get_value(range);
}

void add_line_width_widget_to(GtkContainer *box){
  /* add label */
  GtkWidget *line_width_label = gtk_label_new("Ancho de pixel:");  
  gtk_misc_set_alignment(GTK_MISC(line_width_label),0,0.5);  
  
  GtkWidget *line_width_widget;
  line_width_widget = gtk_hscale_new_with_range(1,10,1);

  gtk_widget_set_usize (GTK_WIDGET(line_width_widget), 200, 45);
  
  int i;
  char *s = NULL;  
  for (i = 0; i <= 10; i++){
    asprintf (&s, "%d", i);
    gtk_scale_add_mark(GTK_SCALE(line_width_widget),(double)i,GTK_POS_LEFT,s);
  }
  
  g_signal_connect(line_width_widget, "value-changed",G_CALLBACK(adjust_line_width), NULL);
  
  gtk_box_pack_end (GTK_BOX (box), line_width_widget, FALSE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX (box), line_width_label, FALSE, TRUE, 0);
}

GdkColor get_gdk_color_from_xpainter_color(XPainterColor color){
  GdkColor dest_color;
  dest_color.red = color.red*257;
  dest_color.green = color.green*257;
  dest_color.blue = color.blue*257;
  return dest_color;
}

XPainterColor get_xpainter_color_from_gdk_color(GdkColor color){
  XPainterColor dest_color;
  dest_color.red = color.red/257;
  dest_color.green = color.green/257;
  dest_color.blue = color.blue/257;
  return dest_color;
}

void adjust_color(GtkColorButton *widget,gpointer data){  
  int color_number = GPOINTER_TO_INT(data);  
  GdkColor gdk_chosen_color;

  gtk_color_button_get_color(widget,&gdk_chosen_color);
  
  if (color_number==1)
    color1 = get_xpainter_color_from_gdk_color(gdk_chosen_color);
  else
    color2 = get_xpainter_color_from_gdk_color(gdk_chosen_color);
}

void add_color_widgets_to(GtkContainer *box){
  /* color 1 */
  GtkWidget *color1_label = gtk_label_new("Color 1 ");  
  gtk_misc_set_alignment(GTK_MISC(color1_label),0,0.5);  
  
  //initial colors  
  color1.red = 65535;
  color1.blue = 51143;

  color2.green = 65535;
  color2.blue = 65535;
  
  GdkColor gdk_color1 = get_gdk_color_from_xpainter_color(color1);
  GdkColor gdk_color2 = get_gdk_color_from_xpainter_color(color2);

  GtkWidget *color1_widget = gtk_color_button_new_with_color(&gdk_color1);
  g_signal_connect(color1_widget, "color-set",G_CALLBACK(adjust_color), GINT_TO_POINTER(1));
    
  /* color 2 */
  GtkWidget *color2_label = gtk_label_new("Color 2 ");  
  gtk_misc_set_alignment(GTK_MISC(color2_label),0,0.5);  
  
  GtkWidget *color2_widget = gtk_color_button_new_with_color(&gdk_color2);  
  g_signal_connect(color2_widget, "color-set",G_CALLBACK(adjust_color), GINT_TO_POINTER(2));  

  /* add color 2 */
  gtk_box_pack_end (GTK_BOX (box), color2_widget, FALSE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX (box), color2_label, FALSE, TRUE, 0);

  /* add color 1 */
  gtk_box_pack_end (GTK_BOX (box), color1_widget, FALSE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX (box), color1_label, FALSE, TRUE, 0);


  
}
