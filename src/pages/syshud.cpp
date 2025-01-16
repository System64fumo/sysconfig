#include "syshud.hpp"
#include <gtkmm/stringlist.h>
#include <gtkmm/cssprovider.h>
#include <filesystem>

page_syshud::page_syshud(Gtk::Window* win) : main_window(win), box_main(Gtk::Orientation::VERTICAL) {
	config_syshud.load(std::string(getenv("HOME")) + "/.config/sys64/hud/config.conf");
	positions[0] = "top-left";
	positions[1] = "top";
	positions[2] = "top-right";
	positions[3] = "left";
	positions[4] = "";
	positions[5] = "right";
	positions[6] = "bottom-left";
	positions[7] = "bottom";
	positions[8] = "bottom-right";

	alignments = {{
		{Gtk::Align::START, Gtk::Align::START},
		{Gtk::Align::CENTER, Gtk::Align::START},
		{Gtk::Align::END, Gtk::Align::START},
		{Gtk::Align::START, Gtk::Align::CENTER},
		{Gtk::Align::CENTER, Gtk::Align::CENTER},
		{Gtk::Align::END, Gtk::Align::CENTER},
		{Gtk::Align::START, Gtk::Align::END},
		{Gtk::Align::CENTER, Gtk::Align::END},
		{Gtk::Align::END, Gtk::Align::END}
	}};

	setup_ui();
	setup_preview();
	setup_actions();
	load_css();
}

void page_syshud::setup_ui() {
	set_hexpand(true);
	set_vexpand(true);
	set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

	set_child(box_main);
	box_main.set_halign(Gtk::Align::CENTER);
	box_main.set_valign(Gtk::Align::START);
	listbox_main.set_size_request(480, -1);

	box_main.append(scrolledwindow_preview);
	scrolledwindow_preview.get_style_context()->add_class("card");
	scrolledwindow_preview.set_size_request(-1, 260);
	scrolledwindow_preview.set_margin(20);

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
		"↖ Top Left",
		"↑ Top",
		"↗ Top Right",
		"← Left",
		"• Center",
		"→ Right",
		"↙ Bottom Left",
		"↓ Bottom",
		"↘ Bottom Right"
	});
	dropdown_position.set_model(list_model);
	int val;
	for (const auto& kv : positions) {
		if (kv.second == config_syshud.data["main"]["position"]) {
			val = kv.first;
			break;
		}
	}
	dropdown_position.set_selected(val);

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

void page_syshud::setup_preview() {
	scrolledwindow_preview.set_child(revealer_syshud);
	revealer_syshud.set_child(box_syshud);
	revealer_syshud.set_name("syshud");
	revealer_syshud.set_reveal_child(true);
	box_syshud.get_style_context()->add_class("box_layout");
	// TODO: Make preview switch orientation of icon and label
	box_syshud.append(image_syshud_icon);
	box_syshud.append(scale_syshud);
	box_syshud.append(label_syshud_percentage);

	revealer_syshud.set_halign(alignments[dropdown_position.get_selected()].first);
	revealer_syshud.set_valign(alignments[dropdown_position.get_selected()].second);

	scale_syshud.set_hexpand(!switch_orientation.get_state());
	scale_syshud.set_vexpand(switch_orientation.get_state());
	scale_syshud.set_range(0, 2);
	scale_syshud.set_value(1);
	scale_syshud.set_sensitive(false);


	const int width = std::stoi(config_syshud.data["main"]["width"]);
	const int height = std::stoi(config_syshud.data["main"]["height"]);
	const int size = std::min(width, height);
	scale_syshud.set_inverted(switch_orientation.get_state());
	if (switch_orientation.get_state())
		revealer_syshud.set_size_request(height, width);
	else
		revealer_syshud.set_size_request(width, height);
	image_syshud_icon.set_size_request(size, size);
	label_syshud_percentage.set_size_request(size, size);

	box_syshud.set_orientation(switch_orientation.get_state() ? Gtk::Orientation::VERTICAL : Gtk::Orientation::HORIZONTAL);
	scale_syshud.set_orientation(switch_orientation.get_state() ? Gtk::Orientation::VERTICAL : Gtk::Orientation::HORIZONTAL);

	image_syshud_icon.set_pixel_size(scale_isize.get_value());
	image_syshud_icon.set_from_icon_name("audio-volume-medium-symbolic");
	label_syshud_percentage.set_text("50%");
}

