#pragma once
#include "../Effect.h"
#include "../../../../Files.h"
#include <cmath>

namespace Pyewton::Odin
{
	class Bloom : public Effect
	{
	public:
		Bloom();

		void Init();
		void Apply(FrameBuffer& fbo_in, FrameBuffer& fbo_out, int w_width, int w_height);

		float knee = 1;
		float threshold = 1;

	private:
		void Rescale(int w_width, int w_height);
		void Draw(FrameBuffer& out);
		void ClearAll();
	};
}