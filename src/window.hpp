#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/stacksidebar.h>
#include <gtkmm/stack.h>

class sysconfig : public Gtk::Window {
	public:
		sysconfig();

	private:
		Gtk::Box box_main;
		Gtk::StackSidebar stacksidebar_side;
		Gtk::Box box_buttons;
		Gtk::Stack stack_main;
};
