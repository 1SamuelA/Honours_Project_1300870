#include <platform/d3d11/input/mouse_d3d11.h>
#include <gef.h>
#include <platform/d3d11/system/platform_d3d11.h>
#include <platform/win32/system/window_win32.h>

namespace gef
{
#define INVALID_TOUCH_ID		(-1)


	MouseD3D11::MouseD3D11( const PlatformD3D11* platform, LPDIRECTINPUT8 direct_input ) :
		direct_input_( direct_input ),
		mouse_( NULL ),
		platform_( platform )
	{
		HRESULT hresult = S_OK;
		hresult = direct_input_->CreateDevice( GUID_SysMouse, &mouse_, NULL );

		if( SUCCEEDED( hresult ) )
			hresult = mouse_->SetDataFormat( &c_dfDIMouse );

		if( SUCCEEDED( hresult ) )
			hresult = mouse_->SetCooperativeLevel( platform_->top_level_hwnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );


		if( SUCCEEDED( hresult ) )
		{
			DIPROPDWORD dipdw;
			dipdw.diph.dwSize = sizeof( DIPROPDWORD );
			dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			dipdw.diph.dwObj = 0;
			dipdw.diph.dwHow = DIPH_DEVICE;
			dipdw.dwData = 16;

			hresult = mouse_->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		}

		if( FAILED( hresult ) )
			CleanUp();
		/*
		for(Int32 panel_index=0;panel_index<1;++panel_index)
		{
		panel_enabled_[panel_index] = false;

		for(Int32 touch_index=0;touch_index<2;++touch_index)
		{
		Touch* touch = &touches_[panel_index][touch_index];
		touch->type = TT_NONE;
		touch->id = INVALID_TOUCH_ID;
		is_button_down_[touch_index] = false;
		}
		}
		*/
		panels_.push_back( MouseTouchContainer() );

		button_previous_state_[0] = false;
		button_previous_state_[1] = false;

		is_mouse_locked_ = false;

		is_button_down_[0] = false;
		is_button_down_[1] = false;
		is_button_pressed_[0] = false;
		is_button_pressed_[1] = false;
		is_button_released_[0] = false;
		is_button_released_[1] = false;
	}

	MouseD3D11::~MouseD3D11()
	{
		CleanUp();
	}

	void MouseD3D11::CleanUp()
	{
		if( mouse_ )
		{
			mouse_->Unacquire();
			mouse_->Release();
			mouse_ = NULL;
		}
	}

	void MouseD3D11::Update()
	{
		HRESULT hresult = S_OK;
		mouse_->Acquire();

		DIDEVICEOBJECTDATA od;
		DWORD dwElements = 1;

		// store the button status for this update
		// intialise it to the same state as last frame
		live_button_state_[0] =  false;
		live_button_state_[1] = false;

		button_previous_state_[0] = is_button_down_[0];
		button_previous_state_[1] = is_button_down_[1];

		previous_mouse_position_ = mouse_position_;

		while( dwElements != 0 )
		{
			dwElements = 1;
			hresult = mouse_->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), &od, &dwElements, 0 );
			if( FAILED( hresult ) )
				break;




			switch( od.dwOfs )
			{
			case DIMOFS_BUTTON0:
				if( od.dwData & 0x80 )
					live_button_state_[0] = true;
				else
					live_button_state_[0] = false;
				break;
			case DIMOFS_BUTTON1:
				if( od.dwData & 0x80 )
					live_button_state_[1] = true;
				else
					live_button_state_[1] = false;
				break;
			case DIMOFS_BUTTON2:
				if (od.dwData & 0x80)
					live_button_state_[1] = true;
				else
					live_button_state_[1] = false;
				break;
			}
		}

		for (int x = 0; x < 2; x++)
		{
			// Does if down
			is_button_down_[x] = live_button_state_[x];

			//Pressed
			is_button_pressed_[x] = ((live_button_state_[x] == true) && (button_previous_state_[x] == false)) ? true : false;

			//false
			is_button_released_[x] = ((live_button_state_[x] == false) && (button_previous_state_[x] == true)) ? true : false;
		}

		POINT mouse_pos;
		GetCursorPos( &mouse_pos );
		ScreenToClient( platform_->hwnd(), &mouse_pos );
		mouse_position_.x = (float)mouse_pos.x;
		mouse_position_.y = (float)mouse_pos.y;

		if( (mouse_position_.x != previous_mouse_position_.x) &&
			(mouse_position_.y != previous_mouse_position_.y) )
		{
			is_mouse_moved_ = true;
		}
		else
		{
			is_mouse_moved_ = false;
		}

		mouse_pos.x = platform_->width();
		mouse_pos.y = platform_->height();

		float x_distance = mouse_pos.x - mouse_position_.x;
		float y_distance = mouse_pos.y - mouse_position_.y;




		if (is_mouse_locked_)
		{
			ClientToScreen(platform_->hwnd(), &mouse_pos);

			SetCursorPos( mouse_pos.x , mouse_pos.y);
		}

		/*

		for( Int32 panel_index = 0; panel_index<1; ++panel_index )
		{
			if( panel_enabled_[panel_index] )
			{
				// go through existing touches and remove any that were released last update
				for( MouseTouchIterator touch = panels_[panel_index].begin(); touch != panels_[panel_index].end();)
				{
					if( touch->type == TM_RELEASED )
						touch = panels_[panel_index].erase( touch );
					else
						++touch;
				}

				// get the touch data for this panel
				// check to see if we have any data to update existing touches
				for( Int32 touch_index = 0; touch_index<2; ++touch_index )
				{
					MouseTouch touch;
					touch.id = touch_index;
					touch.position = mouse_position_;


					if( is_button_down[touch_index] )
					{
						if( !is_button_down_[touch_index] )
						{
							// NEEDS TESTING
							// make sure mouse_position is actually within drawable area 
							if( (mouse_pos.y >= 0) && (mouse_pos.y < platform_->height()) && (mouse_pos.x >= 0) && (mouse_pos.x < platform_->width()) )
							{
								// new touch
								touch.type = TM_NEW;
								AddTouch( panel_index, touch );
							}
						}
						else
						{
							// touch updated
							touch.type = TM_ACTIVE;
							UpdateTouch( panel_index, touch );
						}
					}
					else
					{
						if( is_button_down_[touch_index] )
						{
							// touch released
							touch.type = TM_RELEASED;
							UpdateTouch( panel_index, touch );
						}
					}

					is_button_down_[touch_index] = is_button_down[touch_index];
				}
			}
			
		}*/
	}

	void MouseD3D11::EnablePanel( const Int32 panel_index )
	{
		panel_enabled_[panel_index] = true;
	}

	void MouseD3D11::DisablePanel( const Int32 panel_index )
	{
		panel_enabled_[panel_index] = false;
	}


}

