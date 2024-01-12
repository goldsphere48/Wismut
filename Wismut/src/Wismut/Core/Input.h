#pragma once
#include "KeyCodes.h"

namespace Wi
{
	class Input
	{
	protected:
		class KeyData {
		public:
			KeyState CurrentState = KeyState::None;
			KeyState OldState = KeyState::None;
		};

	public:
		static void Initialize();

		static bool IsKeyPressed(KeyCode keycode);
		static bool IsKeyDown(KeyCode keycode);
		static bool IsKeyHeld(KeyCode keycode);
		static bool IsKeyReleased(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonHeld(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePosition();

		static void UpdateKeyState(KeyCode key, KeyState newKeyState);
		static void UpdateButtonState(MouseButton button, KeyState newKeyState);
		static void UpdateMousePosition(float x, float y);
		static void ClearReleasedKeys();
		static void TransitionPressedKeys();
		static void TransitionPressedButtons();

	private:
		inline static std::unordered_map<KeyCode, KeyData> s_KeyData;
		inline static std::unordered_map<MouseButton, KeyData> s_MouseData;
		inline static float lastMousePositionX = 0;
		inline static float lastMousePositionY = 0;
	};
}
