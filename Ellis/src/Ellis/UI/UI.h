#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <Ellis/Renderer/Texture.h>

namespace Ellis::UI {

	void ShiftCursorX(float distance);
	void ShiftCursorY(float distance);
	void ShiftCursor(float x, float y);

	ImRect GetItemRect();
	ImRect RectExpanded(const ImRect& rect, float x, float y);
	ImRect RectOffset(const ImRect& rect, float x, float y);
	ImRect RectOffset(const ImRect& rect, ImVec2 xy);

	void DrawButtonImage(const Ref<Texture2D>& imageNormal, const Ref<Texture2D>& imageHovered, const Ref<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax);

	void DrawButtonImage(const Ref<Texture2D>& imageNormal, const Ref<Texture2D>& imageHovered, const Ref<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle);

	void DrawButtonImage(const Ref<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImVec2 rectMin, ImVec2 rectMax);

	void DrawButtonImage(const Ref<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed,
		ImRect rectangle);

	void DrawButtonImage(const Ref<Texture2D>& imageNormal, const Ref<Texture2D>& imageHovered, const Ref<Texture2D>& imagePressed,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed);

	void DrawButtonImage(const Ref<Texture2D>& image,
		ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed);

	void RenderWindowOuterBorders(ImGuiWindow* window);

	bool BeginMenubar(const ImRect& barRectangle);
	void EndMenubar();

	class ScopedStyleColor
	{
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
	private:
		bool m_Set = false;
	};

}