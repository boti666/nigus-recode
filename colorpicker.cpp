#include "includes.h"

// static variables.


Color copied_color{ 255,255,255,255 };
int Colorpicker::texture = 0;
std::unique_ptr< Color[] > Colorpicker::gradient = nullptr;;

Color hue_colors[] = {
		Color(255.f, 0.f, 0.f, 255.f),
		Color(255.f, 255.f, 0.f, 255.f),
		Color(0.f, 255.f, 0.f, 255.f),
		Color(0.f, 255.f, 255.f, 255.f),
		Color(0.f, 0.f, 255.f, 255.f),
		Color(255.f, 0.f, 255.f, 255.f),
		Color(255.f, 0.f, 0.f, 255.f)
};

float get_hue(int red, int green, int blue) {

	int min = 0;
	int max = 255;

	if (red < green && red < blue)
		min = red;
	else if (green < red && green < blue)
		min = green;
	else if (blue < red && blue < green)
		min = blue;

	if (red > green && red > blue)
		max = red;
	else if (green > red && green > blue)
		max = green;
	else if (blue > red && blue > green)
		max = blue;

	if (min == max) {
		return 0;
	}

	float hue = 0.f;
	if (max == red) {
		hue = (float)(green - blue) / (float)(max - min);
	}
	else if (max == green) {
		hue = 2.f + (float)(blue - red) / (float)(max - min);
	}
	else {
		hue = 4.f + (float)(red - green) / (float)(max - min);
	}

	hue = hue * 60.f;

	if (hue < 0)
		hue = hue + 360;

	return hue / 360.f;
}

void Colorpicker::draw() {
	const int w{ COLORPICKER_PICKER_SIZE };
	const int h{ COLORPICKER_PICKER_SIZE };

	if (m_open) {
		// allocate.
		if (!gradient) {
			gradient = std::make_unique< Color[] >(w * h);
		}

		// init.
		float sat{ 0.99f }, val{ 1.f };

		// iterate width.
		for (int i{}; i < w; ++i) {

			// iterate height.
			for (int j{}; j < h; ++j) {

				sat = std::min(std::max(i / (w - 1.f), 0.0f), 1.0f);
				val = 1.f - std::min(std::max(j / (h - 1.f), 0.0f), 1.0f);

				// write back to array.
				*(Color*)(gradient.get() + i + j * w) = Color::hsv_to_rgb(hue, sat, val);
			}
		}

		// allocate new texture in engine.
		if (!texture)
			texture = g_csgo.m_surface->CreateNewTextureID(true);

		// assign allocated memory containing the picker to said texture.
		g_csgo.m_surface->DrawSetTextureRGBA(texture, gradient.get(), w, h);
	}

	Rect  area{ m_parent->GetElementsRect() };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	int hue_pos[] = { p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE + 8, p.y + COlORPICKER_HEIGHT + 2 };
	int alpha_pos[] = { p.x + m_w - COLORPICKER_WIDTH + 2, p.y + COLORPICKER_PICKER_SIZE + COlORPICKER_HEIGHT + 8 };

	int hue_size[] = { 10, COLORPICKER_PICKER_SIZE };
	int alpha_size[] = { COLORPICKER_PICKER_SIZE, 10 };

	// label.
	render::menu_shade.string(p.x + LABEL_OFFSET, p.y - 2, { 205, 205, 205, m_parent->m_alpha }, m_label);

	// outline.
	render::rect(p.x + m_w - COLORPICKER_WIDTH, p.y, COLORPICKER_WIDTH, COlORPICKER_HEIGHT, { 0, 0, 0, m_parent->m_alpha });

	// make a copy of the color.
	Color preview = m_color;

	// modify the alpha channel of this color.
	preview.a() = m_parent->m_alpha;

	// draw preview window.
	render::rect_filled(p.x + m_w - COLORPICKER_WIDTH + 1, p.y + 1, COLORPICKER_WIDTH - 2, COlORPICKER_HEIGHT - 2, preview);

	// fade / shine.
	render::rect_filled_fade(p.x + m_w - COLORPICKER_WIDTH + 1, p.y + 1, COLORPICKER_WIDTH - 2, COlORPICKER_HEIGHT - 2, { 50, 50, 35, m_parent->m_alpha }, 0, 150);

	// draw picker
	if (m_open) {
		// set panel color.
		g_csgo.m_surface->DrawSetColor({ 17, 17, 17, 255 });

		// draw background.
		g_csgo.m_surface->DrawFilledRect(p.x + m_w - COLORPICKER_WIDTH,
			p.y + COlORPICKER_HEIGHT + 2,
			p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE + 18,
			p.y + COlORPICKER_HEIGHT + 18 + COLORPICKER_PICKER_SIZE);

		// set panel color.
		g_csgo.m_surface->DrawSetColor(colors::white);

		// set texture.
		g_csgo.m_surface->DrawSetTexture(texture);

		// draw picker.
		g_csgo.m_surface->DrawTexturedRect(p.x + m_w - COLORPICKER_WIDTH,
			p.y + COlORPICKER_HEIGHT + 2,
			p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE,
			p.y + COlORPICKER_HEIGHT + 2 + COLORPICKER_PICKER_SIZE);

		int max_clrs = ARRAYSIZE(hue_colors) - 1;
		for (int hue_gradient_index = 0; hue_gradient_index < max_clrs; hue_gradient_index++) {
			render::gradient(hue_pos[0], hue_pos[1] + hue_gradient_index * (hue_size[1] / max_clrs), hue_size[0], hue_size[1] / max_clrs, hue_colors[hue_gradient_index], hue_colors[hue_gradient_index + 1]);
		}

		g_csgo.m_surface->DrawSetColor({ 255, 255, 255, 255 });

		if (m_render_alpha)
			g_csgo.m_surface->DrawFilledRect(alpha_pos[0], alpha_pos[1], alpha_pos[0] + alpha_size[0], alpha_pos[1] + alpha_size[1]);

		// set panel color.
		g_csgo.m_surface->DrawSetColor({ 17, 17, 17, 255 });

		g_csgo.m_surface->DrawFilledRect(hue_pos[0],
			hue_pos[1] + hue_size[1] * hue - 1,
			hue_pos[0] + hue_size[0],
			hue_pos[1] + hue_size[1] * hue + 1);

		if (m_render_alpha)
			g_csgo.m_surface->DrawFilledRect(alpha_pos[0] + alpha_size[0] * alpha - 1,
				alpha_pos[1],
				alpha_pos[0] + alpha_size[0] * alpha + 1,
				alpha_pos[1] + alpha_size[1]);

		// picker outline.
		render::rect(p.x + m_w - COLORPICKER_WIDTH,
			p.y + COlORPICKER_HEIGHT + 2,
			COLORPICKER_PICKER_SIZE,
			COLORPICKER_PICKER_SIZE,
			{ 255, 255, 255, m_parent->m_alpha });
	}
}

