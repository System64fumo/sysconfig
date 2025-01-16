#include "window.hpp"
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>

#include "pages/syshud.hpp"

sysconfig::sysconfig() : box_buttons(Gtk::Orientation::VERTICAL) {
	set_child(box_main);
	box_main.append(stacksidebar_side);
	box_main.append(stack_main);

	stacksidebar_side.set_stack(stack_main);
	stacksidebar_side.set_size_request(200, -1);

	auto p_syshud = Gtk::make_managed<page_syshud>(this);
	auto stack_syshud = stack_main.add(*p_syshud, "syshud", "Syshud");
	stack_syshud->set_icon_name("preferences-other-symbolic"); // An 8 year old bug is preventing this from working
	// Should probably add the icons manually..
}
