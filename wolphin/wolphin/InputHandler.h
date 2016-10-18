#pragma once
#include <assert.h>

namespace wholphin {

	struct Key {
		int lastEventFrameTime;
		bool isDown = false;
	};

	class InputHandler {
	public:
		inline void KeyDown(int keyCode, int frameNumber);
		inline void KeyUp(int keyCode, int frameNumber);

		inline bool IsDown(int keyCode) { assert(keyCode < 255); return keys[keyCode].isDown; }
		inline bool WentDownAtFrame(int keyCode, int frameTime) { return IsDown(keyCode) && (keys[keyCode].lastEventFrameTime == frameTime); }
		inline bool IsUp(int keyCode) { assert(keyCode < 255); return !keys[keyCode].isDown; }
		inline bool WentUpAtFrame(int keyCode, int frameTime) { return IsUp(keyCode) && (keys[keyCode].lastEventFrameTime == frameTime); }
	private:
		Key keys[0xff];
	};

	void InputHandler::KeyDown(int keyCode, int frameNumber) {
		assert(keyCode < 255);
		keys[keyCode].isDown = true;
		keys[keyCode].lastEventFrameTime = frameNumber;
	}

	void InputHandler::KeyUp(int keyCode, int frameNumber) {
		assert(keyCode < 255);
		keys[keyCode].isDown = false;
		keys[keyCode].lastEventFrameTime = frameNumber;
	}

}