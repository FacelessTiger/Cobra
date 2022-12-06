#pragma once

#include <imgui/imgui.h>

namespace Ellis::UI {

	class ScopedStyleColor
	{
	private:
		bool m_Set = false;
	public:
		ScopedStyleColor() = default;

		ScopedStyleColor(ImGuiCol idx, const ImVec4& color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate)
		{
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}
	};

}