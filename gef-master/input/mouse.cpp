#include <input/mouse.h>

namespace gef
{
	Mouse::~Mouse()
	{

	}

	void Mouse::AddTouch( const Int32 panel_index, const MouseTouch& touch )
	{
		panels_[panel_index].push_back( touch );
	}

	bool Mouse::RemoveTouch( const Int32 panel_index, const Int32 touch_id )
	{
		bool success = false;
		for( MouseTouchIterator touch_iter = panels_[panel_index].begin(); touch_iter != panels_[panel_index].end(); ++touch_iter )
		{
			if( touch_iter->id == touch_id )
			{
				panels_[panel_index].erase( touch_iter );
				success = true;
				break;
			}
		}
		return success;
	}

	bool Mouse::UpdateTouch( const Int32 panel_index, const MouseTouch& touch )
	{
		bool success = false;
		for( MouseTouchIterator touch_iter = panels_[panel_index].begin(); touch_iter != panels_[panel_index].end(); ++touch_iter )
		{
			if( touch_iter->id == touch.id )
			{
				*touch_iter = touch;
				success = true;
				break;
			}
		}
		return success;
	}

	bool Mouse::FindTouch( const Int32 panel_index, const Int32 touch_id, MouseTouch& touch ) const
	{
		bool found = false;
		for( ConstMouseTouchIterator touch_iter = panels_[panel_index].begin(); touch_iter != panels_[panel_index].end(); ++touch_iter )
		{
			if( touch_iter->id == touch_id )
			{
				touch = *touch_iter;
				found = true;
				break;
			}
		}

		return found;
	}

	void Mouse::CleanupReleasedTouches()
	{
		// remove any touches that were released in the previous update
		for( std::vector<MouseTouchContainer>::iterator panel_iter = panels_.begin(); panel_iter != panels_.end(); ++panel_iter )
		{
			for( MouseTouchIterator touch_iter = panel_iter->begin(); touch_iter != panel_iter->end();)
			{
				if( touch_iter->type == TM_RELEASED )
					touch_iter = panel_iter->erase( touch_iter );
				else
					++touch_iter;
			}
		}
	}

	bool Mouse::is_button_down(int key) const
	{
		return false;
	}

	bool Mouse::is_button_pressed(int key) const
	{
		return false;
	}

	bool Mouse::is_button_released(int key) const
	{
		return false;
	}

}