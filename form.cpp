#include "includes.h"

void Form::draw() {
	// opacity should reach 1 in 500 milliseconds.
	constexpr float frequency = 1.f / 0.5f;

	// the increment / decrement per frame.
	float step = frequency * g_csgo.m_globals->m_frametime;

	// if open		-> increment
	// if closed	-> decrement
	m_open ? m_opacity += step : m_opacity -= step;

	// clamp the opacity.
	math::clamp(m_opacity, 0.f, 1.f);

	m_alpha = 0xff * m_opacity;
	if (!m_alpha)
		return;

	// get gui color.
	Color color = g_gui.m_color;
	color.a() = m_alpha;

	int low_alpha = m_alpha / 2;

	int old_mode = m_mode;

	m_mode = g_menu.main.misc.cheat_mode.get();

	if (old_mode != m_mode) {
		SetSize(m_mode == 1 ? 620 : 570, 490);
	}

	switch (m_mode) {
	case 0: {
		// background.
		render::rect_filled(m_x, m_y, m_width, m_height, { 12, 12, 12, m_alpha });

		// border.
		render::rect(m_x, m_y, m_width, m_height, { 5, 5, 5, m_alpha });
		render::rect(m_x + 1, m_y + 1, m_width - 2, m_height - 2, { 60, 60, 60, m_alpha });
		render::rect(m_x + 2, m_y + 2, m_width - 4, m_height - 4, { 40, 40, 40, m_alpha });
		render::rect(m_x + 3, m_y + 3, m_width - 6, m_height - 6, { 40, 40, 40, m_alpha });
		render::rect(m_x + 4, m_y + 4, m_width - 8, m_height - 8, { 40, 40, 40, m_alpha });
		render::rect(m_x + 5, m_y + 5, m_width - 10, m_height - 10, { 60, 60, 60, m_alpha });

		// draw tabs if we have any.
		if (!m_tabs.empty()) {
			// tabs background and border.
			Rect tabs_area = GetTabsRect();

			render::rect_filled(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 17, 17, 17, m_alpha });
			render::rect(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 0, 0, 0, m_alpha });
			render::rect(tabs_area.x + 1, tabs_area.y + 1, tabs_area.w - 2, tabs_area.h - 2, { 48, 48, 48, m_alpha });

			for (size_t i{}; i < m_tabs.size(); ++i) {
				const auto& t = m_tabs[i];

				render::menu_shade.string(tabs_area.x + 10, tabs_area.y + 5 + (i * 16),
					t == m_active_tab ? color : Color{ 152, 152, 152, m_alpha },
					t->m_title);
			}

			// this tab has elements.
			if (!m_active_tab->m_elements.empty()) {
				// elements background and border.
				Rect el = GetElementsRect();

				render::rect_filled(el.x, el.y, el.w, el.h, { 17, 17, 17, m_alpha });
				render::rect(el.x, el.y, el.w, el.h, { 0, 0, 0, m_alpha });
				render::rect(el.x + 1, el.y + 1, el.w - 2, el.h - 2, { 48, 48, 48, m_alpha });

				if (m_active_tab->m_title == "whitelist") {
					for (int i = 1; i <= g_csgo.m_globals->m_max_clients;) {
						Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);
						if (!player || !player->alive()) {
							i++;
							continue;
						}

						player_info_t info;
						g_csgo.m_engine->GetPlayerInfo(i, &info);

						std::string text = { };

						text += info.m_name;
						text += ": ";
						text += std::to_string(i);

						render::menu_shade.string(el.x + el.w - 5, el.y + 16 + (16 * (i - 1)), { 205, 205, 205, m_alpha }, text, render::ALIGN_RIGHT);

						i++;
					}
				}

				// iterate elements to display.
				for (const auto& e : m_active_tab->m_elements) {

					// draw the active element last.
					if (!e || (m_active_element && e == m_active_element))
						continue;

					if (!e->m_show)
						continue;

					// this element we dont draw.
					if (!(e->m_flags & ElementFlags::DRAW))
						continue;

					e->draw();
				}

				// we still have to draw one last fucker.
				if (m_active_element && m_active_element->m_show)
					m_active_element->draw();
			}
		}
	} break;

	case 1: {
		// background.
		render::rect_filled(m_x, m_y, m_width, m_height, { 12, 12, 12, m_alpha });

		// border.
		render::rect(m_x, m_y, m_width, m_height, { 5, 5, 5, m_alpha });
		render::rect(m_x + 1, m_y + 1, m_width - 2, m_height - 2, { 60, 60, 60, m_alpha });
		render::rect(m_x + 2, m_y + 2, m_width - 4, m_height - 4, { 40, 40, 40, m_alpha });
		render::rect(m_x + 3, m_y + 3, m_width - 6, m_height - 6, { 40, 40, 40, m_alpha });
		render::rect(m_x + 4, m_y + 4, m_width - 8, m_height - 8, { 40, 40, 40, m_alpha });
		render::rect(m_x + 5, m_y + 5, m_width - 10, m_height - 10, { 60, 60, 60, m_alpha });

		// draw tabs if we have any.
		if (!m_tabs.empty()) {
			// tabs background and border.
			Rect tabs_area = GetTabsRect();

			render::rect_filled(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 17, 17, 17, m_alpha });
			render::rect(tabs_area.x, tabs_area.y, tabs_area.w, tabs_area.h, { 0, 0, 0, m_alpha });
			render::rect(tabs_area.x + 1, tabs_area.y + 1, tabs_area.w - 2, tabs_area.h - 2, { 48, 48, 48, m_alpha });

			for (size_t i{}; i < m_tabs.size(); ++i) {
				const auto& t = m_tabs[i];

				render::menu_shade.string(tabs_area.x + 10, tabs_area.y + 5 + (i * 16),
					t == m_active_tab ? color : Color{ 152, 152, 152, m_alpha },
					t->m_title);
			}

			// this tab has elements.
			if (!m_active_tab->m_elements.empty()) {
				// elements background and border.
				Rect el = GetElementsRect();

				render::rect_filled(el.x, el.y, el.w, el.h, { 17, 17, 17, m_alpha });
				render::rect(el.x, el.y, el.w, el.h, { 0, 0, 0, m_alpha });
				render::rect(el.x + 1, el.y + 1, el.w - 2, el.h - 2, { 48, 48, 48, m_alpha });

				if (m_active_tab->m_title == "whitelist") {
					for (int i = 1; i <= g_csgo.m_globals->m_max_clients;) {
						Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);
						if (!player || !player->alive()) {
							i++;
							continue;
						}

						player_info_t info;
						g_csgo.m_engine->GetPlayerInfo(i, &info);

						std::string text = { };

						text += info.m_name;
						text += ": ";
						text += std::to_string(i);

						render::menu_shade.string(el.x + el.w - 5, el.y + 16 + (16 * (i - 1)), { 205, 205, 205, m_alpha }, text, render::ALIGN_RIGHT);

						i++;
					}
				}

				// iterate elements to display.
				for (const auto& e : m_active_tab->m_elements) {

					// draw the active element last.
					if (!e || (m_active_element && e == m_active_element))
						continue;

					if (!e->m_show)
						continue;

					// this element we dont draw.
					if (!(e->m_flags & ElementFlags::DRAW))
						continue;

					e->draw();
				}

				// we still have to draw one last fucker.
				if (m_active_element && m_active_element->m_show)
					m_active_element->draw();
			}
		}
	} break;
	}
}