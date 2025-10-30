#include "syslock.hpp"
#include <gtkmm/stringlist.h>
#include <gtkmm/cssprovider.h>
#include <filesystem>

page_syslock::page_syslock(Gtk::Window* win) : main_window(win), box_main(Gtk::Orientation::VERTICAL) {
	config_syslock.load(std::string(getenv("HOME")) + "/.config/sys64/lock/config.conf");

	setup_ui();
	setup_preview();
	setup_actions();
	load_css();
}

void page_syslock::setup_ui() {
	set_hexpand(true);
	set_vexpand(true);
	set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

	set_child(box_main);
	box_main.set_halign(Gtk::Align::CENTER);
	box_main.set_valign(Gtk::Align::START);
	listbox_main.set_size_request(480, -1);

	box_main.append(scrolledwindow_preview);
	scrolledwindow_preview.get_style_context()->add_class("card");
	scrolledwindow_preview.set_size_request(-1, 270); // TODO: Make this use the monitor's aspect ratio
	scrolledwindow_preview.set_margin(20);

	box_main.append(listbox_main);
	listbox_main.get_style_context()->add_class("boxed-list");
	listbox_main.set_selection_mode(Gtk::SelectionMode::NONE);
	listbox_main.set_halign(Gtk::Align::CENTER);
	listbox_main.set_margin(20);

	// Start unlocked
	listbox_main.append(box_startunlocked);
	box_startunlocked.get_style_context()->add_class("header");
	box_startunlocked.append(label_startunlocked);
	label_startunlocked.set_text("Start unlocked");
	label_startunlocked.set_hexpand(true);
	label_startunlocked.set_halign(Gtk::Align::START);
	box_startunlocked.append(switch_startunlocked);
	switch_startunlocked.set_valign(Gtk::Align::CENTER);
	switch_startunlocked.set_active(config_syslock.data["main"]["start-unlocked"] == "true");

	// Keypad
	listbox_main.append(box_keypad);
	box_keypad.get_style_context()->add_class("header");
	box_keypad.append(label_keypad);
	label_keypad.set_text("Keypad");
	label_keypad.set_hexpand(true);
	label_keypad.set_halign(Gtk::Align::START);
	box_keypad.append(switch_keypad);
	switch_keypad.set_valign(Gtk::Align::CENTER);
	switch_keypad.set_active(config_syslock.data["main"]["keypad"] == "true");

	// TODO: This.. Should not be exposed..
	// Password length
	listbox_main.append(box_pwlength);
	box_pwlength.get_style_context()->add_class("header");
	box_pwlength.append(label_pwlength);
	label_pwlength.set_text("Password length");
	label_pwlength.set_hexpand(true);
	label_pwlength.set_halign(Gtk::Align::START);
	box_pwlength.append(scale_pwlength);
	scale_pwlength.set_valign(Gtk::Align::CENTER);
	scale_pwlength.set_hexpand(true);
	scale_pwlength.set_range(0, 16);
	scale_pwlength.set_round_digits(0);
	scale_pwlength.add_mark(0, Gtk::PositionType::BOTTOM, "0");
	scale_pwlength.add_mark(4, Gtk::PositionType::BOTTOM, "4");
	scale_pwlength.add_mark(6, Gtk::PositionType::BOTTOM, "6");
	scale_pwlength.add_mark(8, Gtk::PositionType::BOTTOM, "8");
	scale_pwlength.add_mark(10, Gtk::PositionType::BOTTOM, "10");
	scale_pwlength.set_value(std::stod(config_syslock.data["main"]["password-length"]));

	// TODO: Add the rest of the configs
}

void page_syslock::setup_preview() {
	scrolledwindow_preview.set_child(box_syslock);
	box_syslock.set_name("syslock");
	box_syslock.append(overlay_syslock);
	overlay_syslock.set_hexpand();
	overlay_syslock.set_vexpand();
	box_syslock.set_orientation(Gtk::Orientation::VERTICAL);
	overlay_syslock.set_child(box_syslock_lockscreen);
	box_syslock_lockscreen.set_orientation(Gtk::Orientation::VERTICAL);
	box_syslock_lockscreen.add_css_class("lock_screen");
	box_syslock_lockscreen.append(label_syslock_time);
	label_syslock_time.set_text("9:41");
	label_syslock_time.add_css_class("time");
	box_syslock_lockscreen.append(label_syslock_date);
	label_syslock_date.set_text("Tue 9 Jan");
	label_syslock_date.add_css_class("date");

	overlay_syslock.add_overlay(box_syslock_overlay);
	box_syslock_overlay.add_css_class("box_overlay");

	// Scale down
	auto css = Gtk::CssProvider::create();
	css->load_from_data(".lock_screen { transform: scale(0.25); transform-origin: top center; }");
	box_syslock_lockscreen.get_style_context()->add_provider(css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void page_syslock::setup_actions() {
	// Start unlocked
	switch_startunlocked.signal_state_set().connect([&](bool state) {
		config_syslock.data["main"]["start-unlocked"] = state ? "true" : "false";
		config_syslock.save();
		return false;
	}, false);

	// Keypad
	switch_keypad.signal_state_set().connect([&](bool state) {
		config_syslock.data["main"]["keypad"] = state ? "true" : "false";
		config_syslock.save();
		return false;
	}, false);

	// Password length
	scale_pwlength.signal_value_changed().connect([&]() {
		std::string isize = std::to_string(scale_pwlength.get_value());
		isize.erase (isize.find_last_not_of('0') + 1, std::string::npos);
		isize.erase (isize.find_last_not_of('.') + 1, std::string::npos);
		config_syslock.data["main"]["password-length"] = isize;
		config_syslock.save();
	});
}

void page_syslock::load_css() {
	add_css_class("background");

	const std::string& style_path = "/usr/share/sys64/lock/style.css";
	const std::string& style_path_usr = std::string(getenv("HOME")) + "/.config/sys64/lock/style.css";

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