void Colorpicker::think() {
	Rect  area{ m_parent->GetElementsRect() };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	if (m_open) {
		Rect picker{ p.x + m_w - COLORPICKER_WIDTH, p.y + COlORPICKER_HEIGHT + 2, COLORPICKER_PICKER_SIZE, COLORPICKER_PICKER_SIZE };
		Rect full_picker{ p.x + m_w - COLORPICKER_WIDTH - 5, p.y + COlORPICKER_HEIGHT + 2 - 5, COLORPICKER_PICKER_SIZE + 26 + 10, COLORPICKER_PICKER_SIZE + 26 + 10 };

		int hue_pos[] = { p.x + m_w - COLORPICKER_WIDTH + COLORPICKER_PICKER_SIZE + 8, p.y + COlORPICKER_HEIGHT + 2 };
		int alpha_pos[] = { p.x + m_w - COLORPICKER_WIDTH + 2, p.y + COLORPICKER_PICKER_SIZE + COlORPICKER_HEIGHT + 8 };

		int hue_size[] = { 10, COLORPICKER_PICKER_SIZE };
		int alpha_size[] = { COLORPICKER_PICKER_SIZE, 10 };

		if (g_input.IsCursorInRect(full_picker) && g_input.GetKeyState(VK_LBUTTON)) {
			m_parent->m_active_element = this;
			if (g_input.IsCursorInRect(picker))
				m_color = Colorpicker::ColorFromPos(g_input.m_mouse.x - (p.x + m_w - COLORPICKER_WIDTH), g_input.m_mouse.y - (p.y + COlORPICKER_HEIGHT + 2));
			else {
				Rect hue_picker{ hue_pos[0], hue_pos[1] - 5, hue_size[0], hue_size[1] + 10 };
				Rect alpha_picker{ alpha_pos[0] - 5, alpha_pos[1], alpha_size[0] + 10, alpha_size[1] };

				float normalized_mouse_pos_hue = std::min(std::max((g_input.m_mouse.y - (hue_pos[1] - 1.f)), 0.f), (float)hue_size[1]) / hue_size[1];

				float normalized_mouse_pos_alpha = std::min(std::max((g_input.m_mouse.x - (alpha_pos[0] - 1.f)), 0.f), (float)alpha_size[0]) / alpha_size[0];

				if (g_input.IsCursorInRect(hue_picker)) {
					if (g_input.GetKeyState(VK_LBUTTON)) {
						hue = std::min(std::max(normalized_mouse_pos_hue, 0.f), 1.f);
					}
				}

				if (g_input.IsCursorInRect(alpha_picker)) {
					if (g_input.GetKeyState(VK_LBUTTON)) {
						alpha = std::min(std::max(normalized_mouse_pos_alpha, 0.f), 1.f);
					}
				}
			}
		}

		if (g_input.GetKeyState(VK_CONTROL)) {
			if (g_input.GetKeyState(VK_F1))
				copied_color = m_color;
			else if (g_input.GetKeyState(VK_F2)) {
				m_color = copied_color;

				hue = (float)get_hue(m_color.r(), m_color.g(), m_color.b());
				alpha = m_color.a() / 255.f;
			}
		}

		if (m_parent->m_active_element != this) {
			m_open = false;
			if (m_callback)
				m_callback();
		}
	}
	else {
		hue = (float)get_hue(m_color.r(), m_color.g(), m_color.b());
		alpha = m_color.a() / 255.f;
	}

	if (m_render_alpha)
		m_color.a() = std::clamp(alpha * 255.f, 0.f, 255.f);
	else
		m_color.a() = 255;

	m_color.r() = std::clamp((int)m_color.r(), 0, 255);
	m_color.g() = std::clamp((int)m_color.g(), 0, 255);
	m_color.b() = std::clamp((int)m_color.b(), 0, 255);

	if (m_ptr)
		*m_ptr = m_color;
}

void Colorpicker::click() {
	Rect  area{ m_parent->GetElementsRect() };
	Point p{ area.x + m_pos.x, area.y + m_pos.y };

	Rect preview{ p.x + m_w - COLORPICKER_WIDTH, p.y, COLORPICKER_WIDTH, COlORPICKER_HEIGHT };

	if (g_input.IsCursorInRect(preview))
		m_open = !m_open;
}