#include "syshud.hpp"
#include <gtkmm/stringlist.h>

page_syshud::page_syshud() : box_main(Gtk::Orientation::VERTICAL) {
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

	// Orientation
	listbox_main.append(box_orientation);
	box_orientation.get_style_context()->add_class("header");
	box_orientation.append(label_orientation);
	label_orientation.set_text("Vertical");
	label_orientation.set_hexpand(true);
	label_orientation.set_halign(Gtk::Align::START);
	box_orientation.append(switch_orientation);
	switch_orientation.set_valign(Gtk::Align::CENTER);

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
	box_size.append(entry_height);
	entry_height.set_max_width_chars(13);
	entry_height.set_alignment(0.5);
	entry_height.set_placeholder_text("Height");
	entry_height.set_valign(Gtk::Align::CENTER);

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
	scale_isize.add_mark(16, Gtk::PositionType::BOTTOM, "16");
	scale_isize.add_mark(24, Gtk::PositionType::BOTTOM, "24");
	scale_isize.add_mark(32, Gtk::PositionType::BOTTOM, "32");
	scale_isize.add_mark(48, Gtk::PositionType::BOTTOM, "48");
	scale_isize.add_mark(64, Gtk::PositionType::BOTTOM, "64");

	// Percentage
	listbox_main.append(box_percentage);
	box_percentage.get_style_context()->add_class("header");
	box_percentage.append(label_percentage);
	label_percentage.set_text("Show percentage");
	label_percentage.set_hexpand(true);
	label_percentage.set_halign(Gtk::Align::START);
	box_percentage.append(switch_percentage);
	switch_percentage.set_valign(Gtk::Align::CENTER);
}
