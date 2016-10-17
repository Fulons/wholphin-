#pragma once
#include <assert.h>

namespace wholphin {

	struct Key {

		bool isDown = false;
	};

	class InputHandler {
	public:
		inline void KeyDown(int keyCode);
		inline void KeyUp(int keyCode);

		inline bool IsDown(int keyCode) { assert(keyCode < 255); return keys[keyCode].isDown; }
		inline bool IsUp(int keyCode) { assert(keyCode < 255); return !keys[keyCode].isDown; }
	private:
		Key keys[0xff];
	};

	void InputHandler::KeyDown(int keyCode) {
		assert(keyCode < 255);
		keys[keyCode].isDown = true;
	}

	void InputHandler::KeyUp(int keyCode) {
		assert(keyCode < 255);
		keys[keyCode].isDown = false;
	}

}