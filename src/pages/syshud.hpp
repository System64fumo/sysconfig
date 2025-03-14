#include "../inimanager.hpp"

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/entry.h>
#include <gtkmm/scale.h>
#include <gtkmm/switch.h>
#include <gtkmm/window.h>
#include <gtkmm/listbox.h>
#include <gtkmm/dropdown.h>
#include <gtkmm/revealer.h>
#include <gtkmm/scrolledwindow.h>

class page_syshud : public Gtk::ScrolledWindow {
	public:
		page_syshud(Gtk::Window*);

		ini_manager config_syshud;

	private:
		Gtk::Window* main_window;
		std::map<int, std::string> positions;
		std::array<std::pair<Gtk::Align, Gtk::Align>, 9> alignments;

		// Preview
		Gtk::Revealer revealer_syshud;
		Gtk::Box box_syshud;
		Gtk::Image image_syshud_icon;
		Gtk::Scale scale_syshud;
		Gtk::Label label_syshud_percentage;

		Gtk::Box box_main;
		Gtk::ScrolledWindow scrolledwindow_preview;

		Gtk::ListBox listbox_main;

		Gtk::Box box_position;
		Gtk::Label label_position;
		Gtk::DropDown dropdown_position;

		Gtk::Box box_orientation;
		Gtk::Label label_orientation;
		Gtk::Switch switch_orientation;

		Gtk::Box box_size;
		Gtk::Label label_size;
		Gtk::Entry entry_width;
		Gtk::Entry entry_height;

		Gtk::Box box_isize;
		Gtk::Label label_isize;
		Gtk::Scale scale_isize;

		Gtk::Box box_percentage;
		Gtk::Label label_percentage;
		Gtk::Switch switch_percentage;

		void setup_ui();
		void setup_preview();
		void setup_actions();
		void load_css();
};
