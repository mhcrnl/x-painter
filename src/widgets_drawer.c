#include <gtk/gtk.h>
#include <strings.h>

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

static GtkItemFactoryEntry menu_items[] = {
  { "/Archivo",         NULL,         NULL, 0, "<Branch>" },
  { "/Archivo/Nuevo",     "<control>N", NULL, 0, NULL},
  { "/Archivo/Abrir",    "<control>O", NULL, 0, NULL },
  { "/Archivo/Guardar",    "<control>S", NULL, 0, NULL },
  { "/Archivo/Guardar como", NULL,         NULL, 0, NULL },
  { "/Archivo/separador",     NULL,         NULL, 0, "<Separator>" },
  { "/Archivo/Quit",     "<control>Q", gtk_main_quit, 0, NULL },
  { "/_Options",      NULL,         NULL, 0, "<Branch>" },
  { "/Options/Test",  NULL,         NULL, 0, NULL },
  { "/Ayuda",         NULL,         NULL, 0, "<LastBranch>" },
  { "/Ayuda/Acerca de",   NULL,         NULL, 0, NULL },
};


void get_main_menu( GtkWidget  *window,
                    GtkWidget **menubar )
{
  GtkItemFactory *item_factory;
  GtkAccelGroup *accel_group;
  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

  accel_group = gtk_accel_group_new ();

  /* This function initializes the item factory.
     Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
              or GTK_TYPE_OPTION_MENU.
     Param 2: The path of the menu.
     Param 3: A pointer to a gtk_accel_group.  The item factory sets up
              the accelerator table while generating menus.
  */

  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
                                       accel_group);

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