void page_syshud::setup_actions() {
	// TODO: VERY IMPORTANT!!!
	// Try to reduce disk write cycles, aka don't update until the last change or add a timeout.
	// Please.. Otherwise R.I.P disk.

	// Position
	dropdown_position.property_selected().signal_changed().connect([&]() {
		int selected = dropdown_position.get_selected();
		revealer_syshud.set_halign(alignments[selected].first);
		revealer_syshud.set_valign(alignments[selected].second);

		config_syshud.data["main"]["position"] = positions[selected];
		config_syshud.save();
	});

	// Orientation
	switch_orientation.signal_state_set().connect([&](bool state) {
		const int width = std::stoi(config_syshud.data["main"]["width"]);
		const int height = std::stoi(config_syshud.data["main"]["height"]);
		const int size = std::min(width, height);
		scale_syshud.set_inverted(state);
		if (state)
			revealer_syshud.set_size_request(height, width);
		else
			revealer_syshud.set_size_request(width, height);
		image_syshud_icon.set_size_request(size, size);
		label_syshud_percentage.set_size_request(size, size);

		box_syshud.set_orientation(state ? Gtk::Orientation::VERTICAL : Gtk::Orientation::HORIZONTAL);
		scale_syshud.set_orientation(state ? Gtk::Orientation::VERTICAL : Gtk::Orientation::HORIZONTAL);
		scale_syshud.set_hexpand(!state);
		scale_syshud.set_vexpand(state);

		config_syshud.data["main"]["orientation"] = state ? "v" : "h";
		config_syshud.save();
		return false;
	}, false);

	// Size
	entry_width.signal_changed().connect([&]() {
		const int width = std::stoi(entry_width.get_text());
		const int height = std::stoi(entry_height.get_text());
		const int size = std::min(width, height);
		if (switch_orientation.get_state())
			revealer_syshud.set_size_request(height, width);
		else
			revealer_syshud.set_size_request(width, height);
		image_syshud_icon.set_size_request(size, size);
		label_syshud_percentage.set_size_request(size, size);

		config_syshud.data["main"]["width"] = entry_width.get_text();
		config_syshud.save();
	});
	entry_height.signal_changed().connect([&]() {
		const int width = std::stoi(entry_width.get_text());
		const int height = std::stoi(entry_height.get_text());
		const int size = std::min(width, height);
		if (switch_orientation.get_state())
			revealer_syshud.set_size_request(height, width);
		else
			revealer_syshud.set_size_request(width, height);
		image_syshud_icon.set_size_request(size, size);
		label_syshud_percentage.set_size_request(size, size);

		config_syshud.data["main"]["height"] = entry_height.get_text();
		config_syshud.save();
	});

	// Icon size
	scale_isize.signal_value_changed().connect([&]() {
		image_syshud_icon.set_pixel_size(scale_isize.get_value());
		std::string isize = std::to_string(scale_isize.get_value());
		isize.erase (isize.find_last_not_of('0') + 1, std::string::npos);
		isize.erase (isize.find_last_not_of('.') + 1, std::string::npos);
		config_syshud.data["main"]["icon-size"] = isize;
		config_syshud.save();
	});

	// Percentage
	switch_percentage.signal_state_set().connect([&](bool state) {
		label_syshud_percentage.set_visible(state);
		config_syshud.data["main"]["show-percentage"] = state ? "true" : "false";
		config_syshud.save();
		return false;
	}, false);
}

void page_syshud::load_css() {
	const std::string& style_path = "/usr/share/sys64/hud/style.css";
	const std::string& style_path_usr = std::string(getenv("HOME")) + "/.config/sys64/hud/style.css";

	// Load base style
	if (std::filesystem::exists(style_path)) {
		auto css = Gtk::CssProvider::create();
		css->load_from_path(style_path);
		get_style_context()->add_provider_for_display(main_window->property_display(), css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	}
	// Load user style
	if (std::filesystem::exists(style_path_usr)) {
		auto css = Gtk::CssProvider::create();
		css->load_from_path(style_path_usr);
		get_style_context()->add_provider_for_display(main_window->property_display(), css, GTK_STYLE_PROVIDER_PRIORITY_USER);
	}
}
