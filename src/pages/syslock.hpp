#include "../inimanager.hpp"

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/scale.h>
#include <gtkmm/switch.h>
#include <gtkmm/window.h>
#include <gtkmm/listbox.h>
#include <gtkmm/overlay.h>
#include <gtkmm/scrolledwindow.h>

class page_syslock : public Gtk::ScrolledWindow {
	public:
		page_syslock(Gtk::Window*);

		ini_manager config_syslock;

	private:
		Gtk::Window* main_window;

		// Preview
		Gtk::Box box_syslock;
		Gtk::Overlay overlay_syslock;
		Gtk::Box box_syslock_lockscreen;
		Gtk::Box box_syslock_overlay;
		Gtk::Label label_syslock_time;
		Gtk::Label label_syslock_date;

		Gtk::Box box_main;
		Gtk::ScrolledWindow scrolledwindow_preview;

		Gtk::ListBox listbox_main;

		Gtk::Box box_startunlocked;
		Gtk::Label label_startunlocked;
		Gtk::Switch switch_startunlocked;

		Gtk::Box box_keypad;
		Gtk::Label label_keypad;
		Gtk::Switch switch_keypad;

		Gtk::Box box_pwlength;
		Gtk::Label label_pwlength;
		Gtk::Scale scale_pwlength;

		void setup_ui();
		void setup_preview();
		void setup_actions();
		void load_css();
};
