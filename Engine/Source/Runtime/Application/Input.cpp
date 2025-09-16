#include "Input.h"

#include <ranges>

#include "Core/Logger/Logger.h"

namespace Wi
{
	void Input::Initialize()
	{
		Log::Info("Initializing input...");

		for (int i = 0; i < static_cast<int>(KeyCode::LastKeyCode); ++i) {
			s_KeyData[static_cast<KeyCode>(i)] = KeyData();
		}

		for (int i = 0; i < static_cast<int>(MouseButton::LastMouseButton); ++i) {
			s_MouseData[static_cast<MouseButton>(i)] = KeyData();
		}
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		const auto it = s_KeyData.find(keycode);
		if (it != s_KeyData.end()) {
			return it->second.CurrentState == KeyState::Pressed;
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyCode keycode)
	{
		const auto it = s_KeyData.find(keycode);
		if (it != s_KeyData.end()) {
			return it->second.CurrentState == KeyState::Released;
		}
		return false;
	}

	bool Input::IsKeyHeld(KeyCode keycode)
	{
		const auto it = s_KeyData.find(keycode);
		if (it != s_KeyData.end()) {
			return it->second.CurrentState == KeyState::Held;
		}
		return false;
	}

	bool Input::IsKeyDown(KeyCode keycode)
	{
		const auto it = s_KeyData.find(keycode);
		if (it != s_KeyData.end()) {
			return it->second.CurrentState == KeyState::Held || it->second.CurrentState == KeyState::Pressed;
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		const auto it = s_MouseData.find(button);
		if (it != s_MouseData.end()) {
			return it->second.CurrentState == KeyState::Pressed;
		}
		return false;
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		const auto it = s_MouseData.find(button);
		if (it != s_MouseData.end()) {
			return it->second.CurrentState == KeyState::Released;
		}
		return false;
	}

	bool Input::IsMouseButtonHeld(MouseButton button)
	{
		const auto it = s_MouseData.find(button);
		if (it != s_MouseData.end()) {
			return it->second.CurrentState == KeyState::Held;
		}
		return false;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{

		const auto it = s_MouseData.find(button);
		if (it != s_MouseData.end()) {
			return it->second.CurrentState == KeyState::Held || it->second.CurrentState == KeyState::Pressed;
		}
		return false;
	}

	int Input::GetMouseX()
	{
		return lastMousePositionX;
	}

	int Input::GetMouseY()
	{
		return lastMousePositionY;
	}

	float Input::GetMouseScroll()
	{
		return lastMouseScrollDelta;
	}

	std::pair<int, int> Input::GetMousePosition()
	{
		return { lastMousePositionX, lastMousePositionY};
	}

	void Input::UpdateKeyState(KeyCode key, KeyState newKeyState)
	{
		KeyData& data = s_KeyData[key];
		data.OldState = data.CurrentState;
		data.CurrentState = newKeyState;
	}

	void Input::UpdateMouseButtonState(MouseButton button, KeyState newKeyState)
	{
		KeyData& data = s_MouseData[button];
		data.OldState = data.CurrentState;
		data.CurrentState = newKeyState;
	}

	void Input::UpdateMousePosition(int x, int y)
	{
		lastMousePositionX = x;
		lastMousePositionY = y;
	}

	void Input::UpdateMouseScrollDelta(float delta)
	{
		lastMouseScrollDelta = delta;
	}

	void Input::ClearReleasedKeys()
	{
		for (auto& [key, data] : s_KeyData)
		{
			if (data.CurrentState == KeyState::Released)
				UpdateKeyState(key, KeyState::None);
		}

		for (auto& [key, data] : s_MouseData)
		{
			if (data.CurrentState == KeyState::Released)
				UpdateMouseButtonState(key, KeyState::None);
		}

		lastMouseScrollDelta = 0;
	}

	void Input::TransitionPressedKeys()
	{
		for (KeyData& data : s_KeyData | std::views::values)
		{
			if (data.CurrentState == KeyState::Pressed)
				data.CurrentState = KeyState::Held;
		}
	}

	void Input::TransitionPressedButtons()
	{
		for (KeyData& data : s_MouseData | std::views::values)
		{
			if (data.CurrentState == KeyState::Pressed)
				data.CurrentState = KeyState::Held;
		}
	}
}
