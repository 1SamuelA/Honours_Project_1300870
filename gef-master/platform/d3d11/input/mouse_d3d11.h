
#ifndef _PLATFORM_D3D11_INPUT_MOUSE_H
#define _PLATFORM_D3D11_INPUT_MOUSE_H

#include <input/mouse.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace gef
{
	class PlatformD3D11;

	class MouseD3D11 : public Mouse
	{
	public:
		MouseD3D11( const PlatformD3D11* platform, LPDIRECTINPUT8 direct_input );
		~MouseD3D11();

		void Update();

		void EnablePanel( const Int32 panel_index );
		void DisablePanel( const Int32 panel_index );
		const MouseTouch& GetTouch( const Int32 panel_index, const Int32 touch_index ) { return touches_[panel_index][touch_index]; }
		const Int32 max_num_touches() const { return 2; }
		const Int32 max_num_panels() const { return 1; }
		const bool panel_enabled( const Int32 panel_index ) const { return panel_enabled_[panel_index]; }

		inline const gef::Vector2& mouse_position() const { return mouse_position_; }
		inline bool is_button_down( Int32 button_num ) const { return is_button_down_[button_num]; }

		inline bool is_button_pressed( Int32 button_num ) const { return is_button_pressed_[button_num]; }
		inline bool is_button_released( Int32 button_num ) const { return is_button_released_[button_num]; }
		inline bool is_mouse_moved() const { return is_mouse_moved_; }


		bool is_mouse_locked() const { return is_mouse_locked_; }
		void set_is_mouse_locked(bool locked) { is_mouse_locked_ = locked; }

	private:
		void CleanUp();
		LPDIRECTINPUT8			direct_input_;
		LPDIRECTINPUTDEVICE8	mouse_;
		const PlatformD3D11* platform_;
		MouseTouch touches_[1][2];
		bool panel_enabled_[1];

		bool is_button_down_[2];
		bool is_button_pressed_[2];
		bool is_button_released_[2];

		bool live_button_state_[2];
		bool button_previous_state_[2];

		bool is_mouse_locked_;
		bool is_mouse_moved_;

		gef::Vector2 mouse_position_;
		gef::Vector2 previous_mouse_position_;
	};
}

#endif // _GEF_TOUCH_INPUT_MANAGER_D3D11_H
