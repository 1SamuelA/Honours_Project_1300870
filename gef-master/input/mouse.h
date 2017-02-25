#pragma once
#ifndef _GEF_INPUT_MOUSE_H
#define _GEF_INPUT_MOUSE_H

#include <gef.h>
#include <maths/vector2.h>
#include <vector>
#include <list>

namespace gef
{
	enum MouseType
	{
		TM_NONE = 0,
		TM_NEW,
		TM_ACTIVE,
		TM_RELEASED
	};

	struct MouseTouch
	{
		Int32 id;
		MouseType type;
		Vector2 position;
	};

	enum MouseButton
	{
		VM_LEFTBUTTON = 0,
		VM_RIGHTBUTTON = 0
	};

	typedef std::list<struct MouseTouch> MouseTouchContainer;
	typedef std::list<struct MouseTouch>::iterator MouseTouchIterator;
	typedef std::list<struct MouseTouch>::const_iterator ConstMouseTouchIterator;

	class Mouse
	{
	public:
		virtual ~Mouse();

		bool FindTouch( const Int32 panel_index, const Int32 touch_id, MouseTouch& touch ) const;

		inline const MouseTouchContainer& touches( const Int32 panel_index ) const
		{
			return panels_[panel_index];
		}

		virtual void Update() = 0;

		virtual void EnablePanel( const Int32 panel_index ) = 0;
		virtual void DisablePanel( const Int32 panel_index ) = 0;

		//virtual const Touch& GetTouch(const Int32 panel_index, const Int32 touch_index)  = 0;
		//		virtual const Int32 max_num_touches() const  = 0;
		virtual const Int32 max_num_panels() const = 0;
		virtual const bool panel_enabled( const Int32 panel_index ) const = 0;
		//virtual const Touch* const FindTouch(const Int32 panel_index, const Int32 touch_id) const = 0;

		//virtual const Int32 num_touches() const  = 0;

		virtual const gef::Vector2& mouse_position() const = 0;
		virtual bool is_button_pressed( int button_num ) const = 0;
		virtual bool is_button_down( int button_num ) const = 0;
		virtual bool is_button_released( int button_num ) const = 0;

		virtual bool is_mouse_moved() const = 0;

		virtual bool is_mouse_locked() const = 0;
		virtual void set_is_mouse_locked(bool locked) = 0;


		void CleanupReleasedTouches();
	protected:
		void AddTouch( const Int32 panel_index, const MouseTouch& touch );
		bool RemoveTouch( const Int32 panel_index, const Int32 touch_id );
		bool UpdateTouch( const Int32 panel_index, const MouseTouch& touch );

		std::vector<MouseTouchContainer> panels_;
	};
}

#endif // _GEF_TOUCH_MANAGER_H
