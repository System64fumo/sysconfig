#include "syshud.hpp"
#include <gtkmm/stringlist.h>

page_syshud::page_syshud() : box_main(Gtk::Orientation::VERTICAL) {
	config_syshud.load(std::string(getenv("HOME")) + "/.config/sys64/hud/config.conf");
	setup_ui();
	setup_actions();
}

void page_syshud::setup_ui() {
	set_hexpand(true);
	set_vexpand(true);
	set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

	set_child(box_main);
	box_main.set_halign(Gtk::Align::CENTER);
	listbox_main.set_size_request(480, -1);

	box_main.append(box_preview);
	box_preview.get_style_context()->add_class("card");
	box_preview.set_size_request(-1, 260);
	box_preview.set_margin(20);

	box_main.append(listbox_main);
	listbox_main.get_style_context()->add_class("boxed-list");
	listbox_main.set_selection_mode(Gtk::SelectionMode::NONE);
	listbox_main.set_halign(Gtk::Align::CENTER);
	listbox_main.set_margin(20);

	// Position
	listbox_main.append(box_position);
	box_position.get_style_context()->add_class("header");
	box_position.append(label_position);
	label_position.set_text("Position");
	label_position.set_hexpand(true);
	label_position.set_halign(Gtk::Align::START);
	box_position.append(dropdown_position);
	dropdown_position.set_valign(Gtk::Align::CENTER);
	dropdown_position.set_show_arrow(false);
	auto list_model = Gtk::StringList::create({ // TODO: Replace this with a more intuitive UI
		"Top Left",
		"Top",
		"Top Right",
		"Left",
		"Center",
		"Right",
		"Bottom Left",
		"Bottom",
		"Bottom Right"
	});
	dropdown_position.set_model(list_model);
	// TODO: Add position reading

	// Orientation
	listbox_main.append(box_orientation);
	box_orientation.get_style_context()->add_class("header");
	box_orientation.append(label_orientation);
	label_orientation.set_text("Vertical");
	label_orientation.set_hexpand(true);
	label_orientation.set_halign(Gtk::Align::START);
	box_orientation.append(switch_orientation);
	switch_orientation.set_valign(Gtk::Align::CENTER);
	switch_orientation.set_active(config_syshud.data["main"]["orientation"] == "v");

	// Size
	listbox_main.append(box_size);
	box_size.get_style_context()->add_class("header");
	box_size.append(label_size);
	label_size.set_text("Size");
	label_size.set_hexpand(true);
	label_size.set_halign(Gtk::Align::START);
	box_size.append(entry_width);
	entry_width.set_max_width_chars(13);
	entry_width.set_alignment(0.5);
	entry_width.set_placeholder_text("Width");
	entry_width.set_valign(Gtk::Align::CENTER);
	entry_width.set_text(config_syshud.data["main"]["width"]);
	box_size.append(entry_height);
	entry_height.set_max_width_chars(13);
	entry_height.set_alignment(0.5);
	entry_height.set_placeholder_text("Height");
	entry_height.set_valign(Gtk::Align::CENTER);
	entry_height.set_text(config_syshud.data["main"]["height"]);

	// Icon size
	listbox_main.append(box_isize);
	box_isize.get_style_context()->add_class("header");
	box_isize.append(label_isize);
	label_isize.set_text("Icon size");
	label_isize.set_hexpand(true);
	label_isize.set_halign(Gtk::Align::START);
	box_isize.append(scale_isize);
	scale_isize.set_valign(Gtk::Align::CENTER);
	scale_isize.set_hexpand(true);
	scale_isize.set_range(16, 64);
	scale_isize.set_round_digits(0);
	scale_isize.add_mark(16, Gtk::PositionType::BOTTOM, "16");
	scale_isize.add_mark(24, Gtk::PositionType::BOTTOM, "24");
	scale_isize.add_mark(32, Gtk::PositionType::BOTTOM, "32");
	scale_isize.add_mark(48, Gtk::PositionType::BOTTOM, "48");
	scale_isize.add_mark(64, Gtk::PositionType::BOTTOM, "64");
	scale_isize.set_value(std::stod(config_syshud.data["main"]["icon-size"]));

	// Percentage
	listbox_main.append(box_percentage);
	box_percentage.get_style_context()->add_class("header");
	box_percentage.append(label_percentage);
	label_percentage.set_text("Show percentage");
	label_percentage.set_hexpand(true);
	label_percentage.set_halign(Gtk::Align::START);
	box_percentage.append(switch_percentage);
	switch_percentage.set_valign(Gtk::Align::CENTER);
	switch_percentage.set_active(config_syshud.data["main"]["show-percentage"] == "true");
}

void page_syshud::setup_actions() {
	// TODO: VERY IMPORTANT!!!
	// Try to reduce disk write cycles, aka don't update until the last change or add a timeout.
	// Please.. Otherwise R.I.P disk.

	// Position
	dropdown_position.property_selected().signal_changed().connect([&]() {
		std::map<int, std::string> positions = {
			{0, "top-left"},
			{1, "top"},
			{2, "top-right"},
			{3, "left"},
			{4, ""},
			{5, "right"},
			{6, "bottom-left"},
			{7, "bottom"},
			{8, "bottom-right"},
		};
		config_syshud.data["main"]["position"] = positions[dropdown_position.get_selected()];
		config_syshud.save();
	});

	// Orientation
	switch_orientation.signal_state_set().connect([&](bool state) {
		config_syshud.data["main"]["orientation"] = state ? "v" : "h";
		config_syshud.save();
		return false;
	}, false);

	// Size
	entry_width.signal_changed().connect([&]() {
		config_syshud.data["main"]["width"] = entry_width.get_text();
		config_syshud.save();
	});
	entry_height.signal_changed().connect([&]() {
		config_syshud.data["main"]["height"] = entry_height.get_text();
		config_syshud.save();
	});

	// Icon size
	scale_isize.signal_value_changed().connect([&]() {
		std::string isize = std::to_string(scale_isize.get_value());
		isize.erase (isize.find_last_not_of('0') + 1, std::string::npos);
		isize.erase (isize.find_last_not_of('.') + 1, std::string::npos);
		config_syshud.data["main"]["icon-size"] = isize;
		config_syshud.save();
	});

	// Percentage
	switch_percentage.signal_state_set().connect([&](bool state) {
		config_syshud.data["main"]["show-percentage"] = state ? "true" : "false";
		config_syshud.save();
		return false;
	}, false);
}
