#include <gtk/gtk.h>

static void click_cb(GtkButton *btn, GtkWindow *win) {
  gtk_window_destroy(win);
}

static void app_activate(GApplication *app) {
  GtkWidget *win;
  GtkWidget *box;
  GtkWidget *lab;
  GtkWidget *btn;

  // Initialize the main window
  win = gtk_application_window_new(GTK_APPLICATION(app));
  gtk_window_set_title(GTK_WINDOW(win), "Hello, World!");
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);

  // initialize a simple label
  lab = gtk_label_new("hello");

  // Initialize a box
  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
  gtk_window_set_child(GTK_WINDOW(win), box);

  // Initialize a button
  btn = gtk_button_new_with_label("Click me");
  g_signal_connect(btn, "clicked", G_CALLBACK(click_cb), win);

  gtk_box_append(GTK_BOX(box), lab);
  gtk_box_append(GTK_BOX(box), btn);

  gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int stat;

  app = gtk_application_new("com.github.guilherssousa.pr1",
                            G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
  stat = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return stat;
}
