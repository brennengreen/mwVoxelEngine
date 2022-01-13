#pragma once

#include <glm/glm.hpp>

namespace mw {
	static constexpr int MAX_CHUNKS = 8;
	static constexpr int CHUNK_SIZE = 32;

	struct Particle {
		Particle(glm::vec2 _pos) {pos = _pos;}

		glm::vec2 pos;
		glm::vec2 speed = glm::vec2(0.0);

		float volume = 1.0;
		float sediment = 0.0;
	};
}