#pragma once

namespace Ellis {

	class ScriptGlue
	{
	public:
		static void RegisterComponents();
		static void RegisterFunctions();

		static void SetEditorViewportCursorPos(const glm::vec2& position);
		static void SetEditorViewportSize(const glm::vec2& size);
	};

}